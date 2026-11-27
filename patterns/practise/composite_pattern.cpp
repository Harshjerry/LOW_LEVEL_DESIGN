
#include <bits/stdc++.h>
using namespace std;

// ================= SHAPE INTERFACE =================

class Shape {
public:
    virtual double area() = 0;

    virtual void display() = 0;

    virtual ~Shape() = default;
};

// ================= CIRCLE =================

class Circle : public Shape {
    double radius;

public:
    Circle(double radius) : radius(radius) {}

    double area() override {
        return 3.14 * radius * radius;
    }

    void display() override {
        cout << "Circle: radius = " << radius
             << ", area = " << area() << "\n";
    }
};

// ================= RECTANGLE =================

class Rectangle : public Shape {
    double length;
    double width;

public:
    Rectangle(double length, double width)
        : length(length), width(width) {}

    double area() override {
        return length * width;
    }

    void display() override {
        cout << "Rectangle: length = " << length
             << ", width = " << width
             << ", area = " << area() << "\n";
    }
};

// ================= COMPOSITE =================

class ShapeGroup : public Shape {
    vector<unique_ptr<Shape>> shapes;

public:
    void addShape(unique_ptr<Shape> shape) {
        shapes.push_back(move(shape));
    }

    double area() override {
        double total = 0;

        for (auto& shape : shapes) {
            total += shape->area();
        }

        return total;
    }

    void display() override {
        cout << "Shape Group:\n";

        for (auto& shape : shapes) {
            shape->display();
        }

        cout << "Group total area: " << area() << "\n";
    }
};

// ================= MAIN =================

int main() {

    // Create individual shapes
    auto circle1 = make_unique<Circle>(5);
    auto rectangle1 = make_unique<Rectangle>(4, 6);

    // Create first group
    auto group1 = make_unique<ShapeGroup>();

    group1->addShape(move(circle1));
    group1->addShape(move(rectangle1));

    // Create another group
    auto circle2 = make_unique<Circle>(3);
    auto rectangle2 = make_unique<Rectangle>(2, 8);

    auto group2 = make_unique<ShapeGroup>();

    group2->addShape(move(circle2));
    group2->addShape(move(rectangle2));

    // Create outer group containing both groups
    ShapeGroup outerGroup;

    outerGroup.addShape(move(group1));
    outerGroup.addShape(move(group2));

    // Display everything
    outerGroup.display();

    cout << "\nOverall area: "
         << outerGroup.area() << "\n";

    return 0;
}
