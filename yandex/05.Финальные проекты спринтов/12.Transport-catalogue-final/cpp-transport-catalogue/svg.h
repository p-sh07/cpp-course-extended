#pragma once

#include <cstdint>
#include <iostream>
#include <iterator>
#include <memory>
#include <string>
#include <unordered_set>
#include <vector>
#include <variant>
#include <optional>

namespace svg {

using std::operator""s;

struct Rgb {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
};

struct Rgba {
    uint8_t red = 0;
    uint8_t green = 0;
    uint8_t blue = 0;
    double opacity = 1.0;
};

using Color = std::variant<std::monostate, std::string, svg::Rgb, svg::Rgba>;
const static inline std::string NoneColor("none");
    
std::ostream& operator<<(std::ostream& os, Color clr);

enum class StrokeLineCap {
    BUTT,
    ROUND,
    SQUARE,
};

std::ostream& operator<<(std::ostream& os, const StrokeLineCap& slc);

enum class StrokeLineJoin {
    ARCS,
    BEVEL,
    MITER,
    MITER_CLIP,
    ROUND,
};

std::ostream& operator<<(std::ostream& os, const StrokeLineJoin& slj);

struct Point {
    Point() = default;
    Point(double x, double y)
        : x(x)
        , y(y) {
    }
    double x = 0;
    double y = 0;
};

/*
 * Вспомогательная структура, хранящая контекст для вывода SVG-документа с отступами.
 * Хранит ссылку на поток вывода, текущее значение и шаг отступа при выводе элемента
 */
struct RenderContext {
    RenderContext(std::ostream& out)
        : out(out) {
    }

    RenderContext(std::ostream& out, int indent_step, int indent = 0)
        : out(out)
        , indent_step(indent_step)
        , indent(indent) {
    }

    RenderContext Indented() const {
        return {out, indent_step, indent + indent_step};
    }

    void RenderIndent() const {
        for (int i = 0; i < indent; ++i) {
            out.put(' ');
        }
    }

    std::ostream& out;
    int indent_step = 0;
    int indent = 0;
};

/*
 * Абстрактный базовый класс Object служит для унифицированного хранения
 * конкретных тегов SVG-документа
 * Реализует паттерн "Шаблонный метод" для вывода содержимого тега
 */
class Object {
public:
    virtual ~Object() = default;
    void Render(const RenderContext& context) const;

private:
    virtual void RenderObject(const RenderContext& context) const = 0;
};

template <typename Owner>
class PathProps {
public:
    //задаёт значение свойства fill — цвет заливки. По умолчанию свойство не выводится.
    Owner& SetFillColor(Color color);
    //задаёт значение свойства stroke — цвет контура. По умолчанию свойство не выводится.
    Owner& SetStrokeColor(Color color);
    //задаёт значение свойства stroke-width — толщину линии. По умолчанию свойство не выводится.
    Owner& SetStrokeWidth(double width);
    //задаёт значение свойства stroke-linecap — тип формы конца линии. По умолчанию свойство не выводится.
    Owner& SetStrokeLineCap(StrokeLineCap line_cap);
    //задаёт значение свойства stroke-linejoin — тип формы соединения линий. По умолчанию свойство не выводится.
    Owner& SetStrokeLineJoin(StrokeLineJoin line_join);
    
protected:
    ~PathProps() = default;
    
    void RenderAttrs(std::ostream& out) const;
    
private:
    
    Owner& AsOwner() {
        // static_cast безопасно преобразует *this к Owner&,
        // если класс Owner — наследник PathProps
        return static_cast<Owner&>(*this);
    }
    
