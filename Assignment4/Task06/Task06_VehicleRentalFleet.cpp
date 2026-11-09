/**
 * ============================================================================
 * ASSIGNMENT 4 - CO4: DESIGNING REUSABLE CLASS HIERARCHIES
 * TASK 06: DESIGN CHALLENGE - VEHICLE RENTAL FLEET HIERARCHY
 * 
 * Student Name   : Manoj Kumar Chinthala
 * Register Number: 192472008
 * Institution    : SIMATS Engineering, Saveetha University
 * Bloom's Level  : K5 (Evaluate) - 13 Marks
 * Compiler       : g++ (MinGW-W64 16.2.0) -std=c++17 -Wall -Wextra -pedantic
 * ============================================================================
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <memory>

// ============================================================================
// STAGE 2 & 4: CLASS DECLARATIONS & HIERARCHY DESIGN
// ============================================================================

/**
 * @brief Abstract Base Class representing any rentable vehicle in the fleet.
 * Defines the polymorphic interface and common rental state tracking.
 */
class Vehicle {
protected:
    std::string regNo;
    double dailyRate;
    bool isRented;
    int bookedDays;

public:
    Vehicle(std::string reg, double rate)
        : regNo(std::move(reg)), dailyRate(rate), isRented(false), bookedDays(0) {}

    virtual ~Vehicle() = default; // Mandatory virtual destructor for polymorphic deletion

    // Pure Virtual Member Functions (Abstract Interface)
    virtual double calculateRental(int days) const = 0;
    virtual void displayDetails() const = 0;
    virtual std::string getCategory() const = 0;

    // Concrete Rental Operations
    bool rentVehicle(int days) {
        if (isRented) {
            std::cout << "  [REJECTED] Vehicle " << regNo << " is currently already rented out!\n";
            return false;
        }
        if (days <= 0) {
            std::cout << "  [ERROR] Invalid rental duration: " << days << " days.\n";
            return false;
        }
        isRented = true;
        bookedDays = days;
        std::cout << "  [SUCCESS] Vehicle " << regNo << " (" << getCategory() 
                  << ") successfully rented for " << days << " day(s).\n";
        return true;
    }

    double returnVehicle(int actualDays, double lateFeeRatePerDay = 1.5) {
        if (!isRented) {
            std::cout << "  [ERROR] Vehicle " << regNo << " was not rented out!\n";
            return 0.0;
        }

        double baseCharge = calculateRental(bookedDays);
        double lateFee = 0.0;

        if (actualDays > bookedDays) {
            int extraDays = actualDays - bookedDays;
            lateFee = extraDays * (dailyRate * lateFeeRatePerDay);
            std::cout << "  [NOTICE] Vehicle " << regNo << " returned " << extraDays 
                      << " day(s) late. Late penalty applied @ " << (lateFeeRatePerDay * 100) 
                      << "% of daily rate.\n";
        }

        double totalPayable = baseCharge + lateFee;
        isRented = false;
        int priorBooked = bookedDays;
        bookedDays = 0;

        std::cout << "  [BILLING] RegNo: " << regNo << " | Booked: " << priorBooked 
                  << "d | Actual: " << actualDays << "d\n"
                  << "            Base Charge: Rs. " << std::fixed << std::setprecision(2) << baseCharge
                  << " | Late Fee: Rs. " << lateFee 
                  << " | Total: Rs. " << totalPayable << "\n";

        return totalPayable;
    }

    // Accessors
    const std::string& getRegNo() const { return regNo; }
    double getDailyRate() const { return dailyRate; }
    bool getIsRented() const { return isRented; }
};

/**
 * @brief Non-motorized vehicle derived directly from Vehicle.
 */
class Bicycle : public Vehicle {
private:
    int gearCount;

public:
    Bicycle(std::string reg, double rate, int gears)
        : Vehicle(std::move(reg), rate), gearCount(gears) {}

    double calculateRental(int days) const override {
        // Flat daily rate with a 5% eco-discount for long rentals (>= 7 days)
        double total = dailyRate * days;
        if (days >= 7) total *= 0.95;
        return total;
    }

    void displayDetails() const override {
        std::cout << "  [Bicycle] Reg: " << std::setw(12) << std::left << regNo 
                  << " | Rate/day: Rs. " << std::setw(6) << dailyRate 
                  << " | Gears: " << std::setw(2) << gearCount 
                  << " | Status: " << (isRented ? "RENTED" : "AVAILABLE") << '\n';
    }

