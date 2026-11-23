
#include <bits/stdc++.h>
using namespace std;

class Coffee {
public:
    virtual int cost() = 0;
    virtual ~Coffee() = default;
};

class SimpleCoffee : public Coffee {
public:
    int cost() override {
        return 20;
    }
};

class CoffeeDecorator : public Coffee {
protected:
    Coffee* coffee;

public:
    CoffeeDecorator(Coffee* c) {
        this->coffee = c;
    }
};

class MilkDecorator : public CoffeeDecorator {
public:
    MilkDecorator(Coffee* c) : CoffeeDecorator(c) {}

    int cost() override {
        return coffee->cost() + 10;
    }
};

class SugarDecorator : public CoffeeDecorator {
public:
    SugarDecorator(Coffee* c) : CoffeeDecorator(c) {}

    int cost() override {
        return coffee->cost() + 5;
    }
};

int main() {
    Coffee* coffee = new SimpleCoffee();

    coffee = new MilkDecorator(coffee);

    coffee = new SugarDecorator(coffee);

    cout << coffee->cost() << endl;  // 35

    return 0;
}
