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

emulator_t::emulator_t(void)
{
    this->_program_counter = 32768;
    this->_accumulator = 0;
    this->_index_x = 0;
    this->_index_y = 0;
    this->_stack_pointer = 0;
    this->_status_flag = 0;
}

void
emulator_t::interrupt(uint16_t address)
{
    if (!(this->_status_flag | _MOS_RF_NOINTERRUPT)) {
        debug("Interrupt overflow!\n");
        return;
    }

    debug("Interrupt!\n");

    this->_push_word(this->_program_counter);
    this->_push_byte(this->_status_flag);
    this->_update_flag(_MOS_RF_NOINTERRUPT, 1);

    this->_program_counter = this->_read_word(address);
}

int
emulator_t::step(void)
{
    uint8_t instruction;
    instruction = this->_progress_byte();

    debug("Executing at %hx\n", this->_program_counter);

    switch (instruction) {
        case 0x00:
            debug("0x00: NOARG(BRK)\n");
            this->_noarg(&emulator_t::_ins_brk);
            break;

        case 0x01:
            debug("0x01: LOAD_XIND(ORA)\n");
            this->_load_xind(&emulator_t::_ins_ora);
            break;

        case 0x05:
            debug("0x05: LOAD_ZPG(ORA)\n");
            this->_load_zpg(&emulator_t::_ins_ora);
            break;

        case 0x06:
            debug("0x06: LOAD_STORE_ZPG(ASL)\n");
            this->_load_store_zpg(&emulator_t::_ins_asl);
            break;

        case 0x08:
            debug("0x08: NOARG(PHP)\n");
            this->_noarg(&emulator_t::_ins_php);
            break;

        case 0x09:
            debug("0x09: LOAD_IMM(ORA)\n");
            this->_load_imm(&emulator_t::_ins_ora);
            break;

        case 0x0a:
            debug("0x0a: LOAD_STORE_ACC(ASL)\n");
            this->_load_store_acc(&emulator_t::_ins_asl);
            break;

        case 0x0d:
            debug("0x0d: LOAD_ABS(ORA)\n");
            this->_load_abs(&emulator_t::_ins_ora);
            break;

        case 0x0e:
            debug("0x0e: LOAD_STORE_ABS(ASL)\n");
            this->_load_store_abs(&emulator_t::_ins_asl);
            break;

        case 0x10:
            debug("0x10: LOAD_IMM(BPL)\n");
            this->_load_imm(&emulator_t::_ins_bpl);
            break;

        case 0x11:
            debug("0x11: LOAD_INDY(ORA)\n");
            this->_load_indy(&emulator_t::_ins_ora);
            break;

        case 0x15:
            debug("0x15: LOAD_ZPGX(ORA)\n");
            this->_load_zpgx(&emulator_t::_ins_ora);
            break;

        case 0x16:
            debug("0x16: LOAD_STORE_ZPGX(ASL)\n");
            this->_load_store_zpgx(&emulator_t::_ins_asl);
            break;

        case 0x18:
            debug("0x18: NOARG(CLC)\n");
            this->_noarg(&emulator_t::_ins_clc);
            break;

        case 0x19:
            debug("0x19: LOAD_ABSY(ORA)\n");
            this->_load_absy(&emulator_t::_ins_ora);
            break;

        case 0x1d:
            debug("0x1d: LOAD_ABSX(ORA)\n");
            this->_load_absx(&emulator_t::_ins_ora);
            break;

        case 0x1e:
            debug("0x1e: LOAD_STORE_ABSX(ASL)\n");
            this->_load_store_absx(&emulator_t::_ins_asl);
            break;

        case 0x20:
            debug("0x20: LOAD16_IMM(JSR)\n");
            this->_load_word_imm(&emulator_t::_ins_jsr);
            break;

        case 0x21:
            debug("0x21: LOAD_XIND(AND)\n");
            this->_load_xind(&emulator_t::_ins_and);
            break;

        case 0x24:
            debug("0x24: LOAD_ZPG(BIT)\n");
            this->_load_zpg(&emulator_t::_ins_bit);
            break;

        case 0x25:
            debug("0x25: LOAD_ZPG(AND)\n");
            this->_load_zpg(&emulator_t::_ins_and);
            break;

        case 0x26:
            debug("0x26: LOAD_STORE_ZPG(ROL)\n");
            this->_load_store_zpg(&emulator_t::_ins_rol);
            break;

        case 0x28:
            debug("0x28: NOARG(PLP)\n");
            this->_noarg(&emulator_t::_ins_plp);
            break;

        case 0x29:
            debug("0x29: LOAD_IMM(AND)\n");
            this->_load_imm(&emulator_t::_ins_and);
            break;

        case 0x2a:
            debug("0x2a: LOAD_STORE_ACC(ROL)\n");
            this->_load_store_acc(&emulator_t::_ins_rol);
            break;

        case 0x2c:
            debug("0x2c: LOAD_ABS(BIT)\n");
            this->_load_abs(&emulator_t::_ins_bit);
            break;

        case 0x2d:
            debug("0x2d: LOAD_ABS(AND)\n");
            this->_load_abs(&emulator_t::_ins_and);
            break;

        case 0x2e:
            debug("0x2e: LOAD_STORE_ABS(ROL)\n");
            this->_load_store_abs(&emulator_t::_ins_rol);
            break;

        case 0x30:
            debug("0x30: LOAD_IMM(BMI)\n");
            this->_load_imm(&emulator_t::_ins_bmi);
            break;

        case 0x31:
            debug("0x31: LOAD_INDY(AND)\n");
            this->_load_indy(&emulator_t::_ins_and);
            break;

        case 0x35:
            debug("0x35: LOAD_ZPGX(AND)\n");
            this->_load_zpgx(&emulator_t::_ins_and);
            break;

        case 0x36:
            debug("0x36: LOAD_STORE_ZPGX(ROL)\n");
            this->_load_store_zpgx(&emulator_t::_ins_rol);
            break;

        case 0x38:
            debug("0x38: NOARG(SEC)\n");
            this->_noarg(&emulator_t::_ins_sec);
            break;

        case 0x39:
            debug("0x39: LOAD_ABSY(AND)\n");
            this->_load_absy(&emulator_t::_ins_and);
            break;

        case 0x3d:
            debug("0x3d: LOAD_ABSX(AND)\n");
            this->_load_absx(&emulator_t::_ins_and);
            break;

        case 0x3e:
            debug("0x3e: LOAD_STORE_ABSX(ROL)\n");
            this->_load_store_absx(&emulator_t::_ins_rol);
            break;

        case 0x40:
            debug("0x40: NOARG(RTI)\n");
            this->_noarg(&emulator_t::_ins_rti);
            break;

        case 0x41:
            debug("0x41: LOAD_XIND(EOR)\n");
            this->_load_xind(&emulator_t::_ins_eor);
            break;

        case 0x45:
            debug("0x45: LOAD_ZPG(EOR)\n");
            this->_load_zpg(&emulator_t::_ins_eor);
            break;

        case 0x46:
            debug("0x46: LOAD_STORE_ZPG(LSR)\n");
            this->_load_store_zpg(&emulator_t::_ins_lsr);
            break;

        case 0x48:
            debug("0x48: NOARG(PHA)\n");
            this->_noarg(&emulator_t::_ins_pha);
            break;

        case 0x49:
            debug("0x49: LOAD_IMM(EOR)\n");
            this->_load_imm(&emulator_t::_ins_eor);
            break;

        case 0x4a:
            debug("0x4a: LOAD_STORE_ACC(LSR)\n");
            this->_load_store_acc(&emulator_t::_ins_lsr);
            break;

        case 0x4c:
            debug("0x4c: LOAD16_IMM(JMP)\n");
            this->_load_word_imm(&emulator_t::_ins_jmp);
            break;

        case 0x4d:
            debug("0x4d: LOAD_ABS(EOR)\n");
            this->_load_abs(&emulator_t::_ins_eor);
            break;

        case 0x4e:
            debug("0x4e: LOAD_STORE_ABS(LSR)\n");
            this->_load_store_abs(&emulator_t::_ins_lsr);
            break;

        case 0x50:
            debug("0x50: LOAD_IMM(BVC)\n");
            this->_load_imm(&emulator_t::_ins_bvc);
            break;

        case 0x51:
            debug("0x51: LOAD_INDY(EOR)\n");
            this->_load_indy(&emulator_t::_ins_eor);
            break;

        case 0x55:
            debug("0x55: LOAD_ZPGX(EOR)\n");
            this->_load_zpgx(&emulator_t::_ins_eor);
            break;

        case 0x56:
            debug("0x56: LOAD_STORE_ZPGX(LSR)\n");
            this->_load_store_zpgx(&emulator_t::_ins_lsr);
            break;

        case 0x58:
            debug("0x58: NOARG(CLI)\n");
            this->_noarg(&emulator_t::_ins_cli);
            break;

        case 0x59:
            debug("0x59: LOAD_ABSY(EOR)\n");
            this->_load_absy(&emulator_t::_ins_eor);
            break;

        case 0x5d:
            debug("0x5d: LOAD_ABSX(EOR)\n");
            this->_load_absx(&emulator_t::_ins_eor);
            break;

        case 0x5e:
            debug("0x5e: LOAD_STORE_ABSX(LSR)\n");
            this->_load_store_absx(&emulator_t::_ins_lsr);
            break;

        case 0x60:
            debug("0x60: NOARG(RTS)\n");
            this->_noarg(&emulator_t::_ins_rts);
            break;

        case 0x61:
            debug("0x61: LOAD_XIND(ADC)\n");
            this->_load_xind(&emulator_t::_ins_adc);
            break;

        case 0x65:
            debug("0x65: LOAD_ZPG(ADC)\n");
            this->_load_zpg(&emulator_t::_ins_adc);
            break;

        case 0x66:
            debug("0x66: LOAD_STORE_ZPG(ROR)\n");
            this->_load_store_zpg(&emulator_t::_ins_ror);
            break;

        case 0x68:
            debug("0x68: NOARG(PLA)\n");
            this->_noarg(&emulator_t::_ins_pla);
            break;

        case 0x69:
            debug("0x69: LOAD_IMM(ADC)\n");
            this->_load_imm(&emulator_t::_ins_adc);
            break;

        case 0x6a:
            debug("0x6a: LOAD_STORE_ACC(ROR)\n");
            this->_load_store_acc(&emulator_t::_ins_ror);
            break;

        case 0x6c:
            debug("0x6c: LOAD16_ABS(JMP)\n");
            this->_load_word_abs(&emulator_t::_ins_jmp);
            break;

        case 0x6d:
            debug("0x6d: LOAD_ABS(ADC)\n");
            this->_load_abs(&emulator_t::_ins_adc);
            break;

        case 0x6e:
            debug("0x6e: LOAD_STORE_ABS(ROR)\n");
            this->_load_store_abs(&emulator_t::_ins_ror);
            break;

        case 0x70:
            debug("0x70: LOAD_IMM(BVS)\n");
            this->_load_imm(&emulator_t::_ins_bvs);
            break;

        case 0x71:
            debug("0x71: LOAD_INDY(ADC)\n");
            this->_load_indy(&emulator_t::_ins_adc);
            break;

        case 0x75:
            debug("0x75: LOAD_ZPGX(ADC)\n");
            this->_load_zpgx(&emulator_t::_ins_adc);
            break;

        case 0x76:
            debug("0x76: LOAD_STORE_ZPGX(ROR)\n");
            this->_load_store_zpgx(&emulator_t::_ins_ror);
            break;

        case 0x78:
            debug("0x78: NOARG(SEI)\n");
            this->_noarg(&emulator_t::_ins_sei);
            break;

        case 0x79:
            debug("0x79: LOAD_ABSY(ADC)\n");
            this->_load_absy(&emulator_t::_ins_adc);
            break;

        case 0x7d:
            debug("0x7d: LOAD_ABSX(ADC)\n");
            this->_load_absx(&emulator_t::_ins_adc);
            break;

        case 0x7e:
            debug("0x7e: LOAD_STORE_ABSX(ROR)\n");
            this->_load_store_absx(&emulator_t::_ins_ror);
            break;

        case 0x81:
            debug("0x81: STORE_XIND(STA)\n");
            this->_store_xind(&emulator_t::_ins_sta);
            break;

        case 0x84:
            debug("0x84: STORE_ZPG(STY)\n");
            this->_store_zpg(&emulator_t::_ins_sty);
            break;

        case 0x85:
            debug("0x85: STORE_ZPG(STA)\n");
            this->_store_zpg(&emulator_t::_ins_sta);
            break;

        case 0x86:
            debug("0x86: STORE_ZPG(STX)\n");
            this->_store_zpg(&emulator_t::_ins_stx);
            break;

        case 0x88:
            debug("0x88: NOARG(DEY)\n");
            this->_noarg(&emulator_t::_ins_dey);
            break;

        case 0x8a:
            debug("0x8a: NOARG(TXA)\n");
            this->_noarg(&emulator_t::_ins_txa);
            break;

        case 0x8c:
            debug("0x8c: STORE_ABS(STY)\n");
            this->_store_abs(&emulator_t::_ins_sty);
            break;

        case 0x8d:
            debug("0x8d: STORE_ABS(STA)\n");
            this->_store_abs(&emulator_t::_ins_sta);
            break;

        case 0x8e:
            debug("0x8e: STORE_ABS(STX)\n");
            this->_store_abs(&emulator_t::_ins_stx);
            break;

        case 0x90:
            debug("0x90: LOAD_IMM(BCC)\n");
            this->_load_imm(&emulator_t::_ins_bcc);
            break;

        case 0x91:
            debug("0x91: STORE_INDY(STA)\n");
            this->_store_indy(&emulator_t::_ins_sta);
            break;

        case 0x94:
            debug("0x94: STORE_ZPGX(STY)\n");
            this->_store_zpgx(&emulator_t::_ins_sty);
            break;

        case 0x95:
            debug("0x95: STORE_ZPGX(STA)\n");
            this->_store_zpgx(&emulator_t::_ins_sta);
            break;

        case 0x96:
            debug("0x96: STORE_ZPGY(STX)\n");
            this->_store_zpgy(&emulator_t::_ins_stx);
            break;

        case 0x98:
            debug("0x98: NOARG(TYA)\n");
            this->_noarg(&emulator_t::_ins_tya);
            break;

        case 0x99:
            debug("0x99: STORE_ABSY(STA)\n");
            this->_store_absy(&emulator_t::_ins_sta);
            break;

        case 0x9a:
            debug("0x9a: NOARG(TXS)\n");
            this->_noarg(&emulator_t::_ins_txs);
            break;

        case 0x9d:
            debug("0x9d: STORE_ABSX(STA)\n");
            this->_store_absx(&emulator_t::_ins_sta);
            break;

        case 0xa0:
            debug("0xa0: LOAD_IMM(LDY)\n");
            this->_load_imm(&emulator_t::_ins_ldy);
            break;

        case 0xa1:
            debug("0xa1: LOAD_XIND(LDA)\n");
            this->_load_xind(&emulator_t::_ins_lda);
            break;

        case 0xa2:
            debug("0xa2: LOAD_IMM(LDX)\n");
            this->_load_imm(&emulator_t::_ins_ldx);
            break;

        case 0xa4:
            debug("0xa4: LOAD_ZPG(LDY)\n");
            this->_load_zpg(&emulator_t::_ins_ldy);
            break;

        case 0xa5:
            debug("0xa5: LOAD_ZPG(LDA)\n");
            this->_load_zpg(&emulator_t::_ins_lda);
            break;

        case 0xa6:
            debug("0xa6: LOAD_ZPG(LDX)\n");
            this->_load_zpg(&emulator_t::_ins_ldx);
            break;

        case 0xa8:
            debug("0xa8: NOARG(TAY)\n");
            this->_noarg(&emulator_t::_ins_tay);
            break;

        case 0xa9:
            debug("0xa9: LOAD_IMM(LDA)\n");
            this->_load_imm(&emulator_t::_ins_lda);
            break;

        case 0xaa:
            debug("0xaa: NOARG(TAX)\n");
            this->_noarg(&emulator_t::_ins_tax);
            break;

        case 0xac:
            debug("0xac: LOAD_ABS(LDY)\n");
            this->_load_abs(&emulator_t::_ins_ldy);
            break;

        case 0xad:
            debug("0xad: LOAD_ABS(LDA)\n");
            this->_load_abs(&emulator_t::_ins_lda);
            break;

        case 0xae:
            debug("0xae: LOAD_ABS(LDX)\n");
            this->_load_abs(&emulator_t::_ins_ldx);
            break;

        case 0xb0:
            debug("0xb0: LOAD_IMM(BCS)\n");
            this->_load_imm(&emulator_t::_ins_bcs);
            break;

        case 0xb1:
            debug("0xb1: LOAD_INDY(LDA)\n");
            this->_load_indy(&emulator_t::_ins_lda);
            break;

        case 0xb4:
            debug("0xb4: LOAD_ZPGX(LDY)\n");
            this->_load_zpgx(&emulator_t::_ins_ldy);
            break;

        case 0xb5:
            debug("0xb5: LOAD_ZPGX(LDA)\n");
            this->_load_zpgx(&emulator_t::_ins_lda);
            break;

        case 0xb6:
            debug("0xb6: LOAD_ZPGY(LDX)\n");
            this->_load_zpgy(&emulator_t::_ins_ldx);
            break;

        case 0xb8:
            debug("0xb8: NOARG(CLV)\n");
            this->_noarg(&emulator_t::_ins_clv);
            break;

        case 0xb9:
            debug("0xb9: LOAD_ABSX(LDA)\n");
            this->_load_absx(&emulator_t::_ins_lda);
            break;

        case 0xba:
            debug("0xba: NOARG(TSX)\n");
            this->_noarg(&emulator_t::_ins_tsx);
            break;

        case 0xbc:
            debug("0xbc: LOAD_ABSX(LDY)\n");
            this->_load_absx(&emulator_t::_ins_ldy);
            break;

        case 0xbd:
            debug("0xbd: LOAD_ABSX(LDA)\n");
            this->_load_absx(&emulator_t::_ins_lda);
            break;

        case 0xbe:
            debug("0xbe: LOAD_ABSY(LDX)\n");
            this->_load_absy(&emulator_t::_ins_ldx);
            break;

        case 0xc0:
            debug("0xc0: LOAD_IMM(CPY)\n");
            this->_load_imm(&emulator_t::_ins_cpy);
            break;

        case 0xc1:
            debug("0xc1: LOAD_XIND(CMP)\n");
            this->_load_xind(&emulator_t::_ins_cmp);
            break;

        case 0xc4:
            debug("0xc4: LOAD_ZPG(CPY)\n");
            this->_load_zpg(&emulator_t::_ins_cpy);
            break;

        case 0xc5:
            debug("0xc5: LOAD_ZPG(CMP)\n");
            this->_load_zpg(&emulator_t::_ins_cmp);
            break;

        case 0xc6:
            debug("0xc6: LOAD_STORE_ZPG(DEC)\n");
            this->_load_store_zpg(&emulator_t::_ins_dec);
            break;

        case 0xc8:
            debug("0xc8: NOARG(INY)\n");
            this->_noarg(&emulator_t::_ins_iny);
            break;

        case 0xc9:
            debug("0xc9: LOAD_IMM(CMP)\n");
            this->_load_imm(&emulator_t::_ins_cmp);
            break;

        case 0xca:
            debug("0xca: NOARG(DEX)\n");
            this->_noarg(&emulator_t::_ins_dex);
            break;

        case 0xcc:
            debug("0xcc: LOAD_ABS(CPY)\n");
            this->_load_abs(&emulator_t::_ins_cpy);
            break;

        case 0xcd:
            debug("0xcd: LOAD_ABS(CMP)\n");
            this->_load_abs(&emulator_t::_ins_cmp);
            break;

        case 0xce:
            debug("0xce: LOAD_STORE_ABS(DEC)\n");
            this->_load_store_abs(&emulator_t::_ins_dec);
            break;

        case 0xd0:
            debug("0xd0: LOAD_IMM(BNE)\n");
            this->_load_imm(&emulator_t::_ins_bne);
            break;

        case 0xd1:
            debug("0xd1: LOAD_INDY(CMP)\n");
            this->_load_indy(&emulator_t::_ins_cmp);
            break;

        case 0xd5:
            debug("0xd5: LOAD_ZPGX(CMP)\n");
            this->_load_zpgx(&emulator_t::_ins_cmp);
            break;

        case 0xd6:
            debug("0xd6: LOAD_STORE_ZPGX(DEC)\n");
            this->_load_store_zpgx(&emulator_t::_ins_dec);
            break;

        case 0xd8:
            debug("0xd8: /* NOARG(CLD) */\n");
            /* noarg(cld) */;
            break;

        case 0xd9:
            debug("0xd9: LOAD_ABSY(CMP)\n");
            this->_load_absy(&emulator_t::_ins_cmp);
            break;

        case 0xdd:
            debug("0xdd: LOAD_ABSX(CMP)\n");
            this->_load_absx(&emulator_t::_ins_cmp);
            break;

        case 0xde:
            debug("0xde: LOAD_STORE_ABSX(DEC)\n");
            this->_load_store_absx(&emulator_t::_ins_dec);
            break;

        case 0xe0:
            debug("0xe0: LOAD_IMM(CPX)\n");
            this->_load_imm(&emulator_t::_ins_cpx);
            break;

        case 0xe1:
            debug("0xe1: LOAD_XIND(SBC)\n");
            this->_load_xind(&emulator_t::_ins_sbc);
            break;

        case 0xe4:
            debug("0xe4: LOAD_ZPG(CPX)\n");
            this->_load_zpg(&emulator_t::_ins_cpx);
            break;

        case 0xe5:
            debug("0xe5: LOAD_ZPG(SBC)\n");
            this->_load_zpg(&emulator_t::_ins_sbc);
            break;

        case 0xe6:
            debug("0xe6: LOAD_STORE_ZPG(INC)\n");
            this->_load_store_zpg(&emulator_t::_ins_inc);
            break;

        case 0xe8:
            debug("0xe8: NOARG(INX)\n");
            this->_noarg(&emulator_t::_ins_inx);
            break;

        case 0xe9:
            debug("0xe9: LOAD_IMM(SBC)\n");
            this->_load_imm(&emulator_t::_ins_sbc);
            break;

        case 0xea:
            debug("0xea: NOP\n");
            /* nop */;
            break;

        case 0xec:
            debug("0xec: LOAD_ABS(CPX)\n");
            this->_load_abs(&emulator_t::_ins_cpx);
            break;

        case 0xed:
            debug("0xed: LOAD_ABS(SBC)\n");
            this->_load_abs(&emulator_t::_ins_sbc);
            break;

        case 0xee:
            debug("0xee: LOAD_STORE_ABS(INC)\n");
            this->_load_store_abs(&emulator_t::_ins_inc);
            break;

        case 0xf0:
            debug("0xf0: LOAD_IMM(BEQ)\n");
            this->_load_imm(&emulator_t::_ins_beq);
            break;

        case 0xf1:
            debug("0xf1: LOAD_INDY(SBC)\n");
            this->_load_indy(&emulator_t::_ins_sbc);
            break;

        case 0xf5:
            debug("0xf5: LOAD_ZPGX(SBC)\n");
            this->_load_zpgx(&emulator_t::_ins_sbc);
            break;

        case 0xf6:
            debug("0xf6: LOAD_STORE_ZPGX(INC)\n");
            this->_load_store_zpgx(&emulator_t::_ins_inc);
            break;

        case 0xf9:
            debug("0xf9: LOAD_ABSY(SBC)\n");
            this->_load_absy(&emulator_t::_ins_sbc);
            break;

        case 0xfd:
            debug("0xfd: LOAD_ABSX(SBC)\n");
            this->_load_absx(&emulator_t::_ins_sbc);
            break;

        case 0xfe:
            debug("0xfe: LOAD_STORE_ABSX(INC)\n");
            this->_load_store_absx(&emulator_t::_ins_inc);
            break;

        default:
            debug("Got invalid instruction %hhx\n", instruction);
            return (-1);
    }

    return (0);
}