    std::optional<Color> fill_color_;
    std::optional<Color> stroke_color_;
    std::optional<double> stroke_width_;
    std::optional<StrokeLineCap> line_cap_;
    std::optional<StrokeLineJoin> line_join_;
};

template <typename Owner>
Owner& PathProps<Owner>::SetFillColor(Color color) {
    fill_color_ = color;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeColor(Color color) {
    stroke_color_ = color;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeWidth(double width) {
    stroke_width_ = width;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeLineCap(StrokeLineCap line_cap) {
    line_cap_ = line_cap;
    return AsOwner();
}

template <typename Owner>
Owner& PathProps<Owner>::SetStrokeLineJoin(StrokeLineJoin line_join) {
    line_join_ = line_join;
    return AsOwner();
}

template <typename Owner>
void PathProps<Owner>::RenderAttrs(std::ostream& out) const {
    
    /* from sln:
     using detail::RenderOptionalAttr;
      using namespace std::literals;
      RenderOptionalAttr(out, "fill"sv, fill_color_);
     ...
     */
   if(fill_color_.has_value()) {
       out << "fill=\""s << fill_color_.value() << "\" ";
   }
   if(stroke_color_.has_value()) {
       out << "stroke=\""s << stroke_color_.value() << "\" ";
   }
   if(stroke_width_.has_value()) {
       out << "stroke-width=\""s << stroke_width_.value() << "\" ";
   }
   if(line_cap_.has_value()) {
       out << "stroke-linecap=\""s << line_cap_.value() << "\" ";
   }
   if(line_join_.has_value()) {
       out << "stroke-linejoin=\""s << line_join_.value() << "\" ";
   }
}


/*
 * Класс Circle моделирует элемент <circle> для отображения круга
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/circle
 */
class Circle final : public Object, public PathProps<Circle> {
public:
    Circle& SetCenter(Point center);
    Circle& SetRadius(double radius);

private:
    void RenderObject(const RenderContext& context) const override;

    Point center_;
    double radius_ = 1.0;
};

/*
 * Класс Polyline моделирует элемент <polyline> для отображения ломаных линий
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/polyline
 */
class Polyline : public Object, public PathProps<Polyline> {
public:
    // Добавляет очередную вершину к ломаной линии
    Polyline& AddPoint(Point point);
    
private:
    std::vector<Point> points_;
    void RenderObject(const RenderContext& context) const override;

};

/*
 * Класс Text моделирует элемент <text> для отображения текста
 * https://developer.mozilla.org/en-US/docs/Web/SVG/Element/text
 */
class Text : public Object, public PathProps<Text> {
public:
    // Задаёт координаты опорной точки (атрибуты x и y)
    Text& SetPosition(Point pos);

    // Задаёт смещение относительно опорной точки (атрибуты dx, dy)
    Text& SetOffset(Point offset);

    // Задаёт размеры шрифта (атрибут font-size)
    Text& SetFontPoint(uint32_t size);

    // Задаёт название шрифта (атрибут font-family)
    Text& SetFontFamily(std::string font_family);

    // Задаёт толщину шрифта (атрибут font-weight)
    Text& SetFontWeight(std::string font_weight);

    // Задаёт текстовое содержимое объекта (отображается внутри тега text)
    Text& SetData(std::string data);

private:
    Point position_;
    Point offset_;
    uint32_t font_size_ = 1;
    std::string font_family_;
    std::string font_weight_;
    std::string text_data_;
    
    // unicode numbers : " 34, & 38, ' 39, < 60, > 62;
    void ProcessText(std::ostream& out, std::string_view line) const;

    void RenderObject(const RenderContext& context) const override;
    
};

class ObjectContainer {
public:
    virtual ~ObjectContainer() = default;
    
    template <typename T>
    void Add(T obj);
    
    // Добавляет в svg-документ объект-наследник svg::Object
    virtual void AddPtr(std::unique_ptr<Object>&& obj) = 0;
};

template <typename T>
void ObjectContainer::Add(T obj) {
   AddPtr(std::make_unique<T>(std::move(obj)));
}

class Document : public ObjectContainer {
public:
    ~Document() = default;
    
    Document& operator=(Document&& other) {
        objects_.insert(objects_.end(), std::make_move_iterator(other.objects_.begin()), std::make_move_iterator(other.objects_.end()));
        
        other.objects_.erase(other.objects_.begin(), other.objects_.end());
        
        return *this;
    }
    
    // Добавляет в svg-документ объект-наследник svg::Object
    void AddPtr(std::unique_ptr<Object>&& obj) override;
    
    const std::unique_ptr<Object>& GetObject(size_t obj_id) const { return objects_[obj_id]; }
    
    // Выводит в ostream svg-представление документа
    void Render(std::ostream& out) const;

private:
    std::vector<std::unique_ptr<Object>> objects_;
};

class Drawable {
public:
    virtual ~Drawable() = default;
    virtual void Draw(ObjectContainer& container) const = 0;
};

}  // namespace svg
