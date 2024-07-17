//
//  03.Качество кода
//  Тема
//  Урок
//

#pragma once

namespace raii {
template <typename Provider>
class Booking {
private:
    using BookingId = typename Provider::BookingId;
    
    Provider* provider_;
    BookingId booking_id_;
    
public:
    Booking(Provider* p, BookingId book_id)
    : provider_(p)
    , booking_id_(book_id) {
    }
    
    Booking(const Booking&) = delete;
    Booking(Booking&& other) = default;
    
    Booking& operator=(const Booking&) = delete;
    Booking& operator=(Booking&& other) = default;
    
    ~Booking() {
        if(provider_) {
            Cancel();
        }
    }
    
    void Cancel() {
        provider_->CancelOrComplete(*this);
    }
    
    // Эта функция не требуется в тестах, но в реальной программе она может быть нужна
    BookingId GetId() const {
        return booking_id_;
    }
};

} //namespace raii
