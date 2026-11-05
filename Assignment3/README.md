# ASSIGNMENT 3: Object Lifecycle, Dynamic Memory Management & Operator Overloading

**Course Outcome:** CO3 — Construct C++ programs using constructors, destructors and operator overloading to control object creation, destruction and operator behaviour – default, parameterised, overloaded, copy and dynamic constructors, destructors, defining and overloading operators, and the rules for operator overloading.  
**Bloom’s Level Assessed:** K5 – Evaluate (All Tasks)  
**Total Marks:** 50  
**Submission Date:** 05/11/2026  

---

## Student & Environment Metadata

- **Student Name:** Manoj Kumar Chinthala
- **Register Number:** 192472008
- **Course Code & Title:** DSA01 — Object-Oriented Programming with C++
- **Institution:** SIMATS Engineering, Saveetha University
- **GitHub Repository Link:** [https://github.com/chinthalamanojkumar/DSA0103-C-.git](https://github.com/chinthalamanojkumar/DSA0103-C-.git)
- **Target OS:** Windows 11 (x86_64)
- **Compiler & Version:** `g++ (MinGW-W64 x86_64-ucrt-posix-seh, built by Brecht Sanders, r3) 16.2.0`
- **Standard Compilation Command:**
  ```bash
  g++ -std=c++17 -Wall -Wextra -pedantic TaskXX_Name.cpp -o TaskXX.exe
  ```
- **Execution & Output Redirection:**
  ```bash
  .\TaskXX.exe | Tee-Object -FilePath output.txt
  ```

---

## Master Task Summary & Result Verification Matrix

| Task | Core Concept / Module | Bloom's Level | Implementation & Theoretical Deliverables | Verification Status |
| :--- | :--- | :---: | :--- | :---: |
| [Task 01 (Q1)](#task-01-q1--constructors-and-constructor-overloading) | Constructors & Overloading | K5 | Library Book records; Member initializer lists; Constructor delegation; Negative copy invariant; `-Wreorder` compiler audit | **PASSED** (Invariants verified) |
| [Task 02 (Q2)](#task-02-q2--copy-constructors-shallow-and-deep-copies) | Shallow vs Deep Copies | K5 | Rule of Three deep copy (Completed TODO 1–3: `Ibuprofen ORS...`); Self-assignment guard (`p = p`); Shallow double-free audit; 100k copy benchmark | **PASSED** (Deep copies isolated) |
| [Task 03 (Q3)](#task-03-q3--dynamic-constructors-and-destructors) | Dynamic Ctors & Dtors | K5 | 2D dynamic seat map (`char**`); Logger predict output (`+A +B -B copyAc use -Ac +P -P end -A`); Stack/Block/Heap RAII lifecycles; Memory footprint analysis | **PASSED** (100% exact match) |
| [Task 04 (Q4)](#task-04-q4--overloading-arithmetic-and-stream-operators) | Arithmetic & Stream Operators | K5 | Money class (Completed TODO 1 & 2: `Rs 200.00 true`); Paise carry (`0.75 + 0.50 = 1.25`); 1M addition IEEE 754 drift audit; Dimensionality defense | **PASSED** (Zero drift preserved) |
| [Task 05 (Q5)](#task-05-q5--increment-comparison-and-subscript-operators) | Increment & Subscript Operators | K5 | Counter predict output (`5 7 7`); TrackTime prefix/postfix rollover (`59:59 -> 1:00:00`); Const & non-const `operator[]`; 10M call benchmark | **PASSED** (Prefix 0 copies proved) |
| [Task 06 (Q6)](#task-06-q6--the-rules-of-operator-overloading) | Rules of Operator Overloading | K5 | Text SMS gateway class; Non-overloadable operators (`::`, `.`, `.*`, `?:`, `sizeof`) audit; Chained `a + b + c` copy audit; 20k chunk benchmark vs `std::string` | **PASSED** (Syntax invariants confirmed) |
| [Task 07 (Q7)](#task-07-q7--design-a-complex-number-class-from-first-principles) | Complex Number AC Calculator | K5 | 4-Stage engineering design; Series RLC ($100 - 50j\,\Omega, |Z|=111.803$); Parallel RLC ($50 + 50j\,\Omega, |Z|=70.711$); Division by zero exception guard | **PASSED** (Hand-verified exact) |

---

## Detailed Task-by-Task Evaluative Reports (K5 Level)

### Task 01 (Q1) — Constructors and Constructor Overloading
**Source Directory:** [`Assignment3/Task01/`](./Task01/)  
**Primary Source File:** [`Task01_BookRecords.cpp`](./Task01/Task01_BookRecords.cpp)  
**Compiler Audit File:** [`compiler_diagnostics.txt`](./Task01/compiler_diagnostics.txt)  
**Execution Log:** [`output.txt`](./Task01/output.txt)  

#### 1. Problem Context & Implementation
The library database manages `Book` records created under varying situations:
1. *Full Parameterised Record:* Title, author, ISBN, price, copies.
2. *Donated Book:* Title only (author set to "Donated Community Contributor", 1 copy).
3. *Placeholder Record:* Default constructor ("Untitled Placeholder", 0 copies).
4. *Invariant Validation:* Rejects and clamps negative copies to 0, preventing garbage state.

#### 2. Design & Analysis
- **Compiler-Synthesized Default Constructor:**
  - The compiler synthesizes an implicit default constructor only if **zero** constructors are declared by the programmer. Declaring any constructor (e.g., parameterised) immediately suppresses the synthesized default constructor.
  - If `Book()` is removed, instantiating `Book catalog[5];` fails with:
    `error: no matching function for call to 'Book::Book()'`.
- **Why Member Initializer Lists are Preferred:**
  - Directly constructs data members in place using the target constructor.
  - Body assignment forces default initialization first, followed by an redundant copy/move assignment.
  - Initializer lists are syntactically mandatory for `const` members, reference members (`&`), and class members lacking default constructors.
- **Initialization Order:**
  - Members are initialized strictly in the order they are **declared** in the class definition, regardless of their order in the constructor initializer list. Reversing list order triggers `-Wreorder`:
    `warning: 'ReorderDemo::title' will be initialized after 'int ReorderDemo::copies' [-Wreorder]`.
- **Explicit Constructors:**
  - Marking single-argument constructors `explicit` prevents unintended implicit conversions (e.g. preventing a string literal from silently converting to a temporary `Book` during function calls).

---

### Task 02 (Q2) — Copy Constructors: Shallow and Deep Copies
**Source Directory:** [`Assignment3/Task02/`](./Task02/)  
**Primary Source File:** [`Task02_PrescriptionCopies.cpp`](./Task02/Task02_PrescriptionCopies.cpp)  
**Crash Audit File:** [`shallow_copy_crash_log.txt`](./Task02/shallow_copy_crash_log.txt)  
**Execution Log:** [`output.txt`](./Task02/output.txt)  

#### 1. The Shallow Copy Double-Free Flaw
- When a class managing a raw heap pointer (`std::string* medicines`) uses the default compiler copy constructor, it performs a memberwise pointer copy (shallow copy).
- Both instances point to the exact same heap memory block. When both objects go out of scope, the destructor attempts to execute `delete[] medicines` twice on the identical pointer address, triggering an immediate OS heap corruption abort (`STATUS_HEAP_CORRUPTION / Exception 0xC0000374`).

#### 2. Rule of Three Implementation (TODO 1, 2, 3)
- **TODO 1 (Copy Constructor):** Allocates a fresh heap buffer `new std::string[other.n]` and copies all medicine elements.
- **TODO 2 (Copy Assignment Operator):** Guards against self-assignment (`this != &other`), allocates a new array first (exception safety), deallocates the old array, updates pointers, and returns `*this`.
- **TODO 3 (Destructor):** Releases the dynamically allocated array `delete[] medicines`.
- **Complete the Code Output:**
  `Ibuprofen ORS | Paracetamol Cetirizine | Paracetamol ORS` (Verified exact match).
  Buffer addresses: `p = 0x2095ba86458`, `q = 0x2095ba85dc8`, `r = 0x2095ba86138` (All distinct!).

#### 3. Copy Count & Multi-Scale Benchmark
- Pass-by-value makes **1 full deep copy**; pass-by-const-reference makes **0 copies**.
- Benchmark: 10 medicines = **3.9 μs** | 1,000 medicines = **193.5 μs** | 100,000 medicines = **14.0 ms**.
- **Rule of Zero Alternative:** Replacing raw pointers with `std::vector<std::string>` automates deep copying and memory reclamation, eliminating manual destructor and copy assignment boilerplate.

---

### Task 03 (Q3) — Dynamic Constructors and Destructors
**Source Directory:** [`Assignment3/Task03/`](./Task03/)  
**Primary Source File:** [`Task03_AuditoriumBooking.cpp`](./Task03/Task03_AuditoriumBooking.cpp)  
**Execution Log:** [`output.txt`](./Task03/output.txt)  

#### 1. Predict the Output Verification
- **Given Program:** `Logger a("A")`, inner block `Logger b("B")`, `use(a)`, `new Logger("P")`, `delete p`.
- **Predicted Output Before Running:**
  `+A +B -B copyAc use -Ac +P -P end -A `
- **Observed Actual Output:** Identical exact match.
  - Proves that inner block variables destruct immediately at `}`, pass-by-value copies are destroyed at function return, heap objects are freed exclusively on `delete`, and outer stack objects destruct in reverse order of declaration (LIFO).

#### 2. Dynamic 2D Seat Map & RAII
- Implemented `AuditoriumEvent` managing `char** seatMap`. Dynamic constructor allocates rows and columns; destructor loops through rows, calls `delete[] seatMap[i]`, calls `delete[] seatMap`, and prints a closing audit summary.
- **Destructor Properties:** Destructors take no arguments and cannot be overloaded because they are invoked automatically by the runtime upon scope termination.
- **Memory Footprint:** For $1000 \times 2000$ seats, pointer array consumes 8 KB, seat buffer consumes 2 MB.
- **Alternative Concept:** A single flat 1D array (`char* seatMap = new char[rows * cols]`) eliminates $rows$ individual heap allocation calls, saves 8 KB of pointer overhead, and delivers superior CPU L1 cache locality.

---

### Task 04 (Q4) — Overloading Arithmetic and Stream Operators
**Source Directory:** [`Assignment3/Task04/`](./Task04/)  
**Primary Source File:** [`Task04_MoneyArithmetic.cpp`](./Task04/Task04_MoneyArithmetic.cpp)  
**Execution Log:** [`output.txt`](./Task04/output.txt)  

#### 1. Complete the Code Verification
- Overloaded `+` as member function returning a new `Money` object.
- Overloaded `==` as member function and `<<` as non-member friend.
- Output: `Rs 200.00` | `true` (Verified exact match).
- Tested paise carry-over: `Rs 0.75 + Rs 0.50 = Rs 1.25` (Verified).

#### 2. Cumulative Drift Proof: Integer Paise vs IEEE 754 Double (1,000,000 Operations)
- Adding 10 paise ($0.10) 1,000,000 times:
  - True Expected Value: **Rs. 100,000.00**
  - Integer `Money` Class Total: **Rs. 100,000.00** (Exact match)
  - IEEE 754 `double` Total: **Rs. 100,000.0000013329** (Drift: +0.0000013329)
  - **Significance:** In commercial billing with millions of ledger entries, floating point binary fraction truncation yields financial discrepancies. Integer paise arithmetic guarantees exact precision.

#### 3. Engineering Decision: Dimensional Analysis
- `Money * Money` is **strictly prohibited** because $\text{Rupees} \times \text{Rupees} = \text{Rupees}^2$, which has no physical financial meaning.
- Multiplying `Money` by a dimensionless scalar (`double` quantity/discount percentage) yields `Money` and is fully supported.

---

### Task 05 (Q5) — Overloading Increment, Comparison and Subscript Operators
**Source Directory:** [`Assignment3/Task05/`](./Task05/)  
**Primary Source File:** [`Task05_PlaylistTimer.cpp`](./Task05/Task05_PlaylistTimer.cpp)  
**Execution Log:** [`output.txt`](./Task05/output.txt)  

#### 1. Predict the Output Verification
- **Counter Program:** `Counter c{5}; Counter a = c++; Counter b = ++c;`
- **Predicted Output:** `5 7 7` (Observed: `5 7 7` exact match).
  - Postfix `c++` returns a copy of the pre-increment value (5) and increments `c` to 6.
  - Prefix `++c` increments `c` to 7 and returns a reference to `c`.

#### 2. TrackTime Rollover & Playlist Subscripting
- Tested `TrackTime` advancing seconds: rolls over seamlessly across minute and hour boundaries (`00:59:59` $\to$ `01:00:00`).
- Implemented `const` and non-`const` versions of `operator[]`:
  - Non-const allows in-place track renaming: `pl[1] = "Remastered Track";`
  - Const provides read-only indexing on `const Playlist&`.
  - Boundary checking intercepts `pl[-1]` and `pl[size]`, safely throwing `std::out_of_range`.

#### 3. Prefix vs Postfix Benchmark (10,000,000 Operations)
- Postfix `operator++(int)` must allocate a temporary copy to preserve the pre-increment state.
- Prefix `operator++()` mutates in place and returns `*this&` with **0 temporary object allocations**, making `++i` the universally preferred idiom for class types and iterators.

---

### Task 06 (Q6) — The Rules of Operator Overloading
**Source Directory:** [`Assignment3/Task06/`](./Task06/)  
**Primary Source File:** [`Task06_TextSmsGateway.cpp`](./Task06/Task06_TextSmsGateway.cpp)  
**Execution Log:** [`output.txt`](./Task06/output.txt)  

#### 1. Non-Overloadable Operators
1. `::` (Scope Resolution): Essential for compile-time namespace and class resolution.
2. `.` (Member Access): Direct field access guarantee.
3. `.*` (Pointer-to-Member): Fundamental language dereference syntax.
4. `?:` (Ternary Conditional): Implements short-circuit evaluation; function overloading evaluates all arguments before dispatch.
5. `sizeof`: Compile-time constant operator evaluated by the compiler frontend.

#### 2. Rules of Operator Overloading
- Precedence and associativity cannot be changed.
- Arity cannot change (binary cannot become ternary).
- At least one operand must be a user-defined type (prevents altering built-in `int + int`).
- `operator=` MUST be a member function to prevent external hijacking of class assignment semantics.

#### 3. Chained Concatenation & Benchmark
- In `a + b + c`, C++17 move semantics and RVO eliminate redundant copies.
- Benchmark: Custom `Text` class took 94.58 ms for 20,000 concatenations vs 0.89 ms for `std::string` (due to `std::string`'s exponential buffer doubling).
- **Engineering Justification:** A custom `Text` class is vital in bare-metal telecom firmwares or kernel drivers where `std::string` and heap allocators are forbidden.

---

### Task 07 (Q7) — Design a Complex-Number Class from First Principles
**Source Directory:** [`Assignment3/Task07/`](./Task07/)  
**Primary Source File:** [`Task07_ComplexACImpedance.cpp`](./Task07/Task07_ComplexACImpedance.cpp)  
**Execution Log:** [`output.txt`](./Task07/output.txt)  

#### 1. Problem Context: AC Circuit Impedance Calculator
Impedances are complex numbers:
- Resistor: $Z_R = R + 0j$
- Inductor: $Z_L = 0 + j\omega L$
- Capacitor: $Z_C = 0 - j / (\omega C)$
- Series: $Z_{series} = Z_1 + Z_2$
- Parallel: $Z_{parallel} = (Z_1 \times Z_2) / (Z_1 + Z_2)$

#### 2. Four-Stage Engineering Design
- **Stage 1 (Interpretation):** Real part (resistance $\Omega$), imaginary part (reactance $\Omega$). Default ($0+0j$), scalar converting ($R+0j$), and full ($R+Xj$) constructors.
- **Stage 2 (Design & Formulas):** Arithmetic operators overloaded as non-member friends for symmetric mixed-mode arithmetic (`50.0 + Z` and `Z + 50.0`). Division formula guards denominator with $\epsilon = 10^{-12}$.
- **Stage 3 (Implementation & Verification):**
  - Given: $R = 100\,\Omega$, $L = 0.05\,\text{H}$, $C = 10\,\mu\text{F}$, $\omega = 1000\,\text{rad/s}$
  - $Z_R = 100.000 + 0.000j\,\Omega$
  - $Z_L = 0.000 + 50.000j\,\Omega$
  - $Z_C = 0.000 - 100.000j\,\Omega$
  - **Series RLC:**
    - Computed $Z_{series} = 100.000 - 50.000j\,\Omega$
    - Magnitude $|Z| = \sqrt{100^2 + (-50)^2} = \mathbf{111.803\,\Omega}$ (Phase: $-26.565^\circ$)
    - Hand calculation: $100 - (100 - 50)j = 100 - 50j$. Exact match!
  - **Parallel RLC:**
    - Computed $Z_{parallel} = 50.000 + 50.000j\,\Omega$
    - Magnitude $|Z| = \sqrt{50^2 + 50^2} = \mathbf{70.711\,\Omega}$ (Phase: $+45.000^\circ$)
    - Hand calculation: $Z_L \parallel Z_C = \frac{-5000 j^2}{-50j} = 100j$; $100 \parallel 100j = 50 + 50j$. Exact match!
  - **Division by Zero:** Safely intercepts $Z / 0$, throwing `std::runtime_error` without generating `NaN` or crashing.
- **Stage 4 (Defense):** Compared with `std::complex<double>`. Custom class enforces explicit domain-specific division-by-zero validation and intuitive rectangular AC circuit formatting.

---

## Instructions to Build and Verify All Tasks

Each task can be compiled and executed independently from the repository root:

```bash
# Task 01: Book Constructors & Delegation
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task01/Task01_BookRecords.cpp -o Task01.exe
.\Task01.exe

# Task 02: Prescription Rule of Three Deep Copies
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task02/Task02_PrescriptionCopies.cpp -o Task02.exe
.\Task02.exe

# Task 03: Auditorium Dynamic Memory & RAII
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task03/Task03_AuditoriumBooking.cpp -o Task03.exe
.\Task03.exe

# Task 04: Money Operator Overloading
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task04/Task04_MoneyArithmetic.cpp -o Task04.exe
.\Task04.exe

# Task 05: Playlist Subscript & Timer Prefix/Postfix
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task05/Task05_PlaylistTimer.cpp -o Task05.exe
.\Task05.exe

# Task 06: Text SMS Operator Rules
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task06/Task06_TextSmsGateway.cpp -o Task06.exe
.\Task06.exe

# Task 07: Complex AC Circuit Calculator
g++ -std=c++17 -Wall -Wextra -pedantic Assignment3/Task07/Task07_ComplexACImpedance.cpp -o Task07.exe
.\Task07.exe
```
