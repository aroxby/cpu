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

## JFF (Just For Fun) CPU architecture v0.1

* Register width
  * 8-bit

* Flags:
  * 0 - Carry (CF)
  * 1 - Zero (ZF)
  * 2 - Sign (SF)
  * 3 - Trap (TF)
  * 4 - Interrupt (IF)
  * 5 - Overflow (OF)
  * 6 - User Flag A (AF)
  * 7 - User Flag B (BF)

* Bit ordering
  * LSB

* Special Purpose Regisers:
  * IP (S0)
  * BP (S1)
  * SP (S2)
  * FLAGS (S3)
  * Vector Table (S4)
  * Reserved (S5)
  * Reserved (S6)
  * Reserved (S7)

* General Purpose Registers:
  * R0-R8

* CPU Interrupts:
  * 0 - Double Fault
  * 1 - Bad opcode
  * 2 - Bad operand (eg: memory out of range)
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