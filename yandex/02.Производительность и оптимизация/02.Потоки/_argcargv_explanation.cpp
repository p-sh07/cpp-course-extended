#include <iostream>
#include <string>

#include "log_duration.h"

using namespace std;

// main с аргументами означает, что программа принимает аргументы командной строки,
// при запуске из консоли их количество будет сохранено в переменную argc,
// а значения — в argv.
// Тип const char** будет обсуждаться позже в курсе, работать с ним можно
// отчасти как с вектором: argv[0] — нулевой аргумент, argv[1] — первый.
// argv[i] можно конвертировать в string
int main(int argc, const char** argv) {
    // Нулевой аргумент — это всегда имя программы,
    // поэтому нам нужен первый
    int arg = stoi(argv[1]);

    if (arg == 1) {
        LOG_DURATION("endl"s);
        int i;
        while (cin >> i) {
            cout << i * i << endl;
        }
    }

    if (arg == 2) {
        // Чтобы "\n" воспринималось не как перевод строки, а как
        // слэш и буква 'n', добавим перед ним ещё один слеш:
        // два слеша в строковом (и символьном) литерале воспринимаются
        // как один (\)
        LOG_DURATION("\\n"s);
        int i;
        while (cin >> i) {
            cout << i * i << "\n"s;
        }
    }
}
/* Tie & untie
 if (arg == 3) {
     LOG_DURATION("\\n with tie"s);
     auto tied_before = cin.tie(nullptr);

     int i;
     while (cin >> i) {
         cout << i * i << "\n"s;
     }

     cin.tie(tied_before);
 }
 //ios_base::sync_with_stdio(false)
 */

/*
 Запускать будем, перенаправляя стандартный ввод и вывод:
 $ sqnums 1 >numbers2.txt <numbers.txt
 Перенаправление символами > и < позволяет проассоциировать потоки cin и cout с файлами. Так программа будет через поток cin получать данные из файла numbers.txt. Благодаря > данные из cout пойдут в файл numbers2.txt, а не в окно консоли. Если хотим использовать \n, меняем параметр 1 на 2:
 $ sqnums 2 >numbers2.txt <numbers.txt
 Смотрим на результаты:
 $ sqnums 1 >numbers2.txt <numbers.txt
 endl: 410 ms
 $ sqnums 2 >numbers2.txt <numbers.txt
 \n: 408 ms
 📖  В команде sqnums 2 >numbers2.txt <numbers.txt пробел между 2 и > важен: без него получится конструкция 2>, которая выполняет перенаправление потока cerr.
 */
