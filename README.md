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
    <td>MEM</td>
    <td>Main Memory</td>
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

## Pre-installation

1. Ensure you have a [`C++ Compiler`](https://sourceforge.net/projects/mingw/) installed and available on system path.
2. Ensure you have [`Logisim`](https://sourceforge.net/projects/circuit/#) installed.  

> **Note**  
> It is preferable to use the `GNU C/C++` compiler.  
> Ensure you have `C++11` or greater (`C++17` is recommended).  
> `Logisim` requires `Java 5` or later to run.

## Setting Up

- Install the [**Digital Model of Processor**](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/Digital%20Model%20of%20Processor.circ).

- For `Windows x64` architecture users, install this file, [`Assembler_x64.exe`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/bin/Assembler_x64.exe)  

- For `MacOS` users, install this file, [`Assembler_MacOS`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/bin/Assembler_MacOS)  

- For `Linux (Ubuntu)` users, install this file, [`Assembler`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/bin/Assembler)

> **NOTE**
> For `Linux` and `MacOS` users, before being able to run the executable file, run the following commands in the terminal
>
> ``` bash
> # First getting the terminal to the file location
> cd "Path/to/the/directory/in/which/executable/is/located"
>
> # changing permission for the file to make it executable
> chmod +x filename
> ```
>
> For people using `Linux` distros other than `Ubuntu`, do you really need a step by step guide to obtain the executable file for the [`source code`][4]? Just download the [`source code`][4] and compile it on your system, preferably with `C++17` or greater.  

## Using

- Write your `assembly` program in a `text file` and name it `asmcode.txt`.
- Place the executable file in the same folder as `asmcode.txt`.
- Run the executable file.
- A text file with the name `hexcode.txt` should be formed.
- You can upload this file in the ROM of your processor.

### MacOS and Linux Users

In case the executable file that was compiled does not run upon clicking it, especially in the case of `MacOS`, try the following steps:

- Open the `terminal`
- Type in the following command
  
  ``` Bash
  cd "path/to/your/executable"
  ```

- Make sure your `asmcode.txt` is in the same `directory`.
- Run the command below

  ``` Bash
  # For MacOS Users
  ./Assembler_MacOS

  # For Linux (Ubuntu) Users
  ./Assemnler  
  ```

- `hexcode.txt` should now be generated.

## Syntax

Welcome to the syntax of the assembly language, where we will go over some nitpicky stuff while you might be writing your code.

### Statements

All statements start with a valid `opcode`. This is usually followed by the required `parameters` for that `opcode`. For example, for `opcode` `AND` you might write

``` txt
AND,R12,R1,R2;
```

Notice the use of **commas** to seperate the `parameters` of the `opcode`. Use of these commas is **important**, both for successful compilation, as well as for your understanding. For the correct `syntax` and `parameters` for each `opcode`, kindly refer to [**Valid OP Codes**](./Valid%20OP%20Codes.txt)

### Ending a Line

**Each** line ends at the **first** **semicolon** encountered in the line. For example:

``` txt
AND,R12,R1,R2;
```

In this case, the line ends after `R2`, which is expected. However, when we write

``` txt
AND,R12;,R1,R2;
```

The line ends after `R12`.

### Comments

The **first** **semicolon** of the line demarcates the start of comments for that line. For example:

``` txt
AND,R12,R1,R2;        This is a comment
;                     This is also a comment
                      This is not a comment and will be parsed
```

### Blank Lines

Blank lines are skipped while parsing

### Points to remember

- **All** `opcodes` are `keywords`.
- **All** `regs` are referenced by their decimal values. For example:
  - `R1` is correct
  - `R10` is correct
  - `R01` is correct and also equivalent to `R1`
  - `RA` is incorrect
- **All** `DAT`, `PORT Address`, `MEM Address` values are passed, parsed and taken as `8-bit hexadecimals`.

## Technical Details

- The processor is an `8-bit` processor, i.e., its `regs` are capable of storing `8 bits`, or a `byte`, of data at a time.
- The processor has a `25-bit` wide `bus`, where:
  - The first `5 bits` are reserved for `OP Code`.
  - The next `12 bits` are distributed among 3, `4bit` wide `reg address`. The distribution is:
    - The first `4 bits` are reserved for `Rw`, i.e., `Register Write`.
    - The next `4 bits` are reserved for `Rx`, i.e., `Register Read1`.
    - The last `4 bits` are reserved for `Ry`, i.e., `Register Read2`.
  - The last `8 bits` is reserved for `DAT`. It carries `immediate data`, `MEM Address`, `Port Address`.
- The total number of `regs` available are **16**. This means valid `regs` are `R0` - `R15`.
- The `MEM Address` is taken as `8-bit hexadecimal`. This means valid `MEM Address` are `00` - `FF`.
- Data can be read from and written to `MEM` only by `regs`.
- Valid `input` ports are `F1`, `F2`, `F3` and `F4`.
- Valid `ouput` ports are `F8`, `F9`, `FA` and `FB`.
- Specifying any other `input` or `output` port will result in a `logic error`.
- Same as `MEM`, to read / write data to any `I/O` port, you will need to use `regs`.
- The compiled binaries are compatible with `x64` architecture **only**.

[1]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/people-1/mazad-zaveri/
[2]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/
[3]: https://ahduni.edu.in/
[4]: https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/New%20Code/main.cpp
