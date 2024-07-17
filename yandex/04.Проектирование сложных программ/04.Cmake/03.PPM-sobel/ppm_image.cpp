#include "ppm_image.h"

#include <array>
#include <algorithm>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <ranges>
#include <string_view>

using namespace std;

namespace img_lib {

static const string_view PPM_SIG = "P6"sv;
static const int PPM_MAX = 255;

// реализуйте эту функцию самостоятельно
bool SavePPM(const Path& file, const Image& img) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать даные в двоичном формате
    ofstream ofs(file, ios::binary);
    
    if(!ofs.is_open()) {
        return false;
    }
    
    //HEADER
    ofs << PPM_SIG << '\n' << img.GetWidth() << ' ' << img.GetHeight() << '\n' << PPM_MAX << '\n';
    
    //DATA
    const int w = img.GetWidth();
    const int h = img.GetHeight();
    std::vector<char> buff(w * 3);
    
    for (int y = 0; y < h; ++y) {
        const Color* line = img.GetLine(y);
        
        for (int x = 0; x < w; ++x) {
            buff[x * 3 + 0] = static_cast<char>(line[x].r);
            buff[x * 3 + 1] = static_cast<char>(line[x].g);
            buff[x * 3 + 2] = static_cast<char>(line[x].b);
        }
        ofs.write(buff.data(), w * 3);
    }
    return ofs.good();
}

Image LoadPPM(const Path& file) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать даные в двоичном формате
    ifstream ifs(file, ios::binary);
    std::string sign;
    int w, h, color_max;
    
    // читаем заголовок: он содержит формат, размеры изображения
    // и максимальное значение цвета
    ifs >> sign >> w >> h >> color_max;
    
    // мы поддерживаем изображения только формата P6
    // с максимальным значением цвета 255
    if (sign != PPM_SIG || color_max != PPM_MAX) {
        return {};
    }
    
    // пропускаем один байт - это конец строки
    const char next = ifs.get();
    if (next != '\n') {
        return {};
    }
    
    Image result(w, h, Color::Black());
    std::vector<char> buff(w * 3);
    
    for (int y = 0; y < h; ++y) {
        Color* line = result.GetLine(y);
        ifs.read(buff.data(), w * 3);
        
        for (int x = 0; x < w; ++x) {
            line[x].r = static_cast<byte>(buff[x * 3 + 0]);
            line[x].g = static_cast<byte>(buff[x * 3 + 1]);
            line[x].b = static_cast<byte>(buff[x * 3 + 2]);
        }
    }
    
    return result;
}

byte Negate(byte b) {
    return byte(255 - to_integer<int>(b));
}

img_lib::Color Negate(img_lib::Color c) {
    return {Negate(c.r), Negate(c.g), Negate(c.b), c.a};
}

void NegateInplace(img_lib::Image& image) {
    for (int y : views::iota(0, image.GetHeight())) {
        img_lib::Color* line = image.GetLine(y);
        for (int x : views::iota(0, image.GetWidth())) {
            line[x] = Negate(line[x]);
        }
    }
}

void VMirrInplace(Image& image) {
    const int h = image.GetHeight();
    const int w = image.GetWidth();
    for (int y : views::iota(0, h/2)) {
        img_lib::Color* line_top = image.GetLine(y);
        img_lib::Color* line_bot = image.GetLine(h-y-1);
        
        std::swap_ranges(line_top, (line_top + w), line_bot);
    }
}

void HMirrInplace(Image& image) {
    const int h = image.GetHeight();
    const int w = image.GetWidth();
    
    for (int y : views::iota(0, h)) {
        img_lib::Color* line = image.GetLine(y);
        
        //std::swap_range
        
        for (int x : views::iota(0, w/2)) {
            std::swap(line[x], line[w - x - 1]);
        }
    }
}

int GetIntensity(const Color& pixel) {
    return to_integer<int>(pixel.r) + to_integer<int>(pixel.g) + to_integer<int>(pixel.b);
}

Color DoubleToPixel(double value, byte alpha) {
    byte clr(static_cast<byte>(std::clamp(value, 0.0, 255.0)));
    return {clr, clr, clr, alpha};
}

Color ApplySobel(int x, int y, const Image& image) {
    if(x <= 0 || y <= 0) {
        throw std::logic_error("negative or zero pixel coords passed to Sobel");
        return {};
    }
    // gx=−tl−2tc−tr+bl+2bc+br,
    // gy=−tl−2cl−bl+tr+2cr+br.
    //Manual calculation:
    int gx =
    - 1 * GetIntensity(image.GetPixel(x-1, y-1))
    - 2 * GetIntensity(image.GetPixel(  x, y-1))
    - 1 * GetIntensity(image.GetPixel(x+1, y-1))
    + 1 * GetIntensity(image.GetPixel(x-1, y+1))
    + 2 * GetIntensity(image.GetPixel(  x, y+1))
    + 1 * GetIntensity(image.GetPixel(x+1, y+1));
    
    int gy =
    - 1 * GetIntensity(image.GetPixel(x-1, y-1))
    - 2 * GetIntensity(image.GetPixel(x-1, y  ))
    - 1 * GetIntensity(image.GetPixel(x-1, y+1))
    + 1 * GetIntensity(image.GetPixel(x+1, y-1))
    + 2 * GetIntensity(image.GetPixel(x+1, y  ))
    + 1 * GetIntensity(image.GetPixel(x+1, y+1));
    
    //TODO: use mask?
    
    byte alpha = image.GetPixel(x, y).a;
    return DoubleToPixel(sqrt(gx*gx + gy*gy), alpha);
}

Image Sobel(const Image& src) {
    const int h = src.GetHeight();
    const int w = src.GetWidth();
    
    Image dst(w, h, Color::Black());
    
    //last pixel is w-1, so use second to last
    for (int y : views::iota(1, h-1)) {
        Color* dst_line = dst.GetLine(y);
        for (int x : views::iota(1, w-1)) {
            dst_line[x] = ApplySobel(x, y, src);
        }
    }
    return dst;
}
    
    
    //Sobel:
    // gx=−tl−2tc−tr+bl+2bc+br,
    // gy=−tl−2cl−bl+tr+2cr+br.
    // ans == root(gx^2 + gy^2);
    //Приведите его к диапазону от 0 до 255 алгоритмом std::clamp<double>, а затем преобразуйте в std::byte конструкцией static_cast. Этим значением нужно заполнить все три компоненты цвета.
    //edge pixels = black
}  // namespace img_lib
