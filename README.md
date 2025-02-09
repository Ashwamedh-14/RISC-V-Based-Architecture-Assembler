# RISC-V BASED ARCHITECTURE ASSEMBLER

The assembler is designed to translate assembly instruction into equivalent hexadecimal code for RISC-V based processor as designed in [Digital Model of Processor](./Digital%20Model%20of%20Processor.circ). The full list of Valid OP Codes are given in the text file [Valid OP Codes](./Valid%20OP%20Codes.txt) along with the information that would be required to pass for that OP Code for it to be a valid instruction.

> **NOTE**  
> The design of the processor is done by **[Prof. Mazad Zaveri][1]** of **[School of Engineering and Applied Sciences][2], [Ahmedabad University][3]** for the course **CSE302 Computer Organisation and Architecture**. All rights of the design of the processor rests with him along with the **Instruction Set** that is valid for the processor.

## Abbreviations

Following are the list of abbreviations that will be used:-

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
  1. For `Windows x64` Architecture install this [`file`](./New%20Code/Assembler_x64.exe)
- **Ensure** that the `executable file` is placed in the same folder as your `Assembly Code`.
- **Ensure** that the name of the `file` in which the `assembly code` is stored is named `asmcode.txt`.

[1]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/people-1/mazad-zaveri/
[2]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/
[3]: https://ahduni.edu.in/
