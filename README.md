# Virtual CPU
This my attempt at building a virtual CPU.  This is just for fun and learning.
The entire virtual machine will be implemented in sofware.

#### Extra Features

##### Multicore/Coprocessors:
*Allow multiple CPUs to be added to the same VM (multicore)
  * Could allow for a FORK instruction

*Allow (smaller, supplemental) CPUs to added to bigger (main) CPUs (coprocessors)
  * Think, creating a small CPU which only does IN/OUT and
  adding that to an otherwise complete CPU

##### Feature idea: Flexible address/register width
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

* Reserved Regisers:
  * Instruction Pointer (IP)

* Special Regisers:
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
  * RET
  * IRET (POPA, POPF, RET)
  * PUSHA (PUSH R8-RF)
  * POPA (POP RF-R8)
  * PUSHF
  * POPF
  * NOP
  * HLT
  * STOP (hard lock cpu)
  * BREAK (INT 3)

* 1 operand instructions (16-bits)
  * JMP
  * CALL
  * PUSH
  * POP
  * SET (set flag)
  * CLR (clear flag)
  * NOT
  * TEST (logical AND)
  * CMP (logical SUB)
  * INT
  * INC
  * DEC

* 2 operand instructions (24-bits)
  * MOV
  * ADD
  * SUB
  * DIV
  * MUL
  * AND
  * OR
  * XOR
  * JF (Jump if Flag)
  * IN
  * OUT
  * SHL
  * SHR
  * ROL
  * ROR
  * XCHG