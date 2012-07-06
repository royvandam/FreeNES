/**
 * Copyright (c) 2009 Roy van Dam <roy@8bit.cx>
 * Copyright (c) 2009 Ed Schouten <ed@80386.nl> (original mos6502 emulator in c)
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
#include "mos6502/emulator.hpp"
using namespace mos6502;

uint8_t
emulator_t::_read_byte(uint16_t address)
{
    return (this->read_byte(address));
}

uint16_t
emulator_t::_read_word(uint16_t address)
{
    return (this->_read_byte(address) | (uint16_t) this->_read_byte(address + 1) << 8);
}

void
emulator_t::_write_byte(uint16_t address, uint8_t value)
{
    this->write_byte(address, value);
}

void
emulator_t::_push_byte(uint8_t value)
{
    if ((this->_stack_pointer + 0x100) == 0x1FF) {
        debug("Stack overflow!");
        return;
    }

    this->_stack_pointer--;

    uint16_t address;
    address = (uint16_t)this->_stack_pointer + 0x100;

    debug("PUSH_BYTE [0x%x] = 0x%x\n", address, value);
    this->_write_byte(address, value);
}

void
emulator_t::_push_word(uint16_t value)
{
    this->_push_byte(value >> 8);
    this->_push_byte(value);
}

uint8_t
emulator_t::_pop_byte(void)
{
    if ((this->_stack_pointer + 0x100) == 0x100) {
        debug("Stack underflow!");
        return 0;
    }

    uint16_t address;
    address = (uint16_t)this->_stack_pointer + 0x100;

    this->_stack_pointer++;

    uint8_t value;
    value = this->_read_byte(address);

    debug("POP_BYTE [0x%x] = 0x%x\n", address, value);
    return (value);
}

uint16_t
emulator_t::_pop_word(void)
{
    return (this->_pop_byte() | (uint16_t) this->_pop_byte() << 8);
}

uint8_t
emulator_t::_progress_byte(void)
{
    uint8_t value;
    value = this->_read_byte(this->_program_counter);

    this->_program_counter += 1;
    return (value);
}

uint16_t
emulator_t::_progress_word(void)
{
    uint16_t value;
    value = this->_read_word(this->_program_counter);

    this->_program_counter += 2;
    return (value);
}