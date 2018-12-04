#include "Assm.h"

int main() {
    int sourse_code = open(way_to_in, O_RDONLY|O_BINARY);
    FILE* byte_code = fopen(way_to_tell, "wb");
    Assembler(sourse_code, byte_code);

    //printf("%s", binary_code);
    //CPU(binary_code);

    return 0;
}