#include "grand_bmp.h"
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <memory.h>

int compute_total_padding_rotated(bmp_header *header) {
    return header->width * (header->height % 4);
}

void rotate_pixels_right(void *dest, void const *src, uint32_t width, uint32_t height) {
    assert(dest != NULL);
    assert(src != NULL);

    uint64_t src_padding = 0;
    uint64_t dest_padding = 0;
    for (int h = 0; h < height; ++h) {
        src_padding = h * (width % 4);
        for (int w = 0; w < width; ++w) {
            dest_padding = w * (height % 4);
            *(bmp_pixel *)(dest + (((width - 1 - w) * height) + h) * sizeof(bmp_pixel) + dest_padding) =
                    *(bmp_pixel *)(src + (h * width + w) * sizeof(bmp_pixel) + src_padding);
        }
    }
}

bool image_bmp_fused_rotate_90cw(const char *src_path, const char *dst_path) {
    assert(src_path != NULL);
    assert(dst_path != NULL);

    struct stat src_stat;
    int src_fd = open(src_path, O_RDONLY);
    int dst_fd = open(dst_path, O_RDWR | O_CREAT | O_TRUNC, (mode_t) 0644);

    fstat(src_fd, &src_stat);
    size_t src_size = (size_t) src_stat.st_size;

    void *src = mmap(NULL, src_size, PROT_READ, MAP_SHARED, src_fd, 0);
    bmp_header header = *((bmp_header *) src);

    size_t dst_size = header.width * header.height * sizeof(bmp_pixel) +
            compute_total_padding_rotated(&header) + header.bmp_offset;

    /* "Stretch" the destination to its full size */
    lseek(dst_fd, dst_size - 1, SEEK_SET);
    write(dst_fd, "", 1);

    void *dst = mmap(NULL, dst_size, PROT_READ | PROT_WRITE, MAP_SHARED, dst_fd, 0);

    rotate_pixels_right(dst + header.bmp_offset, src + header.bmp_offset, header.width, header.height);

    uint32_t width = header.width;
    header.width = header.height;
    header.height = width;

    memcpy(dst, &header, sizeof(bmp_header));
    msync(dst, dst_size, MS_SYNC);
    munmap(src, src_size);
    munmap(dst, src_size);

    close(src_fd);
    close(dst_fd);

    return true;
}
