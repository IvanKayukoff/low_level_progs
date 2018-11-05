#ifndef LAB5_BMP_WRAPPER_H
#define LAB5_BMP_WRAPPER_H

#include <stdint.h>

/** Structure which provides a BITMAPV5HEADER format */
typedef struct __attribute__((packed)) {
    uint16_t bf_type;       // The header field used to identify the BMP
    uint32_t filesz;        // The size of the BMP file in bytes
    uint16_t creator1;      // Reserved, actual value depends on the application that creates the image
    uint16_t creator2;      // Reserved, actual value depends on the application that creates the image
    uint32_t bmp_offset;    // The offset, i.e. starting address, of the byte where the bitmap image data
                            //      (pixel array) can be found
    uint32_t header_sz;     // The size of this header

    uint32_t width;         // The bitmap width in pixels
    uint32_t height;        // The bitmap height in pixels
    uint16_t nplanes;       // The number of color planes (must be 1)
    uint16_t bitspp;        // The number of bits per pixel, which is the color depth of the image,
                            //      typical values are 1, 4, 8, 16, 24 and 32
    uint32_t compress_type; // The compression method being used
    uint32_t bmp_bytesz;    // The image size, i.e. the size of the raw bitmap data
    uint32_t hres;          // The horizontal resolution of the image (pixel per meter)
    uint32_t vres;          // The vertical resolution of the image (pixel per meter)
    uint32_t ncolors;       // The number of colors in the color palette
    uint32_t nimpcolors;    // The number of important colors used, or 0 when every color is important,
                            //      generally ignored

    uint8_t v5fields[84];   // The additional header fields for BITMAPV5HEADER support
} bmp_header;

/**
 *  Structure which contains blue, green, red values of the bmp_pixel
 *  Reason of this order is a data storage feature of the BMP
 **/
typedef struct {
    uint8_t b, g, r;
} bmp_pixel;

/**
 *  Contains width and height of the image, and [data] - array of pixels
 **/
typedef struct {
    uint32_t width, height;
    bmp_pixel *data;
} bmp_image;

/** Reads a header from file with [filename] */
bmp_header *read_header(char const *filename);

/**
 *  Reads all pixels from file with [filename]
 *  @return bmp_pixel array, which size you should get from the header instance
 **/
bmp_pixel *read_pixels(char const *filename);

/**
 *  Turns the [img] right, creates new bmp_image instance for containing a result
 **/
bmp_image *rotate_right(bmp_image const *img);

/**
 *  Reads an image from [filename]
 *  @return bmp_image instance, from which we should free allocated memory after using:
 *          free(img->data); free(img);
 **/
bmp_image *read_bmp_image(char const *filename);

/**
 *  Writes an image to [filename] with specified [header]
 *  @param header is a bmp_header which will be modified inside:
 *         header->width and header->height set according to img->width and img->height
 **/
void write_bmp_image(bmp_image const *img, bmp_header *header, char const *filename);

#endif //LAB5_BMP_WRAPPER_H
