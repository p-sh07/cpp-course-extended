//
//  main.cpp
//  cpp-practice
//
//  Created by ps on 18.04.2024.
//

#include <algorithm>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
    IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end) {
    }
    
    Iterator begin() const {
        return first;
    }
    
    Iterator end() const {
        return last;
    }
    
private:
    Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
    return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
    string name;
    int age, income;
    bool is_male;
};

class PersonStats {
public:
    PersonStats() {}
    
    void ReadPeople(istream& input) {
        int count;
        input >> count;
        
        people_.resize(count);
        
        for (Person& p : people_) {
            char gender;
            input >> p.name >> p.age >> p.income >> gender;
            p.is_male = gender == 'M';
        }
    }
    void InitStats() {
        if(!people_.empty()) {
            //build age vector:
            sort(people_.begin(), people_.end(), [](const Person& lhs, const Person& rhs) {
                return lhs.age > rhs.age;
            });
            int max_age = people_[0].age;
            
            vector<int> people_of_age = vector<int>(max_age+1, 0);
            
            for(const auto& person : people_) {
                ++people_of_age[person.age];
            }
            //make sure all the values are init to 0
            cumulative_by_age_ = vector<int>(people_of_age.size(), 0);
            
            size_t vec_size = cumulative_by_age_.size();
            *cumulative_by_age_.rbegin() = *people_of_age.rbegin();
            
            for(size_t i = 2; i <= vec_size; ++i) {
                cumulative_by_age_[vec_size-i] = cumulative_by_age_[vec_size-i+1] + people_of_age[vec_size-i];
            }
            
            //build wealth vector:
            sort(people_.begin(), people_.end(), [](const Person& lhs, const Person& rhs) {
                return lhs.income > rhs.income;
            });
            cumulative_wealth_.resize(people_.size());
            cumulative_wealth_[0] = people_[0].income;
            for(size_t i = 1; i < people_.size(); ++i) {
                cumulative_wealth_[i] = cumulative_wealth_[i-1] + people_[i].income;
            }
            
            //find most popular male and female names:
            top_name_m = FindMostPopularName('M');
            top_name_f = FindMostPopularName('F');
        }
    }
    
    int GetNumAdultPeople(int maturity_age) {
        if(people_.empty()) {
            return 0;
        }
        if(maturity_age >= static_cast<int>(cumulative_by_age_.size())) {
            return 0;
        }
        return cumulative_by_age_[maturity_age];
    }
    
    int GetTopWealth(int N) {
        if(people_.empty() || N == 0) {
            return 0;
        }
        if(N >= static_cast<int>(cumulative_wealth_.size())) {
            return cumulative_wealth_.back();
        }
        return cumulative_wealth_[N-1];
    }
    
    string GetMostPopularName(char gender) {
        return gender == 'M' ? top_name_m : top_name_f;
    }
    
private:
    vector<Person> people_;
    string top_name_m = "No people of gender M";
    string top_name_f = "No people of gender F";
    
    string FindMostPopularName(char gender) {
                unordered_map<string_view, int> names;
                //count names
                for(const auto& p : people_) {
                    if(p.is_male == (gender == 'M')) {
                        ++names[p.name];
                    }
                }
                //find max
                int max_count = 0;
                string_view most_freq;
                for(const auto& [name, count] : names) {
                    if(count > max_count) {
                        max_count = count;
                        most_freq = name;
                    } else if(count == max_count) {
                        most_freq = most_freq.compare(name) < 0 ? most_freq : name;
                    }
                }
                return std::string(most_freq);
//        IteratorRange range{people_.begin(), partition(people_.begin(), people_.end(),
//                                                      [gender](Person& p) {
//            return p.is_male = (gender == 'M');
//        })};
//        if (range.begin() == range.end()) {
//            string ans = "No people of gender ";
//            ans.push_back(gender);
//            return ans;
//        } else {
//            sort(range.begin(), range.end(),
//                 [](const Person& lhs, const Person& rhs) {
//                return lhs.name < rhs.name;
//            });
//            const string* most_popular_name = &range.begin()->name;
//            int count = 1;
//            for (auto i = range.begin(); i != range.end();) {
//                auto same_name_end = find_if_not(i, range.end(),
//                                                 [i](const Person& p) {
//                    return p.name == i->name;
//                });
//                auto cur_name_count = distance(i, same_name_end);
//                if (cur_name_count > count) {
//                    count = static_cast<int>(cur_name_count);
//                    most_popular_name = &i->name;
//                }
//                i = same_name_end;
//            }
//            return *most_popular_name;
//        }
    }
    
    vector<int> cumulative_wealth_;
    vector<int> cumulative_by_age_;
};

vector<Person> ReadPeople(istream& input) {
    int count;
    input >> count;
    
    vector<Person> result(count);
    for (Person& p : result) {
        char gender;
        input >> p.name >> p.age >> p.income >> gender;
        p.is_male = gender == 'M';
    }
    
    return result;
}

int main() {
    PersonStats ps;
    
    ps.ReadPeople(cin);
    ps.InitStats();
    
    for (string command; cin >> command;) {
        if (command == "AGE"s) {
            int adult_age;
            cin >> adult_age;
            
            cout << "There are "s << ps.GetNumAdultPeople(adult_age) << " adult people for maturity age "s
            << adult_age << '\n';
        } else if (command == "WEALTHY"s) {
            int count;
            cin >> count;
            
            cout << "Top-"s << count << " people have total income "s << ps.GetTopWealth(count) << '\n';
        } else if (command == "POPULAR_NAME"s) {
            char gender;
            cin >> gender;
            
            cout << "Most popular name among people of gender "s << gender << " is "s << ps.GetMostPopularName(gender) << '\n';
        }
    }
}
