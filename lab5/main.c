#include <stdio.h>
#include <stdlib.h>
#include "bmp_wrapper.h"

int main() {
    bmp_header *header = read_header("resources/meme.bmp");

    free(header);
    return 0;
}
