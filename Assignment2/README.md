# ASSIGNMENT 2: Functions, Object-Oriented Concepts & Class Architecture

**Course Outcome:** CO2 — Design C++ programs by applying functions and object-oriented concepts – function prototyping, call by reference, return by reference, inline functions, default arguments, function overloading, specifying a class, member functions, access specifiers, arrays within a class, static data members and member functions, arrays of objects and friend functions.  
**Bloom’s Level Assessed:** K5 – Evaluate (All Tasks)  
**Total Marks:** 50  
**Submission Date:** 31/10/2026  

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
| [Task 01 (Q1)](#task-01-q1--call-by-value-by-reference-and-by-pointer) | Call by Value, Ref & Pointer | K5 | Concession updater (3 ways + return); Predict Output (`change`, `bigger`, `show`); Complete TODO 1; 896B object benchmark | **VERIFIED MATCH** (4.27x speedup) |
| [Task 02 (Q2)](#task-02-q2--return-by-reference-and-its-dangers) | Return by Ref & Dangers | K5 | Moderation on LHS (`+= 2`, `= 99`); `-Wreturn-local-addr` compiler audit; `std::vector` reallocation address invalidation proof | **VERIFIED MATCH** (Dangling ref demonstrated) |
| [Task 03 (Q3)](#task-03-q3--default-arguments-and-function-overloading) | Overloading & Default Arguments | K5 | Resolved ambiguity error; Predict 6 calls (`charge(2)`, `charge(2.0f)`); Overload resolution ranks; `ParcelOptions` struct | **VERIFIED MATCH** (Zero ambiguity) |
| [Task 04 (Q4)](#task-04-q4--specifying-a-class-with-an-array-inside-it) | Class with Array Inside It | K5 | SavingsAccount (in/out member methods); 11th txn handling ($O(k)$ shift vs $O(1)$ ring buffer); Deque vs Circular benchmark | **VERIFIED MATCH** (4.32x speedup over shift) |
| [Task 05 (Q5)](#task-05-q5--static-members-and-arrays-of-objects) | Static Members & Object Arrays | K5 | Token predict output (`101 103 104 4`); Complete `BusTicket` TODO 1 & 2; Static lifetime across functions; `BusRoute` multi-bus | **VERIFIED MATCH** (Exact match) |
| [Task 06 (Q6)](#task-06-q6--friend-functions-and-inline-functions) | Friend & Inline Functions | K5 | Symmetric `Time` friend functions (`205 false`); Hand-verified tests; 10M call benchmark at `-O0` and `-O2` (5.60x speedup) | **VERIFIED MATCH** (Empirical timing proved) |
| [Task 07 (Q7)](#task-07-q7--day-4-design-challenge-diagnose-redesign-validate) | Day-4 Design Challenge | K5 | Diagnosed $O(k)$ shifting bottleneck; Zero-heap `CircularRingBuffer<T, Cap>`; Multi-capacity benchmark ($k=10$ to $1000$, up to 289x speedup) | **VERIFIED MATCH** (Comprehensive defense) |

---

## Detailed Task-by-Task Evaluative Reports (K5 Level)

### Task 01 (Q1) — Call by Value, by Reference and by Pointer
**Source Directory:** [`Assignment2/Task01/`](./Task01/)  
**Primary Source File:** [`Task01_ConcessionUpdater.cpp`](./Task01/Task01_ConcessionUpdater.cpp)  
**Execution Log:** [`output.txt`](./Task01/output.txt)  

#### 1. Problem Context & Implementations
The hostel office applies concessions (Merit 10%, Sports 15%, Single Parent 25%) to student fees (base Rs. 45,000). Three implementations were constructed:
1. `applyConcessionByValue(long long fee, int percent)`: Parameter is a local copy allocated on the callee's stack frame. Modifying it has zero effect on the caller's variable.
2. `applyConcessionByReference(long long& fee, int percent)`: Parameter is an lvalue reference directly bound to the caller's variable address, modifying it in place.
3. `applyConcessionByPointer(long long* fee, int percent)`: Parameter receives the memory address of the caller's variable and mutates it via pointer dereferencing (`*fee`).
4. `applyConcessionReturn(long long fee, int percent)`: Pure functional alternative returning the updated fee, enabling method chaining.

#### 2. Inquiry Questions Answered
- **Parameter Storage:**
  - *By Value:* Parameter is placed in the activation frame (stack slot or register `RCX`/`RDI`) of the invoked function. Discarded upon function epilogue.
  - *By Reference:* Under the hood, the compiler passes the caller variable's memory address in a register. The reference acts as an alias bound to the caller's stack slot.
  - *By Pointer:* A local pointer variable on the callee stack frame stores the memory address passed by the caller.
- **Why only two change the caller's fee:**
  - Reference and pointer both establish direct indirection to the caller's lvalue memory location. Call-by-value creates an independent duplicate lvalue, leaving the caller untouched.
- **When a `const` reference should be used:**
  - Used for large structures/objects (`std::string`, `std::vector`, heavy `struct`) where copying is computationally expensive ($O(n)$ time/space), but the function only requires read-only access and guarantees not to modify the caller's data.

#### 3. Design Decision Defense: In-Place Mutation vs. Returning New Fee
- **In-Place Mutation (`long long& fee`):** Guarantees that the student's stored canonical record is directly updated, preventing discarded-result bugs.
- **Returning New Fee (`long long applyConcession(...)`):** Enables referential transparency, immutability, and function chaining:
  ```cpp
  fee = applyConcessionReturn(applyConcessionReturn(fee, 10), 15);
  ```
- **Defense:** For transaction processing systems where intermediate audit states must be verified before commit, the functional return pattern prevents partial corruption during multi-step concession calculations.

#### 4. Predict the Output Verification
- **Given Program:** `change(a, b, &c)`, `bigger(a, c) = 100`, `show(1)`, `show(1, 2)`, `show(1, 2, 3)`.
- **Predicted Output Before Running:**
  ```text
  1 12 13
  1 100
  16 13 6 
  ```
- **Observed Actual Output:** Identical exact match.
  - `change`: `a` modified locally (1); `b` modified via reference (12); `c` modified via pointer (13).
  - `bigger(a, c)` evaluates `1 > 13` (false), returning reference to `c`. The assignment `bigger(a, c) = 100` mutates `c` to 100.
  - `show`: `show(1)` uses defaults $y=5, z=10 \to 16$; `show(1, 2)` uses default $z=10 \to 13$; `show(1, 2, 3)` uses no defaults $\to 6$.

#### 5. Complete the Code (TODO 1)
- Completed `applyConcession(long long& fee, int percent)` using integer arithmetic:
  ```cpp
  fee -= (fee * percent) / 100;
  ```
- Output: `after call by value : 45000` | `after call by reference: 40500` (Verified exact match).

#### 6. Empirical Complexity & Memory Analysis (100,000 calls on 896-byte `StudentRecord`)
- `sizeof(long long)`: 8 bytes | `sizeof(StudentRecord)`: 896 bytes | `sizeof(StudentRecord*)`: 8 bytes.
- Benchmark: Pass by Value = **0.646 ms** | Pass by Reference = **0.151 ms** (4.27x speedup).

---

### Task 02 (Q2) — Return by Reference and Its Dangers
**Source Directory:** [`Assignment2/Task02/`](./Task02/)  
**Primary Source File:** [`Task02_MarksRegister.cpp`](./Task02/Task02_MarksRegister.cpp)  
**Execution Log:** [`output.txt`](./Task02/output.txt)  
**Compiler Warning Log:** [`compiler_warning.txt`](./Task02/compiler_warning.txt)  

#### 1. Array Return by Reference & Moderation
- Implemented `int& getHighestMark(int arr[], int n)` returning an lvalue reference to `arr[maxIdx]`.
- Demonstrated LHS assignment and in-place moderation:
  ```cpp
  getHighestMark(marks, 5) += 2; // Increases highest mark 92 -> 94
  getHighestMark(marks, 5) = 99; // Direct LHS assignment sets mark to 99
  ```

#### 2. Dangling Reference & Compiler Warning Evidence
- Captured g++ compiler warning in `compiler_warning.txt`:
  ```text
  Assignment2\Task02\dangling_ref_test.cpp:4:12: warning: reference to local variable 'temp' returned [-Wreturn-local-addr]
      4 |     return temp;
        |            ^~~~
  ```
- **Undefined Behaviour Explanation:** Local variable `temp` is allocated on the activation frame (stack). Upon return, the stack frame is popped. The returned reference points to reclaimed stack memory. Subsequent function calls overwrite this memory slot, leading to silent data corruption, unpredictable garbage values, or `SIGSEGV`.

#### 3. Return Semantics Contrast Matrix
| Feature | `int&` (Modifiable Reference) | `const int&` (Const Reference) | `int` (By Value / Copy) |
| :--- | :--- | :--- | :--- |
| **LHS Modifiable?** | Yes (`f() = val;`) | No (Compile error) | No (Rvalue prvalue) |
| **Copy Overhead** | 0 bytes (Direct memory binding)| 0 bytes (Direct memory binding)| Copies 4 bytes to register |
| **Safe for Locals?** | **FATAL: Undefined behaviour** | **FATAL: Undefined behaviour** | **100% Safe (independent copy)** |

#### 4. Changed Condition: `std::vector` Reallocation & Dangling Proof
- Empirically observed vector heap memory movement:
  - Initial buffer `vec.data()`: `0x2acd78175d0` (Capacity 4)
  - `highestRef` bound to: `0x2acd78175d8`
  - After appending 2 elements: New buffer `vec.data()` moved to `0x2acd7817f50` (Capacity 8)
  - `&highestRef` (`0x2acd78175d8`) remained pointing to the deallocated heap memory block!
  - **Conclusion:** Any read/write to `highestRef` after reallocation invokes Heap Use-After-Free.

#### 5. Alternative Concept: Index (`int`) vs Pointer (`int*`) vs Reference (`int&`)
- Finding highest mark takes $O(n)$ time and $O(1)$ space in all three approaches.
- **Safety Evaluation:** Returning an integer index (`int`) is **vastly superior** for dynamic containers (`std::vector`). The logical index remains valid across dynamic reallocations because elements preserve their logical index positions in the new heap buffer.

---

### Task 03 (Q3) — Default Arguments and Function Overloading
**Source Directory:** [`Assignment2/Task03/`](./Task03/)  
**Primary Source File:** [`Task03_CourierCharge.cpp`](./Task03/Task03_CourierCharge.cpp)  
**Execution Log:** [`output.txt`](./Task03/output.txt)  
**Compiler Error Log:** [`compiler_ambiguity_error.txt`](./Task03/compiler_ambiguity_error.txt)  

#### 1. Ambiguity Cause & Compiler Diagnostic Evidence
- Captured compiler error in `compiler_ambiguity_error.txt`:
  ```text
  error: call of overloaded 'charge(double)' is ambiguous
    candidate 1: 'double charge(double)'
    candidate 2: 'double charge(double, bool, bool)'
  error: call of overloaded 'charge(double, bool)' is ambiguous
    candidate 1: 'double charge(double, bool)'
    candidate 2: 'double charge(double, bool, bool)'
  ```
- **Explanation:** Candidate functions with default arguments become indistinguishable from shorter signatures after default argument substitution.

#### 2. Redesigned Unambiguous Function Overload Set
- `double charge(double weightKg, bool express = false, bool insured = false)`: Weight in kg.
- `double charge(int grams, bool express = false, bool insured = false)`: Weight in grams.
- `double charge(const Parcel& p)`: Weight and options bundled in a user-defined type.

#### 3. Six Predict-Before-Execution Calls
1. `charge(2.5)` $\to$ `charge(double, bool, bool)` (Exact match double: Rs. 125.00).
2. `charge(500)` $\to$ `charge(int, bool, bool)` (Exact match int grams: Rs. 30.00).
3. `charge(2)` $\to$ `charge(int, bool, bool)` (Literal `2` is `int`, exact match without conversion: Rs. 0.12).
4. `charge(2.0f)` $\to$ `charge(double, bool, bool)` (Literal `2.0f` is `float`. Float-to-double promotion rank takes precedence over float-to-int conversion with truncation: Rs. 100.00).
5. `charge(3.0, true)` $\to$ `charge(double, bool, bool=false)` (Express courier: Rs. 270.00).
6. `charge(Parcel{4.0, true, true, ...})` $\to$ `charge(const Parcel&)` (Rs. 400.00).
- **Status:** All 6 predictions matched actual execution with 100% precision.

#### 4. Inquiry & Alternative Concept (`ParcelOptions`)
- **Default Arguments from the Right:** C++ binds arguments positionally from left to right. Enforcing defaults from the right ensures that supplied arguments bind unambiguously to leading parameters.
- **Alternative `ParcelOptions` Struct:** Takes a single structure with default field values. Prevents "boolean trap" bugs (`charge(5.0, true, false)`), allows easy addition of new features (`fragileHandling`, `saturdayDelivery`) without signature bloat, and resolves at compile-time with 0 runtime cost.

---

### Task 04 (Q4) — Specifying a Class with an Array Inside It
**Source Directory:** [`Assignment2/Task04/`](./Task04/)  
**Primary Source File:** [`Task04_SavingsAccount.cpp`](./Task04/Task04_SavingsAccount.cpp)  
**Execution Log:** [`output.txt`](./Task04/output.txt)  

#### 1. Class Specification & Encapsulation
- Specified `SavingsAccount` with private data:
  - `long long accountNumber;`
  - `double balance;`
  - `double transactions[10];` (stores signed amounts)
  - `int count;`
- Member functions defined inside class: `deposit(double amount)`.
- Member functions defined outside using scope resolution `::`: `withdraw(double amount)` and `miniStatement() const`.

#### 2. Inquiry & 11th Transaction Handling
- **Why Array Must Be Private:** Prevents external tampering with the financial audit ledger, preserving double-entry accounting integrity and hiding internal data structure representation.
- **11th Transaction Cost:**
  - *Shifting Array:* Copies 9 elements leftwards (`transactions[i] = transactions[i+1]`). Time complexity: $O(k)$ data moves on every single transaction ($k=10$).
  - *Circular Ring Buffer:* Updates `transactions[head]` and advances `head = (head + 1) % 10`. Time complexity: strictly $O(1)$ constant time with zero copying.
- **Scaling to 50 Transactions:** For shifting array, every transaction forces 49 memory copies ($5\times$ more overhead). For circular buffer, only `CAPACITY = 50` is modified; insertion cost remains invariant at $O(1)$.

#### 3. Empirical Benchmark (500,000 Transactions)
- Shifting Array ($k=10$): **6.885 ms**
- Circular Buffer ($k=10$): **1.592 ms** (4.32x faster than shifting array)
- Circular Buffer ($k=50$): **1.515 ms** (Constant $O(1)$ runtime invariant)
- `std::deque`: **11.449 ms** (7.19x slower due to dynamic heap node allocation and pointer traversal)

---

### Task 05 (Q5) — Static Members and Arrays of Objects
**Source Directory:** [`Assignment2/Task05/`](./Task05/)  
**Primary Source File:** [`Task05_BusTickets.cpp`](./Task05/Task05_BusTickets.cpp)  
**Execution Log:** [`output.txt`](./Task05/output.txt)  

#### 1. Core Principle & Token Prediction
- **Principle:** "Data that belongs to the whole class – not to any one object – should exist only once, however many objects are created."
- **Token Prediction Output:** `101 103 104 4` (Predicted before running, verified exact match).
  - Static counter starts at 100 via file-scope definition `int Token::next = 100;`.
  - Constructor ran 4 times (3 array objects + 1 extra object).

#### 2. Complete the Code (`BusTicket`)
- Implemented TODO 1 (Constructor) & TODO 2 (`count()`):
  ```cpp
  if (issued < CAPACITY) {
      seat = ++issued;
  } else {
      seat = 0; // waiting list
  }
  ```
- Output matches specification:
  ```text
  Asha: Seat 1
  Bala: Seat 2
  Chitra: Seat 3
  Dev: Waiting
  Issued: 3
  ```

#### 3. Function Called Twice Verification & Static Lifetime
- Created tickets inside `issueBatch()` called twice.
- Observed: During call 1, passengers receive Seat 1 and Seat 2. During call 2, `BusTicket::issued` persists across stack frame deallocations, correctly assigning Seat 3 to Passenger 2A and placing Passenger 2B on the Waiting List.

#### 4. Inquiry & Multi-Bus Isolation Challenge
- **Why static functions cannot use `this`:** Static member functions bind to the class type itself, not an instance. The compiler passes no `this` pointer in the call instruction, leaving no base address to calculate member variable offsets.
- **Multi-Bus Concurrency Flaw:** Static variables tie state to the entire class type. Running Route 101 and Route 202 simultaneously corrupts seat counts because tickets from Route 202 consume seats from Route 101.
- **Alternative Solution:** Implemented `BusRoute` class where capacity and issued counts are instance variables per route, supporting concurrent fleets while maintaining encapsulation.

---

### Task 06 (Q6) — Friend Functions and Inline Functions
**Source Directory:** [`Assignment2/Task06/`](./Task06/)  
**Primary Source File:** [`Task06_TrainPlanner.cpp`](./Task06/Task06_TrainPlanner.cpp)  
**Benchmark Source:** [`benchmark_inline.cpp`](./Task06/benchmark_inline.cpp)  
**Execution Log:** [`output.txt`](./Task06/output.txt)  
**Inline Benchmark Log:** [`inline_benchmark_results.txt`](./Task06/inline_benchmark_results.txt)  

#### 1. Complete the Code Verification
- Implemented `minutesBetween` and `earlier`:
  ```cpp
  inline int minutesBetween(const Time& a, const Time& b) {
      int totalA = a.h * 60 + a.m;
      int totalB = b.h * 60 + b.m;
      int diff = totalA - totalB;
      return diff < 0 ? -diff : diff;
  }
  ```
- Tested: `Time departure(9, 45), arrival(13, 10)` $\to$ Output: `205 false` (Verified exact match).

#### 2. Correctness & Boundary Tests
- Either side of noon (11:45 and 12:15): Diff = 30 min, `earlier` = true (PASSED).
- Same time twice (14:30 and 14:30): Diff = 0 min, `earlier` = false (PASSED).
- Extremes of day (00:05 and 23:55): Diff = 1430 min, `earlier` = true (PASSED).

#### 3. Growth Investigation: 10,000,000 Call Benchmark (-O0 vs -O2)
| Compilation Level | Without Inline (ms) | With Inline (ms) | Latency per Call | Speedup Factor |
| :--- | :---: | :---: | :---: | :---: |
| **`-O0` (Unoptimized Debug)** | 17.346 ms | 23.340 ms | 2.33 ns | 0.74x |
| **`-O2` (Optimized Release)** | 8.684 ms | **1.551 ms** | **0.155 ns** | **5.60x** |

- **Analysis of Result:** In unoptimized `-O0` mode, inline expansion increases register pressure without global optimization. In `-O2` mode, inlining enables the optimizer to perform constant folding, dead-code elimination, and register-allocated arithmetic, yielding a massive **5.60x performance speedup** (0.155 ns per call).

#### 4. Deployment Decision Defense
- We defend retaining the **friend function** design for railway connection engines. It enforces symmetric syntax (`minutesBetween(a, b)`), preserves private encapsulation of `h` and `m`, and eliminates call overhead via inlining.

---

### Task 07 (Q7) — Day-4 Design Challenge: Diagnose, Redesign, Validate
**Source Directory:** [`Assignment2/Task07/`](./Task07/)  
**Primary Source File:** [`Task07_DesignChallenge.cpp`](./Task07/Task07_DesignChallenge.cpp)  
**Execution Log:** [`output.txt`](./Task07/output.txt)  

#### Part A: Diagnose the Limitation (2 Marks)
- In Task 04, the banking system records recent transactions using a fixed array. When full, every subsequent transaction executes an array shift:
  ```cpp
  for (size_t i = 0; i < Cap - 1; ++i) history[i] = history[i + 1];
  history[Cap - 1] = t;
  ```
- **Root Cause:** $O(k)$ linear time complexity per transaction, where $k$ is ledger capacity.
- **Empirical Failure Point:** In real-world banking ledgers requiring $k=50$ (mobile passbook), $k=200$ (commercial accounts), or $k=1000$ (algorithmic high-frequency trading audit trails), inserting a single transaction triggers up to 999 memory copies, causing CPU cache line thrashing and memory bus saturation.

#### Part B: Define the Measurable Design Objective (2 Marks)
1. Reduce transaction insertion time complexity from $O(k)$ to strictly $O(1)$ constant time.
2. Maintain zero dynamic heap allocations (deterministic stack/embedded allocation with 0 heap fragmentation).
3. Achieve $\ge 4\times$ speedup at $k=50$ and $\ge 50\times$ speedup at $k=1000$ over 100,000 transactions.
4. Guarantee 100% chronological FIFO preservation for mini-statements.

#### Part C: Design the Revised Solution (4 Marks)
- Designed a zero-allocation, bounded `CircularRingBuffer<T, Cap>` template using non-type template parameters:
  ```cpp
  template <typename T, size_t Cap>
  class CircularRingBuffer {
      T buffer[Cap];
      size_t head = 0;
      size_t count = 0;
  public:
      void push(const T& item) {
          buffer[head] = item;
          head = (head + 1) % Cap;
          if (count < Cap) ++count;
      }
      const T& operator[](size_t i) const {
          size_t start = (count < Cap) ? 0 : head;
          return buffer[(start + i) % Cap];
      }
  };
  ```
- **Class Relationships:**
  ```text
  +-----------------------------+        +---------------------------+
  |      RedesignedAccount      |        | CircularRingBuffer<T,Cap> |
  +-----------------------------+        +---------------------------+
  | - accountNumber: long long  | <>---> | - buffer: T[Cap]          |
  | - balance: double           |        | - head: size_t            |
  | - history: RingBuffer<Txn,K>|        | - count: size_t           |
  +-----------------------------+        +---------------------------+
  | + recordTransaction(amount) |        | + push(item): O(1)        |
  | + printStatement(): void    |        | + operator[](idx): const& |
  +-----------------------------+        +---------------------------+
  ```

#### Part D: Predict Before Implementing (1 Mark)
- Shifting array runtime will scale linearly with $k$: $T_{shift}(N, k) = \Theta(N \cdot k)$.
- Circular ring buffer runtime will remain constant across all $k$: $T_{ring}(N, k) = \Theta(N)$.
- Expected Speedup: $\sim 3-5\times$ at $k=10$, $\sim 15-20\times$ at $k=50$, $>100\times$ at $k=1000$.

#### Part E: Implement and Validate the Redesign (2 Marks)
- **Chronological Verification:** Validated that both original and redesigned accounts produce 100% identical ATM mini-statements across interleaved deposits and withdrawals.
- **Empirical Multi-Scale Benchmark (100,000 Transactions per Test):**

| Capacity ($k$) | Original Shifting Array | Circular Ring Buffer | Speedup Factor |
| :---: | :---: | :---: | :---: |
| **$k = 10$** | 3.11 ms (32.18M ops/s) | **0.54 ms** (185.49M ops/s) | **5.76x** |
| **$k = 50$** | 11.65 ms (8.58M ops/s) | **0.64 ms** (156.08M ops/s) | **18.19x** |
| **$k = 200$** | 45.65 ms (2.19M ops/s) | **0.41 ms** (241.66M ops/s) | **110.31x** |
| **$k = 1000$**| 156.33 ms (0.64M ops/s) | **0.54 ms** (185.19M ops/s) | **289.50x** |

#### Part F: Defend the Final Design (2 Marks)
1. **Objective Met:** Transitioned insertion from $O(k)$ to $O(1)$. At $k=1000$, throughput jumped from 0.64M ops/s to 185M ops/s (**289.5x speedup**).
2. **Deterministic Memory:** Zero heap allocations ensure safety for mission-critical embedded banking hardware.
3. **Where Original Might Still Be Used:** For tiny buffers ($k \le 3$), array shifting requires no index arithmetic and has a slightly smaller flash ROM footprint on microcontrollers without hardware division. For all other production systems, the Circular Ring Buffer is mathematically and architecturally optimal.

---

## Instructions to Build and Verify All Tasks

Each task can be compiled and executed independently from the repository root:

```bash
# Task 01: Concession Updater
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task01/Task01_ConcessionUpdater.cpp -o Task01.exe
.\Task01.exe

# Task 02: Marks Register
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task02/Task02_MarksRegister.cpp -o Task02.exe
.\Task02.exe

# Task 03: Courier Calculator
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task03/Task03_CourierCharge.cpp -o Task03.exe
.\Task03.exe

# Task 04: Savings Account
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task04/Task04_SavingsAccount.cpp -o Task04.exe
.\Task04.exe

# Task 05: Bus Tickets
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task05/Task05_BusTickets.cpp -o Task05.exe
.\Task05.exe

# Task 06: Train Planner
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task06/Task06_TrainPlanner.cpp -o Task06.exe
.\Task06.exe

# Task 07: Day-4 Design Challenge
g++ -std=c++17 -Wall -Wextra -pedantic Assignment2/Task07/Task07_DesignChallenge.cpp -o Task07.exe
.\Task07.exe
```
