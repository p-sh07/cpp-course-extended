#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

struct BMPFileHeader {
    uint16_t file_type{0x4D42};          // File type always BM which is 0x4D42
    uint32_t file_size{0};               // Size of the file (in bytes)
    uint16_t reserved1{0};               // Reserved, always 0
    uint16_t reserved2{0};               // Reserved, always 0
    uint32_t offset_data{0};              // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // width of bitmap in pixels
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };                 // No. of bits per pixel
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

PACKED_STRUCT_BEGIN BitmapFileHeader {
    // поля заголовка Bitmap File Header
    unsigned short type;
    unsigned int   size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int   off_bits;
}
PACKED_STRUCT_END

PACKED_STRUCT_BEGIN BitmapInfoHeader {
    // поля заголовка Bitmap Info Header
    unsigned int   size;
    unsigned int   width;
    unsigned int   height;
    unsigned short planes;
    unsigned short bit_count;
    unsigned int   compression;
    unsigned int   size_image;
    unsigned int   x_pels_per_meter;
    unsigned int   y_pels_per_meter;
    unsigned int   clr_used;
    unsigned int   clr_important;
}
PACKED_STRUCT_END

template <typename T>
inline void write_to_stream(std::ofstream& stream,const T& t) const
{
    stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
}

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    /*
     out.write(reinterpret_cast<const char*>(&file_header), sizeof(file_header));
     */
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать данные в двоичном формате
    ifstream ifs(file, ios::binary);
    std::string sign;
    int w, h, color_max;
    
    // читаем заголовок
    uint8_t headerbytes[54] = {};
    inputfile.read((char*)headerbytes, sizeof(headerbytes));
    
    // Jump to the pixel data location
    inp.seekg(file_header.offset_data, inp.beg);
    
    size =  sizeof(BitmapFileHeader) + sizeof(BitmapInfoHeader) + stride * image.GetHeight();
    
    uint32_t filesize = *(uint32_t*)(headerbytes+2);
    uint32_t dibheadersize = *(uint32_t*)(headerbytes + 14);
    uint32_t width = *(uint32_t*)(headerbytes + 18);
    uint32_t height = *(uint32_t*)(headerbytes + 22);
    uint16_t planes = *(uint16_t*)(headerbytes + 26);
    uint16_t bitcount = *(uint16_t*)(headerbytes + 28);
    
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

}  // namespace img_lib
