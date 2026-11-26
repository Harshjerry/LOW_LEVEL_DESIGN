// 2. What is Proxy Pattern?
// A Proxy is an object that acts as a representative of another object.

// The client communicates with the Proxy instead of directly communicating with the actual object.

// The Proxy can perform some operations before forwarding the request to the actual object.

#include<bits/stdc++.h>
using namespace std;

class Internet {
public:
    virtual void connect(string website) = 0;

    virtual ~Internet() = default;
};

class RealInternet : public Internet {
public:
    void connect(string website) override {
        cout << "Connecting to " << website << "\n";
    }
};

class InternetProxy : public Internet {

private:
    RealInternet realInternet;

    set<string> blockedWebsites = {
        "facebook.com",
        "instagram.com"
    };

public:
    void connect(string website) override {

        if (blockedWebsites.count(website)) {
            cout << "Access Denied: " << website << "\n";
            return;
        }

        cout << "Proxy: Access allowed\n";

        realInternet.connect(website);
    }
};
int main() {

    InternetProxy proxy;

    proxy.connect("google.com");

    proxy.connect("facebook.com");

    proxy.connect("youtube.com");

    return 0;
}
