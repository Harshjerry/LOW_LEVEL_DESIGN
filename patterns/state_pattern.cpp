
#include <bits/stdc++.h>
using namespace std;

// ================= ENUMS =================

enum Direction {
    UP,
    DOWN,
    IDLE
};

// Forward declaration
class Elevator;

// ================= DOOR =================

class Door {
public:
    bool isOpen = false;

    void open_door(int floor) {
        if (!isOpen) {
            isOpen = true;
            cout << "Door opened at floor " << floor << "\n";
        }
    }

    void close_door(int floor) {
        if (isOpen) {
            isOpen = false;
            cout << "Door closed at floor " << floor << "\n";
        }
    }
};

// ================= DISPLAY =================

class Display {
public:
    void show(int floor, Direction direction) {
        cout << "Elevator at floor: " << floor << " | Direction: ";

        if (direction == UP)
            cout << "UP\n";
        else if (direction == DOWN)
            cout << "DOWN\n";
        else
            cout << "IDLE\n";
    }
};

// ================= STATE INTERFACE =================

class ElevatorState {
public:
    virtual void move(Elevator& elevator) = 0;
    virtual ~ElevatorState() = default;
};

// Forward declarations of concrete states
class IdleState : public ElevatorState {
public:
    void move(Elevator& elevator) override;
};

class MovingUpState : public ElevatorState {
public:
    void move(Elevator& elevator) override;
};

class MovingDownState : public ElevatorState {
public:
    void move(Elevator& elevator) override;
};

// ================= ELEVATOR =================

class Elevator {
private:
    shared_ptr<ElevatorState> currentState;

public:
    int id;
    int currentFloor;
    int totalFloors;

    Direction current_direction = IDLE;

    Door door;
    Display display;

    // Four request sets
    set<int> pickup_req_up;
    set<int> pickup_req_down;

    set<int> dest_req_up;
    set<int> dest_req_down;

    // Constructor
    Elevator(int id, int totalFloors)
        : id(id),
          currentFloor(1),
          totalFloors(totalFloors),
          currentState(make_shared<IdleState>()) {}

    // Change current state
    void set_state(shared_ptr<ElevatorState> state, Direction direction) {
        currentState = std::move(state);
        current_direction = direction;
    }

    // Called for every simulation step
    void step() {
        auto state = currentState;
        state->move(*this);
    }

    // Add an external pickup request
    void add_pickup_request(int floor, Direction direction) {
        if (floor < 1 || floor > totalFloors) {
            cout << "Invalid pickup floor\n";
            return;
        }

        if (direction == UP) {
            pickup_req_up.insert(floor);
        }
        else if (direction == DOWN) {
            pickup_req_down.insert(floor);
        }
    }

    // Add an internal destination request
    void select_destination(int floor) {
        if (floor < 1 || floor > totalFloors) {
            cout << "Invalid destination floor\n";
            return;
        }

        if (floor == currentFloor) {
            cout << "Already at floor " << floor << "\n";
            return;
        }

        if (floor > currentFloor) {
            dest_req_up.insert(floor);
        }
        else {
            dest_req_down.insert(floor);
        }
    }

    // Check whether a set has requests above current floor
    bool has_above(const set<int>& requests) {
        return requests.upper_bound(currentFloor) != requests.end();
    }

    // Check whether a set has requests below current floor
    bool has_below(const set<int>& requests) {
        return requests.begin() != requests.lower_bound(currentFloor);
    }

    bool has_requests_above() {
        return has_above(pickup_req_up) ||
               has_above(pickup_req_down) ||
               has_above(dest_req_up) ||
               has_above(dest_req_down);
    }

    bool has_requests_below() {
        return has_below(pickup_req_up) ||
               has_below(pickup_req_down) ||
               has_below(dest_req_up) ||
               has_below(dest_req_down);
    }

    bool has_any_requests() {
        return !pickup_req_up.empty() ||
               !pickup_req_down.empty() ||
               !dest_req_up.empty() ||
               !dest_req_down.empty();
    }

    // Process all requests at the current floor
    void process_current_floor() {
        bool hasRequest =
            pickup_req_up.count(currentFloor) ||
            pickup_req_down.count(currentFloor) ||
            dest_req_up.count(currentFloor) ||
            dest_req_down.count(currentFloor);

        if (!hasRequest)
            return;

        cout << "\nElevator " << id
             << " stopped at floor " << currentFloor << "\n";

        door.open_door(currentFloor);

        pickup_req_up.erase(currentFloor);
        pickup_req_down.erase(currentFloor);
        dest_req_up.erase(currentFloor);
        dest_req_down.erase(currentFloor);

        door.close_door(currentFloor);
    }

    // Move one floor up
    void move_up() {
        if (currentFloor < totalFloors) {
            currentFloor++;
            cout << "Elevator " << id
                 << " moving UP to floor " << currentFloor << "\n";
        }
    }

