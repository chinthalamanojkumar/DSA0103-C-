# ASSESSMENT 4 — CO4: Designing Reusable Class Hierarchies

## Student & Environment Metadata
- **Student Name:** Manoj Kumar Chinthala
- **Register Number:** 192472008
- **Course:** DSA01 — Object-Oriented Programming with C++ (CO4)
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
| **[Task01](./Task01/)** | `Task01_ClassDesignReview.cpp` | Diagnosis and refactoring of duplicated payroll classes; unified `Employee` base class; abstract polymorphism; `SalariedEmployee` vs `HourlyEmployee`. | **PASSED:** All 5 design deficiencies resolved; payroll totals verified; polymorphism array executes in $O(1)$ per employee. |
| **[Task02](./Task02/)** | `Task02_UniversityHierarchy.cpp` | Multi-level, hierarchical, and multiple inheritance; Diamond Problem resolution via `virtual` base inheritance (`Person`); `TeachingAssistant` dual role. | **PASSED:** Complete the Code exact match; single `Person` subobject verified; ambiguity eliminated without memory duplication. |
| **[Task03](./Task03/)** | `Task03_OrderOfConstruction.cpp` | Exact construction and destruction sequence across base classes, member objects, and derived classes; static member construction timing. | **PASSED:** Predict output matched 100% with C++ standard order: `Base1` $\to$ `Base2` $\to$ Member $\to$ Derived, destroyed in strict reverse order. |
| **[Task04](./Task04/)** | `Task04_PolymorphismForensics.cpp` | Static vs dynamic binding; upcasting with base-class pointers; object slicing prevention; memory forensic of virtual destructor vs leak. | **PASSED:** Complete the Code matched `Woof`; virtual destructor verified to execute both `~Dog()` and `~Animal()` without leaks. |
| **[Task05](./Task05/)** | `Task05_ThisPointerCart.cpp` | Four ways to reach an object; `this` pointer for method chaining (`Cart& add(...)`); member disambiguation (`this->discount`); array of pointers search. | **PASSED:** Complete the Code matched expected output `324.00`; highest total customer cart identified accurately. |
| **[Task06](./Task06/)** | `Task06_VehicleRentalFleet.cpp` | 5-Stage engineering design of Vehicle Rental Fleet; abstract base class `Vehicle` with pure virtual functions; array of base-class pointers; Open-Closed Principle (OCP) extension (`ElectricBus`). | **PASSED:** Polymorphic rental and return billing with late fees verified; dynamic extension class added with zero edits to existing classes. |

---

## Steps to Compile and Run Each Task

```bash
# Task 01: Class Design Review
cd Assessment4/Task01
g++ -std=c++17 -Wall -Wextra -pedantic Task01_ClassDesignReview.cpp -o Task01.exe
.\Task01.exe

# Task 02: University Hierarchy (Diamond Problem)
cd ../Task02
g++ -std=c++17 -Wall -Wextra -pedantic Task02_UniversityHierarchy.cpp -o Task02.exe
.\Task02.exe

# Task 03: Order of Construction & Destruction
cd ../Task03
g++ -std=c++17 -Wall -Wextra -pedantic Task03_OrderOfConstruction.cpp -o Task03.exe
.\Task03.exe

# Task 04: Polymorphism Forensics & Virtual Destructor
cd ../Task04
g++ -std=c++17 -Wall -Wextra -pedantic Task04_PolymorphismForensics.cpp -o Task04.exe
.\Task04.exe

# Task 05: Pointers to Objects & 'this' Pointer Chaining
cd ../Task05
g++ -std=c++17 -Wall -Wextra -pedantic Task05_ThisPointerCart.cpp -o Task05.exe
.\Task05.exe

# Task 06: Vehicle Rental Fleet Design Challenge (K5)
cd ../Task06
g++ -std=c++17 -Wall -Wextra -pedantic Task06_VehicleRentalFleet.cpp -o Task06.exe
.\Task06.exe
```
