# MSCS-632 Assignment 5: Class-Based Ride Sharing System

A Ride Sharing System implemented in **C++17** and **GNU Smalltalk**, demonstrating
encapsulation, inheritance, and polymorphism.

## Structure

```
cpp/main.cpp                 C++ implementation
cpp/output.txt               Sample output
smalltalk/ridesharing.st     Smalltalk implementation
smalltalk/output.txt         Sample output
screenshots/                 Code and output images
```

## Classes

| Class | Description |
|-------|-------------|
| `Ride` | Base class: rideID, pickup, dropoff, distance; `fare()` and `rideDetails()` |
| `StandardRide` | $2.00 base + $1.75 per mile |
| `PremiumRide` | $5.00 service fee + $3.25 per mile |
| `SharedRide` | $1.00 + per-mile cost split among passengers |
| `Driver` | driverID, name, rating, private `assignedRides`; `addRide`, `getDriverInfo` |
| `Rider` | riderID, name, private `requestedRides`; `requestRide`, `viewRides` |

Each program stores rides of different types in one collection and calls `fare()` and
`rideDetails()` polymorphically.

## Running the C++ version

Requires a C++17 compiler (clang++ or g++).

```bash
cd cpp
clang++ -std=c++17 -Wall -Wextra -o ridesharing main.cpp
./ridesharing
```

(Use `g++` in place of `clang++` if you prefer.)

## Running the Smalltalk version

Requires [GNU Smalltalk](https://www.gnu.org/software/smalltalk/) (`gst`) 3.2.5 or later.

```bash
# macOS
brew install gnu-smalltalk
# Ubuntu / Debian
sudo apt install gnu-smalltalk

cd smalltalk
gst ridesharing.st
```

Both programs print the same output (see `cpp/output.txt` and `smalltalk/output.txt`).
