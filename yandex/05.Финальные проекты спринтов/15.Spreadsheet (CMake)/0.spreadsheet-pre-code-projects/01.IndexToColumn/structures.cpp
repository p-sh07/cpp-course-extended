#include "common.h"

#include <algorithm>
#include <cctype>
#include <tuple>
#include <sstream>

const int NUM_LETTERS = 26;
const int MAX_LENGTH = 8;
const int MAX_POS_LETTER_COUNT = 3;

const Position Position::NONE = {-1, -1};

// Реализуйте методы:
bool Position::operator==(const Position rhs) const {
    return row == rhs.row && col == rhs.col;
}

bool Position::operator<(const Position rhs) const {
    return std::tie(row, col) < std::tie(rhs.row, rhs.col);
}

bool Position::IsValid() const {
    return row >= 0 && col >= 0
    && row < MAX_ROWS && col < MAX_COLS;
}

std::string Position::ToString() const {
    if(!IsValid()) {
        return "";
    }
    std::string result;
    
    //solution adapted from A == 1, since it is much simpler
    for(int n = col+1; n > 0; ) {
        int remainder = (n - 1) % NUM_LETTERS;
        result.push_back('A' + remainder);
        n = (n - remainder) / NUM_LETTERS;
    }
    //Reverse vecause used push back instead of str = str(char) + str;
    std::reverse(result.begin(), result.end());
    
    result.append(std::to_string(row+1));
    
    return result;
}

Position Position::FromString(std::string_view str) {
    if(str.empty() || std::isdigit(str[0]) || str.size() > MAX_LENGTH) {
        return Position::NONE;
    }
    auto first_digit = std::find_if(str.begin(), str.end(), [](const char& c) {
        return std::isdigit(c);
    });
    size_t pos = first_digit - str.begin();
    
    if(first_digit == str.end() || pos > MAX_POS_LETTER_COUNT) {
        return Position::NONE;
    }
    //check the rest are all numbers:
    auto non_digit = std::find_if(first_digit, str.end(), [](const char& c) {
        return !std::isdigit(c);
    });
    if(non_digit != str.end()) {
        return Position::NONE;
    }
    
    Position result;
    std::string_view col_str = str.substr(0, pos);
    str.remove_prefix(pos);
    
    //https://en.wikipedia.org/wiki/Horner%27s_method
    result.col = 0;
    for(const char c : col_str) {
        int diff = c - 'A';
        if(std::islower(c) || diff < 0 || diff > NUM_LETTERS) {
            return Position::NONE;
        }
        result.col = NUM_LETTERS*result.col + diff + 1;
    }
    //Above solution works for A == 1, so decrement
    --result.col;
    
    result.row = std::stoi(std::string(str)) - 1;
    return result.IsValid() ? result : Position::NONE;
}
