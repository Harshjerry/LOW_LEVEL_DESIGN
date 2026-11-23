
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

class Observer {
public:
    virtual void update(double price) = 0;

    virtual ~Observer() = default;
};

// =====================================
// 2. CONCRETE OBSERVERS
// =====================================

class MobileApp : public Observer {
public:
    void update(double price) override {
        cout << "Mobile App: Stock price = "
             << price << endl;
    }
};

class EmailService : public Observer {
public:
    void update(double price) override {
        cout << "Email: Stock price = "
             << price << endl;
    }
};

class TradingDashboard : public Observer {
public:
    void update(double price) override {
        cout << "Dashboard: Stock price = "
             << price << endl;
    }
};

// =====================================
// 3. SUBJECT
// =====================================

class Stock {

private:
    double price;
    vector<Observer*> observers;

public:

    Stock() {
        price = 0;
    }

    // Subscribe
    void subscribe(Observer* observer) {
        observers.push_back(observer);
    }

    // Unsubscribe
    void unsubscribe(Observer* observer) {
        observers.erase(
            remove(observers.begin(),
                   observers.end(),
                   observer),
            observers.end()
        );
    }

    // Notify all observers
    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update(price);
        }
    }

    // Update stock price
    void setPrice(double newPrice) {
        price = newPrice;
        notifyObservers();
    }
};

// =====================================
// 4. MAIN FUNCTION
// =====================================

int main() {

    Stock stock;

    // Create observers
    MobileApp mobile;
    EmailService email;
    TradingDashboard dashboard;

    // Subscribe observers
    stock.subscribe(&mobile);
    stock.subscribe(&email);
    stock.subscribe(&dashboard);

    // Update stock price
    cout << "Price changed to 100\n";
    stock.setPrice(100);

    cout << "\nPrice changed to 150\n";
    stock.setPrice(150);

    // Unsubscribe email
    stock.unsubscribe(&email);

    cout << "\nPrice changed to 200\n";
    stock.setPrice(200);

    return 0;
}