    std::string getCategory() const override { return "Bicycle"; }
};

/**
 * @brief Intermediate base class representing vehicles powered by a motor/engine/battery.
 */
class MotorVehicle : public Vehicle {
protected:
    double fuelOrChargeLevel; // Percentage 0.0 - 100.0%
    std::string serviceDueDate;

public:
    MotorVehicle(std::string reg, double rate, double fuelLevel, std::string serviceDate)
        : Vehicle(std::move(reg), rate), fuelOrChargeLevel(fuelLevel), serviceDueDate(std::move(serviceDate)) {}

    double getFuelOrChargeLevel() const { return fuelOrChargeLevel; }
    const std::string& getServiceDueDate() const { return serviceDueDate; }
};

/**
 * @brief Two-wheeler motor vehicle (scooter/motorcycle).
 */
class TwoWheeler : public MotorVehicle {
private:
    int engineCC;
    bool helmetIncluded;

public:
    TwoWheeler(std::string reg, double rate, double fuel, std::string serviceDate, int cc, bool helmet)
        : MotorVehicle(std::move(reg), rate, fuel, std::move(serviceDate)),
          engineCC(cc), helmetIncluded(helmet) {}

    double calculateRental(int days) const override {
        double cost = dailyRate * days;
        if (helmetIncluded) cost += (50.0 * days); // Helmet hygiene fee
        return cost;
    }

    void displayDetails() const override {
        std::cout << "  [TwoWheeler] Reg: " << std::setw(10) << std::left << regNo 
                  << " | Rate/day: Rs. " << std::setw(6) << dailyRate 
                  << " | Engine: " << engineCC << "cc"
                  << " | Fuel: " << fuelOrChargeLevel << "%"
                  << " | Helmet: " << (helmetIncluded ? "Yes" : "No")
                  << " | Due: " << serviceDueDate
                  << " | Status: " << (isRented ? "RENTED" : "AVAILABLE") << '\n';
    }

    std::string getCategory() const override { return "Two-Wheeler"; }
};

/**
 * @brief Standard internal combustion passenger car.
 */
class Car : public MotorVehicle {
protected:
    int seatingCapacity;
    std::string fuelType; // "Petrol" / "Diesel"

public:
    Car(std::string reg, double rate, double fuel, std::string serviceDate, int seats, std::string fType)
        : MotorVehicle(std::move(reg), rate, fuel, std::move(serviceDate)),
          seatingCapacity(seats), fuelType(std::move(fType)) {}

    double calculateRental(int days) const override {
        double cost = dailyRate * days;
        // Premium surcharge for larger passenger capacities (> 5 seater SUV/MUV)
        if (seatingCapacity > 5) cost += (300.0 * days);
        return cost;
    }

    void displayDetails() const override {
        std::cout << "  [Car] Reg: " << std::setw(17) << std::left << regNo 
                  << " | Rate/day: Rs. " << std::setw(6) << dailyRate 
                  << " | Seats: " << seatingCapacity 
                  << " | Fuel: " << fuelType << " (" << fuelOrChargeLevel << "%)"
                  << " | Due: " << serviceDueDate 
                  << " | Status: " << (isRented ? "RENTED" : "AVAILABLE") << '\n';
    }

    std::string getCategory() const override { return "Car (" + fuelType + ")"; }
};

/**
 * @brief Electric Car derived from Car. Charges energy consumption per kWh instead of litre.
 */
class ElectricCar : public Car {
private:
    double batteryCapacityKWh;
    double chargeRatePerKWh;

public:
    ElectricCar(std::string reg, double rate, double chargeLevel, std::string serviceDate, 
                int seats, double batteryKWh, double ratePerKWh)
        : Car(std::move(reg), rate, chargeLevel, std::move(serviceDate), seats, "Electric"),
          batteryCapacityKWh(batteryKWh), chargeRatePerKWh(ratePerKWh) {}

    double calculateRental(int days) const override {
        // Base rental plus fixed energy prepaid recharge plan based on battery size
        double baseCost = dailyRate * days;
        double energyPrep = (batteryCapacityKWh * 0.25) * chargeRatePerKWh; // standard bundled recharge
        return baseCost + energyPrep;
    }

    void displayDetails() const override {
        std::cout << "  [ElectricCar] Reg: " << std::setw(11) << std::left << regNo 
                  << " | Rate/day: Rs. " << std::setw(6) << dailyRate 
                  << " | Battery: " << batteryCapacityKWh << " kWh (" << fuelOrChargeLevel << "%)"
                  << " | Energy Rate: Rs. " << chargeRatePerKWh << "/kWh"
                  << " | Status: " << (isRented ? "RENTED" : "AVAILABLE") << '\n';
    }

