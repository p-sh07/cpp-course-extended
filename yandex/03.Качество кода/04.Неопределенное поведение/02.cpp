#include <cstdint>
#include <iostream>
#include <limits>
#include <numeric>

using namespace std;

int main() {
    int64_t a;
    int64_t b;
    cin >> a >> b;
    uint64_t result = static_cast<uint64_t>(abs(a)) + abs(b);
    if(result > numeric_limits<int64_t>::max()) {
        cout << "Overflow!"s;
    } else {
        cout << a + b << endl;
    }
}


//sln:

#include <cstdint>
#include <iostream>
#include <limits>
#include <optional>
#include <string_view>


using namespace std;

template <typename SignedInt>
optional<SignedInt> SafeAdd(SignedInt a, SignedInt b) {
    // Переполнение может возникнуть только в следующих ситуациях:
    //  - при сложении двух чисел с одним знаком
    //  - при нахождении разности двух чисел с разными знаками

    if (a > 0 && b > 0) {
        // Здесь проверяется, что a + b > numeric_limits<SignedInt>::max()
        // Так как a и b - положительные числа, при их сложении может возникнуть переполнение.
        // Однако если перенести a или b в правую часть, поменяв знак, результат
        // будет тот же, но переполнения при этом не будет
        if (a > numeric_limits<SignedInt>::max() - b) {
            return nullopt; // Переполнение при сложении положительных чисел
        }
    } else if (a < 0 && b < 0) {
        // Здесь проверяется, что a + b < numeric_limits<SignedInt>::min()
        // Так как a и b - отрицательные числа, при их сложении может возникнуть переполнение.
        // Однако если перенести a или b в правую часть, поменяв знак, результат
        // будет тот же, но переполнения при этом не будет
        if (a < numeric_limits<SignedInt>::min() - b) {
            return nullopt; // Переполнение при сложении отрицательных чисел
        }
    }
    // Здесь гарантируется, что переполнения не возникнет
    return a + b;
}

int main() {
    int64_t a;
    int64_t b;
    cin >> a >> b;
    if (const auto sum = SafeAdd(a, b)) {
        cout << *sum << endl;
    } else {
        cout << "Overflow!"sv << endl;
    }
}
