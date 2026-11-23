
#include <bits/stdc++.h>
using namespace std;

// 1. Abstract Product Interfaces

class Button {
public:
    virtual void render() = 0;
    virtual ~Button() = default;
};

class Checkbox {
public:
    virtual void render() = 0;
    virtual ~Checkbox() = default;
};

// 2. Windows Concrete Products

class WindowsButton : public Button {
public:
    void render() override {
        cout << "Rendering Windows Button\n";
    }
};

class WindowsCheckbox : public Checkbox {
public:
    void render() override {
        cout << "Rendering Windows Checkbox\n";
    }
};

// 3. Mac Concrete Products

class MacButton : public Button {
public:
    void render() override {
        cout << "Rendering Mac Button\n";
    }
};

class MacCheckbox : public Checkbox {
public:
    void render() override {
        cout << "Rendering Mac Checkbox\n";
    }
};

// 4. Abstract Factory Interface

class GUIFactory {
public:
    virtual unique_ptr<Button> createButton() = 0;

    virtual unique_ptr<Checkbox> createCheckbox() = 0;

    virtual ~GUIFactory() = default;
};

// 5. Windows Factory

class WindowsFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<WindowsButton>();
    }

    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<WindowsCheckbox>();
    }
};

// 6. Mac Factory

class MacFactory : public GUIFactory {
public:
    unique_ptr<Button> createButton() override {
        return make_unique<MacButton>();
    }

    unique_ptr<Checkbox> createCheckbox() override {
        return make_unique<MacCheckbox>();
    }
};

// 7. Main

int main() {

    // Select Windows family
    unique_ptr<GUIFactory> factory =
        make_unique<WindowsFactory>();

    // Create related objects
    auto button = factory->createButton();
    auto checkbox = factory->createCheckbox();

    // Use objects
    button->render();
    checkbox->render();

    return 0;
}
