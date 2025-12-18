# RISC-V BASED ARCHITECTURE ASSEMBLER

## About

The assembler is designed to translate assembly instruction into equivalent hexadecimal code for RISC-V based processor as designed in [Digital Model of Processor](./Digital%20Model%20of%20Processor.circ). The full list of Valid OP Codes are given in the text file [Valid OP Codes](./Valid%20OP%20Codes.txt) along with the information that would be required to pass for that OP Code for it to be a valid instruction.

> **NOTE**  
> The design of the processor is done by **[Prof. Mazad Zaveri][1]** of **[School of Engineering and Applied Sciences][2], [Ahmedabad University][3]** for the course **CSE302 Computer Organization and Architecture**. All rights of the design of the processor rests with him along with the **Instruction Set** that is valid for the processor.

## Abbreviations

Following are the list of abbreviations that have / will be used:-


| Abbreviations | Refers To                                 |
|---------------|-------------------------------------------|
| COA           | Computer Organization and Architecture    |
| CSE302        | Refers to COA                             |
| SEAS          | School of Engineering and Applied Sciences|
| AU            | Ahmedabad University                      |
| OP Code       | Operation Code                            |
| Hex           | Hexadecimal                               |
| Reg           | Registers                                 |
| MEM           | Main Memory                               |
| DAT           | Data Line                                 |
| PC            | Program Counter                           |

## Pre-installation

