void write_string( int colour, const char *string );

int os_main(){

    int colour = 11;
    const char *string = "Hello, World!";

    write_string(colour, string);

    while (1)
        ;

    return 0;
}

void write_string( int colour, const char *string )
{
    volatile char *video = (volatile char*)0xB8000;
    while( *string != 0 )
    {
        *video++ = *string++;
        *video++ = colour;
    }
}
