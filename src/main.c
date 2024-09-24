#include <stdio.h>
#include "cpu.h"
#include "display.h"

int main(int argc, char* argv[]) {
    (void) argc;
    (void) argv;

	display gfx;
    // TODO remove render() call in init
	initDisplay(&gfx);

    //set up input
    

    return 0;
}
