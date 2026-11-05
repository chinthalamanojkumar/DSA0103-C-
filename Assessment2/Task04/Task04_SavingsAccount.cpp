/**
 * ============================================================================
 * ASSIGNMENT 2 - CO2: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 04 (Q4): Specifying a Class with an Array Inside It
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - SAVINGS ACCOUNT MINI-STATEMENT:
 * A bank wants each SavingsAccount object to remember its last 10 transactions
 * for the mini-statement printed at an ATM. Older transactions are discarded.
 * 
 * SECTIONS:
 * 1. Complete Class Specification with Private Array & State
 * 2. Member Functions Defined Inside Class (Inline) & Outside (Scope Resolution ::)
 * 3. Array Shifting vs Circular Ring Buffer Handling of 11th+ Transactions
 * 4. Design Decision Defense: Scaling from 10 to 50 Transactions
 * 5. Alternative Concept & Empirical Complexity Analysis (Array vs Circular vs Deque)
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <deque>
#include <chrono>
#include <cassert>

// ============================================================================
// SECTION 1 & 2: SAVINGS ACCOUNT CLASS SPECIFICATION
// ============================================================================

class SavingsAccount {
private:
    long long accountNumber;
    double balance;
    static constexpr int CAPACITY = 10;
    double transactions[CAPACITY]; // Stores signed amounts (+ for deposit, - for withdrawal)
    int count;                     // Total transactions recorded so far (capped at CAPACITY)

    // Private helper: Handles shifting of array when capacity is exceeded
    void recordTransactionShift(double amount);

public:
    // Constructor defined inside class
    SavingsAccount(long long accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance), count(0) {
        for (int i = 0; i < CAPACITY; ++i) transactions[i] = 0.0;
    }

    // 1. Member Function Defined INSIDE Class: deposit()
    // Small, high-frequency operation candidate for inline optimization
    bool deposit(double amount) {
        if (amount <= 0.0) {
            std::cout << "  [Error] Deposit amount must be positive.\n";
            return false;
        }
        balance += amount;
        recordTransactionShift(amount);
        return true;
    }

    // 2. Member Function Declared Inside, Defined OUTSIDE: withdraw()
    bool withdraw(double amount);

    // 3. Member Function Declared Inside, Defined OUTSIDE: miniStatement()
    void miniStatement() const;

    // Getters defined inside class
    double getBalance() const { return balance; }
    long long getAccountNumber() const { return accountNumber; }
    int getTransactionCount() const { return count; }
};

// ============================================================================
// MEMBER FUNCTIONS DEFINED OUTSIDE CLASS USING SCOPE RESOLUTION OPERATOR (::)
// ============================================================================

// Private helper defined outside: Shifting array logic for the 11th transaction
void SavingsAccount::recordTransactionShift(double amount) {
    if (count < CAPACITY) {
        transactions[count++] = amount;
    } else {
        // CAPACITY reached (11th+ transaction arrived):
        // Discard oldest transaction at index 0 by shifting elements [1..9] to [0..8]
        for (int i = 0; i < CAPACITY - 1; ++i) {
            transactions[i] = transactions[i + 1];
        }
        // Place newest transaction at the end
        transactions[CAPACITY - 1] = amount;
    }
}

// Public withdraw defined outside using ::
bool SavingsAccount::withdraw(double amount) {
    if (amount <= 0.0) {
        std::cout << "  [Error] Withdrawal amount must be positive.\n";
        return false;
    }
    if (amount > balance) {
        std::cout << "  [Declined] Insufficient funds! Current Balance: Rs. "
                  << std::fixed << std::setprecision(2) << balance
                  << ", Requested: Rs. " << amount << '\n';
        return false;
    }
    balance -= amount;
    recordTransactionShift(-amount); // Recorded as negative
    return true;
}

// Public miniStatement defined outside using ::
void SavingsAccount::miniStatement() const {
    std::cout << "--------------------------------------------------------\n";
    std::cout << "               ATM MINI-STATEMENT                       \n";
    std::cout << " Account Number: " << accountNumber << '\n';
    std::cout << " Current Balance: Rs. " << std::fixed << std::setprecision(2) << balance << '\n';
    std::cout << " Last " << count << " Recent Transactions:\n";
    std::cout << " No. | Type       | Amount (INR)   \n";
    std::cout << "--------------------------------------------------------\n";
    for (int i = 0; i < count; ++i) {
        double txn = transactions[i];
        std::cout << " #" << std::setw(2) << (i + 1) << " | "
                  << (txn >= 0 ? "DEPOSIT   " : "WITHDRAWAL ") << " | Rs. "
                  << std::setw(10) << std::fixed << std::setprecision(2) << (txn >= 0 ? txn : -txn) << '\n';
    }
    std::cout << "--------------------------------------------------------\n";
}

// ============================================================================
// SECTION 3 & 4: CIRCULAR BUFFER REDESIGN (FOR COMPARISON & 50-TXN SCALING)
// ============================================================================

template <int Cap = 10>
class CircularSavingsAccount {
private:
    long long accountNumber;
    double balance;
    double transactions[Cap];
    int head;  // Next write position
    int count; // Number of elements currently stored

public:
    CircularSavingsAccount(long long accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance), head(0), count(0) {}

    void deposit(double amount) {
        balance += amount;
        transactions[head] = amount;
        head = (head + 1) % Cap;
        if (count < Cap) count++;
    }

    void withdraw(double amount) {
        balance -= amount;
        transactions[head] = -amount;
        head = (head + 1) % Cap;
        if (count < Cap) count++;
    }

    void miniStatement() const {
        std::cout << "--------------------------------------------------------\n";
        std::cout << "        CIRCULAR BUFFER MINI-STATEMENT (Cap=" << Cap << ")\n";
        std::cout << " Account Number: " << accountNumber << " | Balance: Rs. " << balance << '\n';
        int start = (count < Cap) ? 0 : head;
        for (int i = 0; i < count; ++i) {
            int idx = (start + i) % Cap;
            double txn = transactions[idx];
            std::cout << " #" << std::setw(2) << (i + 1) << " | "
                      << (txn >= 0 ? "DEPOSIT   " : "WITHDRAWAL ") << " | Rs. "
                      << (txn >= 0 ? txn : -txn) << '\n';
        }
        std::cout << "--------------------------------------------------------\n";
    }
};

// ============================================================================
// DEMONSTRATION & BENCHMARK SUITE
// ============================================================================

void runBasicMiniStatementDemo() {
    std::cout << "========================================================\n";
    std::cout << "1. SAVINGS ACCOUNT CLASS SPECIFICATION & EXECUTION TEST\n";
    std::cout << "========================================================\n";
    SavingsAccount acc(19247200801LL, 25000.00);

    std::cout << "Account created. Applying 12 transactions (exceeding capacity of 10)...\n";
    acc.deposit(1000.0);  // Txn 1
    acc.withdraw(500.0);  // Txn 2
    acc.deposit(2500.0);  // Txn 3
    acc.withdraw(1200.0); // Txn 4
    acc.deposit(800.0);   // Txn 5
    acc.withdraw(300.0);  // Txn 6
    acc.deposit(4000.0);  // Txn 7
    acc.withdraw(1500.0); // Txn 8
    acc.deposit(600.0);   // Txn 9
    acc.withdraw(200.0);  // Txn 10 (Buffer full)
    std::cout << ">>> 10 transactions completed. Now triggering 11th and 12th transactions:\n";
    acc.deposit(7500.0);  // Txn 11 (Discards Txn 1: +1000)
    acc.withdraw(350.0);  // Txn 12 (Discards Txn 2: -500)

    acc.miniStatement();
    std::cout << "Observed: Exactly 10 most recent transactions retained.\n";
    std::cout << "The oldest transactions (+1000 and -500) were successfully discarded via array shifting.\n";
    std::cout << "========================================================\n\n";
}

void runInquiryEvaluation() {
    std::cout << "========================================================\n";
    std::cout << "2. INQUIRY: DATA ENCAPSULATION & 11TH TRANSACTION COST\n";
    std::cout << "========================================================\n";
    std::cout << "A. Why the Array Must Be Private:\n";
    std::cout << "   1. Financial Audit Invariant: If 'transactions' were public, malicious or buggy\n";
    std::cout << "      caller code could alter past ledger history (e.g. changing -5000 to +5000)\n";
    std::cout << "      without updating 'balance', violating double-entry consistency.\n";
    std::cout << "   2. Implementation Independence: Encapsulation allows changing the internal storage\n";
    std::cout << "      from a shifting array to a circular ring buffer or std::deque without breaking\n";
    std::cout << "      any client code invoking acc.deposit() or acc.miniStatement().\n\n";
    std::cout << "B. What Happens When the 11th Transaction Arrives:\n";
    std::cout << "   - Shifting Fixed Array: Requires a loop copying 9 elements: transactions[i] = transactions[i+1].\n";
    std::cout << "     Time Complexity: O(k) where k is buffer capacity (k=10 moves).\n";
    std::cout << "   - Circular Index / Ring Buffer: Writes to transactions[head] and updates head = (head + 1) % k.\n";
    std::cout << "     Time Complexity: Strictly O(1) constant time, 0 data copying!\n\n";
    std::cout << "C. Design Decision: Expanding to 50 Transactions:\n";
    std::cout << "   - In Shifting Array: Every new transaction now forces 49 memory copies (5x more overhead)!\n";
    std::cout << "   - In Circular Buffer: Cost remains strictly 1 write + 1 modulo. The code changes LEAST\n";
    std::cout << "     because only the capacity constant CAPACITY = 50 is updated, while insertion time\n";
    std::cout << "     remains exactly identical O(1).\n";
    std::cout << "========================================================\n\n";
}

void runEmpiricalBenchmark() {
    std::cout << "========================================================\n";
    std::cout << "3. EMPIRICAL BENCHMARK: SHIFTING ARRAY VS CIRCULAR BUFFER VS DEQUE\n";
    std::cout << "========================================================\n";
    constexpr int NUM_OPERATIONS = 500'000;

    // 1. Shifting Array (Capacity 10)
    SavingsAccount shiftingAcc(101, 100000.0);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        shiftingAcc.deposit(10.0);
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    double timeShift10 = std::chrono::duration<double, std::milli>(t2 - t1).count();

    // 2. Circular Buffer (Capacity 10)
    CircularSavingsAccount<10> circularAcc10(102, 100000.0);
    auto t3 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        circularAcc10.deposit(10.0);
    }
    auto t4 = std::chrono::high_resolution_clock::now();
    double timeCirc10 = std::chrono::duration<double, std::milli>(t4 - t3).count();

    // 3. Circular Buffer (Capacity 50)
    CircularSavingsAccount<50> circularAcc50(103, 100000.0);
    auto t5 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        circularAcc50.deposit(10.0);
    }
    auto t6 = std::chrono::high_resolution_clock::now();
    double timeCirc50 = std::chrono::duration<double, std::milli>(t6 - t5).count();

    // 4. std::deque (Capacity 10: push_back and pop_front)
    std::deque<double> deq;
    auto t7 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < NUM_OPERATIONS; ++i) {
        deq.push_back(10.0);
        if (deq.size() > 10) deq.pop_front();
    }
    auto t8 = std::chrono::high_resolution_clock::now();
    double timeDeque = std::chrono::duration<double, std::milli>(t8 - t7).count();

    std::cout << "Execution Time for " << NUM_OPERATIONS << " Transactions:\n";
    std::cout << "  1. Shifting Array (Cap=10)       : " << std::fixed << std::setprecision(3) << timeShift10 << " ms\n";
    std::cout << "  2. Circular Buffer (Cap=10)      : " << std::fixed << std::setprecision(3) << timeCirc10 << " ms\n";
    std::cout << "  3. Circular Buffer (Cap=50)      : " << std::fixed << std::setprecision(3) << timeCirc50 << " ms\n";
    std::cout << "  4. std::deque (push/pop_front)   : " << std::fixed << std::setprecision(3) << timeDeque << " ms\n\n";

    std::cout << "Performance Comparison:\n";
    std::cout << "  - Circular Buffer is " << std::setprecision(2) << (timeShift10 / timeCirc10)
              << "x faster than Shifting Array at Cap=10!\n";
    std::cout << "  - Circular Buffer is " << std::setprecision(2) << (timeDeque / timeCirc10)
              << "x faster than std::deque (avoids heap allocation & pointer indirection)!\n\n";

    std::cout << "Theoretical Complexity Comparison Table:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Design Approach          | Time to Add (Cap=k) | Auxiliary Space | Heap Allocations |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Shifting Fixed Array     | O(k) (k copies)     | k * 8 bytes     | 0 (Contiguous)   |\n";
    std::cout << "| Circular Ring Buffer     | O(1) (Strictly 1 wr)| k * 8 bytes     | 0 (Contiguous)   |\n";
    std::cout << "| std::deque (push/pop)    | Amortized O(1)      | Dynamic nodes   | Multiple on heap |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "========================================================\n";
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 2 - TASK 04: SAVINGS ACCOUNT MINI-STATEMENT  \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runBasicMiniStatementDemo();
    runInquiryEvaluation();
    runEmpiricalBenchmark();

    return 0;
}
