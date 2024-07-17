Инструменты для обнаружения неопределённого поведения
В предыдущих уроках вы увидели интересные и необычные проявления неопределённого поведения. В этом вы познакомитесь с доступными инструментами, которые помогают обнаруживать его в программах.
Статический анализатор Clang-tidy

Первый инструмент — статический анализатор Clang-tidy. Он, как компилятор, анализирует код и выдаёт предупреждения о реальных и потенциальных ошибках, таких как:
неопределённое поведение,
ошибки copy-paste,
некорректное использование библиотечных функций,
использование устаревших языковых конструкций или библиотечных функций.
Утилита Clang-Tidy входит в состав проекта LLVM вместе с компилятором Clang. Загрузить Clang для распространённых операционных систем можно на странице проекта: releases.llvm.org/download.html.
Рассмотрим работу статического анализатора на примере программы, проверяющей, считается ли небесное тело планетой. Создайте в каталоге с исходным кодом программы файл с именем .clang-tidy и следующим содержимым:
---
Checks:          '*,-llvm-*,-llvmlibc-*,-google-build-using-namespace,-modernize-use-trailing-return-type' 
В этом файле задаются настройки, которые распространяются на все файлы в каталоге и его подкаталогах. Параметр Checks — строка, где через запятую перечислены имена включённых проверок. Символ * в имени проверки означает «любые символы». Например, llvm-* включает все проверки, имена которых начинаются с llvm-, а * включает проверки с любыми именами. Символ - перед именем проверки означает, что её надо исключить.
В конфигурационном файле выше включены все проверки (опция *), кроме тех, перед которыми стоит -:
проверки, относящиеся к сборке проекта LLVM — диагностика llvm-* и llvmlibc-*;
предупреждение об использовании директивы using namespace — диагностика google-build-using-namespace;
предложение заменить объявление функций на альтернативную форму записи — диагностика modernize-use-trailing-return-type.
Создайте в этом каталоге файл planets.cpp со следующим содержимым:
#include <iostream>
#include <string_view>

using namespace std;

const int NUM_PLANETS = 8;
const string_view PLANETS[] = {
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

bool IsPlanet(string_view name) {
    for (int i = 0; i < NUM_PLANETS; ++i) {
        if (PLANETS[i] == name) {
            return true;
        }
    }
    return false;
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv) << "a planet"sv << endl;
}

