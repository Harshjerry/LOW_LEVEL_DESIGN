#include<bits/stdc++.h>
using namespace std;
// It is a design pattern in which we pass a request through a chain of objects.

// Each object gets an opportunity to handle the request.
// - If it can handle the request, it handles it.
// - If it cannot, it passes the request to the next object.
// - This continues until someone handles it or the chain ends.

class SupportHandler {
protected:
    SupportHandler* nextHandler = nullptr;

public:
    void setNextHandler(SupportHandler* handler) {
        nextHandler = handler;
    }

    virtual void handleRequest(int priority) = 0;

    virtual ~SupportHandler() = default;
};
class Executive : public SupportHandler {
public:
    void handleRequest(int priority) override {

        if (priority == 1) {
            cout << "Executive handled the request\n";
        }
        else if (nextHandler != nullptr) {
            nextHandler->handleRequest(priority);
        }
    }
};
class Manager : public SupportHandler {
public:
    void handleRequest(int priority) override {

        if (priority == 2) {
            cout << "Manager handled the request\n";
        }
        else if (nextHandler != nullptr) {
            nextHandler->handleRequest(priority);
        }
    }
};
class TechnicalHead : public SupportHandler {
public:
    void handleRequest(int priority) override {

        if (priority == 3) {
            cout << "Technical Head handled the request\n";
        }
        else if (nextHandler != nullptr) {
            nextHandler->handleRequest(priority);
        }
        else {
            cout << "No one can handle this request\n";
        }
    }
};

int main() {

    Executive executive;
    Manager manager;
    TechnicalHead technicalHead;

    executive.setNextHandler(&manager);
    manager.setNextHandler(&technicalHead);

    executive.handleRequest(2);

    return 0;
}

// 6. When should you use it?
// Some real-world examples:
// Application	Chain
// Customer      support	Executive → Manager → Head
// Logging	     Debug → Info → Warning → Error
// Expense approval	Manager → Director → CEO
// HTTP middleware	Authentication → Authorization → Logging
// Event processing	Multiple event handlers