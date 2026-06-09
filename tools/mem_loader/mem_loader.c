/*!
 * Copyright (c) 2026 Fraunhofer IPMS, (www.ipms.fraunhofer.de)
 * Copyright (c) 2026 Accemic Technologies GmbH, (www.accemic.com)
 *
 * Licensed under the Solderpad Hardware License v2.1 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *     http://solderpad.org/licenses/SHL-2.1/
 * Unless required by applicable law or agreed to in writing, distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 * OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * \file   mem_loader.c
 * \author Martin Zimmerling, Albert Schulz
 * \brief  Tool to upload binaries into ITCM or BRAM of the TRACE_FS demonstrator.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>

#define ITCM_MEMORY_ADDRESS ((off_t)0xA0100000)
#define BRAM_MEMORY_ADDRESS ((off_t)0xA1040000)
#define WINDOW_SIZE ((size_t)0x00008000)
#define FILE_ALIGNMENT_MIN 4
#define FILE_ALIGNMENT_PREFERRED 8

static void print_usage(const char *program_name) {
    fprintf(stderr, "Usage: %s <itcm-binary-file> [<bram-binary-file>]\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Loads:\n");
    fprintf(stderr, "  <itcm-binary-file> -> 0x%08llX\n", (unsigned long long)ITCM_MEMORY_ADDRESS);
    fprintf(stderr, "  <bram-binary-file> -> 0x%08llX  (optional; omit for ITCM-only apps)\n",
            (unsigned long long)BRAM_MEMORY_ADDRESS);
    fprintf(stderr, "\n");
    fprintf(stderr, "Limits:\n");
    fprintf(stderr, "  each file must be a multiple of %d bytes\n", FILE_ALIGNMENT_MIN);
    fprintf(stderr, "  each file is limited to %zu bytes after optional padding\n", WINDOW_SIZE);
    fprintf(stderr, "  files that are only %d-byte aligned are padded with zero to %d bytes\n",
            FILE_ALIGNMENT_MIN, FILE_ALIGNMENT_PREFERRED);
}

static int check_elf_magic(const char *filePath) {
    FILE *f = fopen(filePath, "rb");
    unsigned char magic[4];

    if (!f)
        return 0;
    if (fread(magic, 1, 4, f) == 4 &&
        magic[0] == 0x7f && magic[1] == 'E' && magic[2] == 'L' && magic[3] == 'F') {
        fclose(f);
        return 1;
    }
    fclose(f);
    return 0;
}

static int load_file_to_window(int fd, off_t memoryAddress, const char *windowName, const char *filePath) {
    FILE *binFile = NULL;
    unsigned char *map = MAP_FAILED;
    unsigned char *fileBuffer = NULL;
    volatile uint32_t *mapWords;
    long fileSizeLong;
    size_t fileSize;
    size_t paddedSize;
    size_t bytesRead;
    size_t wordCount;
    size_t i;
    int status = 1;

    if (check_elf_magic(filePath)) {
        fprintf(stderr, "Error: '%s' is an ELF file. Please convert to raw binary first, e.g.:\n", filePath);
        fprintf(stderr, "  riscv32-unknown-elf-objcopy -O binary %s output.bin\n", filePath);
        return 1;
    }

    map = mmap(NULL, WINDOW_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, memoryAddress);
    if (map == MAP_FAILED) {
        perror("Memory mapping failed");
        return 1;
    }

    binFile = fopen(filePath, "rb");
    if (!binFile) {
        perror("Failed to open binary file");
        goto cleanup;
    }

    if (fseek(binFile, 0, SEEK_END) != 0) {
        perror("Failed to seek to end of binary file");
        goto cleanup;
    }

    fileSizeLong = ftell(binFile);
    if (fileSizeLong < 0) {
        perror("Failed to determine binary file size");
        goto cleanup;
    }

    fileSize = (size_t)fileSizeLong;
    if ((fileSize % FILE_ALIGNMENT_MIN) != 0) {
        fprintf(stderr, "Error: %s file size (%zu bytes) is not a multiple of %d bytes.\n",
                windowName, fileSize, FILE_ALIGNMENT_MIN);
        goto cleanup;
    }

    paddedSize = fileSize;
    if ((fileSize % FILE_ALIGNMENT_PREFERRED) != 0) {
        paddedSize += FILE_ALIGNMENT_PREFERRED - (fileSize % FILE_ALIGNMENT_PREFERRED);
    }

    if (paddedSize > WINDOW_SIZE) {
        fprintf(stderr,
                "Error: %s file size (%zu bytes, %zu bytes after padding) exceeds configured window size (%zu bytes).\n",
                windowName, fileSize, paddedSize, WINDOW_SIZE);
        goto cleanup;
    }

    fileBuffer = calloc(1, paddedSize);
    if (!fileBuffer) {
        perror("Failed to allocate staging buffer");
        goto cleanup;
    }

    if (fseek(binFile, 0, SEEK_SET) != 0) {
        perror("Failed to seek to start of binary file");
        goto cleanup;
    }

    bytesRead = fread(fileBuffer, 1, fileSize, binFile);
    if (bytesRead != fileSize) {
        fprintf(stderr, "Failed to read full %s binary file: expected %zu bytes, got %zu bytes.\n",
                windowName, fileSize, bytesRead);
        goto cleanup;
    }

    mapWords = (volatile uint32_t *)map;
    wordCount = paddedSize / sizeof(uint32_t);
    for (i = 0; i < wordCount; ++i) {
        uint32_t wordValue;

        memcpy(&wordValue, fileBuffer + (i * sizeof(uint32_t)), sizeof(wordValue));
        mapWords[i] = wordValue;
    }

    if ((fileSize % FILE_ALIGNMENT_PREFERRED) != 0) {
        printf("Info: %s file is only %d-byte aligned; padded %zu trailing bytes with zero.\n",
               windowName, FILE_ALIGNMENT_MIN, paddedSize - fileSize);
    }

    printf("%s binary '%s' loaded successfully (%zu bytes) at 0x%08llX.\n",
           windowName,
           filePath,
           fileSize,
           (unsigned long long)memoryAddress);
    status = 0;

cleanup:
    free(fileBuffer);
    if (binFile) {
        fclose(binFile);
    }
    if (map != MAP_FAILED) {
        munmap(map, WINDOW_SIZE);
    }
    return status;
}

int main(int argc, char *argv[]) {
    int fd = -1;

    if (argc != 2 && argc != 3) {
        print_usage(argv[0]);
        return 1;
    }

    // Open /dev/mem
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd < 0) {
        perror("Failed to open /dev/mem");
        return 1;
    }

    if (load_file_to_window(fd, ITCM_MEMORY_ADDRESS, "ITCM", argv[1]) != 0) {
        close(fd);
        return 1;
    }

    if (argc == 3) {
        if (load_file_to_window(fd, BRAM_MEMORY_ADDRESS, "BRAM", argv[2]) != 0) {
            close(fd);
            return 1;
        }
    }

    close(fd);
    return 0;
}
