// ═══════════════════════════════════════════
// Moonscript Test Suite
// ═══════════════════════════════════════════

// ── arithmetic ──────────────────────────────
print(1 + 2);           // 3
print(10 - 3);          // 7
print(3 * 4);           // 12
print(10 / 4);          // 2.5
print(10 % 3);          // 1
print(2 # 8);           // 256

// ── strings ─────────────────────────────────
print("hello" + " moon");     // hello moon
print("n=" + 42);             // n=42
print(42 + " is the answer"); // 42 is the answer

// ── booleans ────────────────────────────────
print(true);            // true
print(false);           // false
print(!true);           // false
print(!nil);            // true

// ── comparisons ─────────────────────────────
print(1 < 2);           // true
print(2 > 1);           // true
print(1 == 1);          // true
print(1 != 2);          // true
print(1 <= 1);          // true
print(2 >= 2);          // true

// ── variables ───────────────────────────────
let x = 10;
let y = 20;
print(x + y);           // 30
x = 99;
print(x);               // 99

// ── scope and shadowing ─────────────────────
let a = "outer";
{
    let a = "inner";
    print(a);           // inner
}
print(a);               // outer

// ── logical operators ───────────────────────
print(true and true);   // true
print(true and false);  // false
print(false or true);   // true
print(nil or "yes");    // yes
print("hi" or "no");    // hi

// ── if / else ───────────────────────────────
if (10 > 5) print("gt");           // gt
if (1 == 2) print("no"); else print("yes");  // yes

let score = 85;
if (score >= 90) {
    print("A");
} else if (score >= 80) {
    print("B");                     // B
} else {
    print("C");
}

// ── while ───────────────────────────────────
let i = 0;
while (i < 3) {
    print(i);           // 0 1 2
    i = i + 1;
}

// ── for ─────────────────────────────────────
for (let j = 0; j < 3; j = j + 1) {
    print(j);           // 0 1 2
}

// ── functions ───────────────────────────────
fn add(a, b) {
    ret a + b;
}
print(add(3, 4));       // 7
print(add(10, -3));     // 7

fn greet(name) {
    ret "hello " + name;
}
print(greet("ahmad"));  // hello ahmad

// ── recursion ───────────────────────────────
fn fib(n) {
    if (n <= 1) ret n;
    ret fib(n - 2) + fib(n - 1);
}
print(fib(0));          // 0
print(fib(1));          // 1
print(fib(10));         // 55

fn factorial(n) {
    if (n <= 1) ret 1;
    ret n * factorial(n - 1);
}
print(factorial(5));    // 120
print(factorial(10));   // 3628800

// ── closures ────────────────────────────────
fn makeCounter() {
    let i = 0;
    fn count() {
        i = i + 1;
        print(i);
    }
    ret count;
}

let c = makeCounter();
c();                    // 1
c();                    // 2
c();                    // 3

let d = makeCounter();  // independent counter
d();                    // 1
c();                    // 4  -- c continues from where it left off

// ── higher order functions ───────────────────
fn apply(f, x) {
    ret f(x);
}

fn double(x) {
    ret x * 2;
}

print(apply(double, 5));    // 10
print(apply(double, 21));   // 42

// ── functions as values ──────────────────────
           // this would error — functions aren't anonymous yet
                           // but storing a returned fn works:
fn makeAdder(n) {
    fn adder(x) {
        ret x + n;
    }
    ret adder;
}

let add5  = makeAdder(5);
let add10 = makeAdder(10);
print(add5(3));             // 8
print(add10(3));            // 13
print(add5(add10(1)));      // 16

// ── bitwise ─────────────────────────────────
print(6 & 3);           // 2
print(6 | 3);           // 7
print(6 ^ 3);           // 5

// ── nil ─────────────────────────────────────
let empty;
print(empty);           // NIL
print(empty == nil);    // true