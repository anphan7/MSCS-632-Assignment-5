// Ride Sharing System - C++ implementation
// Demonstrates encapsulation, inheritance, and polymorphism.
//
// Build: clang++ -std=c++17 -o ridesharing main.cpp && ./ridesharing
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ---------------------------------------------------------------- Ride (base)
// Base class holding the details every ride has. Subclasses change how the
// fare is calculated by overriding the virtual methods (polymorphism).
class Ride {
protected:  // ENCAPSULATION: visible to subclasses, hidden from outside code
    int rideID;
    std::string pickupLocation;
    std::string dropoffLocation;
    double distance;  // miles

public:
    Ride(int id, std::string pickup, std::string dropoff, double miles)
        : rideID(id), pickupLocation(std::move(pickup)),
          dropoffLocation(std::move(dropoff)), distance(miles) {}
    virtual ~Ride() = default;  // virtual so deleting via Ride* is safe

    // Default fare; subclasses override this. "virtual" makes the call
    // resolve at run time, based on the object's real type.
    virtual double fare() const { return distance * 1.50; }
    virtual std::string rideType() const { return "Basic"; }

    // Prints one line of ride info. It calls fare() and rideType(), so each
    // subclass automatically prints its own fare and type name.
    virtual void rideDetails() const {
        std::cout << "Ride #" << rideID << " [" << rideType() << "] "
                  << pickupLocation << " -> " << dropoffLocation << ", "
                  << std::fixed << std::setprecision(1) << distance << " mi, fare $"
                  << std::setprecision(2) << fare() << "\n";
    }

    int getRideID() const { return rideID; }
};

// ------------------------------------------------------------- StandardRide
// INHERITANCE: reuses Ride's fields and constructor; only the fare differs.
class StandardRide : public Ride {
public:
    using Ride::Ride;  // inherit the base constructor
    double fare() const override { return 2.00 + distance * 1.75; }  // base + per mile
    std::string rideType() const override { return "Standard"; }
};

// -------------------------------------------------------------- PremiumRide
// Higher per-mile rate plus a flat service fee.
class PremiumRide : public Ride {
    static constexpr double kServiceFee = 5.00;

public:
    using Ride::Ride;
    double fare() const override { return kServiceFee + distance * 3.25; }
    std::string rideType() const override { return "Premium"; }

    // Extends the base version: print the normal line, then add a note.
    void rideDetails() const override {
        Ride::rideDetails();
        std::cout << "    (includes $" << std::fixed << std::setprecision(2)
                  << kServiceFee << " premium service fee, luxury vehicle)\n";
    }
};

// ---------------------------------------------------------------- SharedRide
// Adds its own field (passengers); the per-mile cost is split among them.
class SharedRide : public Ride {
    int passengers;

public:
    SharedRide(int id, std::string p, std::string d, double miles, int pax)
        : Ride(id, std::move(p), std::move(d), miles), passengers(pax) {}
    double fare() const override {
        return (1.25 * distance) / (passengers > 0 ? passengers : 1) + 1.00;  // avoid /0
    }
    std::string rideType() const override { return "Shared x" + std::to_string(passengers); }
};

// -------------------------------------------------------------------- Driver
// A driver and the rides they completed. The list is private (ENCAPSULATION),
// so it can only change through addRide(). shared_ptr<Ride> lets the list hold
// any ride subtype and share ownership with the rider.
class Driver {
    int driverID;
    std::string name;
    double rating;
    std::vector<std::shared_ptr<Ride>> assignedRides;

public:
    Driver(int id, std::string n, double r) : driverID(id), name(std::move(n)), rating(r) {}

    void addRide(std::shared_ptr<Ride> ride) { assignedRides.push_back(std::move(ride)); }

    // Sum of all fares; each fare() call picks the right formula per ride type.
    double totalEarnings() const {
        double total = 0;
        for (const auto& r : assignedRides) total += r->fare();
        return total;
    }

    // Prints driver details followed by each completed ride.
    void getDriverInfo() const {
        std::cout << "Driver #" << driverID << " " << name << " | rating "
                  << std::fixed << std::setprecision(1) << rating << " | rides completed: "
                  << assignedRides.size() << " | earnings $" << std::setprecision(2)
                  << totalEarnings() << "\n";
        for (const auto& r : assignedRides) { std::cout << "  - "; r->rideDetails(); }
    }
};

// --------------------------------------------------------------------- Rider
// A rider and the rides they requested (private, same idea as Driver).
class Rider {
    int riderID;
    std::string name;
    std::vector<std::shared_ptr<Ride>> requestedRides;

public:
    Rider(int id, std::string n) : riderID(id), name(std::move(n)) {}

    void requestRide(std::shared_ptr<Ride> ride) { requestedRides.push_back(std::move(ride)); }

    // Prints the rider's ride history.
    void viewRides() const {
        std::cout << "Rider #" << riderID << " " << name << " - ride history ("
                  << requestedRides.size() << "):\n";
        for (const auto& r : requestedRides) { std::cout << "  - "; r->rideDetails(); }
    }
};

// ---------------------------------------------------------------------- main
int main() {
    // POLYMORPHISM: one list holds different ride types, all accessed as Ride.
    std::vector<std::shared_ptr<Ride>> rides = {
        std::make_shared<StandardRide>(101, "Airport", "Downtown", 12.5),
        std::make_shared<PremiumRide>(102, "Hotel", "Convention Center", 6.0),
        std::make_shared<SharedRide>(103, "Campus", "Mall", 8.0, 3),
        std::make_shared<StandardRide>(104, "Library", "Stadium", 4.2),
    };

    // Same call on every element; each ride runs its own fare() and rideDetails().
    std::cout << "=== Polymorphism: fare() and rideDetails() on mixed Ride list ===\n";
    double total = 0;
    for (const auto& r : rides) {
        r->rideDetails();
        total += r->fare();
    }
    std::cout << "Total fares: $" << std::fixed << std::setprecision(2) << total << "\n\n";

    Driver d1(1, "Maria Lopez", 4.9), d2(2, "James Chen", 4.6);
    Rider r1(10, "Alice Johnson"), r2(11, "Bob Smith");

    // Each ride is requested by a rider and completed by a driver.
    r1.requestRide(rides[0]); d1.addRide(rides[0]);
    r1.requestRide(rides[1]); d2.addRide(rides[1]);
    r2.requestRide(rides[2]); d1.addRide(rides[2]);
    r2.requestRide(rides[3]); d2.addRide(rides[3]);

    std::cout << "=== Drivers ===\n";
    d1.getDriverInfo();
    d2.getDriverInfo();
    std::cout << "\n=== Riders ===\n";
    r1.viewRides();
    r2.viewRides();
    return 0;
}
