/**
 * ============================================================================
 * Assignment 1 - Task 06: Control Structures & Fare Computation Logic
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Complete the Code:
 * - TODO 1: switch statement on Zone with integer surcharge
 * - TODO 2: canTravel balance boundary validation
 * - Boundary tests: exactly 10:00 AM and 8:00 PM
 * ============================================================================
 */

#include <iomanip>
#include <iostream>

enum class Zone { Inner, Middle, Outer };

constexpr int BASE_PAISE[] = {1000, 2000, 3000};   // Rs 10, Rs 20, Rs 30
constexpr int PEAK_SURCHARGE_PERCENT = 20;
constexpr long long MIN_BALANCE_PAISE = 5000;     // Rs 50 must remain on the card

long long fare(Zone z, bool peak)
{
    // TODO 1: Use a switch on z to pick the base fare from BASE_PAISE, then add
    //   PEAK_SURCHARGE_PERCENT % in peak hours using integer arithmetic only
    long long base = 0;
    switch (z) {
        case Zone::Inner:
            base = BASE_PAISE[0];
            break;
        case Zone::Middle:
            base = BASE_PAISE[1];
            break;
        case Zone::Outer:
            base = BASE_PAISE[2];
            break;
        default:
            return 0; // Safeguard against invalid zone
    }

    if (peak) {
        base += (base * PEAK_SURCHARGE_PERCENT) / 100;
    }
    return base;
}

bool canTravel(long long balance, long long f)
{
    // TODO 2: Return true only if the balance left after paying the fare is at least
    //   MIN_BALANCE_PAISE
    return (balance - f) >= MIN_BALANCE_PAISE;
}

// Peak rule helper according to standard [8, 10) and [17, 20)
bool isPeakHour(int hour24) {
    return (hour24 >= 8 && hour24 < 10) || (hour24 >= 17 && hour24 < 20);
}

int main()
{
    std::cout << "====================================================================\n";
    std::cout << " TASK 06: CONTROL STRUCTURES & COMPLETE THE CODE VERIFICATION       \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    std::cout << "--- [ORIGINAL ASSIGNMENT SPECIFICATION TRACE] ---\n";
    long long balance = 10000;                       // Rs 100.00 in paise
    struct Trip { Zone zone; bool peak; };
    Trip trips[] = {{Zone::Inner, false}, {Zone::Outer, true}, {Zone::Middle, false}};
    for (const Trip& t : trips) {
        long long f = fare(t.zone, t.peak);
        if (canTravel(balance, f)) {
            balance -= f;
            std::cout << "Fare Rs " << f / 100 << '.' << std::setw(2)
                      << std::setfill('0') << f % 100 << std::setfill(' ');
        } else {
            std::cout << "Denied (low balance)";
        }
        std::cout << "  balance Rs " << balance / 100 << '\n';
    }

    std::cout << "\n--- [BOUNDARY TEST: EXACTLY 10:00 AM AND 8:00 PM (20:00)] ---\n";
    long long testBalance = 10000; // Rs 100.00

    struct TimedTrip { const char* desc; Zone zone; int hour; };
    TimedTrip boundaryTrips[] = {
        {"Trip at 09:59 AM (Morning Peak Boundary)", Zone::Inner, 9},
        {"Trip at 10:00 AM (Morning Peak Ends -> Off-Peak)", Zone::Inner, 10},
        {"Trip at 19:59 PM (Evening Peak Boundary)", Zone::Outer, 19},
        {"Trip at 20:00 PM (8:00 PM Evening Peak Ends -> Off-Peak)", Zone::Outer, 20}
    };

    for (const auto& bt : boundaryTrips) {
        bool peak = isPeakHour(bt.hour);
        long long f = fare(bt.zone, peak);
        std::cout << bt.desc << ": Hour " << bt.hour << ":00 -> "
                  << (peak ? "PEAK (+20%)" : "OFF-PEAK (Base)")
                  << " | Fare: Rs " << f / 100 << '.' << std::setw(2) << std::setfill('0') << f % 100
                  << " | Result: " << (canTravel(testBalance, f) ? "ALLOWED" : "DENIED") << '\n';
    }

    std::cout << "\n====================================================================\n";
    return 0;
}
