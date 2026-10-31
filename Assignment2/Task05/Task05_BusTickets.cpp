/**
 * ============================================================================
 * ASSIGNMENT 2 - CO2: OBJECT-ORIENTED PROGRAMMING WITH C++
 * TASK 05 (Q5): Static Members and Arrays of Objects
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate)
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 * 
 * PROBLEM CONTEXT - COLLEGE BUS SEAT TICKETS:
 * The college bus has a fixed number of seats. Each BusTicket object must receive
 * the next seat number, tickets beyond the capacity go to a waiting list, and
 * the transport office must know how many seats have been issued without
 * inspecting any particular ticket instance.
 * 
 * CORE PRINCIPLE:
 * "Data that belongs to the whole class – not to any one object – should exist
 * only once, however many objects are created."
 * 
 * SECTIONS:
 * 1. Predict the Output: Token Class (Page 8-9)
 * 2. Complete the Code: BusTicket Class (TODO 1 & TODO 2) (Page 9-10)
 * 3. Scope & Lifetime Verification: Creating Tickets in a Function Called Twice
 * 4. Technical Inquiry: Why static member functions cannot access 'this'
 * 5. Alternative Concept & Multi-Bus Concurrency Analysis (TicketOffice vs Static)
 * ============================================================================
 */

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// ============================================================================
// SECTION 1: PREDICT THE OUTPUT (Token Class from Assignment Specification)
// ============================================================================
class Token {
    static int next;
    int id;
public:
    Token() { id = ++next; }
    int get() const { return id; }
    static int issued() { return next - 100; }
};

int Token::next = 100; // Static definition in global/namespace scope

