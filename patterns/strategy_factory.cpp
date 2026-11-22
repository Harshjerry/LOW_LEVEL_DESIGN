
#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
using namespace std;

// ==========================================
// 1. STRATEGY INTERFACE
// ==========================================

class DeliveryStrategy {
public:
    virtual int calculateCost(int distance) = 0;

    virtual ~DeliveryStrategy() = default;
};

// ==========================================
// 2. CONCRETE STRATEGIES
// ==========================================

// Strategy 1: UPI Delivery
class UPIDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 5;
    }
};

// Strategy 2: Cash Delivery
class CashDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 10;
    }
};

// Strategy 3: Premium Delivery
class PremiumDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 20;
    }
};

// ==========================================
// 3. FACTORY: CREATE STRATEGY
// ==========================================

unique_ptr<DeliveryStrategy>
createDeliveryStrategy(string type) {

    if (type == "UPI") {
        return make_unique<UPIDelivery>();
    }

    if (type == "CASH") {
        return make_unique<CashDelivery>();
    }

    if (type == "PREMIUM") {
        return make_unique<PremiumDelivery>();
    }

    throw invalid_argument("Invalid delivery type");
}

// ==========================================
// 4. CONTEXT CLASS
// ==========================================

class PaymentService {

private:
    unique_ptr<DeliveryStrategy> strategy;

public:

    // Constructor Injection
    PaymentService(unique_ptr<DeliveryStrategy> s)
        : strategy(move(s)) {}

    // Change strategy at runtime
    void setStrategy(unique_ptr<DeliveryStrategy> s) {
        strategy = move(s);
    }

    // Delegate calculation to selected strategy
    int calculateCost(int distance) {
        return strategy->calculateCost(distance);
    }
};

// ==========================================
// 5. MAIN FUNCTION
// ==========================================

int main() {

    // Create strategy using Factory
    PaymentService service(
        createDeliveryStrategy("UPI")
    );

    cout << "UPI Delivery: "
         << service.calculateCost(10) << endl;

    // Change to Cash Delivery
    service.setStrategy(
        createDeliveryStrategy("CASH")
    );

    cout << "Cash Delivery: "
         << service.calculateCost(10) << endl;

    // Change to Premium Delivery
    service.setStrategy(
        createDeliveryStrategy("PREMIUM")
    );

    cout << "Premium Delivery: "
         << service.calculateCost(10) << endl;

    return 0;
}
