# Task 01: Object-Oriented Domain Analysis & Verification

**Student Name:** Manoj Kumar Chinthala  
**Register Number:** 192472008  
**Course:** DSA01 - Object-Oriented Programming with C++  
**Bloom's Level:** K5 – Evaluate  

---

## 1. Domain Problem Overview
The Namma Metro Smart-Card fare logic requires calculating dynamic fares across three network zones (`Inner`, `Middle`, `Outer`) with a 20% surcharge during peak hours (08:00–10:00 and 17:00–20:00). A stored-value smart card must never drop below a mandatory safety threshold of **Rs 50.00** remaining after any trip deduction. The original hasty implementation exhibited off-by-a-paisa discrepancies, inconsistent boundary conditions (treating 10:00 AM as peak and 20:00 as off-peak), duplicate hardcoded constants, and illegal negative balances.

---

## 2. Object Identification & Responsibilities

| Object / Entity | Type / Class | Encapsulated State (Data) | Public Interface (Behavior) | Domain Justification |
| :--- | :--- | :--- | :--- | :--- |
| **Zone** | `enum class Zone` | Value set: `Inner` (0), `Middle` (1), `Outer` (2). | Type-safe strongly typed token; prevents arithmetic operations or invalid integer assignments. | Eliminates magic numbers and restricts travel destinations to valid geography. |
| **SmartCard** | `class SmartCard` | `cardId` (`std::string`), `balancePaise` (`long long` private). | `canTravel(fare)`, `deductFare(fare)`, `recharge(paise)`, `getBalancePaise()`. | Protects the card balance from unauthorized external mutation; enforces the non-negative and Rs 50 minimum balance invariant. |
| **Passenger** | `class Passenger` | `name` (`std::string`), `type` (`enum class PassengerType`). | `getName()`, `getType()`. | Encapsulates concession eligibility (Student, Senior Citizen, Regular). |
| **FareTable** | `class FareTable` | `BASE_FARES_PAISE[]`, `PEAK_SURCHARGE_PCT`, `CONCESSIONS`. | `calculateFare(zone, hour, passengerType)`, `isPeakHour(hour)`. | Single source of truth for fare calculation; isolates business pricing policies from gate turnstile hardware. |
| **Journey** | `class Journey` | `journeyId`, `destinationZone`, `entryHour24`. | `process(passenger, card)`. | Models a single transactional transit event; links passenger, smart card, pricing rules, and time. |

---

## 3. Procedural vs. Object-Oriented Style Comparison

| Dimension | Procedural Style (C-Style) | Object-Oriented Style (C++ OOP) | Evaluative Impact |
| :--- | :--- | :--- | :--- |
| **State Storage** | Global variables or raw structs with public members (e.g., `double g_balance`). | Private member variables encapsulated inside `SmartCard` and `FareTable`. | In procedural code, any function can accidentally set `balance = -100` or forget the Rs 50 check. OOP makes invalid states unrepresentable. |
| **Coupling** | High coupling; fare calculation logic is interspersed with balance deduction and gate I/O across free functions. | Loose coupling; `FareTable` only computes fares, `SmartCard` only enforces balance rules, and `Journey` orchestrates. | Changing a fare rule in procedural requires touching multiple free functions. In OOP, only `FareTable` is updated. |
| **Invariant Safety** | Invariants must be manually checked before every mutation (`if (bal - f >= 50) bal -= f;`). | Enforced internally by `SmartCard::deductFare()`. External code cannot bypass the safety check. | Procedural approach fails when a junior programmer writes `bal -= f` without the guard. |
| **Code Churn** | Adding a zone or concession requires auditing and rewriting callsites throughout the entire program. | Polymorphic extension or modifying a single configuration array in `FareTable`. | Dramatically lower cyclomatic complexity and regression risk. |

---

## 4. Fundamental OOP Principles Applied

1. **Encapsulation:**
   The `SmartCard` class keeps `balancePaise` strictly `private`. The gate cannot directly perform `card.balancePaise -= fare`. It must invoke `card.deductFare(fare)`, which guarantees that `(balancePaise - fare) >= 5000` is satisfied before deduction.

2. **Abstraction:**
   The turnstile gate does not need to know whether the fare is computed via a lookup table, piecewise formulas, or distance calculations. It simply calls `FareTable::calculateFare(zone, hour, passengerType)`. The mathematical complexity of surcharges and concessions is abstracted away.

3. **Inheritance & Extensibility:**
   Specialized cards (e.g., `MonthlyPassCard`, `TouristCard`, `StudentSmartCard`) can derive from a base `Card` interface, overriding deduction policies while inheriting identity and balance ledgering.

4. **Polymorphism:**
   Different fare calculation strategies (Standard, Concession, Weekend Discount) can be dispatched dynamically or statically through polymorphic discount evaluators, allowing new discount types to be plugged in without modifying the core gate loop.

---

## 5. System Invariants & Rules

1. **Minimum Post-Journey Balance Rule:** A journey is permitted if and only if:
   $$\text{balance}_{\text{post}} = \text{balance}_{\text{current}} - \text{fare} \ge 5000\text{ paise (Rs 50.00)}$$
2. **Deterministic Peak Interval Rule:**
   $$\text{Peak} \iff (\text{hour} \in [8, 10)) \lor (\text{hour} \in [17, 20))$$
   - Exactly 08:00 is Peak; 09:59 is Peak; **10:00 is Off-Peak**.
   - Exactly 17:00 is Peak; 19:59 is Peak; **20:00 is Off-Peak**.
3. **Exact Monetary Storage Rule:** All monetary quantities are strictly represented in integer paise (`long long`) to eliminate IEEE 754 floating-point drift.
4. **Farthest Zone Pricing Rule:** The fare is charged according to the farthest zone reached during the trip.

---

## 6. Alternative Concept & Complexity Analysis

### Alternative: Purely Procedural Architecture (Global Arrays & Free Functions)
```cpp
// Procedural representation
long long g_card_balances[MAX_CARDS];
int g_fare_table[3] = {1000, 2000, 3000};

bool procedural_deduct(int card_idx, int zone, int hour) {
    long long f = g_fare_table[zone];
    if ((hour >= 8 && hour < 10) || (hour >= 17 && hour < 20)) f += f * 20 / 100;
    if (g_card_balances[card_idx] - f >= 5000) {
        g_card_balances[card_idx] -= f;
        return true;
    }
    return false;
}
```

### Complexity & Impact Evaluation:
- **Time Complexity:**
  - **Procedural:** $O(1)$ constant time per journey (direct array index access and arithmetic).
  - **Object-Oriented:** $O(1)$ constant time per journey (inlined member function calls with zero overhead).
- **Space Complexity:**
  - **Procedural:** $O(1)$ auxiliary space per card (8 bytes for `long long`).
  - **Object-Oriented:** $O(1)$ space per card (object memory layout is identical to the raw data member with 0-byte vtable overhead if non-virtual).
- **Maintainability & Ripple Effect:**
  - When the minimum balance or surcharge formula changes, procedural code requires scanning all free functions where `g_card_balances` is manipulated. The ripple effect is severe (risk of bypassing the check in new transit modules).
  - In OOP, the change is localized to a single method `SmartCard::canTravel()` and `FareTable::calculateFare()`. The code churn is bounded to exactly 1 translation unit, guaranteeing zero regressions across the codebase.
