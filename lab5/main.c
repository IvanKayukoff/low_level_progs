#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "bmp_wrapper.h"

int print_read_status(bmp_read_status status) {
    if      (status == READ_INVALID_SIGNATURE)   fprintf(stderr, "Wrong BMP signature\n");
    else if (status == READ_INVALID_BITS)        fprintf(stderr, "BMP must be 24 bits per pixel\n");
    else if (status == READ_INVALID_BMP_FORMAT)  fprintf(stderr, "BMP must be in the V5 format\n");
    else if (status == READ_ERROR)               fprintf(stderr, "Error while reading, maybe file is missing\n");

    if (status != READ_OK)  return 1;
    else                    return 0;
}

int print_write_status(bmp_write_status status) {
    if (status == WRITE_ERROR) {
        fprintf(stderr, "Error while writing, probably file is missing");
        return 1;
    } else return 0;
}

int main(int argc, char *argv[]) {
    if (argc <= 1) {
        fprintf(stderr, "This program requires the path to the bmp file as the first cli parameter\n");
        return 1;
    }
    char *filename = argv[1];

    bmp_header *header = NULL;
    bmp_read_status rh_status = read_header(filename, &header);
    if (print_read_status(rh_status) != 0) return 1;

    bmp_image *img = NULL;
    bmp_read_status rp_status = read_bmp_image(filename, &img);
    if (print_read_status(rp_status) != 0) {
        free(header);
        return 1;
    }

    bmp_image *rotated = rotate_right(img);

    char *out_filename = ".rotated.bmp";
    strcat(filename, out_filename);
    bmp_write_status w_status = write_bmp_image(rotated, header, filename);
    int is_write_failed = 0;
    if (print_write_status(w_status) != 0) is_write_failed = 1;

    free(header);
    free(img->data);
    free(img);
    free(rotated->data);
    free(rotated);
    return is_write_failed;
}
