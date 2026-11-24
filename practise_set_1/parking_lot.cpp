
#include <bits/stdc++.h>
using namespace std;

// ================= ENUM =================

enum VehicleSize {
    SMALL,
    MEDIUM,
    LARGE
};

// ================= VEHICLE =================

class Vehicle {
public:
    string license_plate;
    VehicleSize size;

    Vehicle(string plate, VehicleSize size) {
        this->license_plate = plate;
        this->size = size;
    }

    virtual ~Vehicle() = default;
};

class Car : public Vehicle {
public:
    Car(string plate) : Vehicle(plate, SMALL) {}
};

class Bus : public Vehicle {
public:
    Bus(string plate) : Vehicle(plate, LARGE) {}
};

class Bike : public Vehicle {
public:
    Bike(string plate) : Vehicle(plate, SMALL) {}
};

// ================= PARKING SPOT =================

class ParkingSpot {
public:
    int spot_number;
    VehicleSize size;
    Vehicle* vehicle;

    ParkingSpot(int id, VehicleSize size) {
        this->spot_number = id;
        this->size = size;
        this->vehicle = nullptr;
    }

    bool isAvailable() {
        return vehicle == nullptr;
    }

    void parkVehicle(Vehicle* v) {
        vehicle = v;
    }

    void removeVehicle() {
        vehicle = nullptr;
    }
};

class SmallParkingSpot : public ParkingSpot {
public:
    SmallParkingSpot(int id) : ParkingSpot(id, SMALL) {}
};

class MediumParkingSpot : public ParkingSpot {
public:
    MediumParkingSpot(int id) : ParkingSpot(id, MEDIUM) {}
};

class LargeParkingSpot : public ParkingSpot {
public:
    LargeParkingSpot(int id) : ParkingSpot(id, LARGE) {}
};

// ================= PARKING FLOOR =================

class ParkingFloor {
private:
    int floor_number;

    map<VehicleSize, vector<ParkingSpot*>> available_parking_spots;

public:
    ParkingFloor(
        int floor_number,
        map<VehicleSize, vector<ParkingSpot*>> spots
    ) {
        this->floor_number = floor_number;
        this->available_parking_spots = move(spots);
    }

    int getFloorNumber() {
        return floor_number;
    }

    // Find a suitable spot on THIS floor
    ParkingSpot* findAvailableSpot(VehicleSize size) {

        for (int i = size; i <= LARGE; i++) {

            VehicleSize current_size =
                static_cast<VehicleSize>(i);

            auto it = available_parking_spots.find(current_size);

            if (it != available_parking_spots.end() &&
                !it->second.empty()) {

                return it->second.back();
            }
        }

        return nullptr;
    }

    // Remove spot from available list
    void occupySpot(ParkingSpot* spot) {

        auto& spots = available_parking_spots[spot->size];

        auto it = find(spots.begin(), spots.end(), spot);

        if (it != spots.end()) {
            spots.erase(it);
        }
    }

    // Return spot to available list
    void releaseSpot(ParkingSpot* spot) {
        available_parking_spots[spot->size].push_back(spot);
    }
};

// ================= TICKET =================

class Ticket {
public:
    int ticket_number;

    Vehicle* vehicle;
    ParkingSpot* spot;
    ParkingFloor* floor;

    string entry_time;

    Ticket(
        int id,
        Vehicle* vehicle,
        ParkingSpot* spot,
        ParkingFloor* floor,
        string entry_time
    ) {
        this->ticket_number = id;
        this->vehicle = vehicle;
        this->spot = spot;
        this->floor = floor;
        this->entry_time = entry_time;
    }

    int calculateParkingFee(string exit_time) {

        int entry_hour = stoi(entry_time.substr(0, 2));
        int entry_minute = stoi(entry_time.substr(3, 2));

        int exit_hour = stoi(exit_time.substr(0, 2));
        int exit_minute = stoi(exit_time.substr(3, 2));

        int total_minutes =
            (exit_hour * 60 + exit_minute) -
            (entry_hour * 60 + entry_minute);

        // Handle overnight parking
        if (total_minutes < 0) {
            total_minutes += 24 * 60;
        }

        int fee_per_hour = 0;

        switch (vehicle->size) {
            case SMALL:
                fee_per_hour = 10;
                break;

            case MEDIUM:
                fee_per_hour = 20;
                break;

            case LARGE:
                fee_per_hour = 30;
                break;
        }

        // Charge every started hour
        int hours = (total_minutes + 59) / 60;

        return hours * fee_per_hour;
    }
};

