# ASSESSMENT 3 — CO3: Constructors, Destructors and Operator Overloading

## Student & Environment Metadata
- **Student Name:** Manoj Kumar Chinthala
- **Register Number:** 192472008
- **Course:** DSA01 — Object-Oriented Programming with C++ (CO3)
- **Institution:** SIMATS Engineering, Saveetha University
- **Repository Link:** [https://github.com/chinthalamanojkumar/DSA0103-C-.git](https://github.com/chinthalamanojkumar/DSA0103-C-.git)
- **Compiler & Version:** `g++ (MinGW-W64 16.2.0)`
- **Compilation Command:**
  ```bash
  g++ -std=c++17 -Wall -Wextra -pedantic TaskXX_Name.cpp -o TaskXX.exe
  ```
- **Execution Command:**
  ```bash
  .\TaskXX.exe
  ```

---

## Tasks & Results Summary

| Task Folder | Program Name | Key Concepts Demonstrated | Result Note / Status |
| :--- | :--- | :--- | :--- |
| **[Task01](./Task01/)** | `Task01_BookRecords.cpp` | Default, parameterised, and overloaded constructors; member initializer lists; delegation; explicit keyword; copies validation. | **PASSED:** All constructor situations initialized valid objects; negative copy invariant clamped safely to 0. |
| **[Task02](./Task02/)** | `Task02_PrescriptionCopies.cpp` | Copy constructors: shallow vs deep copy; Rule of Three (destructor, copy ctor, copy assignment); self-assignment guard (`p = p`). | **PASSED:** Complete the Code matched expected output (`Ibuprofen ORS...`); distinct memory buffers verified. |
| **[Task03](./Task03/)** | `Task03_AuditoriumBooking.cpp` | Dynamic 2D seat map constructor/destructor (`char**`); RAII lifecycle; stack, block, and heap allocations; Logger predict output. | **PASSED:** Logger output `+A +B -B copyAc use -Ac +P -P end -A` matched 100%; seat booking verified. |
| **[Task04](./Task04/)** | `Task04_MoneyArithmetic.cpp` | Overloading arithmetic (`+`, `-`, `+=`), comparison (`==`), and stream (`<<`, `>>`) operators; integer paise vs IEEE 754 drift audit. | **PASSED:** Complete the Code matched `Rs 200.00 true`; carry `0.75 + 0.50 = 1.25` passed with zero drift. |
| **[Task05](./Task05/)** | `Task05_PlaylistTimer.cpp` | Prefix and postfix increment (`++t`, `t++`); relational operators (`==`, `<`); const and non-const subscript `operator[]`. | **PASSED:** Counter predict output `5 7 7` matched; playlist boundary exceptions caught; rollover verified. |
| **[Task06](./Task06/)** | `Task06_TextSmsGateway.cpp` | Telecom Text class; Rules of operator overloading (non-overloadable operators: `::`, `.`, `.*`, `?:`, `sizeof`); self-assignment. | **PASSED:** Concatenation, equality, self-assignment passed; string benchmark and copy counts audited. |
| **[Task07](./Task07/)** | `Task07_ComplexACImpedance.cpp` | 4-Stage engineering design of Complex AC impedance calculator; Series RLC and Parallel RLC combinations; division by zero guard. | **PASSED:** Series RLC ($100 - 50j\,\Omega, |Z|=111.803$) and Parallel RLC ($50 + 50j\,\Omega, |Z|=70.711$) verified by hand. |

---

## Steps to Compile and Run Each Task

```bash
# Task 01:
cd Assessment3/Task01
g++ -std=c++17 -Wall -Wextra -pedantic Task01_BookRecords.cpp -o Task01.exe
.\Task01.exe

# Task 02:
cd ../Task02
g++ -std=c++17 -Wall -Wextra -pedantic Task02_PrescriptionCopies.cpp -o Task02.exe
.\Task02.exe

# Task 03:
cd ../Task03
g++ -std=c++17 -Wall -Wextra -pedantic Task03_AuditoriumBooking.cpp -o Task03.exe
.\Task03.exe

# Task 04:
cd ../Task04
g++ -std=c++17 -Wall -Wextra -pedantic Task04_MoneyArithmetic.cpp -o Task04.exe
.\Task04.exe

# Task 05:
cd ../Task05
g++ -std=c++17 -Wall -Wextra -pedantic Task05_PlaylistTimer.cpp -o Task05.exe
.\Task05.exe

# Task 06:
cd ../Task06
g++ -std=c++17 -Wall -Wextra -pedantic Task06_TextSmsGateway.cpp -o Task06.exe
.\Task06.exe

# Task 07:
cd ../Task07
g++ -std=c++17 -Wall -Wextra -pedantic Task07_ComplexACImpedance.cpp -o Task07.exe
.\Task07.exe
```
