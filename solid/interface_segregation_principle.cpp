// class Machine {
// public:
//     virtual void print() = 0;
//     virtual void scan() = 0;
//     virtual void fax() = 0;

//     virtual ~Machine() = default;
// };

// class SimplePrinter : public Machine {
// public:
//     void print() override {
//         cout << "Printing...\n";
//     }

//     void scan() override {
//         // Not supported
//         throw runtime_error("Scanning not supported");
//     }

//     void fax() override {
//         // Not supported
//         throw runtime_error("Fax not supported");
//     }
// };

// This is bad design.

// Why should a simple printer be forced to implement scanning and faxing?

// We are creating unnecessary dependencies.


class Printer {
public:
    virtual void print() = 0;
    virtual ~Printer() = default;
};

class Scanner {
public:
    virtual void scan() = 0;
    virtual ~Scanner() = default;
};

class Fax {
public:
    virtual void fax() = 0;
    virtual ~Fax() = default;
};

class SimplePrinter : public Printer {
public:
    void print() override {
        cout << "Printing...\n";
    }
};

class MultiFunctionPrinter : public Printer,
                             public Scanner,
                             public Fax {
public:
    void print() override {
        cout << "Printing...\n";
    }

    void scan() override {
        cout << "Scanning...\n";
    }

    void fax() override {
        cout << "Faxing...\n";
    }
};