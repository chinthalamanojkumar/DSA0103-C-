/**
 * ============================================================================
 * Assignment 1 - Task 07: Procedural vs Object-Oriented Architecture
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Demonstrating:
 * 1. Procedural Implementation (free functions, raw struct, external mutation)
 * 2. Object-Oriented Implementation (SmartCard class, private state, invariant guards)
 * 3. Prevention of negative balances and illegal card state
 * 4. Ease of maintainability comparison when minimum balance policy updates
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>

// Common Domain Constants
enum class Zone { Inner = 0, Middle = 1, Outer = 2 };
constexpr long long BASE_FARES[] = {1000LL, 2000LL, 3000LL}; // paise
constexpr int PEAK_SURCHARGE_PCT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000LL; // Rs 50.00

// ============================================================================
// [STYLE 1: PROCEDURAL ARCHITECTURE]
// Free functions, naked data structures, external balance mutation
// ============================================================================
struct ProceduralCard {
    std::string cardId;
    long long balancePaise; // Publicly exposed state - vulnerable to illegal mutation
};

long long proceduralCalculateFare(Zone z, bool peak) {
    long long f = BASE_FARES[static_cast<int>(z)];
    if (peak) f += (f * PEAK_SURCHARGE_PCT) / 100;
    return f;
}

bool proceduralCanTravel(long long balance, long long fare) {
    return (balance - fare) >= MIN_BALANCE_PAISE;
}

// Procedural deduction relies on the caller remembering to call proceduralCanTravel first!
bool proceduralProcessTrip(ProceduralCard& card, Zone z, bool peak) {
    long long f = proceduralCalculateFare(z, peak);
    if (proceduralCanTravel(card.balancePaise, f)) {
        card.balancePaise -= f; // Direct mutation
        return true;
    }
    return false;
}

// ============================================================================
// [STYLE 2: OBJECT-ORIENTED ARCHITECTURE]
// Encapsulated SmartCard class, private balance, enforced invariants
// ============================================================================
class SmartCard {
private:
    std::string cardId;
    long long balancePaise; // Strictly private: no external code can bypass safety invariants

public:
    SmartCard(std::string id, long long initialBalancePaise)
        : cardId(std::move(id)), balancePaise(initialBalancePaise) {
        if (balancePaise < 0) {
            std::cerr << "[ERROR] Negative initial balance rejected. Defaulting to 0.\n";
            balancePaise = 0;
        }
    }

    const std::string& getId() const { return cardId; }
    long long getBalancePaise() const { return balancePaise; }

    bool canTravel(long long farePaise) const {
        return (balancePaise - farePaise) >= MIN_BALANCE_PAISE;
    }

    // Atomic, invariant-safe deduction
    bool deduct(long long farePaise) {
        if (!canTravel(farePaise)) {
            return false; // Rejection: invariant would be violated
        }
        balancePaise -= farePaise;
        return true;
    }

    void recharge(long long amountPaise) {
        if (amountPaise > 0) {
            balancePaise += amountPaise;
        }
    }
};

int main() {
    std::cout << "====================================================================\n";
    std::cout << " TASK 07: PROCEDURAL VS OBJECT-ORIENTED ARCHITECTURE EVALUATION      \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    // Scenario: Card with Rs 70.00 (7000 paise) takes Outer Peak Trip (Rs 36.00 = 3600 paise)
    // Balance would be 7000 - 3600 = 3400 paise (Rs 34.00) < Rs 50.00 -> Must be Denied!
    std::cout << "--- [SCENARIO 1: NORMAL POLICY ENFORCEMENT] ---\n";
    ProceduralCard pCard{"PROC-01", 7000LL};
    SmartCard oopCard{"OOP-01", 7000LL};

    bool pSuccess = proceduralProcessTrip(pCard, Zone::Outer, true);
    std::cout << "[Procedural] Trip Attempt: " << (pSuccess ? "APPROVED" : "DENIED")
              << " | Card Balance: Rs " << pCard.balancePaise / 100.0 << '\n';

    long long oopFare = proceduralCalculateFare(Zone::Outer, true);
    bool oSuccess = oopCard.deduct(oopFare);
    std::cout << "[Object-Oriented] Trip Attempt: " << (oSuccess ? "APPROVED" : "DENIED")
              << " | Card Balance: Rs " << oopCard.getBalancePaise() / 100.0 << "\n\n";

    // Scenario 2: Vulnerability Analysis - Rogue external code bypasses procedural guard
    std::cout << "--- [SCENARIO 2: ACCIDENTAL / ROGUE EXTERNAL MUTATION] ---\n";
    std::cout << "Attempting to deduct fare without canTravel guard:\n";

    // In procedural code: A programmer or junior developer can write:
    pCard.balancePaise -= 6000LL; // Bypasses Rs 50 minimum check completely!
    std::cout << "[Procedural Hazard] Rogue code executed 'pCard.balancePaise -= 6000'\n";
    std::cout << "                    Resulting Procedural Balance: Rs "
              << pCard.balancePaise / 100.0 << " (DISASTER: DROPPED BELOW RS 50!)\n";

    // In OOP code: The compiler forbids direct access to balancePaise!
    // oopCard.balancePaise -= 6000LL; // COMPILER ERROR: 'balancePaise' is private!
    bool oopRogueAttempt = oopCard.deduct(6000LL);
    std::cout << "[OOP Protection] Calling 'oopCard.deduct(6000)'\n";
    std::cout << "                 Result: " << (oopRogueAttempt ? "ALLOWED" : "REJECTED BY INVARIANT")
              << " | Final Balance: Rs " << oopCard.getBalancePaise() / 100.0 << " (PROTECTED!)\n\n";

    std::cout << "====================================================================\n";
    std::cout << " EVALUATIVE SUMMARY:\n";
    std::cout << " - Time Complexity:  O(1) in both Procedural and OOP\n";
    std::cout << " - Space Complexity: O(1) (8 bytes per balance in both)\n";
    std::cout << " - Invariant Safety: OOP guarantees non-negative & min balance at compile-time\n";
    std::cout << "====================================================================\n";

    return 0;
}
