// How does Adapter work?
// We create an adapter class that:
// 1. Implements the interface our application expects.
// 2. Internally uses the existing third-party class.
// 3. Converts our method call into the method the third-party class understands.



#include <bits/stdc++.h>
using namespace std;

// Interface expected by our application
class PaymentProcessor {
public:
    virtual void pay(double amount) = 0;
    virtual ~PaymentProcessor() = default;
};

// Existing third-party class
class OldPaymentGateway {
public:
    void makePaymentInRupees(double amount) {
        cout << "Payment made: Rs. " << amount << "\n";
    }
};

// Adapter
class PaymentAdapter : public PaymentProcessor {
    OldPaymentGateway gateway;

public:
    void pay(double amount) override {
        gateway.makePaymentInRupees(amount);
    }
};

// Our application
class ShoppingCart {
    PaymentProcessor* processor;

public:
    ShoppingCart(PaymentProcessor* processor)
        : processor(processor) {}

    void checkout(double amount) {
        processor->pay(amount);
    }
};

int main() {

    PaymentAdapter adapter;

    ShoppingCart cart(&adapter);

    cart.checkout(1500);

    return 0;
}
