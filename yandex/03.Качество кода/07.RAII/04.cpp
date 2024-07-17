//
//  03.Качество кода
//  Тема
//  Урок
//
//  Created by ps on 22.04.2024.
//
#include <numeric>
#include <iostream>
#include <queue>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

using namespace std;

struct Booking {
    string hotel_name;
    int64_t time;
    int client_id;
    int num_rooms;
};

class HotelManager {
public:
    void Book(const string& hotel_name, int64_t booking_time, int client_id, int num_rooms) {
        //add booking data:
        booking_data_.push_back(shared_ptr<Booking>(new Booking{hotel_name, booking_time, client_id, num_rooms}));
        relevant_bookings_.push(booking_data_.back());
        
        //make sure string view stays valid:
        string_view hotel_name_sv = booking_data_.back()->hotel_name;
        client_booking_stats_[hotel_name_sv][client_id] += num_rooms;

        //remove all outdated bookings from stats:
        EraseOutdatedStats(booking_time);
    }
    
    int ComputeClientCount(string_view hotel) {
        if(client_booking_stats_.count(hotel) == 0) {
            return 0;
        }
        return static_cast<int>(client_booking_stats_.at(hotel).size());
    }
    
    int ComputeRoomCount(string_view hotel) {
        if(client_booking_stats_.count(hotel) == 0) {
            return 0;
        }
        const auto& hotel_stat = client_booking_stats_.at(hotel);
        
        return std::accumulate(hotel_stat.begin(), hotel_stat.end(), 0, [](int sum, const auto& client_rooms_pair) {
            return sum + client_rooms_pair.second;
        });
    }

private:
    const static int64_t relevance_window_ = 86400;
    
    vector<shared_ptr<Booking>> booking_data_;
    queue<shared_ptr<Booking>> relevant_bookings_;
    
    //hotel name -> [client_id, num of rooms booked]
    unordered_map<string_view, unordered_map<int,int>> client_booking_stats_;
    
    void EraseOutdatedStats(int64_t current_time) {
        while (!relevant_bookings_.empty()) {
            const auto& booking = relevant_bookings_.front();
            
            if(std::abs(current_time - booking->time) >= relevance_window_) {
                
                auto& stat_for_hotel = client_booking_stats_.at(booking->hotel_name);
                stat_for_hotel.at(booking->client_id) -= booking->num_rooms;

                if(stat_for_hotel.at(booking->client_id) == 0) {
                    stat_for_hotel.erase(booking->client_id);
                }
                relevant_bookings_.pop();
            } else {
                break;
            }
        }
    }
};

int main() {
    HotelManager manager;

    int query_count;
    
    cin >> query_count;

    for (int query_id = 0; query_id < query_count; ++query_id) {
        string query_type;
        cin >> query_type;

        if (query_type == "BOOK") {
            int64_t time;
            cin >> time;
            string hotel_name;
            cin >> hotel_name;
            int client_id, room_count;
            cin >> client_id >> room_count;
            manager.Book(hotel_name, time, client_id, room_count);
        } else {
            string hotel_name;
            cin >> hotel_name;
            if (query_type == "CLIENTS") {
                cout << manager.ComputeClientCount(hotel_name) << "\n";
            } else if (query_type == "ROOMS") {
                cout << manager.ComputeRoomCount(hotel_name) << "\n";
            }
        }
    }

    return 0;
}
