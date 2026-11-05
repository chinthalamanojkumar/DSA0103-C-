# ASSESSMENT 2 — CO2: C++ Functions and Object-Oriented Programming

## Student & Environment Metadata
- **Student Name:** Manoj Kumar Chinthala
- **Register Number:** 192472008
- **Course:** DSA01 — Object-Oriented Programming with C++ (CO2)
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
| **[Task01](./Task01/)** | `Task01_ConcessionUpdater.cpp` | Call by value, by reference, and by pointer; Complete TODO 1; predictions verification. | **PASSED:** Base 45000 -> By value 45000, by reference 40500. Output matched expected. |
| **[Task02](./Task02/)** | `Task02_MarksRegister.cpp` | Return by reference, LHS assignment (`+= 2`, `= 99`), dangling local reference analysis, vector growth safety. | **PASSED:** Highest mark moderated in place; vector reallocation reference invalidation demonstrated. |
| **[Task03](./Task03/)** | `Task03_CourierCharge.cpp` | Resolving compiler ambiguity, default arguments from right, function overloading with `int`, `double`, and `Parcel`. | **PASSED:** All 6 predict-before-execution calls matched exact expected output with zero ambiguity. |
| **[Task04](./Task04/)** | `Task04_SavingsAccount.cpp` | Class with array inside it, member functions defined in/out via `::`, shifting array vs circular buffer. | **PASSED:** Mini-statement correctly retained 10 recent transactions; oldest transactions discarded. |
| **[Task05](./Task05/)** | `Task05_BusTickets.cpp` | Static data members and functions, Token output prediction, Complete `BusTicket` TODO 1 & 2, waiting list logic. | **PASSED:** Token output `101 103 104 4` matched; 3 seats issued, 4th placed on waiting list. |
| **[Task06](./Task06/)** | `Task06_TrainPlanner.cpp` | Friend functions for symmetric operations (`minutesBetween`, `earlier`), inline function optimization at `-O0` and `-O2`. | **PASSED:** Complete the Code matched expected output (`205 false`); verified noon boundary. |
| **[Task07](./Task07/)** | `Task07_DesignChallenge.cpp` | Day-4 Design Challenge: Diagnosed $O(k)$ shifting bottleneck; redesigned zero-heap `CircularRingBuffer<T, Cap>`. | **PASSED:** Identical chronological statements; achieved up to 289x speedup over shifting array. |

---

## Steps to Compile and Run Each Task

```bash
# Task 01:
cd Assessment2/Task01
g++ -std=c++17 -Wall -Wextra -pedantic Task01_ConcessionUpdater.cpp -o Task01.exe
.\Task01.exe

# Task 02:
cd ../Task02
g++ -std=c++17 -Wall -Wextra -pedantic Task02_MarksRegister.cpp -o Task02.exe
.\Task02.exe

# Task 03:
cd ../Task03
g++ -std=c++17 -Wall -Wextra -pedantic Task03_CourierCharge.cpp -o Task03.exe
.\Task03.exe

# Task 04:
cd ../Task04
g++ -std=c++17 -Wall -Wextra -pedantic Task04_SavingsAccount.cpp -o Task04.exe
.\Task04.exe

# Task 05:
cd ../Task05
g++ -std=c++17 -Wall -Wextra -pedantic Task05_BusTickets.cpp -o Task05.exe
.\Task05.exe

# Task 06:
cd ../Task06
g++ -std=c++17 -Wall -Wextra -pedantic Task06_TrainPlanner.cpp -o Task06.exe
.\Task06.exe

# Task 07:
cd ../Task07
g++ -std=c++17 -Wall -Wextra -pedantic Task07_DesignChallenge.cpp -o Task07.exe
.\Task07.exe
```
