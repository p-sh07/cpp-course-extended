#include "ini.h"

using namespace std;
using ini::Section;
using ini::Document;

// место для реализаций методов и функций библиотеки ini
// не забудьте, что они должны быть помещены в namespace ini

//Section -> std::unordered_map<std::string, std::string>;

// реализация методов должна быть в файле ini.cpp
Section& Document::AddSection(std::string name) {
    //dereference iterator returned by insert
    return sections_[name];
}

const Section& Document::GetSection(const std::string& name) const {
    if(sections_.count(name) > 0) {
        return sections_.at(name);
    } else {
        return empty_;
    }
}

std::size_t Document::GetSectionCount() const {
    return sections_.size();
}

string_view Trim(string_view str) {
    while(!str.empty() && (str[0] == ' ' || str[0] == '[') ) {
        str.remove_prefix(1);
    }
    while(!str.empty() && (str.back() == ' ' || str.back() == ']') ) {
        str.remove_suffix(1);
    }
    return str;
}

pair<string, string> SplitEntry(string_view entry) {
    size_t pos = entry.find('=');
    return { string( Trim(entry.substr(0, pos)) ),
             string( Trim(entry.substr(pos+1, entry.npos)) ) };
}

// определение этой функции должно быть в файле ini.cpp
Document ini::Load(std::istream& input) {
    Document doc;
    string line;
    
    while(getline(input,line)) {
        if(line.empty()) {
            continue;
        } // make a new section
        Section& curr_section = doc.AddSection(string(Trim(line)));
        // fill section, stop when next character shows a new section name
        while(input.peek() != '[' && getline(input, line)) {
            if(!line.empty()) {
                curr_section.insert(SplitEntry(line));
            } // remove any leading whitespace
            input >> std::ws;
        }
    }
    return doc;
}