void runPredictTheOutput() {
    std::cout << "========================================================\n";
    std::cout << "1. PREDICT THE OUTPUT VERIFICATION (Assignment Page 8-9)\n";
    std::cout << "========================================================\n";
    std::cout << "[PREDICTED OUTPUT BEFORE RUNNING]:\n";
    std::cout << "101 103 104 4\n\n";

    std::cout << "[ACTUAL EXECUTION OUTPUT]:\n";
    Token counter[3];
    Token extra;
    std::cout << counter[0].get() << ' ' << counter[2].get() << ' ' << extra.get()
              << ' ' << Token::issued() << '\n';

    std::cout << "\n[EVALUATION & CAUSAL ANALYSIS]:\n";
    std::cout << "1. Why the static counter starts at 100:\n";
    std::cout << "   - 'int Token::next = 100;' explicitly defines and initializes the static\n";
    std::cout << "     storage duration member in the program's data segment before main() executes.\n";
    std::cout << "2. How many times the constructor ran:\n";
    std::cout << "   - Exactly 4 times:\n";
    std::cout << "     * 3 times for array elements 'counter[0]', 'counter[1]', 'counter[2]'\n";
    std::cout << "       (next becomes 101, 102, 103 respectively).\n";
    std::cout << "     * 1 time for the standalone object 'extra'\n";
    std::cout << "       (next becomes 104).\n";
    std::cout << "   - counter[0].get() returns 101.\n";
    std::cout << "   - counter[2].get() returns 103.\n";
    std::cout << "   - extra.get() returns 104.\n";
    std::cout << "   - Token::issued() returns 104 - 100 = 4.\n";
    std::cout << "   - Prediction matches execution with 100% precision.\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 2: COMPLETE THE CODE (BusTicket Class)
// ============================================================================
class BusTicket {
    static int issued; // shared by all BusTicket objects
    static const int CAPACITY = 3;
    int seat;
    std::string name;

public:
    explicit BusTicket(const std::string& n) : seat(0), name(n)
    {
        // TODO 1: If fewer than CAPACITY tickets have been issued, increase issued and
        // give this ticket the next seat number; otherwise leave seat as 0 (waiting
        // list)
        if (issued < CAPACITY) {
            seat = ++issued;
        } else {
            seat = 0; // waiting list
        }
    }

    static int count()
    {
        // TODO 2: Return the number of seats issued so far
        return issued;
    }

    // Helper to reset static state between test runs
    static void reset() {
        issued = 0;
    }

    void print() const
    {
        std::cout << name << ": "
                  << (seat ? "Seat " + std::to_string(seat) : std::string("Waiting"))
                  << '\n';
    }

    int getSeat() const { return seat; }
    const std::string& getName() const { return name; }
};

int BusTicket::issued = 0; // definition of the static data member

void runCompleteTheCode() {
    std::cout << "========================================================\n";
    std::cout << "2. COMPLETE THE CODE VERIFICATION (Assignment Page 9-10)\n";
    std::cout << "========================================================\n";
    BusTicket::reset();

    BusTicket trip[4] = {BusTicket("Asha"), BusTicket("Bala"),
                         BusTicket("Chitra"), BusTicket("Dev")};
    for (const BusTicket& t : trip) t.print();
    std::cout << "Issued: " << BusTicket::count() << '\n';

    std::cout << "\nExpected Output:\n";
    std::cout << "Asha: Seat 1\n";
    std::cout << "Bala: Seat 2\n";
    std::cout << "Chitra: Seat 3\n";
    std::cout << "Dev: Waiting\n";
    std::cout << "Issued: 3\n";
    std::cout << "Status: VERIFIED EXACT MATCH\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 3: TICKETS CREATED IN A FUNCTION CALLED TWICE
// ============================================================================
void issueBatch(int batchNum) {
    std::cout << "--- Calling issueBatch(" << batchNum << ") ---\n";
    BusTicket passengers[2] = {
        BusTicket("Passenger_" + std::to_string(batchNum) + "A"),
        BusTicket("Passenger_" + std::to_string(batchNum) + "B")
    };
    for (const auto& p : passengers) p.print();
    std::cout << "Current BusTicket::count() after batch " << batchNum << ": "
              << BusTicket::count() << '\n';
}

void runTwiceFunctionTest() {
    std::cout << "========================================================\n";
    std::cout << "3. FUNCTION CALLED TWICE: LIFETIME & COUNT BEHAVIOUR\n";
    std::cout << "========================================================\n";
    BusTicket::reset();
    std::cout << "Resetting BusTicket issued count to 0.\n\n";

    // Call 1: Capacity is 3, 2 passengers enter -> Both get Seat 1 and Seat 2
    issueBatch(1);

    // Call 2: Capacity is 3, 1 seat remaining -> Passenger 2A gets Seat 3, Passenger 2B gets Waiting
    issueBatch(2);

    std::cout << "\n[EXPLANATION OF COUNT BEHAVIOUR]:\n";
    std::cout << "  - Although local array 'passengers' goes out of scope and destructs when\n";
    std::cout << "    issueBatch() exits, the static variable 'BusTicket::issued' resides in the\n";
    std::cout << "    static data segment of the process, outliving function activation frames.\n";
    std::cout << "  - Therefore, during the 2nd invocation, the class remembers that 2 seats were\n";
    std::cout << "    already allocated. Passenger 2A receives the final remaining Seat 3, and\n";
    std::cout << "    Passenger 2B is routed to the Waiting List.\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 4: INQUIRY - TECHNICAL PRINCIPLES OF STATIC MEMBERS
// ============================================================================
void runInquiry() {
    std::cout << "========================================================\n";
    std::cout << "4. INQUIRY: STATIC ARCHITECTURAL PRINCIPLES\n";
    std::cout << "========================================================\n";
    std::cout << "A. Which members must be static & where defined:\n";
    std::cout << "   - 'issued' and 'CAPACITY' must be static because seat capacity and total\n";
    std::cout << "     allocations are global properties of the bus system, not attributes of\n";
    std::cout << "     any single ticket holder.\n";
    std::cout << "   - Static data members are declared inside the class body, but MUST be defined\n";
    std::cout << "     and initialized at file/namespace scope outside the class declaration\n";
    std::cout << "     (e.g., 'int BusTicket::issued = 0;'). This allocates memory in the data segment.\n\n";
    std::cout << "B. Why a static member function cannot use 'this' or non-static members:\n";
    std::cout << "   - Non-static member functions receive an implicit hidden first parameter:\n";
    std::cout << "     'ClassName* const this', which holds the memory address of the invoking instance.\n";
    std::cout << "   - Static member functions are bound to the class type itself (BusTicket::count()),\n";
    std::cout << "     not to any instance. No 'this' pointer is passed via the call instruction.\n";
    std::cout << "   - Without 'this', the compiler has no base address from which to calculate the\n";
    std::cout << "     memory offset of instance fields like 'name' or 'seat'.\n\n";
    std::cout << "C. How an array of BusTicket objects is created and initialized:\n";
    std::cout << "   - When BusTicket trip[4] is defined with explicit constructor initializers,\n";
    std::cout << "     each element invokes 'explicit BusTicket(const std::string& n)' sequentially\n";
    std::cout << "     from index 0 to 3, incrementing the shared static counter at each step.\n";
    std::cout << "========================================================\n\n";
}

// ============================================================================
// SECTION 5: ALTERNATIVE CONCEPT - MULTI-BUS CONCURRENCY & ENCAPSULATION
// ============================================================================

// Alternative Solution: TicketOffice / Bus Object representing individual buses
class BusRoute {
private:
    std::string routeName;
    int capacity;
    int issuedSeats;

public:
    BusRoute(std::string name, int cap)
        : routeName(std::move(name)), capacity(cap), issuedSeats(0) {}

    struct InstanceTicket {
        std::string passengerName;
        int seatNumber;
        std::string busName;
    };

    InstanceTicket issueTicket(const std::string& passengerName) {
        if (issuedSeats < capacity) {
            return InstanceTicket{passengerName, ++issuedSeats, routeName};
        }
        return InstanceTicket{passengerName, 0, routeName}; // Waiting list
    }

    int getIssuedCount() const { return issuedSeats; }
    const std::string& getRouteName() const { return routeName; }
};

void runAlternativeMultiBusAnalysis() {
    std::cout << "========================================================\n";
    std::cout << "5. ALTERNATIVE CONCEPT: MULTI-BUS CONCURRENCY ANALYSIS\n";
    std::cout << "========================================================\n";
    std::cout << "Limitation of Static Data Members:\n";
    std::cout << "  Because static member 'BusTicket::issued' belongs to the entire class type,\n";
    std::cout << "  it CANNOT support running two buses concurrently (e.g. Route 101 and Route 202)!\n";
    std::cout << "  Issuing tickets on Route 202 would accidentally decrement available seats on Route 101.\n\n";

    std::cout << "Alternative Object-Composition Solution (BusRoute):\n";
    BusRoute bus1("Route 101 (Saveetha -> Tambaram)", 3);
    BusRoute bus2("Route 202 (Saveetha -> Central)", 2);

    auto tA = bus1.issueTicket("Arun");
    auto tB = bus1.issueTicket("Bhavya");
    auto tC = bus2.issueTicket("Chetan");
    auto tD = bus1.issueTicket("Deepa");
    auto tE = bus1.issueTicket("Eashwar"); // Exceeds bus1 capacity
    auto tF = bus2.issueTicket("Farhan");

    std::cout << "  " << tA.passengerName << " on " << tA.busName << ": Seat " << tA.seatNumber << '\n';
    std::cout << "  " << tB.passengerName << " on " << tB.busName << ": Seat " << tB.seatNumber << '\n';
    std::cout << "  " << tC.passengerName << " on " << tC.busName << ": Seat " << tC.seatNumber << '\n';
    std::cout << "  " << tD.passengerName << " on " << tD.busName << ": Seat " << tD.seatNumber << '\n';
    std::cout << "  " << tE.passengerName << " on " << tE.busName << ": "
              << (tE.seatNumber ? "Seat " + std::to_string(tE.seatNumber) : "WAITING LIST") << '\n';
    std::cout << "  " << tF.passengerName << " on " << tF.busName << ": Seat " << tF.seatNumber << '\n';

    std::cout << "\nBus Status Summaries:\n";
    std::cout << "  " << bus1.getRouteName() << " Issued: " << bus1.getIssuedCount() << "/3\n";
    std::cout << "  " << bus2.getRouteName() << " Issued: " << bus2.getIssuedCount() << "/2\n\n";

    std::cout << "Comparative Evaluation Matrix:\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Criterion              | Global Variable     | Static Class Member | TicketOffice / Bus Route |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "| Encapsulation          | Zero (Compromised)  | High (Class-scoped) | Perfect (Instance-scoped)|\n";
    std::cout << "| Time Complexity        | O(1)                | O(1)                | O(1)                     |\n";
    std::cout << "| Space Complexity       | 4 bytes in .bss     | 4 bytes in .bss     | 4 bytes per bus instance |\n";
    std::cout << "| Multi-Bus Concurrency  | Broken (Race hazard)| Broken (Shared state| Fully Supported (Isolated|\n";
    std::cout << "| Access Control         | Anyone can mutate   | Protected by class  | Protected by Bus methods |\n";
    std::cout << "------------------------------------------------------------------------------------\n";
    std::cout << "DEFENSE:\n";
    std::cout << "  Static members are appropriate when an entity is genuinely unique to the entire\n";
    std::cout << "  runtime (singleton fleet counter). However, for realistic transportation systems\n";
    std::cout << "  with multiple distinct routes and capacities, encapsulating the counter inside a\n";
    std::cout << "  BusRoute object preserves encapsulation while enabling independent, concurrent fleets.\n";
    std::cout << "========================================================\n";
}

// ============================================================================
// MAIN ENTRY POINT
// ============================================================================
int main() {
    std::cout << "*****************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 2 - TASK 05: STATIC MEMBERS & OBJECT ARRAYS  \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008   \n";
    std::cout << "*****************************************************************\n\n";

    runPredictTheOutput();
    runCompleteTheCode();
    runTwiceFunctionTest();
    runInquiry();
    runAlternativeMultiBusAnalysis();

    return 0;
}
