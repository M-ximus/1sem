#include "Assm.h"
#include "soft_CPU.h"

int main() {
    int sourse_code = open(way_to_in, O_RDONLY|O_BINARY);
    FILE* machine_code = fopen(way_to_tell, "wb");
    char* binary_code = Assembler(sourse_code, machine_code);

    CPU(binary_code);

    return 0;
}