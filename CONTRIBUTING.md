# Contributions

So, you have found a bug, or want a new feature in the Assembler. Kindly follow the following steps for contributing to the repository.

## Issues and PR

- Kindly check first whether there is an existing issue for the same thing. If yes you can join in its discussion.
- If not, you can open a new issue for the bug fix or feature.
- Kindly write a detailed description for the changes you want to propose, along with why are they required.
- Upon approval, you may start working on it.

## Development

- Create a fork of the repository
- Clone the forked repository into your local machine
- Make changes as required
- Give a clear and meaningful commit message
- Push the changes onto your forked repository
- Submit the PR, attached with the relevant issue.
- Only after the approval, wil your changes be merged.
- Effort has been made to add documentation in the source code itself. However, if you are unable to understand why is a particular line of code is written the way it is, kindly bring it up in your PR or Issue discussion.
- To test the executable, just run in your terminal (**ONLY ON UNIX LIKE SYSTEMS**)
``` bash
make test
```
- To clean the test directory of output just run
``` bash
make clean
```
- To clean the project directory, run
``` bash
make clean_hard
```

## Things to Remember

- Remember to communicate! Be active in discussions when submitting an issue or a PR.
- Be respectful towards others. Indecent behavior will not be tolerated!!!

## Assumptions

TO work on this project, we assume you have decent knowledge on how to write `C++` code. Apart from that, we assume:

1. You are working on a `UNIX` like system.
2. You have `bash` on your system.
3. Though the makefile is capable of making `Windows` executable, the testing logic will work on `UNIX` like system.
4. Error code line numbers start at 1, `JMP` statements line numbers start from 0.
5. Your system is 64-bit

## Windows

People working on `Windows` can still edit and build the `Assembler` normally, but can't test. If you want to test your build, use `WSL` to execute `make test`.

To build on windows, make sure you have the required tools mentioned in the next section, and follow these steps.

1. Open the root directory of the project
2. In the terminal write the following command
```bash
make windows
```
3. An executable `Assembler_x64.exe` should be generated under `bin/`

## Tools

Some tools you should have on your system path

- Make
- GCC
- MinGW 64-bit (For Windows)

**Happy Coding** 😊😊
