#include <img_lib.h>
#include <jpeg_image.h>
#include <ppm_image.h>

#include <filesystem>
#include <string_view>
#include <iostream>

using namespace std;

enum class Format {
    JPEG,
    PPM,
    UNKNOWN
};

class ImageFormatInterface {
public:
    virtual bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const = 0;
    virtual img_lib::Image LoadImage(const img_lib::Path& file) const = 0;
    virtual ~ImageFormatInterface() = default;
};

class PpmLoader : public ImageFormatInterface {
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SavePPM(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadPPM(file);
    }
    
    ~PpmLoader() override = default;
};

class JpegLoader : public ImageFormatInterface {
    bool SaveImage(const img_lib::Path& file, const img_lib::Image& image) const override {
        return img_lib::SaveJPEG(file, image);
    }
    img_lib::Image LoadImage(const img_lib::Path& file) const override {
        return img_lib::LoadJPEG(file);
    }
    
    ~JpegLoader() override = default;
};

Format GetFormatByExtension(const img_lib::Path& input_file) {
    const string ext = input_file.extension().string();
    if (ext == ".jpg"sv || ext == ".jpeg"sv) {
        return Format::JPEG;
    }

    if (ext == ".ppm"sv) {
        return Format::PPM;
    }

    return Format::UNKNOWN;
}

ImageFormatInterface* GetFormatInterface(const img_lib::Path& path) {
    switch (GetFormatByExtension(path)) {
        case Format::JPEG:
            return new JpegLoader();
            std::cerr << "Returning JpegLoader\n";
            break;
            
        case Format::PPM:
            return new PpmLoader();
            std::cerr << "Returning PpmLoader\n";
            break;
            
        case Format::UNKNOWN:
            return nullptr;
            break;
    }
}

int main(int argc, const char** argv) {
    if (argc != 3) {
        cerr << "Usage: "sv << argv[0] << " <in_file> <out_file>"sv << endl;
        return 1;
    }

    img_lib::Path in_path = argv[1];
    img_lib::Path out_path = argv[2];
    
    auto img_loader = GetFormatInterface(in_path);
    
    if(!img_loader) {
        return 2;
    }
    
    img_lib::Image image = img_loader->LoadImage(in_path);
    
    if (!image) {
        cerr << "Loading failed"sv << endl;
        return 4;
    }
    
    auto img_saver = GetFormatInterface(out_path);
    
    if(!img_saver) {
        return 3; //unknown format of output file
    }
    //Save image
    bool saved = img_saver->SaveImage(out_path, image);

    if (!saved) {
        cerr << "Saving failed"sv << endl;
        return 5;
    }
    
    delete img_loader;
    delete img_saver;

    cout << "Successfully converted"sv << endl;
    return 0;
}
