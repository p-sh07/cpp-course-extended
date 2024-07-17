// 28.03


// Макрос _USE_MATH_DEFINES необходим, чтобы при подключении <cmath> была объявлена константа M_PI
#define _USE_MATH_DEFINES
// Макрос _USE_MATH_DEFINES следует объявить ДО подключения других заголовочных файлов,
// которые могут подключить <cmath> неявно
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <numeric>
#include <string>

using namespace std;

enum class Color { RED, GREEN, BLUE };

ostream& operator<<(ostream& out, Color color) {
    switch (color) {
        case Color::RED:
            out << "red"s;
            break;
        case Color::GREEN:
            out << "green"s;
            break;
        case Color::BLUE:
            out << "blue"s;
            break;
    }
    return out;
}

class Shape {
public:
    Shape(Color color) : color_(color) {}
    void SetColor(Color color) {
        color_ = color;
    }
    Color GetColor() const {
        return color_;
    }
    
    virtual string GetType() const {
        return type_;
    }
    
    virtual double GetArea() const {
        return 0.0;
    }
    
private:
    string type_ = "Shape"s;
    Color color_;
};

class Rectangle : public Shape {
public:
    Rectangle(double width, double height, Color color) 
    : width_(width)
    , height_(height)
    , Shape(color)
    {}
    
    double GetWidth() const {
        return width_;
    }
    
    double GetHeight() const {
        return height_;
    }
    
    void SetSize(double width, double height) {
        width_ = width;
        height_ = height;
    }
    
    string GetType() const override {
        return type_;
    }
    
    double GetArea() const override {
        return width_ * height_;
    }
    
private:
    string type_ = "Rectangle"s;
    double width_ = 0.0;
    double height_ = 0.0;
};

class Circle : public Shape {
public:
    Circle(double radius, Color color)
    : radius_(radius)
    , Shape(color)
    {}
    
    double GetRadius() const {
        return radius_;
    }
    
    void SetRadius(double radius) {
        radius_ = radius;
    }
    
    string GetType() const override {
        return type_;
    }
    
    double GetArea() const override {
        return radius_ * radius_ * M_PI;
    }
private:
    string type_ = "Circle"s;
    double radius_ = 0.0;
};

// Возвращает суммарную площадь фигур, указатели на которые находятся в переданной коллекции collection
template <typename ShapeCollection>
double CalcSumArea(const ShapeCollection& collection) {
    double sum = 0;
    for(const auto& shape : collection) {
        sum += shape->GetArea();
    }
    return sum;
}

void PrintShapeInfo(const Shape& shape) {
    cout << shape.GetType() << ": color: "s << shape.GetColor() << ", area:"s << shape.GetArea() << endl;
}

int main() {
    Circle c{10.0, Color::RED};
    Rectangle r{10, 20, Color::BLUE};
    Shape sh{Color::GREEN};

    const Shape* shapes[] = {&c, &r, &sh};

    assert(sh.GetType() == "Shape"s);
    assert(c.GetType() == "Circle"s);
    assert(r.GetType() == "Rectangle"s);

    assert(sh.GetColor() == Color::GREEN);
    assert(c.GetColor() == Color::RED);
    assert(r.GetColor() == Color::BLUE);
    sh.SetColor(Color::BLUE);
    c.SetColor(Color::GREEN);
    r.SetColor(Color::RED);
    assert(sh.GetColor() == Color::BLUE);
    assert(c.GetColor() == Color::GREEN);
    assert(r.GetColor() == Color::RED);

    assert(std::abs(r.GetArea() - 200.0) < 1e-5);
    assert(std::abs(c.GetArea() - 314.15) < 1e-2);
    c.SetRadius(1.0);
    assert(std::abs(c.GetArea() - 3.1415) < 1e-4);
    r.SetSize(5, 7);
    assert(r.GetWidth() == 5);
    assert(r.GetHeight() == 7);
    assert(std::abs(r.GetArea() - 35.0) < 1e-5);

    assert(abs(CalcSumArea(shapes) - 38.1416) < 1e-4);

    for (const Shape* shape : shapes) {
        PrintShapeInfo(*shape);
    }
    cout << "Total area: " << CalcSumArea(shapes) << endl;
}
