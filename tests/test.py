from threading import Thread, Lock
import os
import subprocess
import time
import socket
import errno


BASE_PORT = 1000
MAX_PORT = 1100

MAX_INSTANCES = 20

TEST_BINARY_PATH = "./bin/"
EXPECTED_PATH = "./expected/"

LOCALHOST = "127.0.0.1"
DATA_DELAY = 1


QEMU_ARGS = ["qemu-system-i386", "-boot", "c", "-hda", "./../mOS.bin", "-no-reboot", "-no-shutdown", "-nographic", "-serial"]
QEMU_SERIAL_DEV = "tcp:localhost:{port},server"

#60 seconds is the limit!
TEST_TIMEOUT = 60

active_instances = 0
instance_mutex = Lock()

# although the ports are assigned iteratively, there are instances where things are in use
# one possibility is that another process is using that port. It will be marked used forever.
# another is that an instance is using it and we've wrapped around. Once that instance ends it will be unmarked. 
used_ports = set()
current_port = BASE_PORT
port_mutex = Lock()

# a list of the results from all tests. It is a tuple of (test_binary, pass_or_fail: bool)
# true indicates a pass, false indicates a failure
results = []
result_mutex = Lock()

# returns list of tuples where the tuple is (path, filename)
def get_all_files(dir):
    out = []
    
    files = os.scandir(dir)

    for file in files:
        if (file.is_dir()):
            out += get_all_files(file.path)
        elif (file.is_file()):
            if (file.name.endswith(".bin") or file.name.endswith(".expect")):
                out.append((file, file.name))

    files.close()

    return out

def get_expected(expecteds, equivalent: str):
    for expected in expecteds:
        if (expected[1] == equivalent):
            return expected

    return ("", "")

def test(port, bin_path, expected_path, qemu):
    
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        try:
            s.connect((LOCALHOST, port))

            # we have connected to the OS, begin sending data
            # we breakup the binary to send it over multiple packets
            s.send(b"test\0")
            
            with open(bin_path, "rb") as binary:
                bin = binary.read()

                s.send(len(bin).to_bytes(4, "little"))

                for chunk in bin[::128]:
                    s.send(chunk)
                    time.sleep(DATA_DELAY)

            with open(expected_path, "r") as expected:
                expect = expected.read()

                for chr in expect:
                    got = s.recv(1)
                    if (chr != got):
                        with result_mutex:
                            results.append((bin_path, False))

                        with port_mutex:
                            used_ports.remove(port)

                        with instance_mutex:
                            active_instances -= 1

                        return

                with result_mutex:
                    results.append((bin_path, True))

        except socket.timeout:
            with result_mutex:
                results.append((bin_path, False))
                print("failed to connect")
        except socket.error as e:
            if (e.errno == errno.EADDRINUSE):
                qemu.join()

                create_instance(bin_path, expected_path)
                with instance_mutex:
                    active_instances -= 1

                return
            else:
                with result_mutex:
                    results.append((bin_path, False))

        
    qemu.join()

    with port_mutex:
        used_ports.remove(port)

    with instance_mutex:
        active_instances -= 1

def qemu(port, bin_path):
    command = QEMU_ARGS
    command.append(QEMU_SERIAL_DEV.format(port))

    try:
        if (subprocess.run(command, timeout=TEST_TIMEOUT) != 0):
            print("QEMU exited with nonzero result for " + bin_path)
    except subprocess.TimeoutExpired:
        print("Test for " + bin_path + " timed out")

def create_instance(bin_path, expected_path):
    
    port_to_use = 0

    with port_mutex:
        if (len(used_ports) > MAX_PORT - BASE_PORT):
            # this should not ever happen.
            print("All ports in use!")
            exit(-1)

        # crude way to get next port to use
        while (current_port in used_ports):
            current_port += 1
            if (current_port > MAX_PORT):
                current_port = BASE_PORT

        port_to_use = current_port
        used_ports.add(port_to_use)  

        current_port += 1
        if (current_port > MAX_PORT):
                current_port = BASE_PORT

    command = QEMU_ARGS
    command.append(QEMU_SERIAL_DEV.format(port = port_to_use))

    with instance_mutex:
        active_instances += 1

    q = Thread(target=qemu, args=(port_to_use, bin_path))
    q.start()

    t = Thread(target=test, args=(port_to_use, bin_path, expected_path, q))
    t.start()
    
    return t

def do_tests():
    expected = get_all_files(EXPECTED_PATH)
    binaries = get_all_files(TEST_BINARY_PATH)

    if (len(binaries) == 0):
        print("No binaries to test")
        return

    if (len(expected) == 0):
        print("No expecteds to test against")
        return

    for binary in binaries:
        equivalent = binary[1][:-4] + ".expect"
        expect = get_expected(expected, equivalent)

        if (expect[1] == ""):
            print("No expected found for test ${binary[1]}")
        else:
            create_instance(binary[0], expect[0])

    
    active = MAX_INSTANCES
    while (active != 0):
        with instance_mutex:
            active = active_instances
        time.sleep(1)

    # this should acquire immediatly since all the threads are done
    with result_mutex:
        for result in results:
            print(result[0] + " : " + str(result[1]))


if __name__ == "__main__":
    do_tests()