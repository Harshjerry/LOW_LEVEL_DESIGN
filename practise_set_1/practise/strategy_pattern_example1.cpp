#include<bits/stdc++.h>
using namespace std;

class DiscountStrategy {
public:
    virtual double calculateDiscount(double price) = 0;
    virtual ~DiscountStrategy() = default;
};

class NoDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double price) override {
        return 0.0;
    }
};

class FestivalDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double price) override {
        return price * 0.1; // 10% discount
    }
};

class ClearanceDiscount : public DiscountStrategy {
public:
    double calculateDiscount(double price) override {
        return price * 0.5; // 50% discount
    }
};

// context class
class DiscountService {
private: 
    unique_ptr<DiscountStrategy> strategy;
    DiscountService(unique_ptr<DiscountStrategy> s) : strategy(std::move(s)) {}
public:
    double getDiscount(double price) {
        return strategy->calculateDiscount(price);
    }
};

int main(){

    // Create strategy using Factory
    DiscountService service(
        make_unique<FestivalDiscount>()
    );

    cout << "Festival Discount: "
         << service.getDiscount(100) << endl;

    // Change to Clearance Discount
    service.setStrategy(
        make_unique<ClearanceDiscount>()
    );

    cout << "Clearance Discount: "
         << service.getDiscount(100) << endl;

    return 0;
}