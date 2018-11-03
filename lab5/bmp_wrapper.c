#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "bmp_wrapper.h"

bmp_header *read_header(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = calloc(1, sizeof(bmp_header));
    assert(header != NULL);

    FILE *f = fopen(filename, "rb");
    assert(f != NULL);

    fread(header, 1, sizeof(bmp_header), f);
    fclose(f);
    return header;
}

bmp_pixel *clean_alignment(bmp_pixel const *dirty_data, bmp_header const *header) {
    assert(dirty_data != NULL);

    bmp_pixel *clean_data = calloc(header->width * header->height, sizeof(bmp_pixel));
    assert(clean_data != NULL);

    for (int i = 0; i < header->width * header->height; ++i) {

    }
}

bmp_pixel *read_data(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = read_header(filename);
    assert(header != NULL);

    // FIXME bmp_bytesz is not always dividable by sizeof(bmp_pixel)
    bmp_pixel *dirty_data = calloc(1, header->bmp_bytesz);
    assert(dirty_data != NULL);

    FILE *f = fopen(filename, "rb");
    assert(f != NULL);

    fread(dirty_data, sizeof(bmp_pixel), header->bmp_bytesz / sizeof(bmp_pixel), f);
    fclose(f);
    free(header);
}

