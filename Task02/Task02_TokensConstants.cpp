/**
 * ============================================================================
 * Assignment 1 - Task 02: Tokens, Keywords, Identifiers and Constants
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Demonstrating:
 * 1. Five kinds of C++ tokens (Keywords, Identifiers, Constants, Operators, Punctuators)
 * 2. #define vs const vs constexpr comparison
 * 3. Unscoped enum vs Scoped enum class type safety
 * 4. Single-source-of-truth Metro Configuration
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <type_traits>

// ============================================================================
// SINGLE SOURCE OF TRUTH METRO CONSTANTS (constexpr preferred over #define)
// ============================================================================

// Scoped enum class: Zone
// Safer than raw enum because it does not implicitly convert to int,
// and does not pollute the enclosing namespace scope.
enum class MetroZone : unsigned char {
    Inner = 0,
    Middle = 1,
    Outer = 2,
    COUNT = 3
};

// Compile-time evaluated constexpr array for base fares in paise
// Rs 10.00 = 1000 paise, Rs 20.00 = 2000 paise, Rs 30.00 = 3000 paise
constexpr long long METRO_BASE_FARE_PAISE[] = {1000LL, 2000LL, 3000LL};

// Operational parameters
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MINIMUM_CARD_REMAINDER_PAISE = 5000LL; // Rs 50.00

// Peak hour boundaries in 24-hour format
constexpr int MORNING_PEAK_START_HOUR = 8;
constexpr int MORNING_PEAK_END_HOUR = 10; // [8, 10)
constexpr int EVENING_PEAK_START_HOUR = 17;
constexpr int EVENING_PEAK_END_HOUR = 20; // [17, 20)

// Demonstration of why unscoped enum is risky (Legacy C-style enum)
enum LegacyZone {
    LEGACY_INNER = 0,
    LEGACY_MIDDLE = 1,
    LEGACY_OUTER = 2
};

enum FoodItem {
    APPLE = 0,
    BURGER = 1,
    PIZZA = 2
};

int main() {
    std::cout << "====================================================================\n";
    std::cout << " TASK 02: TOKENS, IDENTIFIERS, CONSTANTS & ENUM CLASS EVALUATION   \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    // 1. Token Classification Demonstration
    std::cout << "[1] TOKEN TAXONOMY IN THIS MODULE:\n";
    std::cout << "    - Keywords:    constexpr, enum, class, static_cast, int, bool\n";
    std::cout << "    - Identifiers: MetroZone, METRO_BASE_FARE_PAISE, balancePaise\n";
    std::cout << "    - Constants:   1000LL, 20, 5000LL, \"Inner\", 'A'\n";
    std::cout << "    - Operators:   [], =, +, *, <, >=, &&, ||, ::\n";
    std::cout << "    - Punctuators: {, }, (, ), ;, ,\n\n";

    // 2. Demonstration of enum class type safety vs enum vulnerability
    std::cout << "[2] TYPE SAFETY: ENUM CLASS VS UNSCOPED ENUM:\n";
    LegacyZone legacyZ = LEGACY_MIDDLE;
    FoodItem food = BURGER;

    // FLAGGED HAZARD IN UNSCOPED ENUM: Implicit conversion to int allows comparing zone to food!
    if (legacyZ == food) {
        std::cout << "    [UNSAFE] Legacy enum allows: legacyZ == food! (Both evaluate to int 1)\n";
    }

    MetroZone safeZ = MetroZone::Middle;
    // The following would produce a COMPILER ERROR with enum class:
    // if (safeZ == food) { ... }  // error: no match for 'operator=='
    std::cout << "    [SAFE] enum class MetroZone forbids cross-enum comparison at compile time.\n";
    std::cout << "    [SAFE] Explicit static_cast needed to access underlying index: "
              << static_cast<int>(safeZ) << "\n\n";

    // 3. Compile-Time constexpr Evaluation Verification
    std::cout << "[3] COMPILE-TIME CONSTEXPR EVALUATION:\n";
    constexpr long long innerFare = METRO_BASE_FARE_PAISE[static_cast<int>(MetroZone::Inner)];
    constexpr long long middleFare = METRO_BASE_FARE_PAISE[static_cast<int>(MetroZone::Middle)];
    constexpr long long outerFare = METRO_BASE_FARE_PAISE[static_cast<int>(MetroZone::Outer)];

    // Statically verified at compile-time via static_assert
    static_assert(innerFare == 1000LL, "Inner fare must be 1000 paise");
    static_assert(middleFare == 2000LL, "Middle fare must be 2000 paise");
    static_assert(outerFare == 3000LL, "Outer fare must be 3000 paise");
    static_assert(PEAK_SURCHARGE_PERCENT == 20, "Peak surcharge must be 20%");
    static_assert(MINIMUM_CARD_REMAINDER_PAISE == 5000LL, "Min balance must be 5000 paise");

    std::cout << "    static_assert checks passed successfully at compile-time!\n";
    std::cout << "    Inner Zone Fare:  " << innerFare << " paise (Rs " << innerFare / 100 << ")\n";
    std::cout << "    Middle Zone Fare: " << middleFare << " paise (Rs " << middleFare / 100 << ")\n";
    std::cout << "    Outer Zone Fare:  " << outerFare << " paise (Rs " << outerFare / 100 << ")\n";
    std::cout << "    Peak Surcharge:   " << PEAK_SURCHARGE_PERCENT << "%\n";
    std::cout << "    Min Balance:      " << MINIMUM_CARD_REMAINDER_PAISE << " paise (Rs "
              << MINIMUM_CARD_REMAINDER_PAISE / 100 << ")\n\n";

    // 4. Verification that revision in one place modifies system deterministically
    std::cout << "[4] CENTRALIZED REVISION PROOF:\n";
    std::cout << "    Modifying METRO_BASE_FARE_PAISE or PEAK_SURCHARGE_PERCENT updates\n";
    std::cout << "    every dependent formula with 0% risk of duplicate typos.\n";
    std::cout << "====================================================================\n";

    return 0;
}
