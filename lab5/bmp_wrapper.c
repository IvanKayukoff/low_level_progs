#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <memory.h>
#include "bmp_wrapper.h"

bmp_read_status read_header(char const *filename, bmp_header **dest) {
    assert(filename != NULL);
    assert(*dest == NULL);

    *dest = calloc(1, sizeof(bmp_header));
    assert(*dest != NULL);

    FILE *f = fopen(filename, "rb");
    if (NULL == f) return READ_ERROR;

    fread(*dest, 1, sizeof(bmp_header), f);
    fclose(f);

    if ((*dest)->bf_type != 0x4D42) return READ_INVALID_SIGNATURE;  // 0x4D42 is a 'BM' chars in the little endian
    if ((*dest)->bitspp != 24) return READ_INVALID_BITS;
    if ((*dest)->header_sz != 124) return READ_INVALID_BMP_FORMAT; // 124 is a size of bmp header

    return READ_OK;
}

bmp_pixel *remove_alignment(void const *dirty_data, bmp_header const *header) {
    assert(dirty_data != NULL);
    assert(header != NULL);

    bmp_pixel *clean_data = calloc(header->width * header->height, sizeof(bmp_pixel));
    assert(clean_data != NULL);

    int padding = 4 - (sizeof(bmp_pixel) * header->width) % 4;
    if (4 == padding) padding = 0;

    // i - count of pixels in the clean_data
    // count_byte - count of bytes  in the dirty_data
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
    // count_byte - count of bytes  in the dirty_data
    uint32_t count_byte = 0;
    for (int i = 0; i < header->height * header->width; i += header->width) {
        memcpy(dirty_data + count_byte, clean_data + i, header->width * sizeof(bmp_pixel));
        count_byte += header->width * sizeof(bmp_pixel) + padding;
    }

    header->bmp_bytesz = count_byte;
    header->header_sz = sizeof(bmp_header) - 14; // 14 is a size of file header fields
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

bmp_read_status read_pixels(char const *filename, bmp_pixel **dest) {
    assert(filename != NULL);
    assert(*dest == NULL);

    bmp_header *header = NULL;
    bmp_read_status status = read_header(filename, &header);
    if (status != READ_OK) return status;
    assert(header != NULL);

    void *dirty_data = calloc(1, header->bmp_bytesz);
    assert(dirty_data != NULL);

    FILE *f = fopen(filename, "rb");
    assert(f != NULL);

    fseek(f, header->bmp_offset, SEEK_SET);
    fread(dirty_data, 1, header->bmp_bytesz, f);
    *dest = remove_alignment(dirty_data, header);

    fclose(f);
    free(header);
    free(dirty_data);

    return READ_OK;
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
            *(rotated->data + ((img->width - 1 - j) * img->height) + i) = *(img->data + i * img->width + j);
        }
    }

    return rotated;
}

bmp_read_status read_bmp_image(char const *filename, bmp_image **dest) {
    assert(filename != NULL);
    assert(*dest == NULL);

    bmp_header *header = NULL;
    bmp_read_status status = read_header(filename, &header);
    if (status != READ_OK) return status;

    bmp_pixel *pixels = NULL;
    read_pixels(filename, &pixels);
    *dest = calloc(1, sizeof(bmp_image));
    assert(*dest != NULL);

    (*dest)->data = pixels;
    (*dest)->width = header->width;
    (*dest)->height = header->height;

    return READ_OK;
}

bmp_write_status write_bmp_image(bmp_image const *img, bmp_header *header, char const *filename) {
    assert(img != NULL && img->data != NULL);
    assert(header != NULL);
    assert(filename != NULL);

    void *aligned_pixels = add_alignment(img->data, header);
    header->width = img->width;
    header->height = img->height;

    FILE *f = fopen(filename, "wb");
    if (NULL == f) {
        free(aligned_pixels);
        return WRITE_ERROR;
    }

    size_t is_header_written = fwrite(header, sizeof(bmp_header), 1, f);
    size_t is_pixels_written = fwrite(aligned_pixels, header->bmp_bytesz, 1, f);
    fclose(f);
    free(aligned_pixels);

    if (!is_header_written || !is_pixels_written) return WRITE_ERROR;
    else return WRITE_OK;
}
