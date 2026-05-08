# MoonScript 🌙

MoonScript is a dynamically typed interpreted programming language built in **C**, inspired by **jlox** from *Crafting Interpreters*.

I built this project while studying language implementation to better understand how programming languages work internally — from lexical analysis all the way to runtime execution with closures.

---

## Features

- Lexical analysis (scanner/tokenizer)
- Recursive descent parser
- Abstract Syntax Tree (AST)
- Tree-walk interpreter
- Variables (`let`) with lexical scoping
- All basic types — number, string, bool, nil
- Arithmetic, comparison, logical, bitwise operators
- Control flow — `if/else`, `while`, `for`
- First-class functions (`fn`)
- Closures with reference-counted environments
- Return statements (`ret`)
- Recursion
- Higher-order functions
- Block scoping and shadowing
- REPL and script file execution

---

## Example

```moon
fn makeCounter() {
    let i = 0;
    fn count() {
        i = i + 1;
        print(i);
    }
    ret count;
}

let c = makeCounter();
c();    // 1
c();    // 2
c();    // 3
```

```moon
fn fib(n) {
    if (n <= 1) ret n;
    ret fib(n - 2) + fib(n - 1);
}

print(fib(10));    // 55
```

```moon
let x = 10;
let y = 20;
print(x + y);       // 30

if (x < y) {
    print("x is smaller");
}

for (let i = 0; i < 3; i = i + 1) {
    print(i);
}
```

---

## Operators

| Category   | Operators              |
|------------|------------------------|
| Arithmetic | `+ - * / %`            |
| Power      | `#` — `2 # 8` = 256   |
| Comparison | `== != < <= > >=`      |
| Logical    | `and or !`             |
| Bitwise    | `& \| ^`               |

---

## Architecture

```
Source Code
    ↓
Scanner (scanner.c)
    ↓
Tokens
    ↓
Parser (parser.c)
    ↓
AST (expr.c / stmt.c)
    ↓
Interpreter (interpreter.c)
    ↓
Environment (environment.c)
```

---

## Project Structure

```
src/
├── main.c          entry point, REPL, file runner
├── scanner.c       tokenizer
├── parser.c        recursive descent parser
├── expr.c          expression AST nodes
├── stmt.c          statement AST nodes
├── interpreter.c   tree-walk evaluator
├── environment.c   variable storage, scope chaining
├── value.c         runtime value representation
└── fn.c            function objects and closures
```

---

## Running MoonScript

```bash
git clone https://github.com/ahmadodeh9000/Moonscript.git
cd Moonscript
make
```

REPL:
```bash
./moon
```

Run a script:
```bash
./moon program.ms
```

---

## Motivation

I built MoonScript to get hands-on experience with how programming languages actually work — beyond what is covered in typical university courses.

Working through this taught me:

- How a scanner breaks raw text into tokens
- How recursive descent parsing works
- How an AST represents program structure
- How environments chain together to implement scope
- How closures capture and preserve their surrounding scope
- How to debug memory issues in C with no GC

---

## Acknowledgments

Robert Nystrom's *Crafting Interpreters* is the best resource I've found for learning language implementation. MoonScript adapts its tree-walk interpreter approach from Java to C, with its own syntax and additional features.
