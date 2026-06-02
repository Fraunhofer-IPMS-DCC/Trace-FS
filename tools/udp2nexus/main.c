/*!
 * Copyright (c) 2026 Fraunhofer IPMS, (www.ipms.fraunhofer.de)
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
 * \file   main.c
 * \author Martin Zimmerling
 * \date   2025-01-13
 * \brief  Main project file - Tracelog to Binaries
 *
 */

// ----------------------------------------------------------------------------
#include <stdint.h>
#include <stdio.h>

// ----------------------------------------------------------------------------
#define DGRAM_MAX           2048
#define FNAMEBUF            2048

// ----------------------------------------------------------------------------
const char      *origFilename;
uint8_t         _glblBuffer[DGRAM_MAX];
uint16_t        _glblSeqnoExp;
FILE            *_glblstream;
uint32_t        _glblFilecount;
char            _fnamebin[FNAMEBUF];

// ----------------------------------------------------------------------------
static int parse_file(const char *filename);
static int next_entry(FILE *fptr);

// ----------------------------------------------------------------------------
int main(int argc, char **argv) {
    int retval;

    printf("=== Tracelog to Binaries ===\n\n");

    // parse arg -> filename
    if (argc < 2) {
        printf("\tError: argument needed\n");
        printf("\tUsage: %s [file]\n", argv[0]);
        return -1;
    }

    origFilename = argv[1];
    retval = parse_file(origFilename);
    printf("Exit...(%d)\n", retval);
    return retval;
}

// ----------------------------------------------------------------------------

static int parse_file(const char *filename) {
    FILE *fptr;

    printf("Parsing File: %s\n", filename);

    // init
    _glblSeqnoExp   = 0xFFFF;
    _glblstream     = NULL;
    _glblFilecount  = 0;

    fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        printf("Error: file not found\n");
        return -1;
    }

    // LOOP over entries
    while (next_entry(fptr)>0);

    // close & clean
    if (_glblstream != NULL) {
        fclose(_glblstream);
        _glblstream = NULL;
    }
    fclose(fptr);
    return 0;
}

static int next_entry(FILE *fptr) {
    size_t cnt;
    uint32_t tstamp_0;
    uint32_t tstamp_1;
    uint32_t ip;
    uint32_t dgramlen;
    uint8_t *u8ptr;
    uint16_t seqno;

    cnt = fread(&tstamp_0, sizeof(uint32_t), 1, fptr);
    if (cnt == 0) return 0;
    cnt += fread(&tstamp_1, sizeof(uint32_t), 1, fptr);
    cnt += fread(&ip      , sizeof(uint32_t), 1, fptr);
    cnt += fread(&dgramlen, sizeof(uint32_t), 1, fptr);
    if (cnt < 4) {
        printf("Warning: partial header data %zu\n", cnt);
        return -1;
    }

    if (dgramlen > DGRAM_MAX) {
        printf("Warning: datagram size exceeds max size %d\n", DGRAM_MAX);
        return -1;
    }

    cnt = fread(_glblBuffer, sizeof(uint8_t), dgramlen, fptr);
    if (cnt < dgramlen) {
        printf("Warning: partial dgram data %zu\n", cnt);
        return -1;
    }
    printf("--- UDP packet boundary,");
    u8ptr = (uint8_t*)(&ip);
    printf(" ip = %d.%d.%d.%d,", u8ptr[0], u8ptr[1], u8ptr[2], u8ptr[3]);
    u8ptr = (uint8_t*)(&tstamp_0);
    printf(" time = %02d:%02d:%02d.%06u", u8ptr[1], u8ptr[2], u8ptr[3], tstamp_1);
    printf(" (%u Bytes) ---\n", dgramlen);

    // parse / dump to binary
    seqno = _glblBuffer[5];
    seqno = (seqno << 8) + _glblBuffer[4];
    printf("--- Seqno %d", seqno);
    if (seqno != _glblSeqnoExp) {
        if (_glblstream != NULL) {
            fclose(_glblstream);
            _glblstream = NULL;
        }
        _glblFilecount++;
        printf("(--> new bin file %u)\n", _glblFilecount);
        snprintf(_fnamebin, FNAMEBUF, "%s.%u", origFilename, _glblFilecount);
        _glblstream = fopen(_fnamebin, "wb");
    }
    printf("\n");
    _glblSeqnoExp = seqno + 1;

    if (_glblstream != NULL) {
        u8ptr = _glblBuffer + 6;
        cnt = fwrite((void*)u8ptr, 1, dgramlen-6, _glblstream);
        printf("Dump %zu Bytes\n", cnt);
    }

    return 1;
}

// ----------------------------------------------------------------------------

