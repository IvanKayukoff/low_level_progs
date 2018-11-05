#ifndef LAB5_BMP_WRAPPER_H
#define LAB5_BMP_WRAPPER_H

#include <stdint.h>

typedef struct __attribute__((packed)) {
    uint16_t bf_type;
    uint32_t filesz;
    uint16_t creator1;
    uint16_t creator2;
    uint32_t bmp_offset;
    uint32_t header_sz;

    uint32_t width;
    uint32_t height;
    uint16_t nplanes;
    uint16_t bitspp;
    uint32_t compress_type;
    uint32_t bmp_bytesz;
    uint32_t hres;
    uint32_t vres;
    uint32_t ncolors;
    uint32_t nimpcolors;

    uint8_t v4fields[68];
} bmp_header;

typedef struct {
    uint8_t b, g, r;
} bmp_pixel;

typedef struct {
    uint32_t width, height;
    bmp_pixel *data;
} bmp_image;

bmp_header *read_header(char const *filename);

bmp_pixel *read_pixels(char const *filename);

bmp_image *rotate_right(bmp_image const *img);

bmp_image *read_bmp_image(char const *filename);

void write_bmp_image(bmp_image const *img, bmp_header *header, char const *filename);

#endif //LAB5_BMP_WRAPPER_H
