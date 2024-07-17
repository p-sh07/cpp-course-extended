#pragma once
#include "texture.h"

#include <functional>
#include <memory>

// Поддерживаемые виды фигур: прямоугольник и эллипс
enum class ShapeType { RECTANGLE, ELLIPSE};

class Shape {
public:
    // Фигура после создания имеет нулевые координаты и размер,
    // а также не имеет текстуры
    explicit Shape(ShapeType type) 
    : type_(type) {}

    void SetPosition(Point pos) {
        pos_ = pos;
    }

    void SetSize(Size size) {
        size_ = size;
    }

    void SetTexture(std::shared_ptr<Texture> texture) {
        texture_ = texture;
    }

    // Рисует фигуру на указанном изображении
	// В зависимости от типа фигуры должен рисоваться либо эллипс, либо прямоугольник
    // Пиксели фигуры, выходящие за пределы текстуры, а также в случае, когда текстура не задана,
    // должны отображаться с помощью символа точка '.'
    // Части фигуры, выходящие за границы объекта image, должны отбрасываться.
    void Draw(Image& image) const {
        if(image.empty()) {
            return;
        } //get texture size once
        Size texture_size = {};
        if(texture_) {
            texture_size = texture_->GetSize();
        }
        //Starting at top left corner of the shape on the image.
        for(int y = pos_.y; y < pos_.y+size_.height && y < image.size(); ++y) {
            for(int x = pos_.x; x < pos_.x+size_.width && x < image[0].size(); ++x) {
                //if no texture, or ouside texture - paint . if it's inside the figure
                //if rect - always inside figure
                //
                Point figure_pixel{x-pos_.x, y-pos_.y};
                //skip this pixel if shape is an ellipse and point is outside of boundary
                if(type_ == ShapeType::ELLIPSE && !IsPointInEllipse(figure_pixel, size_)) {
                    continue;
                } //check if pixel is within texture
                char pixel_value = '.';
                if(texture_ && isPointWithinSize(figure_pixel, texture_size)) {
                    pixel_value = texture_->GetPixelColor(figure_pixel);
                } //paint pixel
                image[y][x] = pixel_value;
            }
        }
    }
private:
    Size size_{0,0};
    Point pos_{0,0};
    ShapeType type_;
    std::shared_ptr<Texture> texture_{nullptr};
    
    bool isPointWithinSize(const Point& p, const Size& sz) const {
        return p.x < sz.width && p.y < sz.height;
    }
};
