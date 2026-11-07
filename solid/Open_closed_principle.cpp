#include<bits/stdc++.h>
using namespace std;


//open for extension and closed for modification
class Payment {
public:
    virtual void pay(double amount) = 0;

    virtual ~Payment() = default;
};
class UPI : public Payment {
public:
    void pay(double amount) override {
        cout << "UPI payment: " << amount << "\n";
    }
};

class CreditCard : public Payment {
public:
    void pay(double amount) override {
        cout << "Card payment: " << amount << "\n";
    }
};

class PayPal : public Payment {
public:
    void pay(double amount) override {
        cout << "PayPal payment: " << amount << "\n";
    }
};
class PaymentProcessor {
public:
    void process(Payment* payment, double amount) {
        payment->pay(amount);
    }
};
int main() {

    PaymentProcessor processor;

    UPI upi;
    CreditCard card;
    PayPal paypal;

    processor.process(&upi, 1000);
    processor.process(&card, 2000);
    processor.process(&paypal, 3000);

}