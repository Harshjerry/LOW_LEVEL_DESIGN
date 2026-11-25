
#include <bits/stdc++.h>
using namespace std;

// ================= ENUMS =================

enum Direction {
    UP,
    DOWN,
    IDLE
};

// ================= DOOR =================

class Door {
public:
    void open_door(int floor) {
        cout << "Door opened at floor " << floor << "\n";
    }

    void close_door(int floor) {
        cout << "Door closed at floor " << floor << "\n";
    }
};

// ================= DISPLAY =================

class Display {
public:
    void show(int floor, Direction direction) {
        cout << "Current floor: " << floor << " | Direction: ";

        if (direction == UP)
            cout << "UP";
        else if (direction == DOWN)
            cout << "DOWN";
        else
            cout << "IDLE";

        cout << "\n";
    }
};

// ================= ELEVATOR =================

class Elevator {
private:
    Door door;
    Display display;

    bool has_requests_above() {
        return
            (!pickup_req_up.empty() &&
             *pickup_req_up.rbegin() > currentFloor) ||

            (!dest_req_up.empty() &&
             *dest_req_up.rbegin() > currentFloor) ||

            (!pickup_req_down.empty() &&
             *pickup_req_down.rbegin() > currentFloor) ||

            (!dest_req_down.empty() &&
             *dest_req_down.rbegin() > currentFloor);
    }

    bool has_requests_below() {
        return
            (!pickup_req_up.empty() &&
             *pickup_req_up.begin() < currentFloor) ||

            (!dest_req_up.empty() &&
             *dest_req_up.begin() < currentFloor) ||

            (!pickup_req_down.empty() &&
             *pickup_req_down.begin() < currentFloor) ||

            (!dest_req_down.empty() &&
             *dest_req_down.begin() < currentFloor);
    }

    bool has_requests_at_current_floor() {
        return pickup_req_up.count(currentFloor) ||
               pickup_req_down.count(currentFloor) ||
               dest_req_up.count(currentFloor) ||
               dest_req_down.count(currentFloor);
    }

    void process_current_floor() {
        bool should_stop =
            pickup_req_up.count(currentFloor) ||
            pickup_req_down.count(currentFloor) ||
            dest_req_up.count(currentFloor) ||
            dest_req_down.count(currentFloor);

        if (should_stop) {
            door.open_door(currentFloor);

            pickup_req_up.erase(currentFloor);
            pickup_req_down.erase(currentFloor);
            dest_req_up.erase(currentFloor);
            dest_req_down.erase(currentFloor);

            door.close_door(currentFloor);
        }

        display.show(currentFloor, current_direction);
    }

public:
    int id;
    int currentFloor;
    int totalFloors;

    Direction current_direction = IDLE;

    set<int> pickup_req_up;
    set<int> dest_req_up;
    set<int> pickup_req_down;
    set<int> dest_req_down;

    Elevator(int id, int totalFloors)
        : id(id),
          currentFloor(1),
          totalFloors(totalFloors) {}

    int get_id() const {
        return id;
    }

    void add_pickup_request(int floor, Direction direction) {
        if (floor < 1 || floor > totalFloors)
            return;

        if (direction == UP)
            pickup_req_up.insert(floor);
        else if (direction == DOWN)
            pickup_req_down.insert(floor);

        if (current_direction == IDLE) {
            if (floor > currentFloor)
                current_direction = UP;
            else if (floor < currentFloor)
                current_direction = DOWN;
        }
    }

    void select_destination(int destination_floor) {
        if (destination_floor < 1 || destination_floor > totalFloors) {
            cout << "Invalid destination floor\n";
            return;
        }

        if (destination_floor > currentFloor)
            dest_req_up.insert(destination_floor);

        else if (destination_floor < currentFloor)
            dest_req_down.insert(destination_floor);
    }

