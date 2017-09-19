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
  * IP (R0)
  * BP (R1)
  * SP (R2)
  * FLAGS (R3)
  * Vector Table (R4)
  * Reserved (R5)
  * Reserved (R6)
  * Reserved (R7)

* General Purpose Registers:
  * R8-RF

* CPU Interrupts:
  * 0 - Double Fault
  * 1 - Bad opcode
  * 2 - Bad operand (eg: memory out of range)
  * 3 - Debug
  * 4 - Divide By Zero
  * 5 - Reserved
  * 6 - Reserved
  * 7 - Reserved

* List of instructions (from a high level):
  * MOV
  * JMP
  * CALL
  * RET
  * IRET (POPA, POPF, RET)
  * ADD
  * SUB
  * DIV
  * MUL
  * PUSH
  * POP
  * PUSHA (PUSH R8-RF)
  * POPA (POP RF-R8)
  * PUSHF
  * POPF
  * SET (set flag)
  * CLR (clear flag)
  * NOP
  * HLT
  * AND
  * OR
  * XOR
  * NOT
  * TEST (logical AND)
  * CMP (logical SUB)
  * JF (Jump if Flag)
  * IN
  * OUT
  * SHL
  * SHR
  * ROL
  * ROR
  * STOP (hard lock cpu)
  * INT
  * BREAK (INT 3)
  * INC
  * DEC
  * XCHG