1. Ensure you have a [`C++ Compiler`](https://sourceforge.net/projects/mingw/) installed and available on system path.
2. Ensure you have [`Logisim`](https://sourceforge.net/projects/circuit/#) installed.  

> **Note**  
> It is preferable to use the `GNU C/C++` compiler.  
> Ensure you have `C++11` or greater (`C++20` is recommended).  
> `Logisim` requires `Java 5` or later to run.

## Setting Up

Visit the [**Releases**](https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler/releases) page to download precompiled executables:

- For **Windows x64**: Download `Assembler_x64.exe`
- For **Linux**: Download `Assembler`
- Or you can download the `zip` file for either of the two.

> **Note**: macOS users must build from source due to platform restrictions on unsigned executables. See "Building from Source" below.

### Post-download (Linux Users)

Before running the downloaded file:

```bash
cd /path/to/download
chmod +x Assembler
```

If the executable does not run on your Linux system, follow the build instructions below.

### Building from Source

> **Note:** Ensure you have `g++` and `make` installed. Use your system's package manager (e.g., sudo apt install build-essential on Debian based distros).

Clone the repo and build the executable using the below given commands.

``` bash
git clone https://github.com/Ashwamedh-14/RISC-V-Based-Architecture-Assembler.git
cd RISC-V-Based-Architecture-Assembler

# For windows user
make windows VERSION=$( cat version.txt )

# For MacOS
make macos VERSION=$( cat version.txt )

# For Linux users
make VERSION=$( cat version.txt )

```

The compiled binary will be located at bin/Assembler


## Using

- Write your `assembly` program in a `text file` and name it `asmcode.txt`.
- Place the executable file in the same directory as `asmcode.txt`.
- Run the executable file.
- Two text files with the name `hexcode.txt` and `bin.txt` should be formed.
- You can upload this file in the `ROM` of your processor.

### MacOS and Linux Users

In case the executable file does not run upon clicking it, especially in the case of `MacOS`, try the following steps:

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
  ./Assembler  
  ```

- `hexcode.txt` and `bin.txt` should now be generated.

### Command Line Arguments

By default, you don't have to explicitly provide the name of the output file for hexadecimal code or binary code, given the input file, `asmcode.txt` is located in the same directory as the executable and is readable. However, if you wish you can specify all the three files through command line arguments, along with a couple of other functionalities.

The general syntax of command line arguments are:

``` Bash
./Assembler -flag <value> ...
```

Here, `-flag` can mean any-one, or more of the following:

- `-i <input_file>`: Input file containing `assembly` code (default: asmcode.txt)
- `-o <output_file>`: Output file in which `hexadecimal` code will be stored (default: hexcode.txt)
- `-b <binary_file>`: Output file in which `binary` code will be stored (default: bin.txt)
- `-f <format_file>`: Intermediate file in which a formatted assembly code will be stored. Generates the formatted file.
- `-c`: Tells `Assembler` to stop execution after formatted file is constructed. Naturally, generates the formatted file.
- `-n`: Tells `Assembler` to not generate `binary` code
- `-h`: Outputs the help message, as given here

Kindly keep the following points in mind

- All flags followed by `<value>` means that they expect you to pass a value to them when invoked.
- The ordering of the flags do not matter
- `-n` will always override the behavior of `-b`.
- `-h` will always override the behavior of rest of the flags. In fact, passing the `-h` flag means the `Assembler` will only output the help section, and will not assemble your source code.
- All I/O files are supposed to be text files.

### **NEW** Format File

In version 2.0 and onwards, before the actual parsing and assembling of your code occurs, the Assembler first formats your source code and puts it into a text file which by default is named `format.txt`.

In this step, the following things happen:
1. All comments are removed.
2. All leading and trailing spaces are removed
3. All blank lines are removed
4. Labels defined are recorded along with their line numbers.

It is important to remember that you should not attempt to modify the generated formatted code. Rather, just modify your source code.

## Syntax

Welcome to the syntax of the assembly language, where we will go over some nitpicky stuff while you might be writing your code.

### Statements

All statements start with a valid `opcode`. This is usually followed by the required `parameters` for that `opcode`. For example, for `opcode` `AND` you might write

``` txt
AND,R12,R1,R2;
```

Notice the use of **commas** to separate the `parameters` of the `opcode`. Use of these commas is **important**, both for successful compilation, and for your understanding. For the correct `syntax` and `parameters` for each `opcode`, kindly refer to [**Valid OP Codes**](./Valid%20OP%20Codes.txt)

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

**NEW: Version 2 and Onwards**

`//` can be used to demarcate the start of a comment. In fact, its preferable this way.

### **NEW:** Labels

From Version 2 and onwards, the assembler supports labelling blocks of code.

For a label to be valid, it must:

1. Not be a `keyword`. 
2. Start with an alphabet or a `_`.
3. Only contain alphanumeric characters or `_`.

Kindly note that labels are case-insensitive. So `label`, `LABEL`, `lABEL`, `Label`, etc., all mean the same thing.

Therefore, some valid labels are:

``` txt
_
_____
label
_1
_start
f9
example_10
```

Keep in mind while the first 2 are also valid label names, why would you want to do that???

Some invalid labels are:

``` txt
9label
first label
label@1
life!@#$
```

To define a label for a block of code, remember the following:

1. The label for the block comes first.
2. The line with the label should end with a colon `:`. **Do not** end it with a semicolon `;`.

Hence, a valid label definition could be:

```
// Some code

label:

// Code for block label

```

> **NOTE**
> A block of code belongs to a label, as long as the assembler doesn't encounter a new label

Labels are only meant to be used in `jmp`, or similar instruction, statements. Therefore, you can write something along the lines of:

``` txt
// Some code

label1:
    // code for label1

label2:
    // code for label2
    jmpz, label1;
    
label3:
    // code for label3
```

Of course, you can still give the direct hexadecimal value of the line you want to jump to in a `jmp` statement, but it is not really encouraged

```txt
// Some code

label1:
    // code for label1

label2:
    // code for label2
    jmpz, label1;           // recommended method
    
label3:
    // code for label3
    jmp, 0A;                // not recommended method
```

However, if for some reason you still want to give raw hexadecimal values to `jmp` statements, kindly then generate a formatted version of your assembly code to check errors against, if any.


### Points to remember

- **All** `opcodes` are `keywords`.
- **All** `regs` are referenced by their decimal values. For example:
  - `R1` is correct
  - `R10` is correct
  - `R01` is correct and also equivalent to `R1`
  - `RA` is incorrect
- **All** `DAT`, `PORT Address`, `MEM Address` values are passed, parsed and taken as `8-bit hexadecimals`.
- `jmp` statements can have both labels and direct hexadecimal values, though the use of labels is encouraged.

## Error Codes

These are the error numbers that you might encounter during runtime. Attached is their meaning as well

- 00: All good
- 01: Input File is not a text file
- 02: Output File is not a text file
- 03: Binary File is not a text file
- 04: Unable to Find or Open the Input File, probably because not available at the specified path, or no read permission.
- 05: Unable to Find or Open the Output File, probably because not available at the specified path, or no write permission.
- 06: Unable to Find or Open the Binary File, probably because not available at the specified path, or no write permission.
- 07: There was error in Arguments Passed to the Program
- 08: There were some Errors in Assembly Code, due to which the equivalent binary file could not be generated.

> **Kindly Note**
>
> Error Codes 01–07 are command line argument based, and are used to verify the correct type of arguments are passed to the program.

## Technical Details

- For human readability, the line numbering will start from 1 in error messages.
- For the programs themselves, the first line will be numbered 0, i.e, hardcoded `jmp`, or similar instructions, must be done keeping in mind the first line is line 0.
- The assembler is **case-insensitive**.
- The processor is a `8-bit` processor, i.e., its `regs` are capable of storing `8 bits`, or a `byte`, of data at a time.
- The processor has a `25-bit` wide `Instruction Memory Bus`, where:
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


## Third-Party Code and Licenses

This project includes third-party code from the [getopt-for-windows](https://github.com/Chunde/getopt-for-windows) project for cross-platform command-line argument parsing.

- [`getopt.c`](./src/getopt.c) and [`getopt.h`](./include/getopt.h) are derived from the GNU C Library and are licensed under the **GNU Lesser General Public License (LGPL) v2.1**.
- See [`COPYING`](./COPYING) for the full license text.

**Note:** The project includes a modified version of [`getopt.h`](./include/getopt.h) (GNU LGPL 2.1). See top of the file for modification detail.

## License

This project is licensed under the **MIT License**. See the [LICENSE](./LICENSE) file for more details.

[1]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/people-1/mazad-zaveri/
[2]: https://ahduni.edu.in/academics/schools-centres/school-of-engineering-and-applied-science/
[3]: https://ahduni.edu.in/
