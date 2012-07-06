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

#include "mos6502/emulator.hpp"
using namespace mos6502;

uint16_t
emulator_t::_addr_abs (void)
{
	return this->_progress_word();
}

uint16_t
emulator_t::_addr_absx (void)
{
	return this->_addr_abs() + this->_index_x;
}

uint16_t
emulator_t::_addr_absy (void)
{
	return this->_addr_abs() + this->_index_y;
}

uint16_t
emulator_t::_addr_xind (void)
{
	return this->_read_word (this->_addr_zpgx());
}

uint16_t
emulator_t::_addr_indy (void)
{
	return this->_read_word (this->_addr_zpg()) + this->_index_y;
}

uint16_t
emulator_t::_addr_zpg (void)
{
	return (uint16_t) this->_progress_byte ();
}

uint16_t
emulator_t::_addr_zpgx (void)
{
	return (uint16_t) (this->_addr_zpg () + this->_index_x);
}

uint16_t
emulator_t::_addr_zpgy (void)
{
	return (uint16_t) (this->_addr_zpg () + this->_index_y);
}