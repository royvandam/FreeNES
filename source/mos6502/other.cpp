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
emulator_t::_branch_on_clear(uint8_t flag, uint8_t value)
{
    if (!(this->_status_flag & flag)) {
        debug("Taking branch\n");
        this->_program_counter += (int8_t)value;
    } else {
        debug("Skipping branch\n");
    }
}

void
emulator_t::_branch_on_set(uint8_t flag, uint8_t value)
{
    if (this->_status_flag & flag) {
        debug("Taking branch\n");
        this->_program_counter += (int8_t)value;
    } else {
        debug("Skipping branch\n");
    }
}

void
emulator_t::_update_flag(uint8_t flag, uint8_t mode)
{
    if (mode) {
        this->_status_flag |= (flag);
    } else {
        this->_status_flag &= ~(flag);
    }
}

void
emulator_t::_update_carry(uint_least16_t value)
{
    this->_update_flag(_MOS_RF_CARRY, value > UINT8_MAX);
}

void
emulator_t::_update_overflow(int_least16_t value)
{
    this->_update_flag(_MOS_RF_OVERFLOW, (value < INT8_MIN) || (value > INT8_MAX));
}

void
emulator_t::_update_negative(uint8_t value)
{
    this->_update_flag(_MOS_RF_NEGATIVE, (int8_t)(value) < 0);
}

void
emulator_t::_update_zero(uint8_t value)
{
    this->_update_flag(_MOS_RF_ZERO, (uint8_t)(value) == 0);
}

void
emulator_t::_noarg(_ins_noarg_t instruction)
{
    (this->*instruction)();
}

void
emulator_t::_compare(uint8_t value_a, uint8_t value_b)
{
    this->_update_flag(_MOS_RF_CARRY, (value_a >= value_b));
    this->_update_flag(_MOS_RF_NEGATIVE, (value_a < value_b));
    this->_update_flag(_MOS_RF_ZERO, (value_a == value_b));
}

uint8_t
emulator_t::_carry(void)
{
    return ((this->_status_flag & _MOS_RF_CARRY) ? 1 : 0);
}