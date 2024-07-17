//
//  03.Производительность и оптимизация
//  Тема 01.Ассоциативные контейнеры
//  Урок 04.Хэш функция
//
//  Created by Pavel Sh on 25.02.2024
//

#include <array>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VehiclePlate {
public:
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{l0, l1, l2}
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        // чтобы дополнить цифровую часть номера слева нулями
        // до трёх цифр, используем подобные манипуляторы:
        // setfill задаёт символ для заполнения,
        // right задаёт выравнивание по правому краю,
        // setw задаёт минимальное желаемое количество знаков
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }
    
    bool operator==(const VehiclePlate& rhs) const {
        return letters_ == rhs.letters_
        && digits_ == rhs.digits_
        && region_ == rhs.region_;
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}

template <typename T>
class HashableContainer {
public:
    void Insert(T elem) {
        int index = elem.Hash();

        // если вектор недостаточно велик для этого индекса,
        // то увеличим его, выделив место с запасом
        if (index >= int(elements_.size())) {
            elements_.resize(index * 2 + 1);
        }
        auto& bucket = elements_[index];
        if(find(bucket.begin(), bucket.end(), elem) == bucket.end()) {
            bucket.push_back(elem);
        }
    }

    void PrintAll(ostream& out) const {
        for (const auto& bucket : elements_) {
            for(const auto& elem : bucket) {
                out << elem << endl;
            }
        }
    }

    const auto& GetVector() const {
        return elements_;
    }

private:
    vector<vector<T>> elements_;
};

int main() {
    HashableContainer<VehiclePlate> plate_base;
    plate_base.Insert({'B', 'H', 840, 'E', 99});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'H', 'E', 968, 'C', 79});
    plate_base.Insert({'T', 'A', 326, 'X', 83});
    plate_base.Insert({'H', 'H', 831, 'P', 116});
    plate_base.Insert({'P', 'M', 884, 'K', 23});
    plate_base.Insert({'O', 'C', 34, 'P', 24});
    plate_base.Insert({'M', 'Y', 831, 'M', 43});
    plate_base.Insert({'K', 'T', 478, 'P', 49});
    plate_base.Insert({'X', 'P', 850, 'A', 50});

    plate_base.PrintAll(cout);
}
/*
#include <array>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class VehiclePlate {
public:
    VehiclePlate(char l0, char l1, int digits, char l2, int region)
        : letters_{l0, l1, l2}
        , digits_(digits)
        , region_(region) {
    }

    string ToString() const {
        ostringstream out;
        out << letters_[0] << letters_[1];
        // чтобы дополнить цифровую часть номера слева нулями
        // до трёх цифр, используем подобные манипуляторы:
        // setfill задаёт символ для заполнения,
        // right задаёт выравнивание по правому краю,
        // setw задаёт минимальное желаемое количество знаков
        out << setfill('0') << right << setw(3) << digits_;
        out << letters_[2] << setw(2) << region_;

        return out.str();
    }

    int Hash() const {
        return digits_;
    }
    
    bool operator==(const VehiclePlate& rhs) const {
        return letters_ == rhs.letters_
        && digits_ == rhs.digits_
        && region_ == rhs.region_;
    }

private:
    array<char, 3> letters_;
    int digits_;
    int region_;
};

ostream& operator<<(ostream& out, VehiclePlate plate) {
    out << plate.ToString();
    return out;
}



template <typename T>
class HashableContainer {
public:
    void Insert(T elem) {
        int index = elem.Hash();

        auto& bucket = elements_[index];
        for(const auto& e : bucket) {
            if(e == elem) {
                return;
            }
        }
        
        // если вектор недостаточно велик для этого индекса,
        // то увеличим его, выделив место с запасом
        if (index >= int(elements_.size())) {
            elements_.resize(index > 500 ? 1001 : index * 2 + 1);
        }

        bucket.push_front(elem);
    }

    void PrintAll(ostream& out) const {
        for (const auto& bucket : elements_) {
            for(const auto& elem : bucket) {
                out << elem << endl;
            }
        }
    }

    const auto& GetVector() const {
        return elements_;
    }

private:
    vector<forward_list<T>> elements_;
    
};

int main() {
    HashableContainer<VehiclePlate> plate_base;
    plate_base.Insert({'B', 'H', 840, 'E', 99});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'O', 'K', 942, 'K', 78});
    plate_base.Insert({'H', 'E', 968, 'C', 79});
    plate_base.Insert({'T', 'A', 326, 'X', 83});
    plate_base.Insert({'H', 'H', 831, 'P', 116});
    plate_base.Insert({'P', 'M', 884, 'K', 23});
    plate_base.Insert({'O', 'C', 34, 'P', 24});
    plate_base.Insert({'M', 'Y', 831, 'M', 43});
    plate_base.Insert({'K', 'T', 478, 'P', 49});
    plate_base.Insert({'X', 'P', 850, 'A', 50});

    plate_base.PrintAll(cout);
}


 count words example
 
 #include <algorithm>
 #include <cassert>
 #include <iostream>
 #include <fstream>
 #include <set>
 #include <string>
 #include <vector>
 #include <unordered_map>

 using namespace std;
using cmap_it = unordered_map<string, size_t>::const_iterator;


set<iterator> find_top_counts(const unordered_map<string, size_t>& word_counts, size_t n = 5) {
    set<iterator> top_results;
    for(cmap_it it = word_counts.begin(); it != word_counts.end(); ++it) {
        if(top_results.size() < n) {
            top_results.insert(it);
        } //for words with equal counts, order is undefined
        else if(top_results.back()->second < it->second) {
            //replace one of the results with it
            top_results.erase();
        }
    }
}

int main() {
    TestSplitting();
}
*/
