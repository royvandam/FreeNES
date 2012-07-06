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

void
emulator_t::_ins_adc(uint8_t value)  // ADC: Add memory to accumulator with carry.
{
    uint_least16_t _unsigned;
    int_least16_t _signed;

    _unsigned = this->_accumulator + value + this->_carry();
    this->_update_carry(_unsigned);

    _signed = (int8_t)this->_accumulator + (int8_t)value + this->_carry();
    this->_update_overflow(_signed);

    this->_accumulator = _unsigned;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

void
emulator_t::_ins_and(uint8_t value)  // AND: And accumulator with memory.
{
    this->_accumulator &= value;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

uint8_t
emulator_t::_ins_asl(uint8_t value)  // ASL: Arithmetic shift left.
{
    this->_update_flag(_MOS_RF_CARRY, value & 0x80);
    value <<= 1;

    this->_update_negative(value);
    this->_update_zero(value);

    return (value);
}

void
emulator_t::_ins_bit(uint8_t value)  // BIT: Bit test in memory with accumulator.
{
    this->_update_flag(_MOS_RF_NEGATIVE, value & 0x80);
    this->_update_flag(_MOS_RF_OVERFLOW, value & 0x40);
    this->_update_zero(this->_accumulator & value);
}

void
emulator_t::_ins_bcc(uint8_t value)  // BCC: Branch on carry clear.
{
    this->_branch_on_clear(_MOS_RF_CARRY, value);
}

void
emulator_t::_ins_bcs(uint8_t value)  // BCS: Branch on carry set.
{
    this->_branch_on_set(_MOS_RF_CARRY, value);
}

void
emulator_t::_ins_beq(uint8_t value)  // BEQ: Branch on result zero.
{
    this->_branch_on_set(_MOS_RF_ZERO, value);
}

void
emulator_t::_ins_bmi(uint8_t value)  // BMI: Branch on result minus.
{
    this->_branch_on_set(_MOS_RF_NEGATIVE, value);
}

void
emulator_t::_ins_bne(uint8_t value)  // BNE: Branch on result not zero.
{
    this->_branch_on_clear(_MOS_RF_ZERO, value);
}

void
emulator_t::_ins_bpl(uint8_t value)  // BPL: Branch on result plus.
{
    this->_branch_on_clear(_MOS_RF_NEGATIVE, value);
}

void
emulator_t::_ins_brk(void)  // BRK: Force break.
{
    this->_update_flag(_MOS_RF_BREAK, 1);
    this->interrupt(0xfffe);
}

void
emulator_t::_ins_bvc(uint8_t value)  // BVC: Branch on overflow clear.
{
    this->_branch_on_clear(_MOS_RF_OVERFLOW, value);
}

void
emulator_t::_ins_bvs(uint8_t value)  // BVS: Branch on overflow set.
{
    this->_branch_on_set(_MOS_RF_OVERFLOW, value);
}

void
emulator_t::_ins_clc(void)  // CLV: Clear carry flag.
{
    this->_update_flag(_MOS_RF_CARRY, 0);
}

void
emulator_t::_ins_cld(void)  // CLD: Clear decimal flag.
{
    this->_update_flag(_MOS_RF_DECIMAL, 0);
}

void
emulator_t::_ins_cli(void)  // CLI: Clear interrupt disable flag.
{
    this->_update_flag(_MOS_RF_NOINTERRUPT, 0);
}

void
emulator_t::_ins_clv(void)  // CLV: Clear overflow flag.
{
    this->_update_flag(_MOS_RF_OVERFLOW, 0);
}

void
emulator_t::_ins_cmp(uint8_t value)  // CMP: Compare memory with accumulator.
{
    this->_compare(this->_accumulator, value);
}

void
emulator_t::_ins_cpx(uint8_t value)  // CPX: Compare memory with index X.
{
    this->_compare(this->_index_x, value);
}

void
emulator_t::_ins_cpy(uint8_t value)  // CPY: Compare memory with index Y.
{
    this->_compare(this->_index_y, value);
}

uint8_t
emulator_t::_ins_dec(uint8_t value)  // DEC: Decrement memory by one.
{
    value--;

    this->_update_negative(value);
    this->_update_zero(value);

    return (value);
}

void
emulator_t::_ins_dex(void)  // DEX: Decrement index X by one.
{
    this->_index_x--;

    this->_update_negative(this->_index_x);
    this->_update_zero(this->_index_x);
}

void
emulator_t::_ins_dey(void)  // DEY: Decrement index Y by one.
{
    this->_index_y--;

    this->_update_negative(this->_index_y);
    this->_update_zero(this->_index_y);
}

void
emulator_t::_ins_eor(uint8_t value)  // EOR: Exclusive or accumulator with memory.
{
    this->_accumulator ^= value;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

uint8_t
emulator_t::_ins_inc(uint8_t value)  // INC: Increment memory by one.
{
    value++;

    this->_update_negative(value);
    this->_update_zero(value);

    return (value);
}

void
emulator_t::_ins_inx(void)  // INX: Increment index X by one.
{
    this->_index_x++;
    this->_update_negative(this->_index_x);
    this->_update_zero(this->_index_x);
}

void
emulator_t::_ins_iny(void)  // INY: Increment index Y by one.
{
    this->_index_y++;
    this->_update_negative(this->_index_y);
    this->_update_zero(this->_index_y);
}

void
emulator_t::_ins_jmp(uint16_t address)  // JMP: Jump to new location.
{
    this->_program_counter = address;
}

void
emulator_t::_ins_jsr(uint16_t address)  // JSR: Jump to new location saving return address.
{
    this->_push_word(this->_program_counter - 1);
    this->_program_counter = address;
}

void
emulator_t::_ins_lda(uint8_t value)  // LDA: Load accumulator with memory.
{
    this->_accumulator = value;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

void
emulator_t::_ins_ldx(uint8_t value)  // LDX: Load index X with memory.
{
    this->_index_x = value;

    this->_update_negative(this->_index_x);
    this->_update_zero(this->_index_x);
}

void
emulator_t::_ins_ldy(uint8_t value)  // LDY: Load index Y with memory.
{
    this->_index_y = value;

    this->_update_negative(this->_index_y);
    this->_update_zero(this->_index_y);
}

uint8_t
emulator_t::_ins_lsr(uint8_t value)  // LSR: Shift one bit right.
{
    this->_update_flag(_MOS_RF_CARRY, value & 0x01);
    value >>= 1;

    this->_update_zero(value);

    return (value);
}

void
emulator_t::_ins_ora(uint8_t value)  // ORA: Or accumulator with memory.
{
    this->_accumulator |= value;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

void
emulator_t::_ins_pha(void)  // PHA: Push accumulator on stack.
{
    this->_push_byte(this->_accumulator);
}

void
emulator_t::_ins_php(void)  // PHP: Push processor status on stack.
{
    this->_push_byte(this->_status_flag);
}

void
emulator_t::_ins_pla(void)  // PLA: Pull accumulator from stack.
{
    this->_accumulator = this->_pop_byte();
}

void
emulator_t::_ins_plp(void)  // PLP: Pull processor status from stack.
{
    this->_status_flag = this->_pop_byte();
}

uint8_t
emulator_t::_ins_rol(uint8_t value)  // ROL: Rotate one bit left.
{
    uint8_t result;
    result = value << 1;

    if (this->_status_flag & _MOS_RF_CARRY) {
        result |= 0x01;
    }

    this->_update_flag(_MOS_RF_CARRY, value & 0x80);
    this->_update_negative(result);
    this->_update_zero(result);

    return (result);
}

uint8_t
emulator_t::_ins_ror(uint8_t value)  // ROL: Rotate one bit right.
{
    uint8_t result;
    result = value >> 1;

    if (this->_status_flag & _MOS_RF_CARRY) {
        result |= 0x80;
    }

    this->_update_flag(_MOS_RF_CARRY, value & 0x01);
    this->_update_negative(result);
    this->_update_zero(result);

    return (result);
}

void
emulator_t::_ins_rti(void)  // ROL: Return from interrupt.
{
    this->_status_flag = this->_pop_byte();
    this->_program_counter = this->_pop_word();
}

void
emulator_t::_ins_rts(void)  // ROL: Return from subroutine.
{
    this->_program_counter = this->_pop_word() + 1;
}

void
emulator_t::_ins_sbc(uint8_t value)  // SBC: Subtract memory to accumulator with borrow.
{
    uint_least16_t _unsigned;
    int_least16_t _signed;

    _unsigned = this->_accumulator - value - this->_carry();
    this->_update_carry(_unsigned);

    _signed = (int8_t)this->_accumulator - (int8_t)value - this->_carry();
    this->_update_overflow(_signed);

    this->_accumulator = _unsigned;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

void
emulator_t::_ins_sec(void)  // SEC: Set carry flag.
{
    this->_update_flag(_MOS_RF_CARRY, 1);
}

void
emulator_t::_ins_sed(void)  // SED: Set decimal flag.
{
    this->_update_flag(_MOS_RF_DECIMAL, 1);
}

void
emulator_t::_ins_sei(void)  // SEI: Set interrupt disable flag.
{
    this->_update_flag(_MOS_RF_NOINTERRUPT, 1);
}

uint8_t
emulator_t::_ins_sta(void)  // STA: Store accumulator in memory.
{
    return (this->_accumulator);
}

uint8_t
emulator_t::_ins_stx(void)  // STX: Store index X in memory.
{
    return (this->_index_x);
}

uint8_t
emulator_t::_ins_sty(void)  // STY: Store index Y in memory.
{
    return (this->_index_y);
}

void
emulator_t::_ins_tax(void)  // TAX: Transfer accumulator to index X.
{
    this->_index_x = this->_accumulator;

    this->_update_negative(this->_index_x);
    this->_update_zero(this->_index_x);
}

void
emulator_t::_ins_tay(void)  // TAY: Transfer accumulator to index Y.
{
    this->_index_y = this->_accumulator;

    this->_update_negative(this->_index_y);
    this->_update_zero(this->_index_y);
}

void
emulator_t::_ins_tsx(void)  // TSX: Transfer stack pointer to index X.
{
    this->_index_x = this->_stack_pointer;

    this->_update_negative(this->_index_x);
    this->_update_zero(this->_index_x);
}

void
emulator_t::_ins_txa(void)  // TXA: Transfer index X to accumulator.
{
    this->_accumulator = this->_index_x;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}

void
emulator_t::_ins_txs(void)  // TXS: Transfer index X to stack pointer.
{
    this->_stack_pointer = this->_index_x;

    this->_update_negative(this->_stack_pointer);
    this->_update_zero(this->_stack_pointer);
}

void
emulator_t::_ins_tya(void)  // TYA: Transfer index Y to accumulator.
{
    this->_accumulator = this->_index_y;

    this->_update_negative(this->_accumulator);
    this->_update_zero(this->_accumulator);
}