# Keyboards and Keycodes

## Preface

This document does not describe the hardware interface of any keyboard device. For hardware interface focused documentation, see other docs like [ps2.md](./ps2.md). This is focused on the software interface designed to be the middle-man between software and hardware.

## Keycodes

A keycode is an abstract representation of a physical key. There are many keyboard layouts; to reduce complexity and allow software to be independant from harware, keycodes are used. Our keycodes reflect the layout of a ANSI US QWERTY full-size keyboard. For the full list of keycodes, see [keyboard.h](../../../src/lib/device/keyboard.h).
  
Conversion from scancode, the byte(s) a keyboard gives us, to keycode differs from keyboard to keyboard. In addition, keyboards may have states that modify the interpretation of other keys. To abstract this, the state of every key is maintained along with a "code point function" associated with the keyboard. A "code point function" is a function that takes a reference to a keyboard's state and a scancode and returns a `KeyPress` structure. The function should modify and interpret the current state as necessary to perform translation, although it is not strictly necessary to do so. An example of one of these keys is numlock.

## KeyPresses

A `KeyPress` is an abstract representation of a keyboard event. The structure stores the "tick" (taken from the PIT) the event occured, a keycode, a key event, and any key modifiers that were active at the time of the event. If the keycode is `Key_none` or `Key_Code_Count` the structure can and should be ignored. This is necessary since a keyboard's scancodes can be multiple bytes or have keys that are unsupported (ex. multimedia keys). A key event can be any of the following: `KeyPressed`, `KeyReleased`, `KeyToggled`. The modifiers can be a combination of any of the following: capslock, shift (left or right), ctrl (left or right), numlock, scroll lock, alt (left or right), and cmd (left or right). cmd is commonly referred to as the "Windows key", but may be symbolically different on different keyboards. On Apple keyboards this is the "command" key using the looped square symbol.
  
`KeyPressed` is, as the name implies, when a key is pressed down. `KeyReleased` similarly, is when a key is depressed. `KeyToggled` is unique to toggleable keys such as capslock. It will only appear when a key is pressed and causes the toggle, it is going from off to on. Releasing a toggleable key will always report `KeyReleased`. When pressing results in it going from on to off, `KeyPressed` is reported instead. Note that internal state of the keyboard is not the same as this `KeyPress` structure, just related. A toggleable key will have a state of `KeyToggled` while it is on, when it is off it can be in any other state.

### Further Reading

[PS/2 scancodes](https://wiki.osdev.org/PS/2_Keyboard)
[Microsoft Keycodes](https://download.microsoft.com/download/1/6/1/161ba512-40e2-4cc9-843a-923143f3456c/scancode.doc)
