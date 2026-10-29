/**
 * ============================================================================
 * Assignment 1 - Task 08: Safe Stream Input, Error State Recovery & Output Formatting
 * Student Name: Manoj Kumar Chinthala
 * Register Number: 192472008
 * Course: DSA01 - Object-Oriented Programming with C++
 * Bloom's Level: K5 (Evaluate)
 * ============================================================================
 * Demonstrating:
 * 1. Stream state recovery: cin.fail(), cin.clear(), cin.ignore()
 * 2. Formatted ledger reporting with <iomanip> (setw, setfill, fixed, setprecision)
 * 3. Comparative evaluation: cin stream extraction vs getline + stoi try/catch
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <limits>
#include <sstream>

// Safe integer reader with state recovery from any istream
bool readSafeInt(std::istream& in, int& outputValue, const std::string& prompt) {
    while (true) {
        // Skip whitespace and comments
        while (in.good()) {
            int c = in.peek();
            if (c == '#') {
                std::string comment;
                std::getline(in, comment);
            } else if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
                in.get();
            } else {
                break;
            }
        }

        if (!in.good() || in.eof()) return false;

        if (in >> outputValue) {
            return true; // Successfully parsed an integer
        }

        // Stream failed (e.g. non-numeric input encountered)
        std::cout << "    [INPUT RECOVERY] Non-numeric input encountered for " << prompt
                  << "! Clearing cin.fail() and discarding bad token...\n";
        in.clear(); // Reset failure bit
        std::string badToken;
        in >> badToken; // Consume the bad word
        std::cout << "    [DISCARDED BAD TOKEN]: \"" << badToken << "\"\n";
    }
}

// Alternative Concept: Line-oriented parsing with std::getline + std::stoi inside try-catch
bool readSafeIntLineBased(const std::string& line, int& outputValue) {
    try {
        size_t idx = 0;
        outputValue = std::stoi(line, &idx);
        return true;
    } catch (const std::invalid_argument&) {
        std::cout << "    [LINE PARSER ERROR] std::invalid_argument: No integer found in line.\n";
        return false;
    } catch (const std::out_of_range&) {
        std::cout << "    [LINE PARSER ERROR] std::out_of_range: Integer exceeds range.\n";
        return false;
    }
}

void printFormattedReport(const std::string& cardId, long long balancePaise,
                          long long farePaise, bool approved) {
    std::cout << "    " << std::left << std::setw(12) << cardId
              << " | Fare: Rs " << std::right << std::setw(5) << (farePaise / 100) << '.'
              << std::setw(2) << std::setfill('0') << (farePaise % 100) << std::setfill(' ')
              << " | Balance: Rs " << std::right << std::setw(6) << (balancePaise / 100) << '.'
              << std::setw(2) << std::setfill('0') << (balancePaise % 100) << std::setfill(' ')
              << " | " << (approved ? "APPROVED" : "DENIED (LOW BAL)") << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "====================================================================\n";
    std::cout << " TASK 08: SAFE INPUT EXTRACTION & IOMANIP LEDGER FORMATTING         \n";
    std::cout << " Student: Manoj Kumar Chinthala | Reg No: 192472008                 \n";
    std::cout << "====================================================================\n\n";

    // Determine input source: file if provided, otherwise check input.txt, else std::cin
    std::ifstream fileStream;
    std::istream* inputSource = &std::cin;

    if (argc > 1) {
        fileStream.open(argv[1]);
        if (fileStream.is_open()) inputSource = &fileStream;
    } else {
        fileStream.open("input.txt");
        if (fileStream.is_open()) inputSource = &fileStream;
    }

    std::cout << "[1] DEMONSTRATING STREAM RECOVERY FROM MALFORMED INPUT (cin.fail):\n";

    std::string cardId = "CARD-001";
    long long balance = 10000; // Rs 100.00
    int hour = 0;

    // Read hour from input stream with intentional corruption recovery
    std::cout << "    Reading journey hour with intentional bad token 'abc' in input stream:\n";
    if (readSafeInt(*inputSource, hour, "Journey Hour")) {
        std::cout << "    -> Extracted Valid Hour: " << hour << ":00\n\n";
    }

    std::cout << "[2] PROFESSIONAL TURNSTILE TRANSACTION LEDGER (<iomanip>):\n";
    std::cout << "    ----------------------------------------------------------------------\n";
    std::cout << "    Card ID      | Fare Amount   | Remaining Bal | Gate Status          \n";
    std::cout << "    ----------------------------------------------------------------------\n";

    // Simulate 3 journeys
    printFormattedReport(cardId, balance - 2400, 2400, true);
    balance -= 2400;
    printFormattedReport(cardId, balance - 1200, 1200, true);
    balance -= 1200;
    printFormattedReport(cardId, balance, 3600, false); // Denied because balance would drop < 5000

    std::cout << "    ----------------------------------------------------------------------\n\n";

    std::cout << "[3] ALTERNATIVE CONCEPT: LINE-BASED getline() + std::stoi() TEST:\n";
    int val = 0;
    std::cout << "    Testing valid string '42': ";
    if (readSafeIntLineBased("42", val)) std::cout << "Parsed value: " << val << "\n";

    std::cout << "    Testing corrupt string 'INVALID_CHARS': ";
    readSafeIntLineBased("INVALID_CHARS", val);

    std::cout << "====================================================================\n";
    return 0;
}
