#pragma once

#include <fstream>
#include <iostream>
#include <string>

#include "compressor.h"

using namespace std;
// напишите эту функцию
inline bool DecodeRLE(const std::string& src_name, const std::string& dst_name) {
    ifstream src(src_name, ios::binary);
    if(!src) {
        return false;
    }
    ofstream dst(dst_name, ios::binary);
    if(!dst) {
        return false;
    }
    
    //get max needed buffer size from compressor:
    const size_t buff_size = CompressorRLE::max_block_size;
    std::array<char, buff_size> buffer;
    
    do {
        //get header for data block
        char header_char;
        if(!src.get(header_char)) {
            return false;
        }
        unsigned char header = static_cast<unsigned char>(header_char);
        //first bit is 0 for uncompressed block
        bool block_is_compressed = (header & 1);
        int data_size = (header >> 1) + 1;
        //unpacking a compressed block -> consists of two bytes
        if(block_is_compressed) {
            char data = src.get();
            //data size gives num of repeats of data
            const string to_write(data_size, data);
            //NB Writing string to file in binary form, can use .c_str()
            dst.write(to_write.data(), to_write.size());
        } else { //writing uncompressed data block, with known max block size
            src.read(buffer.data(), data_size);
            dst.write(buffer.data(), data_size);
        }
    } while (src);
    return true;
}

// to read n bytes
//Делать свою функцию ReadExact необязательно — для чтения определённого количества байтов у потока istream есть специальный метод read. Он устроен аналогично, за исключением типа возврата — результатом будет не количество прочитанных символов, как в функции ReadExact, а сам поток, как во многих других методах потоков. Чтобы узнать количество прочитанных символов, после read вызовите метод gcount. Сравним по скорости собственную реализацию, использующую get, со встроенным методом read:

//in.read(buff, sizeof buff);
//size_t read = in.gcount();
//source_size += read;
//
//for (size_t i = 0; i < read; ++i) {
//    compressor.PutChar(buff[i]);
//}

/*
size_t ReadExact(istream& input, char* dst, size_t count) {
    for (size_t i = 0; i < count; ++i) {
        int c = input.get();
        if (c == istream::traits_type::eof() || !input) {
            return i;
        }
        dst[i] = static_cast<char>(c);
    }

    return count;
} */
