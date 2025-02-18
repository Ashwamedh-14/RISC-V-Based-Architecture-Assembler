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

## Pre-installation

1. Ensure you have a [`C++ Compiler`](https://sourceforge.net/projects/mingw/) installed and available on system path.
2. Ensure you have [`Logisim`](https://sourceforge.net/projects/circuit/#) installed.  

> **Note**  
> It is preferable to use the `GNU C/C++` compiler.  
> Ensure you have `C++11` or greater (`C++17` is recommended).  
> `Logisim` requires `Java 5` or later to run.

## Setting Up

- Install the [**Digital Model of Processor**](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/Digital%20Model%20of%20Processor.circ).
- For `MacOS` and `Linux` users, install the [`main.cpp`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/New%20Code/main.cpp)

### Windows

For `Windows x64` architecture users, install this file [`Assembler_x64.exe`](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/raw/refs/heads/main/New%20Code/Assembler_x64.exe)

### MacOS and Linux

For `MacOS` and `Linux` user, ensure you have `C++11 Compiler` or greater installed.  
If you are unsure which version you have, run the following command in your terminal:

``` Bash
g++ --version
```

If the above command throws an error, its probably because the `GCC` is not installed on your system, and its most likely using `Clang`. In that case, try the following command:

``` Bash
clang++ --version
```

After you have downloaded the file, open the terminal and run the following command:

``` Bash
cd "Path/to/downloaded/file"
```

To compile the program run the following command:

```Bash
# For GCC users
g++ main.cpp -o Assembler

# For Clang users
clang++ -std=c++11 main.cpp -o Assembler          # Put the latest version of c++ instead of c++11
```

> **NOTE**  
> **Ensure** that the `executable file` is placed in the same folder as your `Assembly Code`.  
> **Ensure** that the **name** of the `file` in which the `assembly code` is stored is named `asmcode.txt`.

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
  ./Assembler
  ```

- `hexcode.txt` should now be generated.

## Syntax

[1]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/people-1/mazad-zaveri/
[2]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/
[3]: https://ahduni.edu.in/