    std::string getCategory() const override { return "Electric Car"; }
};

// ============================================================================
// STAGE 5: EXTENSION - DEMONSTRATING OPEN-CLOSED PRINCIPLE (OCP)
// ============================================================================
/**
 * @brief ElectricBus newly added without touching any existing class!
 * Conforms to Open-Closed Principle (OCP): open for extension, closed for modification.
 */
class ElectricBus : public MotorVehicle {
private:
    int passengerCapacity;
    double batteryCapacityKWh;
    std::string routePermitCode;

public:
    ElectricBus(std::string reg, double rate, double chargeLevel, std::string serviceDate,
                int passengers, double batteryKWh, std::string permit)
        : MotorVehicle(std::move(reg), rate, chargeLevel, std::move(serviceDate)),
          passengerCapacity(passengers), batteryCapacityKWh(batteryKWh),
          routePermitCode(std::move(permit)) {}

    double calculateRental(int days) const override {
        // Fleet bus rental: daily rate + passenger insurance levy + battery servicing charge
        double cost = dailyRate * days;
        cost += (passengerCapacity * 25.0 * days); // Rs. 25 passenger safety levy per seat-day
        cost += (batteryCapacityKWh * 5.0);       // High-voltage pack maintenance buffer
        return cost;
    }

    void displayDetails() const override {
        std::cout << "  [ElectricBus] Reg: " << std::setw(11) << std::left << regNo 
                  << " | Rate/day: Rs. " << std::setw(6) << dailyRate 
                  << " | Seats: " << passengerCapacity
                  << " | Battery: " << batteryCapacityKWh << " kWh"
                  << " | Permit: " << routePermitCode
                  << " | Status: " << (isRented ? "RENTED" : "AVAILABLE") << '\n';
    }

    std::string getCategory() const override { return "Electric Heavy Bus"; }
};

// ============================================================================
// STAGES & REPORTS OUTPUT ROUTINES
// ============================================================================

void displayStage1Interpretation() {
    std::cout << "===============================================================================\n";
    std::cout << "STAGE 1: PROBLEM INTERPRETATION & TESTABLE REQUIREMENTS\n";
    std::cout << "===============================================================================\n";
    std::cout << "1. Problem Decomposition:\n";
    std::cout << "   - Domain: Vehicle Rental Fleet Management System.\n";
    std::cout << "   - Core Challenge: Diverse vehicle modalities (manual, combustion, electric),\n";
    std::cout << "     disparate billing rules (per-day, fuel fees, energy per kWh, helmet levy),\n";
    std::cout << "     unified fleet storage (base-class pointers), and strict extensibility (OCP).\n\n";

    std::cout << "2. Identified Entities & Attributes:\n";
    std::cout << "   ----------------------------------------------------------------------------\n";
    std::cout << "   | Entity       | Unique Data Members             | Billing Policy          |\n";
    std::cout << "   ----------------------------------------------------------------------------\n";
    std::cout << "   | Vehicle      | regNo, dailyRate, isRented      | Abstract interface      |\n";
    std::cout << "   | Bicycle      | gearCount                       | Daily rate - 5% weekly  |\n";
    std::cout << "   | MotorVehicle | fuelOrChargeLevel, serviceDue   | Extends base attributes |\n";
    std::cout << "   | TwoWheeler   | engineCC, helmetIncluded        | Daily rate + helmet levy|\n";
    std::cout << "   | Car          | seatingCapacity, fuelType       | Daily rate + SUV bonus  |\n";
    std::cout << "   | ElectricCar  | batteryCapacityKWh, chargeRate  | Daily rate + kWh bundle |\n";
    std::cout << "   | ElectricBus  | passengerCapacity, permitCode   | Daily rate + seat levy  |\n";
    std::cout << "   ----------------------------------------------------------------------------\n\n";

    std::cout << "3. Testable Requirements:\n";
    std::cout << "   [TR-1] Polymorphism: Fleet manager must invoke operations uniformly via Vehicle*.\n";
    std::cout << "   [TR-2] State Safety: Double renting a vehicle must fail gracefully.\n";
    std::cout << "   [TR-3] Late Fee Accounting: Late returns must apply 150% rate to overdue days.\n";
    std::cout << "   [TR-4] Extensibility (OCP): New vehicle types must plug in with zero modifications\n";
    std::cout << "          to pre-existing working class declarations.\n";
    std::cout << "===============================================================================\n\n";
}

