#pragma once
#include "img_lib.h"

#include <filesystem>

namespace img_lib {
using Path = std::filesystem::path;

bool SavePPM(const Path& file, const Image& image);
Image LoadPPM(const Path& file);

void NegateInplace(img_lib::Image& image);

void HMirrInplace(img_lib::Image& image);
void VMirrInplace(img_lib::Image& image);

img_lib::Image Sobel(const img_lib::Image& image);
}  // namespace img_lib
