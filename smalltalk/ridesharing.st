"Ride Sharing System - GNU Smalltalk implementation
 Run with:  gst ridesharing.st
 Demonstrates encapsulation, inheritance, and polymorphism."

Object subclass: Ride [
    | rideID pickupLocation dropoffLocation distance |

    Ride class >> id: anId from: pickup to: dropoff distance: miles [
        ^self new setId: anId from: pickup to: dropoff distance: miles
    ]

    "Format a number as dollars and cents, e.g. 23.875 -> '23.88'"
    Ride class >> money: amount [
        | cents frac |
        cents := (amount * 100) rounded.
        frac := (cents \\ 100) printString.
        frac size = 1 ifTrue: [frac := '0', frac].
        ^(cents // 100) printString, '.', frac
    ]

    "Format with one decimal place, e.g. 4.2 -> '4.2' (avoids gst 3.2.5 Float printing bug)"
    Ride class >> tenths: amount [
        | t |
        t := (amount * 10) rounded.
        ^(t // 10) printString, '.', (t \\ 10) printString
    ]

    setId: anId from: pickup to: dropoff distance: miles [
        rideID := anId.
        pickupLocation := pickup.
        dropoffLocation := dropoff.
        distance := miles
    ]

    rideID [ ^rideID ]
    distance [ ^distance ]

    "Polymorphic interface"
    fare [ ^distance * 1.5 ]
    rideType [ ^'Basic' ]

    rideDetails [
        ^'Ride #', rideID printString, ' [', self rideType, '] ',
            pickupLocation, ' -> ', dropoffLocation, ', ',
            (Ride tenths: distance), ' mi, fare $', (Ride money: self fare)
    ]
]

Ride subclass: StandardRide [
    fare [ ^2.0 + (distance * 1.75) ]
    rideType [ ^'Standard' ]
]

Ride subclass: PremiumRide [
    fare [ ^5.0 + (distance * 3.25) ]
    rideType [ ^'Premium' ]
    rideDetails [
        ^super rideDetails, '
    (includes $5.00 premium service fee, luxury vehicle)'
    ]
]

Ride subclass: SharedRide [
    | passengers |
    passengers: n [ passengers := n ]
    fare [ ^((1.25 * distance) / (passengers max: 1)) + 1.0 ]
    rideType [ ^'Shared x', passengers printString ]
]

Object subclass: Driver [
    | driverID name rating assignedRides |   "assignedRides is private"

    Driver class >> id: anId name: aName rating: aRating [
        ^self new setId: anId name: aName rating: aRating
    ]

    setId: anId name: aName rating: aRating [
        driverID := anId.
        name := aName.
        rating := aRating.
        assignedRides := OrderedCollection new
    ]

    addRide: aRide [ assignedRides add: aRide ]

    totalEarnings [
        ^assignedRides inject: 0 into: [:sum :r | sum + r fare]
    ]

    getDriverInfo [
        | s |
        s := WriteStream on: String new.
        s nextPutAll: 'Driver #', driverID printString, ' ', name,
            ' | rating ', (Ride tenths: rating),
            ' | rides completed: ', assignedRides size printString,
            ' | earnings $', (Ride money: self totalEarnings); nl.
        assignedRides do: [:r | s nextPutAll: '  - ', r rideDetails; nl].
        ^s contents
    ]
]

Object subclass: Rider [
    | riderID name requestedRides |

    Rider class >> id: anId name: aName [
        ^self new setId: anId name: aName
    ]

    setId: anId name: aName [
        riderID := anId.
        name := aName.
        requestedRides := OrderedCollection new
    ]

    requestRide: aRide [ requestedRides add: aRide ]

    viewRides [
        | s |
        s := WriteStream on: String new.
        s nextPutAll: 'Rider #', riderID printString, ' ', name,
            ' - ride history (', requestedRides size printString, '):'; nl.
        requestedRides do: [:r | s nextPutAll: '  - ', r rideDetails; nl].
        ^s contents
    ]
]

"------------------------------ main program ------------------------------"
| rides total d1 d2 r1 r2 shared |

rides := OrderedCollection new.
rides add: (StandardRide id: 101 from: 'Airport' to: 'Downtown' distance: 12.5).
rides add: (PremiumRide id: 102 from: 'Hotel' to: 'Convention Center' distance: 6.0).
shared := SharedRide id: 103 from: 'Campus' to: 'Mall' distance: 8.0.
shared passengers: 3.
rides add: shared.
rides add: (StandardRide id: 104 from: 'Library' to: 'Stadium' distance: 4.2).

Transcript showCr: '=== Polymorphism: fare and rideDetails on mixed Ride collection ==='.
total := 0.
rides do: [:r |
    Transcript showCr: r rideDetails.
    total := total + r fare].
Transcript showCr: 'Total fares: $', (Ride money: total); cr.

d1 := Driver id: 1 name: 'Maria Lopez' rating: 4.9.
d2 := Driver id: 2 name: 'James Chen' rating: 4.6.
r1 := Rider id: 10 name: 'Alice Johnson'.
r2 := Rider id: 11 name: 'Bob Smith'.

r1 requestRide: (rides at: 1). d1 addRide: (rides at: 1).
r1 requestRide: (rides at: 2). d2 addRide: (rides at: 2).
r2 requestRide: (rides at: 3). d1 addRide: (rides at: 3).
r2 requestRide: (rides at: 4). d2 addRide: (rides at: 4).

Transcript showCr: '=== Drivers ==='.
Transcript show: d1 getDriverInfo; show: d2 getDriverInfo; cr.
Transcript showCr: '=== Riders ==='.
Transcript show: r1 viewRides; show: r2 viewRides.