void displayStage2Hierarchy() {
    std::cout << "===============================================================================\n";
    std::cout << "STAGE 2: CLASS HIERARCHY ARCHITECTURE (UML)\n";
    std::cout << "===============================================================================\n";
    std::cout << "                       +---------------------------+\n";
    std::cout << "                       |     <<abstract>>          |\n";
    std::cout << "                       |          Vehicle          |\n";
    std::cout << "                       +---------------------------+\n";
    std::cout << "                       | # regNo: string           |\n";
    std::cout << "                       | # dailyRate: double       |\n";
    std::cout << "                       | # isRented: bool          |\n";
    std::cout << "                       | # bookedDays: int         |\n";
    std::cout << "                       +---------------------------+\n";
    std::cout << "                       | + calculateRental() = 0   |\n";
    std::cout << "                       | + displayDetails() = 0    |\n";
    std::cout << "                       | + rentVehicle(days)       |\n";
    std::cout << "                       | + returnVehicle(days)     |\n";
    std::cout << "                       | + ~Vehicle() = default    |\n";
    std::cout << "                       +-------------+-------------+\n";
    std::cout << "                                     |\n";
    std::cout << "                    +----------------+----------------+\n";
    std::cout << "                    |                                 |\n";
    std::cout << "        +-----------+-----------+         +-----------+-----------+\n";
    std::cout << "        |        Bicycle        |         |     <<abstract>>      |\n";
    std::cout << "        +-----------------------+         |      MotorVehicle     |\n";
    std::cout << "        | - gearCount: int      |         +-----------------------+\n";
    std::cout << "        +-----------------------+         | # fuelOrCharge: double|\n";
    std::cout << "                                          | # serviceDate: string |\n";
    std::cout << "                                          +-----------+-----------+\n";
    std::cout << "                                                      |\n";
    std::cout << "                    +---------------------------------+-------------------------------+\n";
    std::cout << "                    |                                 |                               |\n";
    std::cout << "        +-----------+-----------+         +-----------+-----------+       +-----------+-----------+\n";
    std::cout << "        |       TwoWheeler      |         |          Car          |       |      ElectricBus      |\n";
    std::cout << "        +-----------------------+         +-----------------------+       |   (OCP Extension)     |\n";
    std::cout << "        | - engineCC: int       |         | # seatingCapacity: int|       +-----------------------+\n";
    std::cout << "        | - helmetIncluded: bool|         | # fuelType: string    |       | - passengers: int     |\n";
    std::cout << "        +-----------------------+         +-----------+-----------+       | - batteryKWh: double  |\n";
    std::cout << "                                                      |                   | - permitCode: string  |\n";
    std::cout << "                                          +-----------+-----------+       +-----------------------+\n";
    std::cout << "                                          |      ElectricCar      |\n";
    std::cout << "                                          +-----------------------+\n";
    std::cout << "                                          | - batteryKWh: double  |\n";
    std::cout << "                                          | - chargeRate: double  |\n";
    std::cout << "                                          +-----------------------+\n";
    std::cout << "===============================================================================\n\n";
}

void displayStage3CriticalDecisions() {
    std::cout << "===============================================================================\n";
    std::cout << "STAGE 3: CRITICAL DESIGN DECISIONS\n";
    std::cout << "===============================================================================\n";
    std::cout << "1. Abstract Base Class & Pure Virtual Functions:\n";
    std::cout << "   - 'calculateRental(days)' and 'displayDetails()' are declared pure virtual (= 0).\n";
    std::cout << "   - Reason: A generic 'Vehicle' cannot compute charges because the physical modality\n";
    std::cout << "     (pedal, petrol, electric) dictates fuel, energy, and maintenance costs.\n";
    std::cout << "2. Virtual Destructor:\n";
    std::cout << "   - 'virtual ~Vehicle() = default;' is placed in the base class to guarantee that\n";
    std::cout << "     when derived objects are deleted through base-class pointers (Vehicle*),\n";
    std::cout << "     the complete destructor hierarchy executes without undefined behaviour.\n";
    std::cout << "3. Fleet Storage Strategy:\n";
    std::cout << "   - Stored as an array of base-class pointers: Vehicle* fleet[N].\n";
    std::cout << "   - Avoids object slicing (which would occur if stored in Vehicle fleet[N] value array).\n";
    std::cout << "   - Enables uniform dynamic dispatch (vptr -> vtable) across heterogeneous objects.\n";
    std::cout << "===============================================================================\n\n";
}

