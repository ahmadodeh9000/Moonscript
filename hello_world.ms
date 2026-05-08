// ── variables ─────────────────────────────────────────
let x = 10;
let y = 20;
print(x + y);        // 30
print(x - y);        // -10
print(x * y);        // 200
print(y / x);        // 2
print(y % 3);        // 2
print(2 # 8);        // 256

// ── strings ───────────────────────────────────────────
let name = "moon";
print("hello " + name);       // hello moon
print("version: " + 1);       // version: 1

// ── booleans ──────────────────────────────────────────
print(true);                  // true
print(false);                 // false
print(!true);                 // false
print(!false);                // true
print(!nil);                  // true

// ── comparisons ───────────────────────────────────────
print(10 > 5);                // true
print(10 < 5);                // false
print(10 >= 10);              // true
print(10 <= 9);               // false
print(10 == 10);              // true
print(10 != 9);               // true

// ── logical operators ─────────────────────────────────
print(true and false);        // false
print(true and true);         // true
print(false or true);         // true
print(false or false);        // false
print(nil or "default");      // default
print("hi" or "never");       // hi

// ── assignment ────────────────────────────────────────
let a = 1;
a = 99;
print(a);                     // 99

// ── if / else ─────────────────────────────────────────
if (10 > 5) print("gt");      // gt

if (1 == 2) {
    print("wrong");
} else {
    print("correct");         // correct
}

// ── nested if ─────────────────────────────────────────
let score = 85;
if (score >= 90) {
    print("A");
} else if (score >= 80) {
    print("B");               // B
} else {
    print("C");
}

// ── scope / shadowing ─────────────────────────────────
let val = "outer";
{
    let val = "inner";
    print(val);               // inner
}
print(val);                   // outer

// ── while loop ────────────────────────────────────────
let i = 0;
while (i < 5) {
    print(i);                 // 0 1 2 3 4
    i = i + 1;
}

// ── for loop ──────────────────────────────────────────
for (let j = 0; j < 5; j = j + 1) {
    print(j);                 // 0 1 2 3 4
}

// ── nested loops ──────────────────────────────────────
for (let r = 1; r <= 3; r = r + 1) {
    for (let c = 1; c <= 3; c = c + 1) {
        print(r * c);         // multiplication table
    }
}

// ── fibonacci ─────────────────────────────────────────
let fa = 0;
let fb = 1;
let temp;
while (fa < 100) {
    print(fa);
    temp = fa;
    fa = fb;
    fb = temp + fb;
}

// ── bitwise ───────────────────────────────────────────
print(6 & 3);                 // 2
print(6 | 3);                 // 7
print(6 ^ 3);                 // 5

// ── nil ───────────────────────────────────────────────
let empty;
print(empty);                 // NIL
print(empty == nil);          // true