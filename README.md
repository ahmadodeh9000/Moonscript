# MoonScript 🌙

MoonScript is a dynamically typed interpreted programming language built in **C**, inspired by **jlox** from **Crafting Interpreters**.

I built this project while studying language implementation to better understand how programming languages work internally—from lexical analysis all the way to runtime execution. While the architecture is heavily inspired by the tree-walk interpreter approach in *Crafting Interpreters*, MoonScript includes its own syntax choices and implementation decisions.

---

## Features

* Lexical analysis (scanner/tokenizer)
* Recursive descent parser
* Abstract Syntax Tree (AST) generation
* Tree-walk interpreter
* Variables (`let`)
* Arithmetic expressions
* String support
* Boolean logic
* Variable assignment
* Block scoping
* Control flow (`if`, `while`, `for`)
* REPL support
* Script file execution

---

## Example

```moon
let x = 10;
let y = 20;

print(x + y);

if (x < y) {
    print("x is smaller");
}

let i = 0;

while (i < 3) {
    print(i);
    i = i + 1;
}
```

Output:

```text
30
x is smaller
0
1
2
```

---

## Operators

### Arithmetic

```moon
+
-
*
/
%
#
```

`#` is used for exponentiation.

Example:

```moon
print(2 # 5);
```

Output:

```text
32
```

---

### Comparison

```moon
>
>=
<
<=
==
!=
```

---

### Bitwise Operators

```moon
&
|
^
```

---

## Architecture

```text
Source Code
    ↓
Scanner / Lexer
    ↓
Tokens
    ↓
Parser
    ↓
AST
    ↓
Interpreter
    ↓
Runtime Environment
```

---

## Project Structure

```text
src/
├── scanner.c
├── parser.c
├── interpreter.c
├── environment.c
├── expr.c
├── stmt.c
├── value.c
└── main.c
```

---

## Running MoonScript

Clone the repository:

```bash
git clone <your-repo-url>
cd moonscript
```

Build:

```bash
make
```

Run the REPL:

```bash
./moon
```

Run a script file:

```bash
./moon program.ms
```

---

## Motivation

I built MoonScript to gain hands-on experience with interpreters, compilers, parsing, and runtime systems beyond typical academic coursework.

This project helped me better understand:

* language design
* parsing strategies
* AST evaluation
* scope handling
* runtime environments
* debugging low-level memory issues in C

---

## Future Work

* Function declarations and calls
* Native functions
* Improved error reporting
* Better memory management
* Standard library support
* Bytecode VM implementation

---

## Acknowledgments

Special thanks to Robert Nystrom and **Crafting Interpreters** for providing one of the best resources for learning programming language implementation.
