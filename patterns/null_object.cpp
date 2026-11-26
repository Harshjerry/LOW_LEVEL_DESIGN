// 2. What Null Object actually changes
// Instead of asking:

// "Should I send a notification or not?"

// We give the application an object that already knows what to do.
// For example:
// Object	What its send() method does
// EmailNotification	Sends an email
// SMSNotification	Sends an SMS
// NullNotification	Does nothing

class Notification {
public:
    virtual void send(string message) = 0;

    virtual ~Notification() = default;
};
class EmailNotification : public Notification {
public:
    void send(string message) override {
        cout << "Sending EMAIL: " << message << "\n";
    }
};
class SMSNotification : public Notification {
public:
    void send(string message) override {
        cout << "Sending SMS: " << message << "\n";
    }
};
class NullNotification : public Notification {
public:
    void send(string message) override {
        // Intentionally do nothing.
    }
};

class OrderService {
private:
    unique_ptr<Notification> notification;

public:
    OrderService(unique_ptr<Notification> notification)
        : notification(move(notification)) {}

    void placeOrder() {

        cout << "Order placed successfully!\n";

        notification->send("Your order has been placed!");
    }
};

int main() {

    // User 1 wants email notifications.
    OrderService user1(
        make_unique<EmailNotification>()
    );

    // User 2 wants SMS notifications.
    OrderService user2(
        make_unique<SMSNotification>()
    );

    // User 3 has disabled notifications.
    OrderService user3(
        make_unique<NullNotification>()
    );

    cout << "\n--- User 1 ---\n";
    user1.placeOrder();

    cout << "\n--- User 2 ---\n";
    user2.placeOrder();

    cout << "\n--- User 3 ---\n";
    user3.placeOrder();

    return 0;
}