    // Move one floor down
    void move_down() {
        if (currentFloor > 1) {
            currentFloor--;
            cout << "Elevator " << id
                 << " moving DOWN to floor " << currentFloor << "\n";
        }
    }

    void show_display() {
        display.show(currentFloor, current_direction);
    }
};

// ================= IDLE STATE =================

void IdleState::move(Elevator& elevator) {

    elevator.process_current_floor();

    if (elevator.has_requests_above()) {
        cout << "Elevator " << elevator.id << " changing to UP state\n";

        elevator.set_state(
            make_shared<MovingUpState>(), UP
        );
    }
    else if (elevator.has_requests_below()) {
        cout << "Elevator " << elevator.id << " changing to DOWN state\n";

        elevator.set_state(
            make_shared<MovingDownState>(), DOWN
        );
    }
    else {
        elevator.current_direction = IDLE;
    }

    elevator.show_display();
}

// ================= MOVING UP STATE =================

void MovingUpState::move(Elevator& elevator) {

    elevator.process_current_floor();

    if (elevator.has_requests_above()) {

        elevator.move_up();

        elevator.process_current_floor();
    }
    else if (elevator.has_requests_below()) {

        cout << "Elevator " << elevator.id
             << " changing direction to DOWN\n";

        elevator.set_state(
            make_shared<MovingDownState>(), DOWN
        );
    }
    else {

        cout << "Elevator " << elevator.id
             << " becoming IDLE\n";

        elevator.set_state(
            make_shared<IdleState>(), IDLE
        );
    }

    elevator.show_display();
}

// ================= MOVING DOWN STATE =================

void MovingDownState::move(Elevator& elevator) {

    elevator.process_current_floor();

    if (elevator.has_requests_below()) {

        elevator.move_down();

        elevator.process_current_floor();
    }
    else if (elevator.has_requests_above()) {

        cout << "Elevator " << elevator.id
             << " changing direction to UP\n";

        elevator.set_state(
            make_shared<MovingUpState>(), UP
        );
    }
    else {

        cout << "Elevator " << elevator.id
             << " becoming IDLE\n";

        elevator.set_state(
            make_shared<IdleState>(), IDLE
        );
    }

    elevator.show_display();
}

// ================= ELEVATOR CONTROLLER =================

class ElevatorController {
private:
    vector<Elevator*>& elevators;

public:
    ElevatorController(vector<Elevator*>& elevators)
        : elevators(elevators) {}

    Elevator* find_elevator(int requestedFloor) {

        Elevator* bestElevator = nullptr;
        int minDistance = INT_MAX;

        for (auto elevator : elevators) {

            int distance = abs(
                elevator->currentFloor - requestedFloor
            );

            if (distance < minDistance) {
                minDistance = distance;
                bestElevator = elevator;
            }
        }

        return bestElevator;
    }

    Elevator* request_elevator(int floor, Direction direction) {

        Elevator* elevator = find_elevator(floor);

        if (elevator != nullptr) {
            cout << "\nElevator " << elevator->id
                 << " assigned to floor " << floor << "\n";

            elevator->add_pickup_request(floor, direction);
        }

        return elevator;
    }
};

// ================= ELEVATOR SYSTEM =================

class ElevatorSystem {
private:
    int totalFloors;

    vector<Elevator*> elevators;

    ElevatorController* controller;

public:
    ElevatorSystem(int totalFloors, int numberOfElevators)
        : totalFloors(totalFloors) {

        for (int i = 0; i < numberOfElevators; i++) {
            elevators.push_back(
                new Elevator(i + 1, totalFloors)
            );
        }

        controller = new ElevatorController(elevators);
    }

    ~ElevatorSystem() {

        delete controller;

        for (auto elevator : elevators) {
            delete elevator;
        }
    }

    // External pickup request
    Elevator* request_elevator(int floor, Direction direction) {

        if (floor < 1 || floor > totalFloors) {
            cout << "Invalid pickup floor\n";
            return nullptr;
        }

        return controller->request_elevator(floor, direction);
    }

    // Passenger selects destination inside an elevator
    void select_destination(int elevatorId, int destinationFloor) {

        for (auto elevator : elevators) {

            if (elevator->id == elevatorId) {
                elevator->select_destination(destinationFloor);
                return;
            }
        }

        cout << "Invalid elevator ID\n";
    }

    // Move all elevators one step
    void step() {

        for (auto elevator : elevators) {
            elevator->step();
        }
    }
};

// ================= MAIN =================

int main() {

    ElevatorSystem system(10, 2);

    // Person on floor 5 requests an elevator to go UP
    Elevator* elevator = system.request_elevator(5, UP);

    if (elevator != nullptr) {

        // Person enters elevator and selects floor 8
        system.select_destination(elevator->id, 8);
    }

    // Run simulation
    for (int i = 0; i < 15; i++) {

        cout << "\n========== STEP "
             << i + 1 << " ==========\n";

        system.step();
    }

    return 0;
}
