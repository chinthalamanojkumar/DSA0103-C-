/**
 * ============================================================================
 * Assignment 1 - Task 01: Object-Oriented Domain Analysis & Verification
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Problem: Namma Metro Smart-Card System Object-Oriented Domain Modeling
 * Demonstrating Objects, Encapsulation, Invariant Enforcement, and OOP vs Procedural.
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <cassert>

// 1. Domain Enumeration: Zone
enum class Zone {
    Inner = 0,   // Rs 10 (1000 paise)
    Middle = 1,  // Rs 20 (2000 paise)
    Outer = 2    // Rs 30 (3000 paise)
};

// 2. Passenger Category
enum class PassengerType {
    Regular,
    Student,
    SeniorCitizen
};

// Helper to convert Zone to string
const char* zoneToString(Zone z) {
    switch (z) {
        case Zone::Inner:  return "Inner";
        case Zone::Middle: return "Middle";
        case Zone::Outer:  return "Outer";
        default:           return "Unknown";
    }
}

// 3. Domain Entity: FareTable (Abstraction & Encapsulation of pricing logic)
class FareTable {
private:
    static constexpr long long BASE_FARES_PAISE[] = {1000, 2000, 3000}; // Rs 10, 20, 30
    static constexpr int PEAK_SURCHARGE_PCT = 20;                        // +20%
    static constexpr int STUDENT_CONCESSION_PCT = 25;                    // 25% discount
    static constexpr int SENIOR_CONCESSION_PCT = 30;                     // 30% discount

public:
    static constexpr long long MIN_BALANCE_PAISE = 5000; // Rs 50.00 required post-journey

    static bool isPeakHour(int hour24) {
        // [08:00, 10:00) and [17:00, 20:00)
        return (hour24 >= 8 && hour24 < 10) || (hour24 >= 17 && hour24 < 20);
    }

    static long long calculateFare(Zone z, int hour24, PassengerType pt) {
        int idx = static_cast<int>(z);
        if (idx < 0 || idx > 2) return -1; // Invalid zone safeguard

        long long fare = BASE_FARES_PAISE[idx];

        // Apply peak surcharge if applicable
        if (isPeakHour(hour24)) {
            fare += (fare * PEAK_SURCHARGE_PCT) / 100;
        }

        // Apply passenger concession
        if (pt == PassengerType::Student) {
            fare -= (fare * STUDENT_CONCESSION_PCT) / 100;
        } else if (pt == PassengerType::SeniorCitizen) {
            fare -= (fare * SENIOR_CONCESSION_PCT) / 100;
        }

        return fare;
    }
};

// 4. Domain Entity: SmartCard (Encapsulation: balance is private, invariants preserved)
class SmartCard {
private:
    std::string cardId;
    long long balancePaise; // Stored in integer paise to eliminate floating-point drift

public:
    SmartCard(std::string id, long long initialPaise)
        : cardId(std::move(id)), balancePaise(initialPaise) {
        assert(initialPaise >= 0 && "Card balance cannot be initialized negative");
    }

    const std::string& getId() const { return cardId; }
    long long getBalancePaise() const { return balancePaise; }

    bool canTravel(long long farePaise) const {
        return (balancePaise - farePaise) >= FareTable::MIN_BALANCE_PAISE;
    }

    bool deductFare(long long farePaise) {
        if (!canTravel(farePaise)) {
            return false;
        }
        balancePaise -= farePaise;
        return true;
    }

    void recharge(long long paise) {
        if (paise > 0) {
            balancePaise += paise;
        }
    }
};

// 5. Domain Entity: Passenger
class Passenger {
private:
    std::string name;
    PassengerType type;

public:
    Passenger(std::string name, PassengerType type)
        : name(std::move(name)), type(type) {}

    const std::string& getName() const { return name; }
    PassengerType getType() const { return type; }
};

// 6. Domain Entity: Journey (Links Passenger, Card, Zone, and Time)
class Journey {
private:
    std::string journeyId;
    Zone destinationZone;
    int entryHour24;

public:
    Journey(std::string jId, Zone zone, int hour)
        : journeyId(std::move(jId)), destinationZone(zone), entryHour24(hour) {}

    bool process(const Passenger& passenger, SmartCard& card) const {
        long long fare = FareTable::calculateFare(destinationZone, entryHour24, passenger.getType());
        bool peak = FareTable::isPeakHour(entryHour24);

        std::cout << "[Journey " << journeyId << "] Passenger: " << passenger.getName()
                  << " | Zone: " << zoneToString(destinationZone)
                  << " | Hour: " << std::setw(2) << std::setfill('0') << entryHour24 << ":00"
                  << " | " << (peak ? "PEAK" : "OFF-PEAK")
                  << " | Fare: Rs " << std::fixed << std::setprecision(2) << (fare / 100.0);

        if (card.deductFare(fare)) {
            std::cout << " | Status: APPROVED | New Balance: Rs "
                      << (card.getBalancePaise() / 100.0) << "\n";
            return true;
        } else {
            std::cout << " | Status: DENIED (Low Balance) | Current Balance: Rs "
                      << (card.getBalancePaise() / 100.0)
                      << " (Needs min Rs 50.00 after fare)\n";
            return false;
        }
    }
};

int main() {
    std::cout << "====================================================================\n";
    std::cout << " NAMMA METRO SMART-CARD FARE SYSTEM: DOMAIN MODEL VALIDATION (TASK 1)\n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008\n";
    std::cout << "====================================================================\n\n";

    // Setup initial entities
    SmartCard card1("CARD-1001", 10000); // Rs 100.00 balance
    Passenger p1("Aarav (Student)", PassengerType::Student);
    Passenger p2("Meera (Regular)", PassengerType::Regular);
    Passenger p3("Rao (Senior Citizen)", PassengerType::SeniorCitizen);

    std::cout << "Initial Card " << card1.getId() << " Balance: Rs "
              << (card1.getBalancePaise() / 100.0) << "\n\n";

    // Trace 1: Student travelling to Inner Zone during Morning Peak (09:00)
    // Base: 1000 paise -> Peak +20% = 1200 paise -> Student -25% = 900 paise (Rs 9.00)
    Journey j1("J001", Zone::Inner, 9);
    j1.process(p1, card1);

    // Trace 2: Regular passenger to Outer Zone during Evening Peak (18:00)
    // Base: 3000 paise -> Peak +20% = 3600 paise (Rs 36.00)
    Journey j2("J002", Zone::Outer, 18);
    j2.process(p2, card1);

    // Trace 3: Senior Citizen to Middle Zone Off-Peak (14:00)
    // Base: 2000 paise -> Off-peak = 2000 paise -> Senior -30% = 1400 paise (Rs 14.00)
    Journey j3("J003", Zone::Middle, 14);
    j3.process(p3, card1);

    // Trace 4: Outer Zone trip attempting to deplete balance below Rs 50
    // Current balance is Rs 41.00. Fare is Rs 30.00. Balance would be Rs 11.00 < Rs 50.00 -> Denied!
    Journey j4("J004", Zone::Outer, 12);
    j4.process(p2, card1);

    std::cout << "\n====================================================================\n";
    std::cout << " Invariant Verification: Balance after denied trip = Rs "
              << (card1.getBalancePaise() / 100.0) << " (Never dropped below Rs 50.00)\n";
    std::cout << "====================================================================\n";

    return 0;
}
