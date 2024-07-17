//
//  03.Качество кода
//  Тема 13.Передача данных в функцию
//  Урок 10.Practice - запрещенные домены
//
//  created by ps on 21.05.2024
//
#include <algorithm>
#include <fstream>
#include <iostream>
#include <ranges>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace rng = std::ranges;
namespace view = std::views;
using namespace std;


class Domain {
public:
    // разработайте класс домена
    // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
    Domain(string name)
    : reverse_(make_move_iterator(name.rbegin()), make_move_iterator(name.rend())) {
        reverse_.push_back('.');
    }
    
    string_view GetReverse() const {
        string_view rev_no_dot = reverse_;
        rev_no_dot.remove_suffix(1);
        return rev_no_dot;
    }
    
    string_view GetReverseWithDot() const {
        return reverse_;
    }
    
    string GetName() const {
        return {std::next(reverse_.rbegin()), reverse_.rend()};
    }
    
    // разработайте operator==
    bool operator==(const Domain& other) const {
        return GetReverse() == other.GetReverse();
    }
    
    // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен (поддомен = содержит другой домен целиком)
    bool IsSubdomain(const Domain& other) const {
        //return true if other is a prefix of *this
        auto other_rev = other.GetReverseWithDot();
        
        auto result = rng::mismatch(other_rev, GetReverse());
        return result.in1 == other_rev.end();
    }
private:
    string reverse_;
};

struct comp_domain {
    bool operator()(const Domain& lhs, const Domain& rhs) const {
        return rng::lexicographical_compare(lhs.GetReverse(), rhs.GetReverse());
    }
};

class DomainChecker {
public:
    // конструктор должен принимать список запрещённых доменов через пару итераторов
    template<typename It>
    DomainChecker(It begin, It end)
    : forbidden_(make_move_iterator(begin), make_move_iterator(end)) {
        //sort & get rid of subdomains
        rng::sort(forbidden_, comp_domain());
        
        auto [first, last] = rng::unique(forbidden_,
                                         [](const Domain& lhs, const Domain& rhs){
            return lhs == rhs || rhs.IsSubdomain(lhs);
        });
        forbidden_.erase(first, last);
    }
    
    // разработайте метод IsForbidden, возвращающий true, если домен запрещён
    bool IsForbidden(const Domain& dmn) {
        auto ub = rng::upper_bound(forbidden_, dmn, comp_domain());
        //cerr << "->upper: " << ub->GetName() << ", ";
        if(ub == forbidden_.begin()) {
            //cerr << "Not Found\n";
            return false;
        } else {
            //cerr << "Checking\n";
            return (*prev(ub) == dmn || dmn.IsSubdomain(*prev(ub)));
        }
    }
private:
    vector<Domain> forbidden_;
};

// разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
vector<Domain> ReadDomains(istream& in, size_t num_lines) {
    vector<Domain> result;
    
    for (size_t n = 0; n < num_lines; ++n) {
        string line;
        getline(in, line);
        
        result.push_back(Domain(std::move(line)));
    }
    return result;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);
    
    Number num;
    std::istringstream(line) >> num;
    
    return num;
}

int main() {
    ifstream in("/Users/ps/Documents/cpp-course/input_files/in.txt");
    //istream& in = cin;
    
    const std::vector<Domain> forbidden_domains = ReadDomains(in, ReadNumberOnLine<size_t>(in));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    
    const std::vector<Domain> test_domains = ReadDomains(in, ReadNumberOnLine<size_t>(in));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}

///Cool way to print range!
/// ranges::copy(lower, upper, std::ostream_iterator<int>(std::cout, " "));
/// std::cout << '\n';

