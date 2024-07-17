#include "bimap.h"

#include <cassert>
#include <deque>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

using std::string;

struct BiMap::Impl {
    
    Impl() = default;
    ~Impl() = default;
    
    
    
    
    bool Add(std::string_view key, std::string_view value) {
        if(key_value_.count(key) > 0 || value_key_.count(value) > 0) {
            return false;
        }
        
        try {
            string_data_.push_front(std::string(value));
            string_data_.push_front(std::string(key));
        } catch (std::exception ex) {
            return false;
        }
        std::string_view local_key = *string_data_.begin();
        std::string_view local_val = *(++string_data_.begin());
        
        key_value_.insert(std::make_pair(local_key, local_val));
        value_key_.insert(std::make_pair(local_val, local_key));
        
        return true;
    }
    std::optional<std::string_view> FindValue(std::string_view key) const noexcept {
        if(key_value_.count(key) == 0) {
            return std::nullopt;
        }
        return key_value_.at(key);
    }
    std::optional<std::string_view> FindKey(std::string_view value) const noexcept {
        if(value_key_.count(value) == 0) {
            return std::nullopt;
        }
        return value_key_.at(value);
    }
    
private:
    std::deque<std::string> string_data_;
    
    std::unordered_map<std::string_view, std::string_view> key_value_;
    std::unordered_map<std::string_view, std::string_view> value_key_;
};

BiMap::BiMap()
: impl_(std::make_unique<Impl>()) {
}

BiMap::~BiMap() = default;

BiMap::BiMap(BiMap&&) noexcept = default;
BiMap& BiMap::operator=(BiMap&&) noexcept = default;


BiMap::BiMap(const BiMap& other)
// Если other не пуст, копируем его поле impl_
: impl_(other.impl_ ? std::make_unique<Impl>(*other.impl_) : nullptr) {
}

BiMap& BiMap::operator=(const BiMap& other) {
    if (this != std::addressof(other)) {
        if (!other.impl_) {     // Правый аргумент пуст?
            impl_.reset();
        } else if (impl_) {     // Левый и правый аргументы не пустые?
            assert(other.impl_);
            *impl_ = *other.impl_;
        } else {                // Левый аргумент пуст, а правый не пуст
            assert(!impl_ && other.impl_);
            impl_ = std::make_unique<Impl>(other.impl_);
        }
    }
    return *this;
}

bool BiMap::Add(std::string_view key, std::string_view value) {
    return impl_->Add(key, value);
}

std::optional<std::string_view> BiMap::FindValue(std::string_view key) const noexcept {
    return impl_->FindValue(key);
}

std::optional<std::string_view> BiMap::FindKey(std::string_view value) const noexcept {
    return impl_->FindKey(value);
}


/** sln
 
 #include "bimap.h"

 #include <cassert>
 #include <deque>
 #include <unordered_map>

 struct BiMap::Impl {
     Impl() = default;

     Impl(const Impl& other)
         : items_(other.items_) {
         for (auto& [k, v] : items_) {
             key_to_value_[k] = v;
             value_to_key_[v] = k;
         }
     }

     // Присваивание реализовано в BiMap за счёт создания нового объекта Impl
     Impl& operator=(const Impl& other) = delete;

     bool Add(std::string_view key, std::string_view value) {
         if (FindValue(key) || FindKey(value)) {
             return false;
         }

         items_.push_back(Item{std::string(key), std::string(value)});

         const auto& inserted_item = items_.back();
         std::string_view k = inserted_item.key;
         std::string_view v = inserted_item.value;

         try {
             key_to_value_.emplace(k, v);
             value_to_key_.emplace(v, k);
         } catch (...) {
             // Откатываем изменения в случае выброшенного исключения
             key_to_value_.erase(k);
             value_to_key_.erase(v);
             items_.pop_back();
             throw;
         }

         return true;
     }

     std::optional<std::string_view> FindValue(std::string_view key) const noexcept {
         if (auto pos = key_to_value_.find(key); pos != key_to_value_.end()) {
             return pos->second;
         }
         return {};
     }

     std::optional<std::string_view> FindKey(std::string_view value) const noexcept {
         if (auto pos = value_to_key_.find(value); pos != value_to_key_.end()) {
             return pos->second;
         }
         return {};
     }

 private:
     struct Item {
         std::string key;
         std::string value;
     };

     std::deque<Item> items_;
     std::unordered_map<std::string_view, std::string_view> key_to_value_;
     std::unordered_map<std::string_view, std::string_view> value_to_key_;
 };

 BiMap::BiMap()
     : impl_(std::make_unique<Impl>()) {
 }

 BiMap::BiMap(BiMap&& other) noexcept = default;

 BiMap& BiMap::operator=(BiMap&& other) noexcept = default;

 BiMap::BiMap(const BiMap& other)
     : impl_{other.impl_ ? std::make_unique<Impl>(*other.impl_) : nullptr} {
 }

 BiMap& BiMap::operator=(const BiMap& other) {
     if (this != std::addressof(other)) {
         impl_ = other.impl_ ? std::make_unique<Impl>(*other.impl_) : nullptr;
     }
     return *this;
 }

 BiMap::~BiMap() = default;

 bool BiMap::Add(std::string_view key, std::string_view value) {
     return impl_->Add(key, value);
 }

 std::optional<std::string_view> BiMap::FindValue(std::string_view key) const noexcept {
     return impl_->FindValue(key);
 }

 std::optional<std::string_view> BiMap::FindKey(std::string_view value) const noexcept {
     return impl_->FindKey(value);
 }
 
 */
