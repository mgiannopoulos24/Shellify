# Shellify

## Introduction

Shellify is a simple UNIX shell implemented in C, providing basic functionality like executing commands, changing directories, printing the working directory, and echoing input. I started this project to escape tutorial hell and gain more experience actually programming by building a fundamental tool. My goal was not to create a full-fledged shell but rather to get the basics of shell programming and understand its core mechanisms.

## Features

- Execute internal commands like `cd`, `pwd`, `echo`, and `exit`.
- Handle external commands by searching for executables in the user's `~/.local/bin` and `/usr/bin` directories.
- Basic input parsing and tokenization.
- Supports continuous user interaction with a command prompt.

## Getting Started

### Prerequisites

- GCC (GNU Compiler Collection) or any C compiler
- Unix-like operating system (Linux, macOS, etc.)

### Installation

1. Clone the repository:

    ```bash
    git clone https://github.com/Seven719/Shellify.git
    ```

2. Compile the source code:

    ```bash
    cd Shellify
    make
    ```

3. Run the shell:

    ```bash
    ./shellify
    ```

## Usage

Once you run `shellify`, you'll see a prompt `sheLLiFY >>>`. You can then input commands just like in a regular shell. Here are some examples of supported commands:

- `cd <directory>`: Change the current directory.
- `pwd`: Print the current working directory.
- `echo <message>`: Print a message to the console.
- `exit`: Exit the shell.

## License

This project is licensed under the GNU General Public License v3.0. See the [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Feel free to fork the repository and submit pull requests for improvements or bug fixes.
