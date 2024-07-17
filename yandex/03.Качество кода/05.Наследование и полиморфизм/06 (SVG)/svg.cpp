#include "svg.h"

namespace svg {

using namespace std::literals;

void Object::Render(const RenderContext& context) const {
    context.RenderIndent();

    // Делегируем вывод тега своим подклассам
    RenderObject(context);

    context.out << std::endl;
}

// ---------- Circle ------------------

Circle& Circle::SetCenter(Point center)  {
    center_ = center;
    return *this;
}

Circle& Circle::SetRadius(double radius)  {
    radius_ = radius;
    return *this;
}

void Circle::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<circle cx=\""sv << center_.x << "\" cy=\""sv << center_.y << "\" "sv;
    out << "r=\""sv << radius_ << "\" "sv;
    out << "/>"sv;
}

Polyline& Polyline::AddPoint(Point point) {
    points_.push_back(point);
    return *this;
}

void Polyline::RenderObject(const RenderContext& context) const {
    // <polyline points="20,40 22.9389,45.9549 29.5106,46.9098" />
    auto& out = context.out;
    out << "<polyline points=\""sv;
    bool is_first = true;
    for(const auto& pt : points_) {
        if(!is_first) {
            out << ' ';
        }
        is_first = false;
        out<< pt.x << ',' << pt.y;
    }
    out << "\" />"sv;
}


// Задаёт координаты опорной точки (атрибуты x и y)
Text& Text::SetPosition(Point pos) {
    position_ = pos;
    return *this;
}

// Задаёт смещение относительно опорной точки (атрибуты dx, dy)
Text& Text::SetOffset(Point offset) {
    offset_ = offset;
    return *this;
}

// Задаёт размеры шрифта (атрибут font-size)
Text& Text::SetFontSize(uint32_t size) {
    font_size_ = size;
    return *this;
}

// Задаёт название шрифта (атрибут font-family)
Text& Text::SetFontFamily(std::string font_family) {
    font_family_ = font_family;
    return *this;
}

// Задаёт толщину шрифта (атрибут font-weight)
Text& Text::SetFontWeight(std::string font_weight) {
    font_weight_ = font_weight;
    return *this;
}

// Задаёт текстовое содержимое объекта (отображается внутри тега text)
Text& Text::SetData(std::string data) {
    text_data_ = data;
    return *this;
}

void Text::ProcessText(std::ostream& out, std::string_view line) const {
    for(auto c : line) {
        switch(c) {
            case '\"':
                out << "&quot;"sv;
                break;
                
            case '&':
                out << "&amp;"sv;
                break;
                
            case '\'':
                out << "&apos;"sv;
                break;
                
            case '<':
                out << "&lt;"sv;
                break;
                
            case '>':
                out << "&gt;"sv;
                break;
                
            default:
                out.put(c);
        }
    }
}

void Text::RenderObject(const RenderContext& context) const {
    auto& out = context.out;
    out << "<text"sv;
    
    // <text x="35" y="20" dx="0" dy="6" font-size="12" font-family="Verdana" font-weight="bold">
    
    out << " x=\"" << position_.x << '\"' << " y=\"" << position_.y << '\"'
        << " dx=\"" << offset_.x << '\"' << " dy=\"" << offset_.y << '\"'
        << " font-size=\"" << font_size_ << '\"';
    
    if(!font_family_.empty()) {
        out << " font-family=\"" << font_family_ << '\"';
    }
    if(!font_weight_.empty()) {
        out << " font-weight=\"" << font_weight_ << '\"';
    }
    
    out << '>';
    
    ProcessText(out, text_data_);
    
    out << "</text>"sv;
}

// Добавляет в svg-документ объект-наследник svg::Object
void Document::AddPtr(std::unique_ptr<Object>&& obj) {
    objects_.emplace_back(std::move(obj));
}

// Выводит в ostream svg-представление документа
/*
 Содержимое, выводимое методом svg::Document::Render, должно состоять из следующих частей:
 <?xml version="1.0" encoding="UTF-8" ?>
 <svg xmlns="http://www.w3.org/2000/svg" version="1.1">
 Объекты, добавленные с помощью svg::Document::Add, в порядке их добавления
 </svg>
 Все свойства объектов выводятся в следующем формате: название свойства, символ =, затем значение свойства в кавычках.
 */
void Document::Render(std::ostream& out) const {
    RenderContext context{out, 2, 2};
    
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n"sv
    << "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n";
    
    for(const auto& obj : objects_) {
        obj->Render(context);
    }
    out << "</svg>\n"sv;
}

}  // namespace svg
