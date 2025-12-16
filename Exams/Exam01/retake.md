### Question 1:
#### Provide the name and purpose of each of the seven registers of the IAS computer.
- MBR (Memory Buffer Register): Holds the word being deposited to or extracted from the specified memory location.
- MAR (Memory Access Register): Holds the memory address of the word being written to or read from the MBR.
- IR (Instruction Register): Holds the current instruction opcode.
- IBR (Instruction Buffer Register): This holds the right-hand instruction opcode or the instruction thats next up to be executed.
- PC (Program Counter): Holds the address of the next instruction pair (left and right) to be fetched from memory.
- AC (Accumulator): Temporarily stores the results of ALU functions.
- MQ (Multiplier Quotient): Holds the multiplier during multiplication and the quotient during division. Works together with the AC for long-number operations.

### Question 2:
#### Given that the address of the variables `M` and `N` are 24 and DC, respectively, write a symbolic representation of an IAS program that executes the statement
$$M= \frac{2N - M}{N}$$

Steps: 
- LDA DC: Load N into the Accumulator.
- LSH: Shift Left (Multiplies AC by 2) AC now holds $2N$
- SUB 24: Subtract M from the AC. AC now holds $2N - M$
- DIV DC: Divide AC by N (The Quotient goes into the MQ and the Remainder goes into the AC).
- LMA: Transfer the Quotient from the MQ to the AC (so we can store it).
- STA 24: Store the final result back into memory location 24 (M).

#### Extra Credit: Convert it to machine language.

Symbolic Program:
| Step | Symbolic | Opcode (Hex) | Address (Hex) | Machine Code (Hex) |
| :--- | :--- | :--- | :--- | :--- |
| 1 | `LDA DC` | 01 | 0DC | `010DC` |
| 2 | `LSH` | 14 | 000 | `14000` |
| 3 | `SUB 24` | 06 | 024 | `06024` |
| 4 | `DIV DC` | 0C | 0DC | `0C0DC` |
| 5 | `LMA` | 0A | 000 | `0A000` |
| 6 | `STA 24` | 21 | 024 | `21024` |

### Question 3:
#### Using the list of instruction commands above, trace the code below using a trace table or list.
Memory:
- 000: 0100305002  
    Disassembling the hex string of Memory 000: 01 003 05 002
    - Left side: Opcode 01 (LDA), Address 003
    - Right side: Opcode 05 (ADD), Address 002
- 001: 1500021002  
    Disassembling the hex string of Memory 001: 15 000 21 002
    - Left side: Opcode 15 (RSH), Address 000
    - Right side: Opcode 21 (STA), Address 002
- 002: 0000000006  
    The string of zeroes indicate a memory location so this is data located at $16^0 * 6 = 6$ => Memory Slot 6
- 003: 0000000010  
    The string of zeroes again indicates a memory location so this is data located at $16^1 * 1 = 16$ => Memory Slot 16

#### Step-by-step Trace
| Step | Instruction | Action | Register Status (AC) | Memory at 002 |
| :--- | :--- | :--- | :--- | :--- |
| 0 | Initial State | Before program starts | 0 | 0000000006 |
| 1 | 000 Left: LDA 003 | Load contents of addr 003 into AC | 10 | 6 |
| 2 | 000 Right: ADD 002 | Add contents of addr 002 to AC $(10 + 6)$ | 16 | 6 |
| 3 | 001 (Left): RSH | Divide AC by 2 $fract{16}{2} = 8$ | 8 | 6 |
| 4 | 001 (Right): STA 002 | Store AC value into addr 002 | 8 | 0000000008 |

### Question 4:
#### Convert each of the following numbers to the requested base. You must show work to receive full credit.
- $DEC.CB_{16}$ to binary
- $595_{10}$ to hexadecimal
- $110110.1_2$ to decimal