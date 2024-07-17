//10.07.24

#include "ppm_image.h"

#include <iostream>
#include <string_view>

using namespace std;

int main(int argc, const char** argv) {
//    if (argc != 3) {
//        cerr << "Usage: "sv << argv[0] << " <input image> <output image>"sv << endl;
//        return 1;
//    }
    
    std::string input_img = "/Users/ps/Documents/cpp-course/input_files/kissy.ppm";
    std::string output_img = "/Users/ps/Documents/cpp-course/input_files/kissy_out.ppm";

    const auto image = img_lib::LoadPPM(input_img);
    if (!image) {
        cerr << "Error loading image"sv << endl;
        return 2;
    }

    if (!img_lib::SavePPM(output_img, image)) {
        cerr << "Error saving image"sv << endl;
        return 3;
    }

    cout << "Image saved successfully!"sv << endl;
}

/* task 3:
void NegateInplace(img_lib::Image& image)
Её задача — обратить изображение, заменив компоненту цвета со значением x на число 255 - x. Компоненту прозрачности менять не нужно. Обратить нужно три компоненты каждого пикселя.
Вызовите эту функцию в main после загрузки изображения, но перед сохранением. Программа будет обращать цвета изображения.
Учтите, вычитать значения типа byte нельзя, их нужно конвертировать к численному типу, а затем обратно. Для конвертации в число можно использовать функцию std::to_integer<int>. Для конвертации обратно в byte — конструктор.

*/
