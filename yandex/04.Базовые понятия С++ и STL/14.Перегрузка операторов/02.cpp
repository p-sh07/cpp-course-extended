//
//  04.Базовые понятия С++ и STL
//  Тема 14.Перегрузка операторов
//  Задача 02.Перегрузка операторов ввода-вывода
//
//  Created by Pavel Sh on 09.12.2023.
//

#include <iostream>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;

    Rational(int numerator)
        : numerator_(numerator)
        , denominator_(1) {
    }

    Rational(int numerator, int denominator)
        : numerator_(numerator)
        , denominator_(denominator) {
        Normalize();
    }

    int Numerator() const {
        return numerator_;
    }

    int Denominator() const {
        return denominator_;
    }

private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        const int divisor = gcd(numerator_, denominator_);
        numerator_ /= divisor;
        denominator_ /= divisor;
    }

    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, Rational r) {
    output << r.Numerator() << '/' << r.Denominator();
    return output;
}

istream& operator>>(istream& input, Rational& r) {
    int numerator = 0, denominator = 1;
    char slash;
    input >> numerator >> slash >> denominator;
    r = Rational{numerator, denominator};
    return input;
}
