#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "bmp_wrapper.h"

bmp_header * read_header(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = calloc(1, sizeof(bmp_header));
    assert(header != NULL);

    FILE *f = fopen(filename, "rb");
    assert(f != NULL);

    fread(header, 1, sizeof(bmp_header), f);
    fclose(f);

    return header;
}

bmp_pixel *remove_alignment(bmp_pixel const *dirty_data, bmp_header const *header) {
    assert(dirty_data != NULL);
    assert(header != NULL);

    bmp_pixel *clean_data = calloc(header->width * header->height, sizeof(bmp_pixel));
    assert(clean_data != NULL);

    int padding = 4 - header->width % 4;
    if (4 == padding) padding = 0;
    for (int i = 0, j = 0, k = 0; k < header->height; i += header->width + padding, j += header->width, ++k) {
        memcpy(clean_data + j, dirty_data + i, sizeof(bmp_pixel) * header->width);
    }

    return clean_data;
}

// FIXME alignment to 4 bytes, not pixels!
bmp_pixel *add_alignment(bmp_pixel const *clean_data, bmp_header const *header) {
    assert(clean_data != NULL);
    assert(header != NULL);

    int padding = 4 - header->width % 4;
    if (4 == padding) padding = 0;
    bmp_pixel *dirty_data = calloc((header->width + padding) * header->width, sizeof(bmp_pixel));
    assert(dirty_data != NULL);

    for (int i = 0, j = 0, k = 0; k < header->height; i += header->width + padding, j += header->width, ++k) {
        memcpy(dirty_data + i, clean_data + j, sizeof(bmp_pixel) * header->width);
        memset(dirty_data, 0, padding * sizeof(bmp_pixel));
    }

    return dirty_data;
}

bmp_pixel *read_pixels(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = read_header(filename);
    assert(header != NULL);

    bmp_pixel *dirty_data = calloc(1, header->bmp_bytesz);
    assert(dirty_data != NULL);

    FILE *f = fopen(filename, "rb");
    assert(f != NULL);

    fseek(f, header->bmp_offset, SEEK_SET);
    fread(dirty_data, 1, header->bmp_bytesz, f);
    bmp_pixel *clean_data = remove_alignment(dirty_data, header);

    fclose(f);
    free(header);
    free(dirty_data);

    return clean_data;
}

bmp_image *rotate_right(bmp_image const *img) {
    assert(img != NULL);

    bmp_image *rotated = calloc(1, sizeof(bmp_image));
    assert(rotated != NULL);

    rotated->width = img->height;
    rotated->height = img->width;
    rotated->data = calloc(img->width * img->height, sizeof(bmp_pixel));
    assert(rotated->data != NULL);

    for (int i = 0; i < img->height; ++i) {
        for (int j = 0; j < img->width; ++j) {
            *(rotated->data + j * img->height + i) = *(img->data + i * img->width + j);
        }
    }

    return rotated;
}

bmp_image *read_bmp_image(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = read_header(filename);
    bmp_pixel *pixels = read_pixels(filename);
    bmp_image *img = calloc(1, sizeof(bmp_image));
    assert(img != NULL);

    img->data = pixels;
    img->width = header->width;
    img->height = header->height;

    return img;
}

void write_bmp_image(bmp_image const *img, bmp_header *header, char const *filename) {
    assert(img != NULL && img->data != NULL);
    assert(header != NULL);
    assert(filename != NULL);

    bmp_pixel *aligned_pixels = add_alignment(img->data, header);
    header->width = img->width;
    header->height = img->height;

    FILE *f = fopen(filename, "wb");
    fwrite(header, sizeof(bmp_header), 1, f);

    int padding = 4 - header->width % 4;
    if (4 == padding) padding = 0;
    fwrite(aligned_pixels, sizeof(bmp_pixel), (img->width + padding) * img->height, f);
    fclose(f);
    free(aligned_pixels);
}
