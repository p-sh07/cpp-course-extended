//
//  04.Базовые понятия С++ и STL
//  Тема 14.Перегрузка операторов
//  Задача 05.Перегрузка операторов сравнения
//
//  Created by Pavel Sh on 09.12.2023.
//

#include <iostream>
#include <numeric>

using namespace std;

class Rational {
public:
    Rational() = default;
    
    Rational(int value)
    : numerator_(value)
    , denominator_(1)
    {
    }
    
    Rational(int numerator, int denominator)
    : numerator_(numerator)
    , denominator_(denominator)
    {
        Normalize();
    }
    
    int Numerator() const {
        return numerator_;
    }
    
    int Denominator() const {
        return denominator_;
    }
//part 1
    Rational& operator+=(Rational right) {
        // Результат операции сохраняется в текущем экземпляре класса
        numerator_ = numerator_ * right.Denominator() + right.Numerator() * denominator_;
        denominator_ *= right.Denominator();
        Normalize();
        
        return *this;
    }
    Rational& operator-=(Rational right) {
        // Результат операции сохраняется в текущем экземпляре класса
        numerator_ = numerator_ * right.Denominator() - right.Numerator() * denominator_;
        denominator_ *= right.Denominator();
        Normalize();
        
        return *this;
    }
    Rational& operator*=(Rational right) {
        // Результат операции сохраняется в текущем экземпляре класса
        numerator_ *= right.Numerator();
        denominator_ *= right.Denominator();
        Normalize();
        
        return *this;
    }
    Rational& operator/=(Rational right) {
        // Результат операции сохраняется в текущем экземпляре класса
        numerator_ *= right.Denominator();
        denominator_ *= right.Numerator();
        Normalize();
        
        return *this;
    }
    
private:
    void Normalize() {
        if (denominator_ < 0) {
            numerator_ = -numerator_;
            denominator_ = -denominator_;
        }
        int n = gcd(numerator_, denominator_);
        numerator_ /= n;
        denominator_ /= n;
    }
    
    int numerator_ = 0;
    int denominator_ = 1;
};

ostream& operator<<(ostream& output, Rational rational) {
    return output << rational.Numerator() << '/' << rational.Denominator();
}

istream& operator>>(istream& input, Rational& rational) {
    int numerator;
    int denominator;
    char slash;
    if ((input >> numerator) && (input >> slash) && (slash == '/') && (input >> denominator)) {
        rational = Rational{numerator, denominator};
    }
    return input;
}

Rational operator+(Rational left, Rational right) {
    return left+=right;
}
Rational operator-(Rational left, Rational right) {
    return left-=right;
}
Rational operator*(Rational left, Rational right) {
    return left*=right;
}
Rational operator/(Rational left, Rational right) {
    return left/=right;
}
Rational operator+(Rational value) {
    return value;
}
Rational operator-(Rational value) {
    return {-value.Numerator(), value.Denominator()};
}

//This task:
bool operator==(Rational left, Rational right) {
    return left.Numerator() == right.Numerator() && left.Denominator() == right.Denominator();
}
bool operator!=(Rational left, Rational right) {
    return !(left == right);
}
bool operator<(Rational left, Rational right) {
    return left.Numerator() * right.Denominator() < right.Numerator() * left.Denominator();
}
bool operator>(Rational left, Rational right) {
    return left.Numerator() * right.Denominator() > right.Numerator() * left.Denominator();
}
bool operator<=(Rational left, Rational right) {
    return (left < right) || (left == right);
}
bool operator>=(Rational left, Rational right) {
    return (left > right) || (left == right);
}
