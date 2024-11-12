#include "../inc/main.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    printf("Hello, world!\n");
    
    int shift_dir;
    int shift_displ;
    uint32_t value_m;

    sscanf(argv[1], "%d", &shift_dir);
    sscanf(argv[2], "%d", &shift_displ);
    sscanf(argv[3], "%u", &value_m);

    printf("value_m at initialization: %u\n", value_m);

    bitshift(shift_dir, shift_displ, &value_m);

    printf("value_m after bitshift left by %d: %u\n", 1, value_m);
    
    
    return 0;
}


int bitshift(int shift_direction, int shift, uint32_t *value) {
    switch (shift_direction)
    {
    case 0:
        *value = *value << shift;
        break;
    
    case 1:
        *value = *value >> shift;
        break;

    default:
        break;
    }
    return 0;
}

