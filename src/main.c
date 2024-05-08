#include <stdio.h>
#include "cpu.h"
#include "display.h"

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;
	display gfx;
	initDisplay(&gfx);

    return 0;
}