void displayStage4Pseudocode() {
    std::cout << "===============================================================================\n";
    std::cout << "STAGE 4: ALGORITHMIC PSEUDOCODE (SIMATS Algorithmic Standard)\n";
    std::cout << "===============================================================================\n";
    std::cout << "ALGORITHM RentVehicle(vehiclePtr, requestedDays):\n";
    std::cout << "  INPUT:  vehiclePtr (Vehicle*), requestedDays (Integer)\n";
    std::cout << "  OUTPUT: Success (Boolean)\n";
    std::cout << "  BEGIN\n";
    std::cout << "    IF vehiclePtr->isRented == TRUE THEN\n";
    std::cout << "      PRINT \"Vehicle already rented\"\n";
    std::cout << "      RETURN FALSE\n";
    std::cout << "    END IF\n";
    std::cout << "    IF requestedDays <= 0 THEN\n";
    std::cout << "      PRINT \"Invalid rental duration\"\n";
    std::cout << "      RETURN FALSE\n";
    std::cout << "    END IF\n";
    std::cout << "    vehiclePtr->isRented <- TRUE\n";
    std::cout << "    vehiclePtr->bookedDays <- requestedDays\n";
    std::cout << "    PRINT \"Vehicle successfully rented\"\n";
    std::cout << "    RETURN TRUE\n";
    std::cout << "  END\n\n";

    std::cout << "ALGORITHM ReturnVehicle(vehiclePtr, actualDays, lateFeeRate):\n";
    std::cout << "  INPUT:  vehiclePtr (Vehicle*), actualDays (Integer), lateFeeRate (Real)\n";
    std::cout << "  OUTPUT: TotalFee (Real)\n";
    std::cout << "  BEGIN\n";
    std::cout << "    IF vehiclePtr->isRented == FALSE THEN\n";
    std::cout << "      PRINT \"Vehicle was not rented\"\n";
    std::cout << "      RETURN 0.0\n";
    std::cout << "    END IF\n";
    std::cout << "    baseCharge <- vehiclePtr->calculateRental(vehiclePtr->bookedDays)\n";
    std::cout << "    lateFee <- 0.0\n";
    std::cout << "    IF actualDays > vehiclePtr->bookedDays THEN\n";
    std::cout << "      overdueDays <- actualDays - vehiclePtr->bookedDays\n";
    std::cout << "      lateFee <- overdueDays * (vehiclePtr->dailyRate * lateFeeRate)\n";
    std::cout << "    END IF\n";
    std::cout << "    totalPayable <- baseCharge + lateFee\n";
    std::cout << "    vehiclePtr->isRented <- FALSE\n";
    std::cout << "    vehiclePtr->bookedDays <- 0\n";
    std::cout << "    RETURN totalPayable\n";
    std::cout << "  END\n";
    std::cout << "===============================================================================\n\n";
}

void displayAlternativeConceptAnalysis() {
    std::cout << "===============================================================================\n";
    std::cout << "ALTERNATIVE CONCEPT & COMPLEXITY ANALYSIS\n";
    std::cout << "===============================================================================\n";
    std::cout << "Alternative Architecture: Monolithic Single 'Vehicle' Class with Type Code & Switch.\n";
    std::cout << "  class MonolithicVehicle {\n";
    std::cout << "    int type; // 1=Bicycle, 2=TwoWheeler, 3=Car, 4=ElectricCar\n";
    std::cout << "    int gearCount, engineCC, seatingCapacity; double batteryKWh;\n";
    std::cout << "    double calculateRental(int days) {\n";
    std::cout << "      switch(type) { case 1: ...; case 2: ...; case 3: ...; case 4: ...; }\n";
    std::cout << "    }\n";
    std::cout << "  };\n\n";

    std::cout << "Rigorous Comparative Evaluation:\n";
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "| Metric            | Monolithic Switch Approach    | Abstract Base Class Hierarchy|\n";
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "| Time Complexity   | O(1) jump table / branch check| O(1) vtable pointer dereference\n";
    std::cout << "| Space per Object  | High (union/wasted padding for| Optimal (only fields needed +\n";
    std::cout << "|                   | unused vehicle attributes)    | one 8-byte vptr per object)  |\n";
    std::cout << "| Extensibility     | VIOLATES OCP: Adding bus      | STRICT OCP: Add 'ElectricBus'\n";
    std::cout << "|                   | requires editing every switch | without modifying any classes|\n";
    std::cout << "| Maintainability   | Fragile: risk of breaking     | Robust: bug in one subclass  |\n";
    std::cout << "|                   | existing vehicle calculations | is isolated to that subclass |\n";
    std::cout << "-----------------------------------------------------------------------------------\n";
    std::cout << "===============================================================================\n\n";
}

