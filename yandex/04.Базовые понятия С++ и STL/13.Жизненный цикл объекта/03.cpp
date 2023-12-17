//
//  04.Базовые понятия С++ и STL
//  Тема 13.Жизненный цикл объекта
//  Задача 03.Параметризованный конструктор
//
//  Created by Pavel Sh on 07.12.2023.
//

#include <iostream>

using namespace std;

class Rational {
public:
    //Constructor with default values + simplifying the rational using the recursive GCD algorithm
    Rational(int numerator = 0, int denominator = 1) : numerator_(numerator), denominator_(denominator) {
        if(numerator != 0) {
            int greatest_common_divisor = numerator_ < denominator_ ? gcd(denominator_, numerator_) : gcd(numerator_, denominator_);
            numerator_ /= greatest_common_divisor;
            denominator_ /= greatest_common_divisor;
        }
    }
    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    int numerator_ = 0;
    int denominator_ = 1;
    
    int gcd(int a, int b) {
    if (b == 0) {
       return a;
    } else {
       return gcd(b, a % b);
    }
}
};

/* Part 1
#include <string>

using namespace std;

enum class AnimalType {
    Cat,
    Dog,
    Mouse,
};

class Animal {
public:
    Animal(AnimalType type, string name, string owner_name) :
    type_(type), name_(name), owner_name_(owner_name) {}
    AnimalType GetType() const {
        return type_;
    }

    const string& GetName() const {
        return name_;
    }

    const string& GetOwnerName() const {
        return owner_name_;
    }

private:
    AnimalType type_;
    string name_;
    string owner_name_;
};
*/