    void move() {
        if (current_direction == IDLE) {
            if (has_requests_above())
                current_direction = UP;
            else if (has_requests_below())
                current_direction = DOWN;
            else {
                display.show(currentFloor, IDLE);
                return;
            }
        }

        // If moving UP
        if (current_direction == UP) {

            if (!has_requests_above()) {
                if (has_requests_below())
                    current_direction = DOWN;
                else {
                    current_direction = IDLE;
                    display.show(currentFloor, IDLE);
                    return;
                }
            }
        }

        // If moving DOWN
        else if (current_direction == DOWN) {

            if (!has_requests_below()) {
                if (has_requests_above())
                    current_direction = UP;
                else {
                    current_direction = IDLE;
                    display.show(currentFloor, IDLE);
                    return;
                }
            }
        }

        // Process requests at current floor before moving.
        if (has_requests_at_current_floor()) {
            process_current_floor();
            return;
        }

        // Move one floor.
        if (current_direction == UP && currentFloor < totalFloors) {
            currentFloor++;
        }
        else if (current_direction == DOWN && currentFloor > 1) {
            currentFloor--;
        }

        // Process requests after reaching the new floor.
        process_current_floor();

        // If no requests remain, become IDLE.
        if (!has_requests_above() && !has_requests_below()) {
            current_direction = IDLE;
            display.show(currentFloor, IDLE);
        }
    }
};

// ================= ELEVATOR CONTROLLER =================

class ElevatorController {
private:
    vector<Elevator*>& elevators;

public:
    ElevatorController(vector<Elevator*>& elevators)
        : elevators(elevators) {}

    Elevator* get_best_elevator(
        int requested_floor,
        Direction requested_direction
    ) {
        Elevator* best_elevator = nullptr;
        int min_distance = INT_MAX;

        for (Elevator* elevator : elevators) {

            int distance =
                abs(elevator->currentFloor - requested_floor);

            if (distance < min_distance) {
                min_distance = distance;
                best_elevator = elevator;
            }
        }

        return best_elevator;
    }

    Elevator* process_request(
        int requested_floor,
        Direction requested_direction
    ) {
        Elevator* best_elevator =
            get_best_elevator(requested_floor, requested_direction);

        if (best_elevator != nullptr) {
            best_elevator->add_pickup_request(
                requested_floor,
                requested_direction
            );
        }

        return best_elevator;
    }
};

// ================= ELEVATOR SYSTEM =================

class ElevatorSystem {
private:
    vector<Elevator*> elevators;
    int totalFloors;

    ElevatorController controller;

public:
    ElevatorSystem(int floors, int numberOfElevators)
        : totalFloors(floors),
          controller(elevators) {

        for (int i = 0; i < numberOfElevators; i++) {
            elevators.push_back(
                new Elevator(i, totalFloors)
            );
        }
    }

    Elevator* request_elevator(int floor, Direction direction) {
        if (floor < 1 || floor > totalFloors) {
            cout << "Invalid pickup floor\n";
            return nullptr;
        }

        return controller.process_request(floor, direction);
    }

    void select_destination(int elevator_id, int destination_floor) {
        for (Elevator* elevator : elevators) {
            if (elevator->get_id() == elevator_id) {
                elevator->select_destination(destination_floor);
                return;
            }
        }

        cout << "Invalid elevator ID\n";
    }

    void step() {
        cout << "\n========== NEXT TIME STEP ==========\n";

        for (Elevator* elevator : elevators) {
            cout << "\nElevator " << elevator->get_id() << "\n";
            elevator->move();
        }
    }

    ~ElevatorSystem() {
        for (Elevator* elevator : elevators) {
            delete elevator;
        }
    }
};

// ================= MAIN =================

int main() {
    ElevatorSystem system(10, 3);

    Elevator* assigned = system.request_elevator(5, UP);

    if (assigned != nullptr) {
        cout << "Assigned Elevator: "
             << assigned->get_id() << "\n";

        system.select_destination(
            assigned->get_id(),
            8
        );
    }

    // Run the simulation.
    for (int i = 0; i < 15; i++) {
        system.step();
    }

    return 0;
}
