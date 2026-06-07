/**
 * @file louis.cpp
 * 
 * @author JoyWonderful
 * @date 2026-06-06
 */
#include "rich.hpp"

int main() {
#if __WINDOWS__
    bool setup_stat = rich::win_vt_setup();
    if(!setup_stat) {
        printf("Warning: VT mode set failed. If you're running this in conhost, styles will be disabled.\n");
    }
#endif
    printf("Hello\n");
    rich::test();

    return 0;
}