# Virtual CPU
This my attempt at building a virtual CPU.  This is just for fun and learning.
The entire virtual machine will be implemented in software.

#### Extra Features

##### Multicore/Coprocessors:
* Allow multiple CPUs to be added to the same VM (multicore)
  * Could allow for a FORK instruction

* Allow (smaller, supplemental) CPUs to added to bigger (main) CPUs (coprocessors)
  * Think, creating a small CPU which only does IN/OUT and
  adding that to an otherwise complete CPU

##### Flexible Address/Register Width
* Allow the CPU to switch from 16 to 32 bit mode (for example) and back again
  * Like modern CPUs allow for 32-bit threads in 64-bit mode
  (probably with page attributes)

## JFF (Just For Fun) CPU architecture v0.2

* Register width
  * 16-bit

* Flags:
  * 0 - Carry (CF)
  * 1 - Zero (ZF)
  * 2 - Sign (SF)
  * 3 - Trap (TF)
  * 4 - Interrupt (IF)
  * 5 - Overflow (OF)
  * 6 - User Flag A (AF)
  * 7 - User Flag B (BF)
  * 8 - Not Used
  * 9 - Not Used
  * A - Not Used
  * B - Not Used
  * C - Not Used
  * D - Not Used
  * E - Not Used
  * F - Not Used

* Bit ordering
  * LSB

* Reserved Registers:
  * Instruction Pointer (IP)

* Special Registers:
  * Reserved (R8)
  * Reserved (R9)
  * Reserved (R10)
  * Reserved (R11)
  * Base Pointer (BP/R12)
  * Stack Pointer (SP/R13)
  * Flags (FR/R14)
  * Vector Table (VT/R15)

* General Purpose Registers:
  * R0-R7

* Number of Interrupts
  * 32

* Reset Vector
  * IP=0
  * R8-R11=0
  * BP=0xFFFF
  * SP=0xFFFF
  * Flags=0
  * VT=0xEDFF

* CPU Interrupts:
  * 0 - Double Fault
  * 1 - Bad opcode
  * 2 - Bad operand (eg: memory out of range, invalid register id)
  * 3 - Debug
  * 4 - Divide By Zero
  * 5 - Reserved
  * 6 - Reserved
  * 7 - Reserved

* 0 operand instructions (8-bits)
  * Debugging Options
    * BREAK (INT 3)
    * NOP
  * Hardware Operations
    * HLT
    * RST (Restore Reset Vector)
    * STOP (hard lock cpu)
  * Program Operations
    * IRET (POPA, POPF, RET)
    * RET
  * Stack Operations
    * POPA (POP FR, POP R7-R0)
    * PUSHA (PUSH R0-R7, PUSH FR)

* 1 operand instructions (16-bits)
  * Arithmetic Operations
    * DEC
    * INC
  * Bitwise Operations
    * NOT
  * Program Operations
    * CALL
    * CLR (clear flag)
    * CMF (Complement flag)
    * INT
    * JMP
    * SET (set flag)
  * Stack Operations
    * POP
    * PUSH

* 2 operand instructions (24-bits)
  * Arithmetic Operations
    * ADD
    * DIV
    * MUL
    * SUB
  * Bitwise Operations
    * AND
    * OR
    * ROL
    * ROR
    * SHL
    * SHR
    * XOR
  * Hardware Operations
    * IN
    * OUT
  * Multipurpose Operations
    * MOV
    * XCHG
  * Program Operations
    * CMP (logical SUB)
    * JF (Jump if Flag)
    * TEST (logical AND)

* *Missing Operations*
  * CMPXCHG (CoMPare and eXCHange)
    * Left out for simplicity, this would be the only 3 operand instruction

* Opcode notation
  * The form of notation should be
    * Instruction ([I][M][R],[I][M][R],...) - Opcode [operand-code [...]]
  * Where I, M, and R denote that the positional operand
  may be an immediate value, memory address, or register index
  * Where operand-code (from the table below) explains to the CPU
  how to interpret the operands

* Operand Codes (xx)
  * 00 - Register
  * 01 - Memory
  * 02 - Immediate

* Opcodes
  * BREAK CC
  * NOP 00/FF
  * HLT FC
  * RST FD
  * STOP FE
  * IRET 01
  * RET 02
  * POPA 05
  * PUSHA 06

  * DEC (MR) 0Axx
  * INC (MR) 0Bxx
  * NOT (MR) 0Exx
  * CALL (IMR) 10xx
  * CLR (I) 12xx
  * CMF (I) 14xx
  * INT (IMR) 16xx
  * JMP(Near)(IMR) 20xx
  * JMP(Far)(IMR)  21xx
  * SET (I) 24xx
  * POP (MR) 28xx
  * PUSH (IMR) 29xx

  * ADD (IMR, MR) 2Bxx xx
  * DIV (IMR, MR) 2Exx xx
  * MOD (IMR, MR) 2Fxx xx
  * MUL (IMR, MR) 2Dxx xx
  * SUB (IMR, MR) 2Cxx xx
  * AND (IMR, MR) 30xx xx
  * OR (IMR, MR) 31xx xx
  * ROL (IMR, MR) 35xx xx
  * ROR (IMR, MR) 36xx xx
  * SHL (IMR, MR) 33xx xx
  * SHR (IMR, MR) 34xx xx
  * XOR (IMR, MR) 32xx xx
  * IN (IMR, MR) 3Axx xx
  * OUT (IMR, IMR) 3Bxx xx
  * MOV (IMR, MR) 3Exx xx
  * XCHG (IMR, MR) 3Fxx xx
  * CMP (IMR, IMR) 40xx xx
  * JF (I, IMR) 44xx xx
  * TEST (IMR, IMR) 41xx xx