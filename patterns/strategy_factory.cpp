
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

class UPIDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 5;
    }
};

class CashDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 10;
    }
};

class PremiumDelivery : public DeliveryStrategy {
public:
    int calculateCost(int distance) override {
        return distance * 20;
    }
};

// ==========================================
// 3. FACTORY PATTERN
// ==========================================

class PaymentFactory {
public:
    static unique_ptr<DeliveryStrategy> createPayment(string type) {

        if (type == "UPI") {
            return make_unique<UPIDelivery>();
        }

        else if (type == "CASH") {
            return make_unique<CashDelivery>();
        }

        else if (type == "PREMIUM") {
            return make_unique<PremiumDelivery>();
        }

        throw invalid_argument("Invalid payment type");
    }
};

// ==========================================
// 4. CONTEXT CLASS (STRATEGY PATTERN)
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

    // Delegate calculation
    int calculateCost(int distance) {
        return strategy->calculateCost(distance);
    }
};

// ==========================================
// 5. MAIN FUNCTION
// ==========================================

int main() {

    // Create PaymentService with UPI strategy
    PaymentService service(
        PaymentFactory::createPayment("UPI")
    );

    cout << "UPI Delivery Cost: "
         << service.calculateCost(10) << endl;

    // Change to Cash strategy
    service.setStrategy(
        PaymentFactory::createPayment("CASH")
    );

    cout << "Cash Delivery Cost: "
         << service.calculateCost(10) << endl;

    // Change to Premium strategy
    service.setStrategy(
        PaymentFactory::createPayment("PREMIUM")
    );

    cout << "Premium Delivery Cost: "
         << service.calculateCost(10) << endl;

    return 0;
}
