# RISC-V BASED ARCHITECTURE ASSEMBLER

The assembler is designed to translate assembly instruction into equivalent hexadecimal code for RISC-V based processor as designed in [Digital Model of Processor](./Digital%20Model%20of%20Processor.circ). The full list of Valid OP Codes are given in the text file [Valid OP Codes](./Valid%20OP%20Codes.txt) along with the information that would be required to pass for that OP Code for it to be a valid instruction.

> **NOTE**  
> The design of the processor is done by **[Prof. Mazad Zaveri][1]** of **[School of Engineering and Applied Sciences][2], [Ahmedabad University][3]** for the course **CSE302 Computer Organisation and Architecture**. All rights of the design of the processor rests with him along with the **Instruction Set** that is valid for the processor.

## Abbreviations

Following are the list of abbreviations that have / will be used:-

<table>
  <tr>
    <th>Abbreviations</th>
    <th>Refers To</th>
  </tr>
  <tr>
    <td>COA</td>
    <td>Computer Organisation and Architecture</td>
  </tr>
  <tr>
    <td>CSE302</td>
    <td>Refers to COA</td>
  </tr>
  <tr>
    <td>SEAS</td>
    <td>School of Engineering and Applied Sciences</td>
  </tr>
  <tr>
    <td>AU</td>
    <td>Ahmedabd University</td>
  </tr>
  <tr>
    <td>OP Code</td>
    <td>OPeration Code</td>
  </tr>
  <tr>
    <td>Hex</td>
    <td>Hexadecimal</td>
  </tr>
  <tr>
    <td>Reg</td>
    <td>Registers</td>
  </tr>
  <tr>
    <td>DAT</td>
    <td>Data Line</td>
  </tr>
  <tr>
    <td>PC</td>
    <td>Program Counter</td>
  </tr>
</table>

## How to Use

### Pre-installation

1. Ensure you have a `C++` compiler installed and available on system path. If not, you can install `C++` from [here](https://sourceforge.net/projects/mingw/).
2. Ensure you have `Logisim` installed. If not, you can install it from [here](https://sourceforge.net/projects/circuit/#).
   > **Note**
   > It is preferable to use the `GNU C/C++` compiler  
   > `Logisim` requires `Java 5` or later to run.

### Setting Up

- Install the [**Digital Model of Processor**](./Digital%20Model%20of%20Processor.circ).
- For **Assembler Executable File**:
  1. For `Windows x64` architecture users, install this [`file`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/blob/main/New%20Code/Assembler_x64.exe)
  2. For `MacOS` and `Linux` users, install the [`main.cpp`](./New%20Code/main.cpp) file and compile it on your machine to get the `executable file`.
- **Ensure** that the `executable file` is placed in the same folder as your `Assembly Code`.
- **Ensure** that the **name** of the `file` in which the `assembly code` is stored is named `asmcode.txt`.

### Syntax

- **Each** line will end with a `semi-colon (;)`.
- **Each** line will have exactly **1** `semi-colon`;
- **All** `DAT` values will be given in `8-bit hex-code` **only**.
- **All** `Reg` references will be given in `decimal values` **only**.
- The **valid range** for `Reg` reference is **0-15**.
  > **Note**  
  > When referencing `registers`, `R1` will be treated the same as `R01`.  
  > I.E., `R1 <=> R01` and similarly for other decimal values upto 9.  
  > This is not valid for `R10` onwards.
- For the `syntax` of each `OP Code`, as well as how to reference `Regs` and use `DAT`, kindly check [**Valid OP Codes.txt**](./Valid%20OP%20Codes.txt)

[1]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/people-1/mazad-zaveri/
[2]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/
[3]: https://ahduni.edu.in/
