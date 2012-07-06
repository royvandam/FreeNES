/**
 * Copyright (c) 2009 Roy van Dam <roy@8bit.cx>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "debug.hpp"
#include "nes/emulator.hpp"
using namespace nes;

int
emulator_t::load(string filename)
{
    int fd;
    struct stat st;
    uint8_t mapper;

    if ((fd = open(filename.c_str(), O_RDONLY)) < 0) {
        perror("open");
        return (1);
    }

    if (fstat(fd, &st) == -1) {
        perror("fstat");
        return (1);
    }

    this->rom = (uint8_t *)mmap(NULL, st.st_size, PROT_READ, MAP_FILE | MAP_PRIVATE, fd, 0);
    this->rom_size = st.st_size - 16;

    if (this->rom == MAP_FAILED) {
        perror("mmap");
        return (1);
    }

    memset(this->ram, 0x42424242, sizeof this->ram);
    this->rom_header = (rom_header_t *)this->rom;
    if (memcmp(this->rom_header->name, "NES\x1A", 4) != 0) {
        fprintf(stderr, "Not a NES rom\n");
        return (1);
    }

    printf("%hhu %hhu %hhx %hhx\n",
        this->rom_header->nrombank, this->rom_header->nvrombank,
        this->rom_header->flags1, this->rom_header->flags2);
        
    mapper = (this->rom_header->flags1 >> 4) | (this->rom_header->flags2 & 0xf0);
    printf("Mapper: %hhu\n", mapper);

    return (0);
}

int
emulator_t::run(void)
{
    for (;;) {
        if (this->step() != 0) {
            fprintf(stderr, "Bad instruction\n");
            return (1);
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    return (0);
}

uint8_t
emulator_t::read_byte(uint16_t address)
{
    if (address < 0x2000) {
        uint8_t value;
        value = this->ram[address % 0x800];

        if (value == 0x42) {
            debug("Address %hx may not be initialized\n", address);
        }

        return (value);
    } else if (address == 0x2002) {
        /* XXX: Hit and vblank flags. */
        return (0xc0);
    } else if ((address >= NES_ROM_OFFSET) && (address < (NES_ROM_OFFSET + this->rom_size))) {
        return rom[address - NES_ROM_OFFSET + 16];
    } else {
        debug("Bad read on %hx\n", address);
        return (0);
    }
}

void
emulator_t::write_byte(uint16_t address, uint8_t value)
{
    if (address < 0x2000) {
        debug("RAM write on %hx\n", address);
        this->ram[address % 0x800] = value;
    } else {
        debug("Bad write on %hx: %hhx\n", address, value);
    }
}