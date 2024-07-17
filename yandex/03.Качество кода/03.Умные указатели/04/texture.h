#pragma once
#include "common.h"

#include <optional>

class Texture {
public:
    explicit Texture(Image image)
        : image_(std::move(image)) {
    }

    Size GetSize() const {
        return GetImageSize(image_);
    }

    char GetPixelColor(Point p) const {
        auto sz = GetSize();
        if(p.x < sz.width && p.y < sz.height) {
            return image_[p.y][p.x];
        }
        return ' ';
    }

private:
    Image image_;
};