// ================= PARKING MANAGER =================

class ParkingManager {
private:
    vector<ParkingFloor*> parking_floors;

    map<Vehicle*, Ticket*> vehicle_tickets;

    int next_ticket_number = 1;

public:

    ParkingManager(vector<ParkingFloor*> floors) {
        parking_floors = move(floors);
    }

    // Find spot across ALL floors
    pair<ParkingFloor*, ParkingSpot*>
    findAvailableSpot(VehicleSize size) {

        for (ParkingFloor* floor : parking_floors) {

            ParkingSpot* spot = floor->findAvailableSpot(size);

            if (spot != nullptr) {
                return {floor, spot};
            }
        }

        return {nullptr, nullptr};
    }

    // ================= PARK VEHICLE =================

    void parkVehicle(Vehicle* vehicle, string entry_time) {

        if (vehicle == nullptr) {
            cout << "Invalid vehicle\n";
            return;
        }

        // Check duplicate parking
        if (vehicle_tickets.find(vehicle) != vehicle_tickets.end()) {
            cout << "Vehicle is already parked\n";
            return;
        }

        auto result = findAvailableSpot(vehicle->size);

        ParkingFloor* floor = result.first;
        ParkingSpot* spot = result.second;

        if (spot == nullptr) {
            cout << "No suitable parking spot available\n";
            return;
        }

        // Occupy spot
        floor->occupySpot(spot);
        spot->parkVehicle(vehicle);

        // Generate ticket
        Ticket* new_ticket = new Ticket(
            next_ticket_number++,
            vehicle,
            spot,
            floor,
            entry_time
        );

        vehicle_tickets[vehicle] = new_ticket;

        cout << "Vehicle " << vehicle->license_plate
             << " parked on Floor " << floor->getFloorNumber()
             << ", Spot " << spot->spot_number
             << ". Ticket: " << new_ticket->ticket_number
             << endl;
    }

    // ================= UNPARK VEHICLE =================

    void unparkVehicle(Vehicle* vehicle, string exit_time) {

        auto it = vehicle_tickets.find(vehicle);

        if (it == vehicle_tickets.end()) {
            cout << "Vehicle is not parked\n";
            return;
        }

        Ticket* t = it->second;

        int fee = t->calculateParkingFee(exit_time);

        cout << "Vehicle " << vehicle->license_plate
             << " exited from Floor " << t->floor->getFloorNumber()
             << ", Spot " << t->spot->spot_number
             << ". Fee: Rs. " << fee
             << endl;

        // Free spot
        t->spot->removeVehicle();

        // Return spot to its ORIGINAL floor
        t->floor->releaseSpot(t->spot);

        // Remove ticket
        vehicle_tickets.erase(it);
        delete t;
    }

    ~ParkingManager() {

        for (auto& entry : vehicle_tickets) {
            delete entry.second;
        }
    }
};

// ================= MAIN =================

class ParkingLot {
private:
    unique_ptr<ParkingManager> manager;

public:
    ParkingLot(vector<ParkingFloor*> floors) {
        manager = make_unique<ParkingManager>(floors);
    }

    void parkVehicle(Vehicle* vehicle, string entry_time) {
        manager->parkVehicle(vehicle, entry_time);
    }

    void unparkVehicle(Vehicle* vehicle, string exit_time) {
        manager->unparkVehicle(vehicle, exit_time);
    }
};
int main() {

    ParkingFloor* floor1 = new ParkingFloor(1, {
        {SMALL, {
            new SmallParkingSpot(1),
            new SmallParkingSpot(2)
        }},
        {MEDIUM, {
            new MediumParkingSpot(3)
        }},
        {LARGE, {
            new LargeParkingSpot(4)
        }}
    });

    ParkingFloor* floor2 = new ParkingFloor(2, {
        {SMALL, {
            new SmallParkingSpot(5)
        }},
        {MEDIUM, {
            new MediumParkingSpot(6)
        }},
        {LARGE, {
            new LargeParkingSpot(7)
        }}
    });

    ParkingLot lot({floor1, floor2});

    Vehicle* car = new Car("ABC123");

    lot.parkVehicle(car, "10:00");
    lot.unparkVehicle(car, "12:00");

    delete car;
    delete floor1;
    delete floor2;

    return 0;
}