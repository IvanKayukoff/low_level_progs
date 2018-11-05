#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
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

bmp_pixel *remove_alignment(void const *dirty_data, bmp_header const *header) {
    assert(dirty_data != NULL);
    assert(header != NULL);

    bmp_pixel *clean_data = calloc(header->width * header->height, sizeof(bmp_pixel));
    assert(clean_data != NULL);

    int padding = 4 - (sizeof(bmp_pixel) * header->width) % 4;
    if (4 == padding) padding = 0;

    // i - count of pixels in the clean_data
    // j - count of bytes  in the dirty_data
    uint32_t count_byte = 0;
    for (int i = 0; i < header->height * header->width; i += header->width) {
        memcpy(clean_data + i, dirty_data + count_byte, header->width * sizeof(bmp_pixel));
        count_byte += header->width * sizeof(bmp_pixel) + padding;
    }

    return clean_data;
}

void *add_alignment(bmp_pixel const *clean_data, bmp_header *header) {
    assert(clean_data != NULL);
    assert(header != NULL);

    int padding = 4 - (sizeof(bmp_pixel) * header->width) % 4;
    if (4 == padding) padding = 0;

    void *dirty_data = calloc((header->width + padding) * header->width, sizeof(bmp_pixel));
    assert(dirty_data != NULL);

    // i - count of pixels in the clean_data
    // j - count of bytes  in the dirty_data
    uint32_t count_byte = 0;
    for (int i = 0; i < header->height * header->width; i += header->width) {
        memcpy(dirty_data + count_byte, clean_data + i, header->width * sizeof(bmp_pixel));
        count_byte += header->width * sizeof(bmp_pixel) + padding;
    }

    header->bmp_bytesz = count_byte;
    header->header_sz = sizeof(bmp_header) - 14; // 14 is size of file header fields
    header->filesz = header->bmp_bytesz + header->header_sz;

    return dirty_data;
}

bmp_pixel *mirror_x(bmp_pixel const *src, uint32_t width, uint32_t height) {
    assert(src != NULL);

    bmp_pixel *mirrored = calloc(width * height, sizeof(bmp_pixel));
    assert(mirrored != NULL);

    for (int i = 0, k = height - 1; i < height; ++i, --k) {
        for (int j = 0; j < width; ++j) {
            memcpy(mirrored + i * width + j, src + k * width + j, sizeof(bmp_pixel));
        }
    }

    return mirrored;
}

bmp_pixel *read_pixels(char const *filename) {
    assert(filename != NULL);

    bmp_header *header = read_header(filename);
    assert(header != NULL);

    void *dirty_data = calloc(1, header->bmp_bytesz);
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

    bmp_pixel *mirrored = mirror_x(rotated->data, rotated->width, rotated->height);

    free(rotated->data);
    rotated->data = mirrored;
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

    void *aligned_pixels = add_alignment(img->data, header);
    header->width = img->width;
    header->height = img->height;

    FILE *f = fopen(filename, "wb");
    fwrite(header, sizeof(bmp_header), 1, f);
    fwrite(aligned_pixels, header->bmp_bytesz, 1, f);
    fclose(f);
    free(aligned_pixels);
}
