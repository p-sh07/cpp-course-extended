#include "ppm_image.h"

#include <array>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <setjmp.h>

#include "jpeglib.h"

using namespace std;

namespace img_lib {

// структура из примера LibJPEG
struct my_error_mgr {
    struct jpeg_error_mgr pub;
    jmp_buf setjmp_buffer;
};

typedef struct my_error_mgr* my_error_ptr;

// функция из примера LibJPEG
METHODDEF(void)
my_error_exit (j_common_ptr cinfo) {
    my_error_ptr myerr = (my_error_ptr) cinfo->err;
    (*cinfo->err->output_message) (cinfo);
    longjmp(myerr->setjmp_buffer, 1);
}

// В эту функцию вставлен код примера из библиотеки libjpeg.

// Задание качества уберите - будет использовано качество по умолчанию
bool SaveJPEG(const Path& file, const Image& image) {
    
    jpeg_compress_struct cinfo;
    jpeg_error_mgr jerr;
    
    /* More stuff */
    FILE * outfile;       /* target file */
    JSAMPROW row_pointer[1];  /* pointer to JSAMPLE row[s] */
    int row_stride;       /* physical row width in image buffer */
    
    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);
    
#ifdef _MSC_VER
    if ((outfile = _wfopen(file.wstring().c_str(), "wb")) == NULL) {
#else
        if ((outfile = fopen(file.c_str(), "wb")) == NULL) {
#endif
            return false;
        }
        
        jpeg_stdio_dest(&cinfo, outfile);
        
        const int image_width = image.GetWidth();
        const int image_height = image.GetHeight();
        
        cinfo.image_width = image_width;  /* image width and height, in pixels */
        cinfo.image_height = image_height;
        cinfo.input_components = 3;       /* # of color components per pixel */
        cinfo.in_color_space = JCS_RGB;   /* colorspace of input image */
        
        jpeg_set_defaults(&cinfo);
        jpeg_start_compress(&cinfo, TRUE);
        
        row_stride = image_width * 3;
        vector<JSAMPLE> buff(row_stride);
        
        for (int y = 0; y < image_height; ++y) {
            //Get line from image and load it into buff
            auto line = image.GetLine(y);
            for(int x = 0; x < image_width; ++x) {
                buff[x * 3] = static_cast<JSAMPLE>(line[x].r);     // 0, 3, 6
                buff[x * 3 + 1] = static_cast<JSAMPLE>(line[x].g); // 1. 4. 7
                buff[x * 3 + 2] = static_cast<JSAMPLE>(line[x].b); // 2, 5, 8
            }
            JSAMPROW rowPtr[1];
            rowPtr[0] = const_cast<JSAMPROW>(buff.data());

            (void) jpeg_write_scanlines(&cinfo, rowPtr, 1);
        }
        
        jpeg_finish_compress(&cinfo);
        
        /* After finish_compress, we can close the output file. */
        fclose(outfile);
        
        /* This is an important step since it will release a good deal of memory. */
        jpeg_destroy_compress(&cinfo);
        
        return true;
    }
    
    // тип JSAMPLE фактически псевдоним для unsigned char
    void SaveSсanlineToImage(const JSAMPLE* row, int y, Image& out_image) {
        Color* line = out_image.GetLine(y);
        for (int x = 0; x < out_image.GetWidth(); ++x) {
            const JSAMPLE* pixel = row + x * 3;
            line[x] = Color{byte{pixel[0]}, byte{pixel[1]}, byte{pixel[2]}, byte{255}};
        }
    }
    
    Image LoadJPEG(const Path& file) {
        jpeg_decompress_struct cinfo;
        my_error_mgr jerr;
        
        FILE* infile;
        JSAMPARRAY buffer;
        int row_stride;
        
        // Тут не избежать функции открытия файла из языка C,
        // поэтому приходится использовать конвертацию пути к string.
        // Под Visual Studio это может быть опасно, и нужно применить
        // нестандартную функцию _wfopen
#ifdef _MSC_VER
        if ((infile = _wfopen(file.wstring().c_str(), "rb")) == NULL) {
#else
            if ((infile = fopen(file.string().c_str(), "rb")) == NULL) {
#endif
                return {};
            }
            
            /* Шаг 1: выделяем память и инициализируем объект декодирования JPEG */
            
            cinfo.err = jpeg_std_error(&jerr.pub);
            jerr.pub.error_exit = my_error_exit;
            
            if (setjmp(jerr.setjmp_buffer)) {
                jpeg_destroy_decompress(&cinfo);
                fclose(infile);
                return {};
            }
            
            jpeg_create_decompress(&cinfo);
            
            /* Шаг 2: устанавливаем источник данных */
            
            jpeg_stdio_src(&cinfo, infile);
            
            /* Шаг 3: читаем параметры изображения через jpeg_read_header() */
            
            (void) jpeg_read_header(&cinfo, TRUE);
            
            /* Шаг 4: устанавливаем параметры декодирования */
            
            // установим желаемый формат изображения
            cinfo.out_color_space = JCS_RGB;
            cinfo.output_components = 3;
            
            /* Шаг 5: начинаем декодирование */
            
            (void) jpeg_start_decompress(&cinfo);
            
            row_stride = cinfo.output_width * cinfo.output_components;
            
            buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
            
            /* Шаг 5a: выделим изображение ImgLib */
            Image result(cinfo.output_width, cinfo.output_height, Color::Black());
            
            /* Шаг 6: while (остаются строки изображения) */
            /*                     jpeg_read_scanlines(...); */
            
            while (cinfo.output_scanline < cinfo.output_height) {
                int y = cinfo.output_scanline;
                (void) jpeg_read_scanlines(&cinfo, buffer, 1);
                
                SaveSсanlineToImage(buffer[0], y, result);
            }
            
            /* Шаг 7: Останавливаем декодирование */
            
            (void) jpeg_finish_decompress(&cinfo);
            
            /* Шаг 8: Освобождаем объект декодирования */
            
            jpeg_destroy_decompress(&cinfo);
            fclose(infile);
            
            return result;
        }
        
    } // of namespace img_lib