int main() {
    Test("Earth"sv);
    Test("Jupiter"sv);
    Test("Pluto"sv);
    Test("Moon"sv);
}  
Вызовите в консоли команду clang-tidy. Передайте ей имя анализируемого файла, а следом за разделителем "--" — настройку, включающую поддержку стандарта C++17. Она нужна для использования string_view.
clang-tidy planets.cpp -- -std=c++17 
Утилита clang-tidy выведет предупреждения о коде:
planets.cpp:7:7: warning: do not declare C-style arrays, use std::array<> instead [cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays]
const string_view PLANETS[] = {
      ^
C:\Users\Alexey\source\repos\practicum-malov\sprint13\ub\planets\planets.cpp:14:5: warning: use range-based for loop instead [modernize-loop-convert]
    for (int i = 0; i < NUM_PLANETS; ++i) {
    ^   ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
        (auto i : PLANETS)
C:\Users\Alexey\source\repos\practicum-malov\sprint13\ub\planets\planets.cpp:15:13: warning: do not use array subscript when the index is not an integer constant expression; use gsl::at() instead [cppcoreguidelines-pro-bounds-constant-array-index]
        if (PLANETS[i] == name) {
            ^ 
Первое предупреждение “do not declare C-style arrays, use std::array<> instead” рекомендует вместо объявления массива фиксированного размера в стиле языка C использовать класс std::array. В отличие от C-массивов, объекты std::array могут быть переданы в функцию по значению и возвращены из неё. Также они предоставляют больше проверок в отладочном режиме работы. Кроме того, у std::array есть метод size(), возвращающий размер массива.
Предупреждение “use range-based for loop instead” рекомендует использовать range-версию цикла for. Если бы количество элементов массива PLANETS и константа NUM_PLANETS были равны, это предупреждение не вывелось бы. А именно это различие и привело к неопределённому поведению при работе программы.
Третье предупреждение “do not use array subscript when the index is not an integer constant expression; use gsl::at()” советует не обращаться к элементам массива по неконстантному индексу, так как это может привести к выходу за пределы массива. Предлагается использовать вспомогательную функцию gsl::at из библиотеки Guidelines Support Library. 
Три предупреждения о массиве PLANETS не свидетельствуют напрямую о наличии неопределённого поведения. Но рекомендуют обратить внимание на потенциально опасные конструкции в коде.
Последуем первым двум советам: заменим C-массив на std::array, а цикл for — на подходящий здесь алгоритм std::find:
#include <algorithm>
#include <array>
#include <iostream>
#include <string_view>

using namespace std;

const array PLANETS{
    "Mercury"sv, "Venus"sv, "Earth"sv,
    "Mars"sv, "Jupiter"sv, "Saturn"sv,
    "Uranus"sv, "Neptune"sv,
};

bool IsPlanet(string_view name) {
    return find(begin(PLANETS), end(PLANETS), name) != end(PLANETS);
}

void Test(string_view name) {
    cout << name << " is " << (IsPlanet(name) ? ""sv : "NOT "sv) << "a planet"sv << endl;
}

int main() {
    Test("Earth"sv);
    Test("Jupiter"sv);
    Test("Pluto"sv);
    Test("Moon"sv);
} 
Повторный прогон анализатора пройдёт без замечаний.
Статический анализатор позволил не только устранить неопределённое поведение в программе, но и упростить код — сделать его более идиоматичным. Используйте подобные инструменты регулярно — так вы обнаружите потенциально опасные места на раннем этапе.
Отладочный режим стандартной библиотеки

С этим инструментом вы уже успели познакомиться в этом курсе. Чтобы включить отладочный режим, нужно добавить следующие опции командной строки:
libstdc++: -D_GLIBCXX_DEBUG (обычно компилятор gcc)
libc++: -D_LIBCPP_DEBUG=1 (обычно компилятор clang)
Компилятор Visual C++: -D_ITERATOR_DEBUG_LEVEL=2
Проверим работу отладочного режима стандартной библиотеки на примере программы, заполняющей сетку из точек:
#include <iostream>
#include <vector>

using namespace std;

struct Point {
    int x, y;
    Point(int x, int y)
        : x(x)
        , y(y) {
    }
};

void DuplicateAlongX(vector<Point>& v, int offset) {
    for (const auto& p : v) {
        v.emplace_back(p.x + offset, p.y);
    }
}

void DuplicateAlongY(vector<Point>& v, int offset) {
    for (const auto& p : v) {
        v.emplace_back(p.x, p.y + offset);
    }
}

int main() {
    vector points = {Point(0, 0)};

    DuplicateAlongX(points, 1);
    DuplicateAlongX(points, 2);
    DuplicateAlongX(points, 4);

    DuplicateAlongY(points, 1);
    DuplicateAlongY(points, 2);
    DuplicateAlongY(points, 4);

    int n = 0;
    for (const auto& p : points) {
        cout << '{' << p.x << ", "sv << p.y << "} "sv;
        ++n;
        if (n == 8) {
            cout << endl;
            n = 0;
        }
    }
} 
Теперь, если во время работы программы попытаться выполнить инкремент невалидного итератора, в консоль будет выведена ошибка. Программа аварийно завершит работу:
Error: attempt to increment a singular iterator.

Objects involved in the operation:
    iterator       type = __gnu_debug::_Safe_iterator<__gnu_cxx::__normal_iterator<Point*, std::__cxx1998::vector<Point, std::allocator<Point> > >, std::__debug::vector<Point, std::allocator<Point> > > (mutable iterator);
      state = singular;
      references sequence with type 'std::__debug::vector<Point, std::allocator<Point> >' @ 0x000000000065FD80
    } 
Undefined behavior sanitizer

Undefined behavior sanitizer — это специальный режим работы компилятора, который добавляет в скомпилированный код дополнительные проверки. Они обнаруживают различные виды неопределённого поведения во время работы программы. Например:
использование нулевого указателя или указателя с неправильным выравниванием;
переполнение в операциях над целыми числами со знаком;
целочисленное деление на ноль.
Как и отладочный режим стандартной библиотеки, санитайзер обнаруживает ошибки во время работы программы. Но такие проверки охватывают код всей программы, а не только код стандартной библиотеки.
В ОС Windows такую опцию компилятор gcc пока не поддерживает. Но есть одноимённая опция командной строки для компилятора clang. Посмотрим, как это работает. Для примера возьмём программу, в которой при сложении возникало переполнение целого числа:
#include <climits>
#include <iomanip>
#include <iostream>

bool TestSigned(int n) {
    return n < n + 1;
}

bool TestUnsigned(unsigned int n) {
    return n < n + 1;
}

using namespace std;
int main() {
    cout << boolalpha;
    cout << TestSigned(INT_MAX) << endl;
    cout << TestUnsigned(UINT_MAX) << endl;
} 
Соберём её со включённым UB-санитайзером:
g++ -fsanitize=undefined -std=c++17 overflow.cpp 
Вместо g++ можно использовать компилятор clang с теми же опциями командной строки. При запуске программы обнаружится неопределённое поведение:
overflow.cpp:6:18: runtime error: signed integer overflow: 2147483647 + 1 cannot be represented in type 'int'
SUMMARY: UndefinedBehaviorSanitizer: undefined-behavior overflow.cpp:6:18 in 
Санитайзер поможет обнаружить неопределённое поведение и в программе, которая выпускает велоцираптора при срабатывании сигнализации:
#include <iostream>

using namespace std;

// Указатель на функцию без параметров, которая возвращает void
using Alarm = void (*)();

namespace {
// Поведение сигнализации определяется значением указателя alarm
Alarm alarm = nullptr;
}  // namespace

// Выпустить велоцираптора
void HostileAlarm() {
    cout << "Attack with velociraptor"sv << endl;
}

// Настраиваем сигнализацию на выпуск велоцираптора
void SetHostileAlarm() {
    cout << "Hostile alarm set" << endl;
    alarm = HostileAlarm;
}

void Test() {
    // Вызываем функцию, на которую ссылается указатель alarm
    alarm();
    cout << "Test succeeded"sv << endl;
}

int main() {
    Test();
} 
Сборка программы под санитайзером:
clang++ alarm.cpp -O2 -std=c++17 -fsanitize=undefined 
Запуск программы:
UndefinedBehaviorSanitizer:DEADLYSIGNAL
==22==ERROR: UndefinedBehaviorSanitizer: SEGV on unknown address 0x00000000a7a0 (pc 0x000000425b93 bp 0x0000004279e0 sp 0x7fffa4174370 T22)
==22==The signal is caused by a READ memory access.
    #0 0x425b93  (/home/jail/prog.exe+0x425b93)
    #1 0x425cfd  (/home/jail/prog.exe+0x425cfd)
    #2 0x7ff16b23882f  (/lib/x86_64-linux-gnu/libc.so.6+0x2082f)
    #3 0x403b48  (/home/jail/prog.exe+0x403b48)

UndefinedBehaviorSanitizer can not provide additional info.
SUMMARY: UndefinedBehaviorSanitizer: SEGV (/home/jail/prog.exe+0x425b93) 
==22==ABORTING 
Вот как заранее обнаружить и предотвратить неопределённое поведение.
