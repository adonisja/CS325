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
- $DEC.CB_{16}$ to binary = 110111101100.11001011_{2}
    Mappings:
    A -> 1010
    B -> 1011
    C -> 1100
    D -> 1101
    E -> 1110
    F -> 1111
- $595_{10} \text{ to hexadecimal} = 253_{16}$  
    $\frac{595}{16} = 37 R 3$  
    $\frac{37}{16} = 2 R 5$  
    $\frac{2}{16} = 0 R 2$   
    Now we write the remainders from the bottom up  
- $110110.1_2$ to decimal  
    $2^5 × 1 = 32$  
    $2^4 × 1 = 16$  
    $2^3 × 0 = 0$  
    $2^2 × 1 = 4$  
    $2^1 x 1 = 2$  
    $2^0 × 0 = 0$  
    $2^{-1} × 1 = 0.5$  
    $\text{Sum them up we get: } 32 + 16 + 4 + 2 + 0.5 = 54.5$


### Question 5:  
#### Convert each of the following numbers into a 10-bit two's complement representation. You must show working
- 360: 0101101000  
    Find the binary of 360:
    360 - 256 ($2^8$) = 104  
    104 - 64 ($2^6$) = 40  
    40 - 32 ($2^5$) = 8  
    8 - 8 ($2^3$) = 0  
    so we place 1s in positions $2^8, 2^6, 2^5 and 2^3$.  
    Res: 101101000 this is 9 bits, so we extend to 10, while considering that the 10th bit is the sign bit.  
    Final Result: 01 0110 1000  
- -240: 1100010000  
    - Find the binary of (positive) 240:  
    240 - 128 ($2^7$) = 112  
    112 - 64 ($2^6$) = 48  
    48 - 32 ($2^5$) = 16  
    16 - 16 ($2^4$) = 0  
    00 1111 0000  
    - Invert the bits from the result (One's Complement)  
    00 1111 0000 => 11 0000 1111
    - Then add 1 to the result.  
    11 0000 1111 + 1 = 11 0001 0000
- 413: 0110011101  
    413 - 256 ($2^8$) = 157  
    157 - 128 ($2^7$) = 29  
    29 - 16 ($2^4$) = 13  
    13 - 8 ($2^3$) = 5  
    5 - 4 ($2^2$) = 1  
    1 - 1 ($2^0$) = 1  
    01 1001 1101

### Question 6: 
#### Convert each of the following binary numbers to their twos-complement negation. Provide the inversion and outcome for each number to receive full credit.
- 1110 1100 0001 011  
    Inversion: 111 0110 0000 1011 => 000 1001 1111 0100  
    Add 1: 000 1001 1111 0100 + 1 = 000 1001 1111 0101
    Outcome: 000 1001 1111 0101   
- 0101 0000 0111 010  
    Inversion: 010 1000 0011 1010 =>  101 0111 1100 0101  
    Add 1: 101 0111 1100 0101 + 1 = 101 0111 1100 0110  
    Outcome: 101 0111 1100 0110  
- 1000 1101 0101 000  
    Inversion: 100 0110 1010 1000 => 011 1001 0101 0111  
    Add 1: 011 1001 0101 0111 + 1 = 011 1001 0101 1000  
    Outcome: 011 1001 0101 1000


### Question 7:
#### Find the solution of each binary expression and state if overflow occurs, if applicable. Provide work to receive full credit.
```
- 011111 + 110111 = 010110 

    Carry: 111111  
            011111  (31 in decimal)  
           +110111  (-9 in decimal)  
           -------  
            010110  

    Since we added a positive and a negative number, overflow is impossible, so we discard the carry of the Most Significant Bit leaving 010110.

- 101011 - 000110 = 100101.  

    To subtract we add the two's complement of the second number:
    Invert 000110 (+6) => 111001  

    Then add 1: 111001 + 1 = 111010 (-6) 

    Then Add together:  
        Carry: 111010
                101011 (-21)  
            +111010 (-6)  
                ------  
                100101  

    Again we discard the extra carry, leaving 100101. Since we added two negative numbers and the result remained negative, then no overflow occurred

- 100101 + 011011 = 000000.

    Carry: 111111
            100101  (-27)
           +011011  (+27)
           -------  
            000000 

    No overflow occurred, added a negative and a positive number of the same magnitude getting a 0 as the result
```

### Question 8:
#### Find the solution of  1010 $\times$ 0011 using Booth's algorithm. Provide work to receive full credit.

**Step 1**: Identify Multiplicand (M) and Multiplier (Q)  
Multiplicand: 1010 
    Because this is a Negative multiplicand, we need to negate 1010 using Two's Complement: Invert 1010 => 0101 +1 = 0110 (+6)
Multiplier: 0011 (+3)  

**Step 2:** Initialize the Table
We'll need four components:
- A (Accumulator): 0000 - The "Running Total" Workspace, starts at 0, and is 4 bits wide to match M
- Q (Multiplier): 0011
- $Q_{-1}$ (Extra Bit): 0
- Count: 4 (Since we are using 4-bit numbers).

| Iteration | Step | A | Q | $Q_{-1}$ | Logic/Action |
| :--- | :--- | :--- | :--- | :--- | :--- |
| Initial | - | 0000 | 0011 | 0 | Start |
| 1 | $Q_0Q_{-1} = 10$ | 0110 | 0011 | 0 | A = A - M (Add - M, which is 0110) |
|  | Shift | 0011 | 0001 | 1 | Arithmetic Right Shift |
| 2 | $Q_0Q_{-1} = 11$ | 0011 | 0001 | 1 | No addition (bits are the same) |
|  | Shift | 0001 | 1000 | 1 | Arithmetic Right Shift |
| 3 |
| 4 |






### Question 9:
#### Perform each logical operator given that lls, lrs, als, ars, lrt and rrt represent logical left shift, logical right shift, arithmetic left shift, arithmetic right shift, left rotate and right rotate, respectively.
- 0001 1011 ars 3
- 1010 1101 lls 4
- 0011 0101 als 4
- 1101 1110 lrt 4
- 1010 1010 rrt 3
- 1000 0101 lrs 5


### Question 10:
### For each Boolean function ($F_i$) in the truth table below, write its product of sums (POS) and sum of products (SOP) representations.
| A | B | C | $F_1$ | $F_2$ | $F_3$ | $F_4$|
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| 0 | 0 | 0 | 0 | 1 | 1 | 0 |
| 0 | 0 | 1 | 0 | 1 | 1 | 0 |
| 0 | 1 | 0 | 0 | 0 | 0 | 1 |
| 0 | 1 | 1 | 1 | 1 | 1 | 1 |
| 1 | 0 | 0 | 1 | 1 | 0 | 1 |
| 1 | 0 | 1 | 1 | 0 | 1 | 1 |
| 1 | 1 | 0 | 0 | 0 | 1 | 0 |
| 1 | 1 | 1 | 1 | 0 | 1 | 0 |



### Question 11:
#### Simplify the Boolean functions from Question 10 using the K-map method. Provide work to receive full credit. 