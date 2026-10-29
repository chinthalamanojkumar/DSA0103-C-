# Task 02: Tokens, Keywords, Identifiers and Constants

**Student Name:** Manoj Kumar Chinthala  
**Register Number:** 192472008  
**Course:** DSA01 - Object-Oriented Programming with C++  
**Bloom's Level:** K5 – Evaluate  

---

## 1. Five Kinds of C++ Tokens

A token is the smallest lexical element recognized by the C++ compiler. The five token classes present in the metro fare module are:

1. **Keywords:** Reserved words with fixed language semantics that cannot be repurposed.
   - *Examples in code:* `constexpr`, `enum`, `class`, `static_cast`, `bool`, `long`, `switch`, `return`.
2. **Identifiers:** Programmer-defined names given to entities (types, variables, constants, functions).
   - *Examples in code:* `MetroZone`, `METRO_BASE_FARE_PAISE`, `PEAK_SURCHARGE_PERCENT`, `balancePaise`.
   - *Rules:* Must begin with a letter or underscore, cannot contain special characters or spaces, cannot collide with keywords, case-sensitive.
   - *Conventions:* `PascalCase` for types (`MetroZone`), `UPPER_SNAKE_CASE` for compile-time constants (`MINIMUM_CARD_REMAINDER_PAISE`), `camelCase` for variables and member functions (`calculateFare`, `balancePaise`).
3. **Constants / Literals:** Invariable data values embedded directly in source code.
   - *Examples in code:* `1000LL` (integer literal with `long long` suffix), `20` (integer literal), `5000LL`, `"Inner"` (string literal), `'A'` (character literal).
4. **Operators:** Symbols specifying mathematical, relational, logical, or type operations.
   - *Examples in code:* `[]` (subscript), `=` (assignment), `+` (addition), `*` (multiplication), `<` (relational), `&&` (logical AND), `||` (logical OR), `::` (scope resolution).
5. **Punctuators / Separators:** Syntactic characters that delimit code structure.
   - *Examples in code:* `{` and `}` (compound statement blocks), `(` and `)` (function parameter / precedence delimiters), `;` (statement terminator), `,` (list separator).

---

## 2. Evaluation: `#define` vs. `const` vs. `constexpr`

| Characteristic | `#define BASE_FARE 1000` | `const long long BASE_FARE = 1000;` | `constexpr long long BASE_FARE = 1000;` |
| :--- | :--- | :--- | :--- |
| **Stage of Processing** | Preprocessor textual substitution (before compilation). | Compiler semantic analysis. | Compiler semantic analysis & compile-time constant expression evaluator. |
| **Type Safety** | **None.** Untyped macro token; susceptible to subtle operator precedence bugs unless heavily parenthesized. | **Strong.** Fully typed; type conversions checked by compiler. | **Strong.** Fully typed; guaranteed compile-time validation. |
| **Scope & Encapsulation** | **Global leakage.** Disregards namespaces and class scopes; pollutes entire compilation unit from definition downwards. | Obeys C++ scope (class, namespace, block). | Obeys C++ scope (can be `public` or `private` member of class/struct). |
| **Debugger Symbol Table** | **Absent.** Replaced with raw digits; invisible in GDB / debuggers. | Present in symbol table if address is taken or in debug builds. | Present in symbol table; inspectable during symbolic debugging. |
| **Guaranteed Compile-Time Evaluation** | Yes (textual substitution). | Not guaranteed (may be evaluated at runtime if initialized with a runtime expression). | **Strictly guaranteed.** Can be used in array bounds, template arguments, and `static_assert`. |

### Evaluative Verdict:
`constexpr` is definitively superior for the fare table. It enforces strict type checking, participates in namespace isolation, allows `static_assert` verification during compilation, and eliminates runtime lookup latency with zero memory footprint.

---

## 3. Evaluation: `enum` vs. `enum class` for Zones

- **Unscoped `enum Zone { Inner, Middle, Outer };`:**
  - Enumerators leak directly into the enclosing scope (colliding with identifiers like `Inner` elsewhere).
  - Implicitly converts to `int`, allowing nonsensical operations like `Zone::Inner + 5` or `Zone::Middle == FoodItem::BURGER` without any compiler diagnostic.
- **Scoped `enum class MetroZone : unsigned char { Inner, Middle, Outer };`:**
  - Names are strictly scoped: `MetroZone::Inner`.
  - **No implicit conversion to `int`:** Comparing a `MetroZone` to an unrelated integer or enum yields a compile-time error.
  - Underlying storage type is explicitly specified as `unsigned char` (1 byte per enum instance instead of default 4-byte `int`), saving cache and memory space.

---

## 4. Alternative Concept & Complexity Analysis

### Alternative: All Fares via `#define` Preprocessor Directives
```cpp
#define FARE_INNER  1000
#define FARE_MIDDLE 2000
#define FARE_OUTER  3000
#define SURCHARGE   20
```

### Analysis of Trade-offs:
1. **Type Checking & Scope:**
   - `#define` lacks type metadata. If someone writes `#define FARE_INNER 10.00`, a downstream integer division or modulo operation will break silently or produce surprising implicit floating-point conversions.
   - `#define` cannot be scoped within `class FareTable`. A third-party library header defining `#define FARE_INNER` would cause compilation failure or catastrophic symbol collisions.
2. **Memory Footprint:**
   - `#define`: 0 bytes of dedicated storage (substituted directly as immediate operands in assembly).
   - `constexpr`: Also 0 bytes of dedicated storage if its address is not taken (the compiler folds `METRO_BASE_FARE_PAISE[0]` directly into an immediate instruction constant `$1000` in x86-64 assembly).
3. **Execution Time:**
   - Both approaches execute in $O(1)$ constant time with identical machine instruction efficiency.
   - However, `constexpr` guarantees compile-time correctness via `static_assert` and catches typos during build time rather than causing post-deployment financial discrepancy.
