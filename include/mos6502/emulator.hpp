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

#ifndef _MOS6502_EMULATOR_HPP_
#define _MOS6502_EMULATOR_HPP_

#include <inttypes.h>

namespace mos6502 {

    #define _MOS_RF_CARRY           0x01
    #define _MOS_RF_ZERO            0x02
    #define _MOS_RF_NOINTERRUPT     0x04
    #define _MOS_RF_DECIMAL         0x08
    #define _MOS_RF_BREAK           0x10
    #define _MOS_RF_OVERFLOW        0x40
    #define _MOS_RF_NEGATIVE        0x80

    /**
     * MOS6502 emulator class
     */
    class emulator_t
    {
            /**
             * MOS6502 instruction types
             */
            typedef void (emulator_t::*_ins_noarg_t)(void);
            typedef void (emulator_t::*_ins_load_byte_t)(uint8_t);
            typedef void (emulator_t::*_ins_load_word_t)(uint16_t);
            typedef uint8_t (emulator_t::*_ins_load_store_t)(uint8_t);
            typedef uint8_t (emulator_t::*_ins_store_t)(void);

            /**
             * MOS6502 registers
             */
        private:
            uint16_t        _program_counter;
            uint8_t         _accumulator;
            uint8_t         _index_x;
            uint8_t         _index_y;
            uint8_t         _stack_pointer;
            uint8_t         _status_flag;

            /**
             * Interface
             */
        public:
                            emulator_t(void);
                            ~emulator_t(void) {};
            int             step(void);
            void            interrupt(uint16_t address);

            /**
             * Public memory I/O
             */
        public:
            virtual uint8_t read_byte(uint16_t address) = 0;
            virtual void    write_byte(uint16_t address, uint8_t value) = 0;

            /**
             * Internal memory I/O
             */
        private:
            uint8_t         _read_byte(uint16_t address);
            uint16_t        _read_word(uint16_t address);

            void            _write_byte(uint16_t address, uint8_t value);

            void            _push_byte(uint8_t value);
            void            _push_word(uint16_t value);

            uint8_t         _pop_byte(void);
            uint16_t        _pop_word(void);

            uint8_t         _progress_byte(void);
            uint16_t        _progress_word(void);

            /**
             * Addressing conventions.
             */
        private:
            uint16_t        _addr_abs(void);
            uint16_t        _addr_absx(void);
            uint16_t        _addr_absy(void);
            uint16_t        _addr_xind(void);
            uint16_t        _addr_indy(void);
            uint16_t        _addr_zpg(void);
            uint16_t        _addr_zpgx(void);
            uint16_t        _addr_zpgy(void);

            /**
             * Load instructions with 8bit input.
             */
        private:
            void            _load(_ins_load_byte_t instruction, uint16_t address);
            void            _load_abs(_ins_load_byte_t instruction);
            void            _load_absx(_ins_load_byte_t instruction);
            void            _load_absy(_ins_load_byte_t instruction);
            void            _load_imm(_ins_load_byte_t instruction);
            void            _load_xind(_ins_load_byte_t instruction);
            void            _load_indy(_ins_load_byte_t instruction);
            void            _load_zpg(_ins_load_byte_t instruction);
            void            _load_zpgx(_ins_load_byte_t instruction);
            void            _load_zpgy(_ins_load_byte_t instruction);

            /**
             * Store instructions with 8bit output.
             */
        private:
            void            _store(_ins_store_t instruction, uint16_t address);
            void            _store_abs(_ins_store_t instruction);
            void            _store_absx(_ins_store_t instruction);
            void            _store_absy(_ins_store_t instruction);
            void            _store_xind(_ins_store_t instruction);
            void            _store_indy(_ins_store_t instruction);
            void            _store_zpg(_ins_store_t instruction);
            void            _store_zpgx(_ins_store_t instruction);
            void            _store_zpgy(_ins_store_t instruction);

            /**
             * Load/Store instructions with 8bit input and output.
             */
        private:
            void            _load_store(_ins_load_store_t instruction, uint16_t address);
            void            _load_store_abs(_ins_load_store_t instruction);
            void            _load_store_absx(_ins_load_store_t instruction);
            void            _load_store_acc(_ins_load_store_t instruction);
            void            _load_store_zpg(_ins_load_store_t instruction);
            void            _load_store_zpgx(_ins_load_store_t instruction);

            /**
             * Load instructions with 16bit input.
             */
        private:
            void            _load_word(_ins_load_word_t instruction, uint16_t address);
            void            _load_word_imm(_ins_load_word_t instruction);
            void            _load_word_abs(_ins_load_word_t instruction);

            /**
             * Other instructions
             */
        private:
            void            _branch_on_clear(uint8_t flag, uint8_t value);
            void            _branch_on_set(uint8_t flag, uint8_t value);

