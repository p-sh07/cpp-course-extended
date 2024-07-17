#pragma once
#include "geo.h"

#include <algorithm>
#include <list>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
#include <vector>

//DEBUG
//#include <iostream>
//#include <iomanip>

namespace transport {
struct Stop {
    explicit Stop(std::string&& stop_name, geo::Coordinates coords);
    std::string name;
    geo::Coordinates location;
};

struct Route {
    //move string during construction
    explicit Route(std::string&& name, std::vector<const Stop*>&& stops);
    
    std::string name;
    std::vector<const Stop*> stops;
    /*
    std::unordered_set<const Stop*> unique_stops;
    Да, но это же всего лишь дублирование указателей?
    И теперь к операции запроса статистики добавляется O(N) на перебор остановок
     + аллокация памяти, копирования и т.д, при подсчете уникальных...
     + было удобно использовать этот set при добавлении маршрута к остановкам
     -> for(auto& stop : unique_stops)->AddRouteToStop;
     + мне кажется, выгоднее вычислять при добавлении, т.к. добавляем один раз,
     а запрос на статистику производится многократно
    (хотя, в таком случае понадобится следить за актуальностью данных)
    
    double length = 0.0;
    С помощью этоЙ переменной можно было избежать повторного вычисления длины маршрута
     (и соответственно итерации по всем остановкам O(N)), при повторном вызове
     запроса на статистику. Но как и с уникальными остановками,
     пришлось бы следить за актуальностью - пересчитывать при изменении маршрута и т.д.
     */
};

struct RouteStats {
    double length = 0.0;
    size_t unique_stops = 0;
    size_t total_stops = 0;
};

struct StopStats {
    StopStats(std::string_view stop_name, const std::unordered_set<std::string_view>& unique_routes);
    
    // check if route contains buses
    explicit operator bool() const {
        return !sorted_routes.empty();
    }
    
    bool operator!() const {
        return !operator bool();
    }
    std::string_view id;
    std::vector<std::string_view> sorted_routes;
};

}

class TransportCatalogue {
public:
    ~TransportCatalogue() {
        ClearData();
    }
    /* Решил здесь заменить на &&, чтобы точно в конструктор передавался именно
     rvalue объект через например std::move;
     (так уже делалось и до этого, когда был параметр std::string name.
     
     Метод вызывался в input_reader вот так:
        catalogue.AddStop(std::move(cmd.id), ParseCoordinates(cmd.description));
     
     Так что копирования не происходило с параметром string не по значению) */
    void AddStop(std::string&& stop_name, geo::Coordinates coords);
    /* Использование const string& разве не приведет как раз к копированию строки?
     Т.к. move не сможет переместить строку по константной ссылке и создаст ее копию.
     
     А нам нужны string stop_name и route_name внутри класса, чтобы string_view на них оставались валидными
     (AddStop производит std::move() строки name в конструктор объекта Stop, аналогично с AddRoute) */
    void AddRoute(std::string&& route_name, const std::vector<std::string_view>& stops);
    
    std::optional<transport::RouteStats> GetRouteStats(std::string_view route_name) const;
    std::optional<transport::StopStats> GetStopStats(std::string_view stop_name) const;
    
private:
    using StopPair = std::pair<const transport::Stop*,const transport::Stop*>;
    
    std::vector<const transport::Stop*> GetStopPtrs(const std::vector<std::string_view>& route_stops) const;
    double GetDistance(const transport::Stop* from, const transport::Stop* to) const;
    void AddRouteToStops(const transport::Route* route);
    std::unordered_set<const transport::Stop*> GetUniqueStops(const transport::Route*) const;
    void ClearData();
    
    std::unordered_map<std::string_view, transport::Stop*> stop_index_;
    std::unordered_map<std::string_view, transport::Route*> route_index_;
    /* Cоздал отдельный словарь для информации по маршрутам для остановок, но не до конца понимаю,
     в чем преимущество - до этого был map Остановка->[различная информация по остановке]
    теперь же для части этой информации создается еще один такой же map с тем же набором ключей...
    
    + в коментарии на строке 24 про координаты:
    "Маршруты лучше убрать из этой структуры и сделать контейнером класса. В случае необходимости 
    получить маршруты их сможете получить по ключу Остановка. (И отсутствие лишней информации в виде координат)"
    Мы же получаем из словаря указатель Stop*, и дальше можно запросить доступ к нужному полю (Stop*->___)?
    Т.е. если координаты не нужны, их же можно и не смотреть)) почему координаты лишние? */
    std::unordered_map<std::string_view, std::unordered_set<std::string_view>> stops_to_routes_;
    
    struct SPHasher {
        size_t operator()(const StopPair& ptr_pair) const;
    };
    //can be modified by const member functions (e.g.GetStats)
    mutable std::unordered_map<StopPair, double, SPHasher> distance_table_;
};
