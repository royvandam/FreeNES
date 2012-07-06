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

void
emulator_t::_load_word(_ins_load_word_t instruction, uint16_t address)
{
    uint16_t value;
    value = this->_read_word(address);

    debug("LOAD16: %hx: %hu\n", address, value);
    (this->*instruction)(value);
}

void
emulator_t::_load_word_imm(_ins_load_word_t instruction)
{
    uint16_t value;
    value = this->_progress_word();

    debug("LOAD16: imm: %hu\n", value);
    (this->*instruction)(value);
}

void
emulator_t::_load_word_abs(_ins_load_word_t instruction)
{
    this->_load_word(instruction, this->_addr_abs());
}