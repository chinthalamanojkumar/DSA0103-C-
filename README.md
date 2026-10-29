# ASSIGNMENT 1: Design, Implement and Validate the Fare Logic of a City Metro Smart-Card System

**Course Outcome:** CO1 — Apply object-oriented programming principles and C++ constructs such as data types, operators and control structures to design and write structured OOP-based programs.  
**Bloom’s Level Assessed:** K5 – Evaluate (All Tasks)  
**Total Marks:** 50  

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

## Quick Navigation & Result Summary Matrix

| Task | Topic / Component | Bloom's Level | Key Implementation Highlights | Verification Result |
| :--- | :--- | :---: | :--- | :---: |
| [Task 01](#task-01--object-oriented-analysis--domain-modeling) | OO Analysis & Domain Modeling | K5 | SmartCard, FareTable, Journey, Passenger, Zone classes | **PASSED** (Invariants verified) |
| [Task 02](#task-02--tokens-keywords-identifiers-and-constants) | Tokens, Keywords & Constants | K5 | `constexpr` tables, `enum class MetroZone`, Token taxonomy | **PASSED** (static_assert valid) |
| [Task 03](#task-03--data-types-and-precision-analysis) | Data Types & Precision Analysis | K5 | Integer paise principle; IEEE 754 drift proof; INT_MAX overflow | **PASSED** (Exact paise preserved) |
| [Task 04](#task-04--control-implicit-conversions) | Control Implicit Conversions | K5 | Predict Output, `-Wsign-compare`, brace init, `static_cast` | **PASSED** (Predictions match) |
| [Task 05](#task-05--operator-precedence-and-expressions) | Expressions & Precedence | K5 | Operator precedence trace, peak expressions, 24-hr table | **PASSED** (Precedence evaluated) |
| [Task 06](#task-06--control-structures--complete-the-code) | Control Structures | K5 | Completed TODO 1 & 2; SIMATS Pseudocode; 10 AM/8 PM checks | **PASSED** (Exact output matched) |
| [Task 07](#task-07--procedural-to-object-oriented-architecture) | Procedural vs. OOP | K5 | Side-by-side comparison; negative balance prevention | **PASSED** (Class invariant intact) |
| [Task 08](#task-08--safe-stream-input-and-formatting) | Read & Print Data Safely | K5 | `cin.fail()`, stream recovery, `<iomanip>` formatted ledger | **PASSED** (Corrupted input handled) |
| [Task 09](#task-09--complete-implementation--boundary-testing) | Implementation & Testing Suite | K5 | 13 automated boundary test cases + 5-journey hand trace | **PASSED** (13/13 test cases passed) |
| [Task 10](#task-10--compare-expectations-with-execution) | Expectation vs. Execution | K5 | Multi-scale simulation (20k, 500k, 3M trips); int overflow | **PASSED** (Overflow demonstrated) |
| [Task 11](#task-11--final-design-recommendation--defense) | Final Design Recommendation | K5 | Decoupled architecture, 8.31 ns tap latency, 120M taps/sec | **PASSED** (Defended with metrics) |
| [Task 12](#task-12--day-4-design-challenge-8-marks) | Day-4 Design Challenge | K5 | Zone 4 Airport (Rs 60), Weekend -10%, MonthlyPass composition | **PASSED** (All policies satisfied) |

---

## Detailed Task-by-Task Evaluative Reports (K5 Level)

### Task 01 — Object-Oriented Analysis & Domain Modeling

- **Domain Problem:** The Namma Metro smart-card system divides its transit network into three zones (`Inner` = Rs 10, `Middle` = Rs 20, `Outer` = Rs 30), with a +20% peak surcharge (08:00–10:00 and 17:00–20:00). A card is only permitted to start/complete a trip if at least **Rs 50.00 (5000 paise)** remains on the card after the fare is deducted.
- **Objects Identified & Encapsulation:**
  - `MetroZone`: Strongly typed scoped enumeration (`enum class`) preventing invalid zones.
  - `SmartCard`: Encapsulates private `balancePaise`. Invariant: Balance can never become negative, and balance after fare deduction must be $\ge 5000$ paise.
  - `FareTable`: Encapsulates pricing constants, surcharge formulas, and concession rules.
  - `Passenger`: Models regular, student, and senior citizen travel entitlements.
  - `Journey`: Transactional link executing tap-and-go validation.
- **OOP Principles:**
  - *Encapsulation:* Private card balance cannot be directly subtracted by external turnstiles.
  - *Abstraction:* Turnstile simply invokes `deductFare(fare)` without needing to know internal balance representation.
  - *Inheritance & Polymorphism:* Strategy pattern for concession calculation and pass derivation.
- **Alternative Procedural Analysis & Complexity:**
  - *Procedural Time:* $O(1)$ constant time.
  - *OOP Time:* $O(1)$ constant time (inlined member functions have zero penalty).
  - *Space:* $O(1)$ (8 bytes per balance in both).
  - *Maintainability & Coupling:* In procedural code, modifying the minimum balance policy requires searching and changing every free function where balance is accessed. In OOP, only `SmartCard::canTravel()` is modified—zero regression risk.

---

### Task 02 — Tokens, Keywords, Identifiers and Constants

- **Five Token Classes:**
  1. *Keywords:* `constexpr`, `enum`, `class`, `int`, `bool`, `static_cast`, `switch`.
  2. *Identifiers:* `MetroZone`, `METRO_BASE_FARE_PAISE`, `PEAK_SURCHARGE_PERCENT`.
  3. *Constants:* `1000LL`, `20`, `5000LL`, `"Inner"`.
  4. *Operators:* `[]`, `=`, `+`, `*`, `<`, `>=`, `&&`, `||`, `::`.
  5. *Punctuators:* `{`, `}`, `(`, `)`, `;`, `,`.
- **`#define` vs `const` vs `constexpr`:**
  - `#define` is untyped textual substitution, lacks scope, and causes symbol collisions.
  - `const` provides type safety, but evaluation may be deferred to runtime.
  - `constexpr` guarantees compile-time constant expression evaluation, permits `static_assert` validation, has zero runtime memory footprint, and lives cleanly within class namespaces.
- **`enum` vs `enum class`:**
  - Unscoped enums leak symbols and implicitly convert to `int`, allowing absurd comparisons such as `Zone::Middle == FoodItem::BURGER` (emits `-Wenum-compare` warning).
  - Scoped `enum class` enforces strict type checking and forbids implicit integer conversion.

---

### Task 03 — Data Types and Precision Analysis

- **System Data Type Audit (x86_64 GCC):**
  - `bool` (1 byte): Peak indicator.
  - `char` (1 byte): Concession category token.
  - `int` (4 bytes): Hours, minutes, percentages.
  - `long long` (8 bytes): Monetary amounts in paise ($-9.22 \times 10^{18}$ to $+9.22 \times 10^{18}$).
- **The Paise Principle (Why Money Must Be Integer Paise):**
  - Fractions like $0.10$ cannot be represented exactly in binary floating-point (IEEE 754), yielding an infinite repeating binary expansion.
  - Accumulating $0.10$ over $1,000,000$ iterations produces a measurable drift ($\text{Rs } 0.0000013329$), which violates financial auditability. Storing money in integer paise ($\mathbb{Z}$) guarantees 100% exact arithmetic.
- **Network Revenue Scale Overflow:**
  - At $3,000,000$ daily journeys with an average fare of Rs 25.00 ($2500$ paise), daily revenue is $7,500,000,000$ paise (Rs 7.5 Crore).
  - 32-bit `INT_MAX` is $2,147,483,647$ paise (Rs 2.14 Crore).
  - $7,500,000,000 > \text{INT\_MAX}$, resulting in two's complement integer overflow into **negative revenue (-Rs 10,899,345.92)**! Thus, `long long` is mandatory.

---

### Task 04 — Control Implicit Conversions

- **Given Program Prediction vs. Actual Execution:**
  ```cpp
  // Code Snippet
  int a = 7, b = 2;
  double c = a / b;
  double d = static_cast<double>(a) / b;
  // Predicted: 3 3.5 | Actual: 3 3.5

  char ch = 'A' + 2;
  // Predicted: C 68  | Actual: C 68

  bool flag = 5;
  // Predicted: 1 true | Actual: 1 true

  unsigned int u = 1; int s = -1;
  // Predicted: s >= u | Actual: s >= u
  ```
- **Conversions Explained:**
  - `a / b`: Both operands are `int`; integer division yields `3`, then implicitly promoted to `double(3.0)`.
  - `static_cast<double>(a) / b`: Explicit cast makes numerator `7.0`; denominator `2` promoted to `double`, giving `3.5`.
  - `'A' + 2`: `'A'` promoted to `int(65) + 2 = 67`, converted back to `char('C')`. In `ch + 1`, `ch` promoted to `int` -> `68`.
  - `bool flag = 5`: Non-zero integer implicitly converted to boolean `true`.
  - `s < u`: Comparison between signed `int` and `unsigned int`. `-1` is converted to `unsigned int`, becoming `4294967295` (`UINT_MAX`), so `4294967295 < 1` is `false`, printing `s >= u`! GCC flags this with `-Wsign-compare`.
- **Mitigation:** Uniform brace initialization (`int x{5};`) prevents narrowing conversions at compile time.

---

### Task 05 — Operator Precedence and Expressions

- **Given Program Output:**
  ```
  11 16
  9 6 4
  6 4
  3 1 -3 -1
  18
  ```
- **Operator Analysis:**
  - `x + y * 2`: Multiplication (`*`) binds tighter than addition (`+`); `3 * 2 = 6`, `5 + 6 = 11`.
  - `int z = x++ + ++y;`: `x++` evaluates to `5` and post-increments `x` to `6`; `++y` pre-increments `y` to `4` and evaluates to `4`. $z = 5 + 4 = 9$.
  - `x % 4 * 2`: `%` and `*` have identical precedence and are left-to-right associative: $(6 \pmod 4) \times 2 = 2 \times 2 = 4$.
  - Negative Division and Modulo (C++11 standard): Truncation is towards zero. $-10 / 3 = -3$, and $-10 \pmod 3 = -1$.
  - Peak Check Logic: `(hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)`
    Without parentheses, `&&` takes precedence over `||`, but GCC issues a `-Wparentheses` warning due to readability hazards.

---

### Task 06 — Control Structures & Complete the Code

- **SIMATS Algorithmic Pseudocode:**
  ```text
  ALGORITHM ComputeFare(zone: Zone, isPeak: Boolean) -> Integer
  BEGIN
      SWITCH zone DO
          CASE Zone::Inner:  baseFare <- 1000
          CASE Zone::Middle: baseFare <- 2000
          CASE Zone::Outer:  baseFare <- 3000
          DEFAULT: RETURN 0
      END SWITCH

      IF isPeak THEN
          baseFare <- baseFare + (baseFare * 20) / 100
      END IF

      RETURN baseFare
  END ALGORITHM

  ALGORITHM CanTravel(balance: Integer, fare: Integer) -> Boolean
  BEGIN
      RETURN (balance - fare) >= 5000
  END ALGORITHM
  ```
- **Completed Code Execution Output:**
  ```
  Fare Rs 10.00  balance Rs 90
  Fare Rs 36.00  balance Rs 54
  Denied (low balance)  balance Rs 54
  ```
- **Boundary Validation:**
  - Exactly 10:00 AM: Treated as **Off-Peak** (Fare Rs 10.00).
  - Exactly 20:00 PM (8:00 PM): Treated as **Off-Peak** (Fare Rs 30.00).
  - Resolved the inconsistent boundary bug from the legacy gate software.

---

### Task 07 — Procedural to Object-Oriented Architecture

- **Side-by-Side Invariant Enforcement:**
  - *Procedural Failure:* In procedural code, `card.balancePaise` is public. Rogue or naive client code can execute `card.balancePaise -= 6000`, dropping the balance to Rs 10.00 without any invariant check.
  - *OOP Guarantee:* In `class SmartCard`, `balancePaise` is strictly `private`. External turnstiles must call `card.deduct(fare)`. If $(balance - fare) < 5000$, the deduction is rejected atomically.
- **Complexity:** Both execute in $O(1)$ time and require $O(1)$ memory.

---

### Task 08 — Safe Stream Input and Formatting

- **Stream Error State Recovery:**
  When non-numeric input (such as `abc`) is entered where an integer hour is expected:
  1. `std::cin.fail()` evaluates to `true`.
  2. `std::cin.clear()` clears the error state bits.
  3. `std::cin >> badToken` consumes and discards the corrupted characters.
  4. The parser recovers and extracts the next valid integer seamlessly.
- **Turnstile Transaction Ledger (<iomanip>):**
  Uses `setw`, `setfill('0')`, and `std::fixed` to print ledger lines aligned with 2 decimal places for paise.
- **Alternative Concept:** `std::getline()` + `std::stoi()` inside `try/catch` consumes $O(L)$ buffer space and handles corrupt lines safely, but direct stream extraction avoids heap allocation.

---

### Task 09 — Complete Implementation & Boundary Testing

- **Table-Driven Test Suite:**
  Executed 13 comprehensive automated test cases:
  1. Inner Off-Peak (Rs 10.00) — **PASSED**
  2. Inner Peak (Rs 12.00) — **PASSED**
  3. Middle Off-Peak (Rs 20.00) — **PASSED**
  4. Middle Peak (Rs 24.00) — **PASSED**
  5. Outer Off-Peak (Rs 30.00) — **PASSED**
  6. Outer Peak (Rs 36.00) — **PASSED**
  7. Morning Peak Start (08:00 AM) — **PASSED**
  8. Morning Peak End (10:00 AM) — **PASSED**
  9. Evening Peak Start (17:00 PM) — **PASSED**
  10. Evening Peak End (20:00 PM) — **PASSED**
  11. Balance Leaving Exactly Rs 50.00 — **PASSED**
  12. Balance Leaving Rs 49.99 (Denied) — **PASSED**
  13. Unknown Zone (Safely Denied) — **PASSED**
- **Hand-Calculated 5-Journey Audit:**
  All 5 hand-calculated steps matched the automated engine output with zero discrepancies.

---

### Task 10 — Expectation vs. Execution

- **Multi-Scale Simulation Results:**
  - **1 Station (20,000 trips):** Total = 43,200,000 paise (Rs 432,000.00). 32-bit signed int: SAFE. Time: 0.17 ms.
  - **1 Metro Line (500,000 trips):** Total = 1,080,000,000 paise (Rs 10,800,000.00). 32-bit signed int: SAFE. Time: 4.86 ms.
  - **Whole Network (3,000,000 trips):** Total = 6,480,000,000 paise (Rs 64,800,000.00).
    **32-bit signed int OVERFLOWS to -2,109,934,592 paise (-Rs 21,099,345.92)**!
- **Compiler Warnings Evaluated:**
  - `-Wenum-compare`: Prevented implicit cross-enum comparisons.
  - `-Wsign-compare`: Prevented dangerous signed vs unsigned evaluations.
  - `-Wparentheses`: Enforced clear logical grouping in boolean expressions.

---

### Task 11 — Final Design Recommendation & Defense

- **Architectural Defense:**
  - Decoupled `TurnstileGate` from `MetroConfig` and `SmartCard`.
  - Fare revisions are carried out in a single location (`MetroConfig`) with zero ripple effects across gate hardware drivers.
  - High-throughput stress test verified **8.31 nanoseconds latency per tap** (over 120 million taps/sec/core).
- **Alternative Config File Analysis:**
  Reading fare tables from a configuration file at boot takes ~65 microseconds, enabling zero-downtime fare revisions without firmware recompilation.

---

### Task 12 — Day-4 Design Challenge (8 Marks)

- **Requirement Changes:**
  1. *Zone 4 (Airport Zone):* Base fare Rs 60.00 (6000 paise).
  2. *Weekend Discount:* 10% discount on Saturday and Sunday; does NOT combine with peak surcharge.
  3. *Monthly Pass:* Unlimited journeys across two chosen zones.
- **Part A: Impact Analysis:**
  - `MetroZone` enum extended with `Airport = 3`.
  - `BASE_FARES_PAISE[]` expanded to 4 elements.
  - New temporal parameter `DayOfWeek` added to fare calculation.
  - `SmartCard` refactored using object composition to embed a `MonthlyPass` component.
- **Part B: Evaluation of Original Design:**
  The original switch statement required manual branch additions and could not express cross-zone passes without polluting smart card state.
- **Part C: Adaptation & Implementation:**
  Implemented `MonthlyPass` class with `isZoneCovered(destination)`.
  If destination is covered, fare is Rs 0.00, and trip is approved even if card wallet balance is Rs 0.00.
  If trip is outside pass zones, system falls back to card balance deduction with Rs 50 minimum check.
- **Part D: Test Verification:**
  - Weekday Airport Peak: Correctly charged Rs 72.00.
  - Weekend Airport Trip: 10% discount applied -> Rs 54.00 charged (no peak surcharge).
  - Monthly Pass Inner & Middle Trips: Allowed at Rs 0.00.
  - Monthly Pass to Airport: Outside pass coverage -> correctly evaluated against wallet balance.

---

## Instructions to Build and Run Every Task

All programs are written in ISO C++17. No binary executables are stored in this repository; each program can be compiled directly using `g++`:

```bash
# Task 01: OO Analysis & Domain Modeling
cd Task01
g++ -std=c++17 -Wall -Wextra -pedantic Task01_Analysis.cpp -o run_task01.exe
.\run_task01.exe

# Task 02: Tokens, Identifiers and Constants
cd ../Task02
g++ -std=c++17 -Wall -Wextra -pedantic Task02_TokensConstants.cpp -o run_task02.exe
.\run_task02.exe

# Task 03: Data Types and Precision Analysis
cd ../Task03
g++ -std=c++17 -Wall -Wextra -pedantic Task03_DataTypes.cpp -o run_task03.exe
.\run_task03.exe

# Task 04: Control Implicit Conversions
cd ../Task04
g++ -std=c++17 -Wall -Wextra Task04_ImplicitConversions.cpp -o run_task04.exe
.\run_task04.exe

# Task 05: Operator Precedence and Expressions
cd ../Task05
g++ -std=c++17 -Wall -Wextra Task05_Expressions.cpp -o run_task05.exe
.\run_task05.exe

# Task 06: Control Structures (Completed Code)
cd ../Task06
g++ -std=c++17 -Wall -Wextra -pedantic Task06_ControlStructures.cpp -o run_task06.exe
.\run_task06.exe

# Task 07: Procedural vs Object-Oriented
cd ../Task07
g++ -std=c++17 -Wall -Wextra -pedantic Task07_ProceduralVsOOP.cpp -o run_task07.exe
.\run_task07.exe

# Task 08: Safe Stream Input and Formatting
cd ../Task08
g++ -std=c++17 -Wall -Wextra -pedantic Task08_SafeIO.cpp -o run_task08.exe
.\run_task08.exe input.txt

# Task 09: Complete Implementation & Boundary Testing
cd ../Task09
g++ -std=c++17 -Wall -Wextra -pedantic Task09_ImplementationTesting.cpp -o run_task09.exe
.\run_task09.exe

# Task 10: Expectation vs Execution (Multi-Scale Simulation)
cd ../Task10
g++ -std=c++17 -Wall -Wextra -pedantic Task10_ExpectationVsExecution.cpp -o run_task10.exe
.\run_task10.exe

# Task 11: Final Design Recommendation & Defense
cd ../Task11
g++ -std=c++17 -Wall -Wextra -pedantic Task11_FinalRecommendation.cpp -o run_task11.exe
.\run_task11.exe

# Task 12: Day-4 Design Challenge (8 Marks)
cd ../Task12
g++ -std=c++17 -Wall -Wextra -pedantic Task12_DesignChallenge.cpp -o run_task12.exe
.\run_task12.exe
```

---
*Report submitted by Manoj Kumar Chinthala (Register Number: 192472008) in fulfillment of Assignment 1 (CO1 - Bloom's Level K5).*
