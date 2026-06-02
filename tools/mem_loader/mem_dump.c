#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

#define ITCM_MEMORY_ADDRESS 0xa0100000
#define ITCM_MEMORY_SIZE 0x8000
#define BRAM_MEMORY_ADDRESS 0xa1040000
#define BRAM_MEMORY_SIZE 0x8000
#define BUFFER_SIZE 0x8000
#define BYTE_COPY true
#define WORD_COPY false

bool read_dev_mem (uint32_t address, uint32_t size, char const * path, bool byte_copy)
{
    int fd;
    FILE *binFile;
    unsigned char *map = NULL;

    // Open /dev/mem
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("Failed to open /dev/mem");
        return false;
    }

    // Map memory
    map = mmap(NULL, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, address);
    if (map == MAP_FAILED) {
        perror("Memory mapping failed");
        close(fd);
        return false;
    }

    // Open binary file
    fprintf(stdout, "Opening binary file '%s'...\n", path);
    binFile = fopen(path, "wb");
    if (!binFile) {
        perror("Failed to open binary file");
        munmap(map, BUFFER_SIZE);
        close(fd);
        return false;
    }

    assert(size <= BUFFER_SIZE);

    // Read memory and write to file
    if (byte_copy) {
        unsigned char buffer[BUFFER_SIZE] = { 0 };
        unsigned char volatile * src = map;
        unsigned char volatile * dst = buffer;
        for (size_t i = 0; i < size; i++) {
//            fprintf(stderr, "0x%lx <- 0x%02x\n", address + i, *src);
            *dst = *src;
            src++;
            dst++;
        }
        fwrite(buffer, 1, size, binFile);
    } else {
        uint32_t buffer[BUFFER_SIZE/4] = { 0 };
        uint32_t volatile * src = (uint32_t volatile *)map;
        uint32_t volatile * dst = buffer;
        for (size_t i = 0; i < BUFFER_SIZE; i += 4) {
//            fprintf(stderr, "0x%lx <- 0x%08x\n", address + i, *src);
            *dst = *src;
            src++;
            dst++;
        }
        fwrite(buffer, 4, size/4, binFile);
    }

    // Clean up
    fclose(binFile);
    munmap(map, BUFFER_SIZE);
    close(fd);

    fprintf(stdout, "Memory successfully dumped.\n");

    return true;
}

int main() {

    read_dev_mem(ITCM_MEMORY_ADDRESS, ITCM_MEMORY_SIZE, "itcm-dump.bin", WORD_COPY);

    return 0;
}

