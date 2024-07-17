#include "bmp_image.h"
#include "pack_defines.h"

#include <array>
#include <fstream>
#include <string_view>

using namespace std;

namespace img_lib {

PACKED_STRUCT_BEGIN BMPHeader {
        // BMP file header
        uint16_t file_type   {0x4D42}; // 'BM'
        uint32_t file_size   {0};
        uint16_t reserved1   {0};
        uint16_t reserved2   {0};
        uint32_t offset_data {54};

        // BMP information header
        uint32_t header_size        {40};
        int32_t  width              {0};
        int32_t  height             {0};
        uint16_t planes             {1};
        uint16_t bit_per_pixel      {24};
        uint32_t compression_method {0};
        uint32_t image_size         {0};
        int32_t  h_pixels_per_meter {11811};
        int32_t  v_pixels_per_meter {11811};
        uint32_t colors_in_palette  {0};
        uint32_t important_colors   {0x1000000};
    } PACKED_STRUCT_END

template <typename T>
inline void write_to_stream(std::ofstream& stream,const T& t) {
    stream.write(reinterpret_cast<const char*>(&t),sizeof(T));
}

// функция вычисления отступа по ширине
static int GetBMPStride(int w) {
    return 4 * ((w * 3 + 3) / 4);
}

// напишите эту функцию
bool SaveBMP(const Path& file, const Image& image) {
    //Создать хэдер по умолчанию
    BMPHeader header;
    
    const int w = image.GetWidth();
    const int h = image.GetHeight();
    const int stride = GetBMPStride(w);
    
    header.width = w;
    header.height = h;
    
    header.image_size = header.height * stride;
    header.offset_data = sizeof(header);
    header.file_size = header.offset_data + header.image_size;
    
    std::ofstream out(file, ios::binary);
    
    if(!out) {
        return false;
    }
    out.write(reinterpret_cast<const char*>(&header), sizeof(header));
    
    //reserve buff for conversion
    std::vector<char> data(header.image_size);
    
    for (int y = 0; y < h; ++y) {
        const Color* line = image.GetLine(y);
        //(height in rows - 1) - y -> backwards
        const int offset_coeff = h - 1 - y;
        const int line_start = offset_coeff * stride;
        
        for (int x = 0; x < w; ++x) {
            data[line_start + x * 3] = static_cast<char>(line[x].b);
            data[line_start + x * 3 + 1] = static_cast<char>(line[x].g);
            data[line_start + x * 3 + 2] = static_cast<char>(line[x].r);
        }
    }
    
    out.write(data.data(), header.image_size);
    out.close();
    
    return true;
}

// напишите эту функцию
Image LoadBMP(const Path& file) {
    // открываем поток с флагом ios::binary
    // поскольку будем читать данные в двоичном формате
    ifstream ifs(file, ios::binary);
    
    BMPHeader header;
    // читаем заголовок
    ifs.read(reinterpret_cast<char*>(&header), sizeof(BMPHeader));
    
    if(header.file_type != 0x4D42) {
        return {};
    }
    
    // Перейти к началу данных изображения
    ifs.seekg(header.offset_data, std::ios::beg);
    
    const int w = header.width, h = header.height;
    const int stride = GetBMPStride(w);
    
    const int img_size = stride * w;
    
    std::vector<char> data(img_size);
    ifs.read(data.data(), data.size());
    
    Image result(w, h, Color::Black());
    
    for (int y = 0; y < h; ++y) {
        Color* line = result.GetLine(y);
        //(height in rows - 1) - y -> backwards
        const int offset_coeff = h - 1 - y;
        const int line_start = offset_coeff * stride;
        
        for (int x = 0; x < w; ++x) {
            line[x].b = static_cast<byte>(data[line_start + x * 3]);
            line[x].g = static_cast<byte>(data[line_start + x * 3 + 1]);
            line[x].r = static_cast<byte>(data[line_start + x * 3 + 2]);
        }
    }
    return result;
}

}  // namespace img_lib
