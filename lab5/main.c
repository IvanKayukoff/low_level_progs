#include <stdio.h>
#include <stdlib.h>
#include "bmp_wrapper.h"

int main() {
    printf("%ld", sizeof(bmp_header));
    bmp_header *header = read_header("resources/meme.bmp");
    bmp_image *img = read_bmp_image("resources/meme.bmp");
    bmp_image *rotated = rotate_right(img);
    write_bmp_image(rotated, header, "resources/rotated.bmp");

    free(header);
    free(img->data);
    free(img);
    free(rotated->data);
    free(rotated);
    return 0;
}