int main() {
    std::cout << "*******************************************************************************\n";
    std::cout << "  DSA01 ASSIGNMENT 4 - TASK 06: VEHICLE RENTAL FLEET DESIGN CHALLENGE           \n";
    std::cout << "  Student: Manoj Kumar Chinthala | Register Number: 192472008                  \n";
    std::cout << "  Institution: SIMATS Engineering, Saveetha University                          \n";
    std::cout << "*******************************************************************************\n\n";

    displayStage1Interpretation();
    displayStage2Hierarchy();
    displayStage3CriticalDecisions();
    displayStage4Pseudocode();

    std::cout << "===============================================================================\n";
    std::cout << "STAGE 5: IMPLEMENTATION & RUNTIME DEMONSTRATION\n";
    std::cout << "===============================================================================\n";

    // Fleet instantiated as an array of Base-Class Pointers (Vehicle*)
    const int FLEET_SIZE = 5;
    Vehicle* fleet[FLEET_SIZE];

    fleet[0] = new Bicycle("TN-01-BK-101", 150.0, 21);
    fleet[1] = new TwoWheeler("TN-02-TW-202", 450.0, 85.0, "2026-12-15", 150, true);
    fleet[2] = new Car("TN-03-CR-303", 2200.0, 90.0, "2027-01-20", 7, "Diesel");
    fleet[3] = new ElectricCar("TN-04-EC-404", 2800.0, 95.0, "2027-03-10", 5, 60.0, 12.0);
    // Dynamic extension class demonstrating OCP without modifying existing classes
    fleet[4] = new ElectricBus("TN-05-EB-505", 8500.0, 100.0, "2027-06-01", 35, 250.0, "TN-PERMIT-409");

    std::cout << "A. Initial Fleet Listing (Polymorphic displayDetails via Vehicle*):\n";
    for (int i = 0; i < FLEET_SIZE; ++i) {
        fleet[i]->displayDetails();
    }
    std::cout << '\n';

    std::cout << "B. Renting Each Vehicle Type:\n";
    fleet[0]->rentVehicle(7);  // Bicycle (7 days -> qualifies for 5% eco discount)
    fleet[1]->rentVehicle(3);  // TwoWheeler (3 days)
    fleet[2]->rentVehicle(4);  // 7-seater Car (4 days)
    fleet[3]->rentVehicle(2);  // Electric Car (2 days)
    fleet[4]->rentVehicle(5);  // Electric Bus (5 days)
    std::cout << '\n';

    std::cout << "C. State Safety Test (Attempting to rent an already rented vehicle):\n";
    fleet[2]->rentVehicle(2); // Should reject gracefully
    std::cout << '\n';

    std::cout << "D. Returning Vehicles (On-time vs Late Returns):\n";
    std::cout << "1. Return Bicycle (Booked: 7 days, Actual: 7 days - On-time):\n";
    fleet[0]->returnVehicle(7);

    std::cout << "\n2. Return TwoWheeler (Booked: 3 days, Actual: 5 days - 2 days LATE):\n";
    fleet[1]->returnVehicle(5);

    std::cout << "\n3. Return Diesel 7-Seater Car (Booked: 4 days, Actual: 4 days - On-time):\n";
    fleet[2]->returnVehicle(4);

    std::cout << "\n4. Return Electric Car (Booked: 2 days, Actual: 3 days - 1 day LATE):\n";
    fleet[3]->returnVehicle(3);

    std::cout << "\n5. Return Electric Bus (Extension Class) (Booked: 5 days, Actual: 5 days - On-time):\n";
    fleet[4]->returnVehicle(5);
    std::cout << '\n';

    std::cout << "E. Final Fleet Status Listing:\n";
    for (int i = 0; i < FLEET_SIZE; ++i) {
        fleet[i]->displayDetails();
    }
    std::cout << "===============================================================================\n\n";

    displayAlternativeConceptAnalysis();

    // Clean up heap memory through polymorphic base destructor
    for (int i = 0; i < FLEET_SIZE; ++i) {
        delete fleet[i];
        fleet[i] = nullptr;
    }

    std::cout << "Fleet memory successfully released via virtual base destructor ~Vehicle().\n";
    return 0;
}
