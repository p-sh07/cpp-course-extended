//
//  04.Базовые понятия С++ и STL
//  Тема 16.Итераторы
//  Задача 07.Стандартные алгоритмы - рекурсия. Решение задачи Ханойские башни
//
//  Created by Pavel Sh on 11.12.2023.
//

#include <iostream>
#include <stdexcept>
#include <vector>
//part 1.Recursive fibonacci based on nth number
int Fibonacci(int n) {
    if(n == 0) {
        return 0;
    } else if(n == 1) {
        return 1;
    }
    return Fibonacci(n-1) + Fibonacci(n-2);
}
//part 2. Is Power Of Two
bool IsPowOfTwo(int num) {
    if(num == 1) {
        return true;
    } else if(num <= 0 || num%2 != 0) {
        return false;
    }
    return IsPowOfTwo(num/2);
}
//part 3. Hanoi. Напишите функцию SolveHanoi, которая принимает ссылку на вектор из трёх стержней-башен. На первой башне надето определённое количество дисков — не обязательно восемь, как в классической задаче. Количество можно узнать, воспользовавшись методом GetDisksNum. Класс Tower уже имеет некоторые методы или части методов. Другие методы вы можете дописывать так, как вам нужно для решения. В результате работы функции SolveHanoi все диски в правильном порядке должны оказаться на третьей башне. Решите эту задачу рекурсивным методом.

#include <iostream>
#include <stdexcept>
#include <vector>

//#define DEBUG_OUTPUT

using namespace std;

class Tower {
public:
    // конструктор и метод SetDisks нужны, чтобы правильно создать башни
    Tower(int disks_num = 0) {
        FillTower(disks_num);
    }
    
    int GetDisksNum() const {
        return static_cast<int>(disks_.size());
    }
    //returns the number of the third (unused) tower
    static int GetOtherTower(int a, int b) {
        if( (a == 1 && b == 2) || (a == 2 && b == 1) )
            return 0;
        else if( (a == 0 && b == 2) || (a == 2 && b == 0) )
            return 1;
        else
            return 2;
    }
    
    void SetDisks(int disks_num) {
        FillTower(disks_num);
    }
    
    // добавляем диск на верх собственной башни
    // обратите внимание на исключение, которое выбрасывается этим методом
    void AddToTop(int disk) {
        if (disks_.empty() || disks_.back() > disk) {
            disks_.push_back(disk);
        } else {
            throw invalid_argument("Невозможно поместить большой диск на маленький");
        }
    }
    bool MoveTopToTower(Tower& dest) {
        int top_disk_num = disks_.back();
        try {
            dest.AddToTop(top_disk_num);
        } catch (exception& ex) {
            cerr << " unable to move disk to the other tower" << endl;
            return false;
        }
        disks_.pop_back();
        return true;
    }
    
    void PrintTowerStatus(ostream& out) const {
        out << '[';
        bool is_first = true;
        for(const auto disk : disks_) {
            if(!is_first) {
                out << ' ';
            }
            is_first = false;
            out << disk;
        }
        out << ']';
    }
    
private:
    vector<int> disks_;
    
    // используем приватный метод FillTower, чтобы избежать дубликации кода
    void FillTower(int disks_num) {
        for (int i = disks_num; i > 0; i--) {
            disks_.push_back(i);
        }
    }
};

ostream& operator<< (ostream& out, const vector<Tower>& vector_of_towers) {
    out << '\n'; // just to make it stand out
    for(const auto& t : vector_of_towers) {
        t.PrintTowerStatus(out);
        out << '\n';
    }
    return out;
}

bool SolveRecursive(vector<Tower>& towers, int start, int finish, int disks_num, int& thread) {
    //move num_disks from start tower to finish tower
#ifdef DEBUG_OUTPUT
    cerr << "-entering thread (" << thread << ") with [" << disks_num << "] disks\n" << towers << endl;
#endif
    //base case when 0 disks:
    if(disks_num == 0) {
        //solved!
#ifdef DEBUG_OUTPUT
        cerr << "=FINISHED thread(" << thread << ")\n" << towers << endl;
#endif
        return true;
    }
    else if(disks_num > 0) {
        //move all disks except for bottm from s to mid,
        int mid = Tower::GetOtherTower(start, finish);
#ifdef DEBUG_OUTPUT
        cerr << " -> [>2]disks: launching new threads from(" << thread << ')' << endl;
#endif
        return SolveRecursive(towers, start, mid, disks_num-1, ++thread)
        //then move bottm to f
        && towers[start].MoveTopToTower(towers[finish])
        //then solve mid to f
        && SolveRecursive(towers, mid, finish, disks_num-1, ++thread);
    }
    cerr << "!!! Disk num is negative !!!" << endl;
    return false;
}

void SolveHanoi(vector<Tower>& towers) {
    int disks_num = towers[0].GetDisksNum();
    int thread = 1;
    cout << "Starting solution with [" << disks_num << "]disks" << endl;
    if(SolveRecursive(towers, 0, 2, disks_num, thread)) {
        cout << "===Solved Successfully!=== with [" << thread << "] threads\n" << endl;
    } else {
        cout << "***Solution Failed =(*** ran [" << thread << "] threads\n" << endl;
    }
}

int main() {
    int n_disks = 0;
    vector<Tower> towers(3);
    cout << "Enter number of disks to solve Tower of Hanoi: " << endl;
    
    cin >> n_disks;
    towers[0].SetDisks(n_disks);
    
    cout << towers << endl;
    
    SolveHanoi(towers);
    
    return 0;
}