/* first attempt:
 class Domain {
 // разработайте класс домена
 public:
 // конструктор должен позволять конструирование из string, с сигнатурой определитесь сами
 Domain(string name)
 : parts_(SplitIntoParts(std::move(name))) {
 //split and store in vector in reverse order
 cerr << "added: " << quoted(GetFull()) << ", parts: ";
 for(auto& s : parts_) {
 cerr << s << ' ';
 }
 cerr << '\n';
 }
 
 string GetFull() const {
 string full_domain;
 
 for(const auto& part : parts_ | view::reverse) {
 full_domain += part + ".";
 }
 //delete last dot
 full_domain.pop_back();
 return full_domain;
 }
 
 const vector<string>& GetParts() const {
 return parts_;
 }
 
 // разработайте operator==
 bool operator==(const Domain& other) const {
 return GetParts()  == other.GetParts();
 }
 
 // разработайте метод IsSubdomain, принимающий другой домен и возвращающий true, если this его поддомен
 bool IsSubdomain(const Domain& other) const {
 //check if *this fully matches the tail of other
 //C++ 23 version:
 //return rng::starts_with(other.GetParts(), GetParts());
 
 const auto& parts = GetParts();
 const auto parts_other = other.GetParts();
 
 if(parts.size() > parts_other.size()) {
 cerr << quoted(GetFull()) << " is NOT subdomain of " << quoted(other.GetFull()) << '\n';
 return false;
 }
 
 for(size_t i = 0; i < parts.size(); ++i) {
 cerr << "comparing: " << parts[i] << " & " << parts_other[i] << '\n';
 if(parts[i] != parts_other[i]) {
 cerr << quoted(GetFull()) << " is NOT subdomain of " << quoted(other.GetFull()) << '\n';
 return false;
 }
 }
 
 cerr << quoted(GetFull()) << " IS subdomain of " << quoted(other.GetFull()) << '\n';
 
 return true;
 }
 
 private:
 //parts of the domain that were separated by '.', in reverse order
 vector<string> parts_;
 
 vector<string> SplitIntoParts(string full_name) {
 //c++ 23 version could use:
 //auto vec = std::ranges::to<std::vector>(view); or auto vec = view | std::ranges::to<std::vector>() | reverse;
 
 //c++ 20 ranges
 auto split_dmn = full_name | view::split('.') | view::transform([](auto r) {
 return std::string(r.begin(), r.end());
 });;
 vector<string> temp(split_dmn.begin(), split_dmn.end());
 
 auto reverse_dmn = temp | view::reverse;
 return vector<string>(make_move_iterator(reverse_dmn.begin()), make_move_iterator(reverse_dmn.end()));
 
 
 //c++ 17
 //        string_view dmn = full_;
 //        size_t dot = dmn.find_last_of('.');
 //
 //        while (dot != dmn.npos) {
 //
 //            parts_.push_back(dmn.substr(dot+1));
 //            dmn.remove_suffix(dmn.size() - dot);
 //
 //            dot = dmn.find_last_of('.');
 //        }
 //        parts_.push_back(dmn);
 }
 };
 
 e.g. forbidden domains:
 3
 gdz.ru
 gdx.ru
 gdv.ru
 
 check ru, gdx, history; ru, gdy, math
 
 lower bound -> issubdomain? until upper bound ?
 find -> equals?
 
 struct DomainLess {
 bool operator() (const Domain& lhs, const Domain& rhs) const {
 const auto& lp = lhs.GetParts();
 const auto& rp = rhs.GetParts();
 
 size_t n_min = std::min(lp.size(), rp.size());
 
 for(size_t i = 0; i < n_min; ++i) {
 if(lp[i] != rp[i]) {
 return lp[i] < rp[i];
 }
 }
 return lp.size() > rp.size();
 }
 };
 
 class DomainChecker {
 public:
 // конструктор должен принимать список запрещённых доменов через пару итераторов
 template<typename It> DomainChecker(It start, It finish)
 : forbidden_(start, finish) {
 cerr << "*Forbidden: \n";
 for(const auto& dmn : forbidden_) {
 cerr << dmn.GetFull() << '\n';
 }
 }
 
 // разработайте метод IsForbidden, возвращающий true, если домен запрещён
 bool IsForbidden (const Domain& domain) {
 auto lower = forbidden_.lower_bound(domain);
 auto upper = forbidden_.upper_bound(domain);
 cerr << "-Checking dmn: " << quoted(domain.GetFull()) << '\n';
 
 cerr << " ->lower: " << (lower == forbidden_.end() ? "end" : lower->GetFull()) << ' ';
 cerr << "->upper: " << (upper == forbidden_.end() ? "end" : upper->GetFull()) << '\n';
 
 if(lower == forbidden_.end()) {
 return false;
 } else if(lower == upper) {
 return lower->IsSubdomain(domain) || *lower == domain;
 }
 
 do{
 cerr << lower->GetFull() << " == " << domain.GetFull() << ": " << boolalpha << (domain == *lower) << " | IsSubdmn?: " << lower->IsSubdomain(domain) << '\n';
 
 if(domain == *lower) {
 return true;
 } else if (lower->IsSubdomain(domain)) {
 return true;
 } else if(lower == upper) {
 return false;
 }
 
 advance(lower, 1);
 cerr << "lower advanced to: " << lower->GetFull() << " is end? " << boolalpha << (lower == forbidden_.end()) << endl;
 } while (lower != forbidden_.end() && lower != upper);
 
 return false;
 }
 private:
 const set<Domain, DomainLess> forbidden_;
 
 };
 
 // разработайте функцию ReadDomains, читающую заданное количество доменов из стандартного входа
 vector<Domain> ReadDomains(istream& in, size_t num_lines) {
 vector<Domain> result;
 
 for (size_t n = 0; n < num_lines; ++n) {
 string line;
 getline(in, line);
 
 result.push_back(Domain(std::move(line)));
 }
 return result;
 }
 
 template <typename Number>
 Number ReadNumberOnLine(istream& input) {
 string line;
 getline(input, line);
 
 if (line.empty()) {
 return 0;
 }
 
 Number num;
 std::istringstream(line) >> num;
 
 return num;
 }
 
 int main() {
 ifstream in("/Users/ps/Documents/cpp-course/input_files/in.txt");
 //istream& in = cin;
 
 const std::vector<Domain> forbidden_domains = ReadDomains(in, ReadNumberOnLine<size_t>(in));
 DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
 
 const std::vector<Domain> test_domains = ReadDomains(in, ReadNumberOnLine<size_t>(in));
 for (const Domain& domain : test_domains) {
 cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
 }
 }
 
 Var code:
 For split:
 //split and store in vector in reverse order
 //c++ 23 version could use:
 //auto vec = std::ranges::to<std::vector>(view); or auto vec = view | std::ranges::to<std::vector>();
 
 //c++ 20 ranges
 auto split_dmn = full_ | view::split('.');
 
 vector<string_view> temp(split_dmn.begin(), split_dmn.end());
 
 auto reverse_dmn = temp | view::reverse;
 parts_ = vector<string_view>(reverse_dmn.begin(), reverse_dmn.end());
 
 //works without transform!, but for reference:
 | view::transform([](auto r) {
 return std::string_view(r.begin(), r.end());
 });
 
 //c++17:
 //        string_view dmn = full_;
 //        size_t dot = dmn.find_last_of('.');
 //
 //        while (dot != dmn.npos) {
 //
 //            parts_.push_back(dmn.substr(dot+1));
 //            dmn.remove_suffix(dmn.size() - dot);
 //
 //            dot = dmn.find_last_of('.');
 //        }
 //        parts_.push_back(dmn);
 
 */