            void            _update_flag(uint8_t flag, uint8_t mode);
            void            _update_carry(uint_least16_t value);
            void            _update_overflow(int_least16_t value);
            void            _update_negative(uint8_t value);
            void            _update_zero(uint8_t value);

            void            _noarg(_ins_noarg_t instruction);
            void            _compare(uint8_t value_a, uint8_t value_b);
            uint8_t         _carry(void);

            /**
             * Core instruction set
             */
        private:
            void            _ins_adc(uint8_t value);                // ADC: Add memory to accumulator with carry.
            void            _ins_and(uint8_t value);                // AND: And accumulator with memory.
            uint8_t         _ins_asl(uint8_t value);                // ASL: Arithmetic shift left.
            void            _ins_bit(uint8_t value);                // BIT: Bit test in memory with accumulator.
            void            _ins_bcc(uint8_t value);                // BCC: Branch on carry clear.
            void            _ins_bcs(uint8_t value);                // BCS: Branch on carry set.
            void            _ins_beq(uint8_t value);                // BEQ: Branch on result zero.
            void            _ins_bmi(uint8_t value);                // BMI: Branch on result minus.
            void            _ins_bne(uint8_t value);                // BNE: Branch on result not zero.
            void            _ins_bpl(uint8_t value);                // BPL: Branch on result plus.
            void            _ins_brk(void);                         // BRK: Force break.
            void            _ins_bvc(uint8_t value);                // BVC: Branch on overflow clear.
            void            _ins_bvs(uint8_t value);                // BVS: Branch on overflow set.
            void            _ins_clc(void);                         // CLV: Clear carry flag.
            void            _ins_cld(void);                         // CLD: Clear decimal flag.
            void            _ins_cli(void);                         // CLI: Clear interrupt disable flag.
            void            _ins_clv(void);                         // CLV: Clear overflow flag.
            void            _ins_cmp(uint8_t value);                // CMP: Compare memory with accumulator.
            void            _ins_cpx(uint8_t value);                // CPX: Compare memory with index X.
            void            _ins_cpy(uint8_t value);                // CPY: Compare memory with index Y.
            uint8_t         _ins_dec(uint8_t value);                // DEC: Decrement memory by one.
            void            _ins_dex(void);                         // DEX: Decrement index X by one.
            void            _ins_dey(void);                         // DEY: Decrement index Y by one.
            void            _ins_eor(uint8_t value);                // EOR: Exclusive or accumulator with memory.
            uint8_t         _ins_inc(uint8_t value);                // INC: Increment memory by one.
            void            _ins_inx(void);                         // INX: Increment index X by one.
            void            _ins_iny(void);                         // INY: Increment index Y by one.
            void            _ins_jmp(uint16_t address);             // JMP: Jump to new location.
            void            _ins_jsr(uint16_t address);             // JSR: Jump to new location saving return address.
            void            _ins_lda(uint8_t value);                // LDA: Load accumulator with memory.
            void            _ins_ldx(uint8_t value);                // LDX: Load index X with memory.
            void            _ins_ldy(uint8_t value);                // LDY: Load index Y with memory.
            uint8_t         _ins_lsr(uint8_t value);                // LSR: Shift one bit right.
            void            _ins_ora(uint8_t value);                // ORA: Or accumulator with memory.
            void            _ins_pha(void);                         // PHA: Push accumulator on stack.
            void            _ins_php(void);                         // PHP: Push processor status on stack.
            void            _ins_pla(void);                         // PLA: Pull accumulator from stack.
            void            _ins_plp(void);                         // PLP: Pull processor status from stack.
            uint8_t         _ins_rol(uint8_t value);                // ROL: Rotate one bit left.
            uint8_t         _ins_ror(uint8_t value);                // ROL: Rotate one bit right.
            void            _ins_rti(void);                         // ROL: Return from interrupt.
            void            _ins_rts(void);                         // ROL: Return from subroutine.
            void            _ins_sbc(uint8_t value);                // SBC: Subtract memory to accumulator with borrow.
            void            _ins_sec(void);                         // SEC: Set carry flag.
            void            _ins_sed(void);                         // SED: Set decimal flag.
            void            _ins_sei(void);                         // SEI: Set interrupt disable flag.
            uint8_t         _ins_sta(void);                         // STA: Store accumulator in memory.
            uint8_t         _ins_stx(void);                         // STX: Store index X in memory.
            uint8_t         _ins_sty(void);                         // STY: Store index Y in memory.
            void            _ins_tax(void);                         // TAX: Transfer accumulator to index X.
            void            _ins_tay(void);                         // TAY: Transfer accumulator to index Y.
            void            _ins_tsx(void);                         // TSX: Transfer stack pointer to index X.
            void            _ins_txa(void);                         // TXA: Transfer index X to accumulator.
            void            _ins_txs(void);                         // TXS: Transfer index X to stack pointer.
            void            _ins_tya(void);                         // TYA: Transfer index Y to accumulator.
    };

} // namespace mos6502

#endif // _MOS6502_EMULATOR_HPP_
