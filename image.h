#pragma once

#include "color.h"

#include "load_image.h"
#include "export_image.h"

#include <vector>
#include <string>

class Image {
public:
    Image() : width_(0), height_(0) {};
    Image(Image& img) : width_(img.width_), height_(img.height_), src_(img.src_) {
        data_.resize(width_ * height_);
        std::vector<Color>& copy_data = img.GetAllPixels();

        for(int i = 0; i < width_ * height_; ++i) {
            data_[i] = copy_data[i];
        }
    };

    Color GetColor(int x, int y) const {
        if (y * width_ + x > width_ * height_) {
            throw "out of range GetColor";
        }
        return data_[y * width_ + x];
    }

    void SetColor(const Color& color, int x, int y) {
        if (y * width_ + x > width_ * height_) {
            throw "out of range SetColor";
        }
        data_[y * width_ + x].red = color.red;
        data_[y * width_ + x].green = color.green;
        data_[y * width_ + x].blue = color.blue;
    }

    std::vector<Color>& GetAllPixels() {
        return data_;
    }

    std::pair<int, int> GetSize() const {
        return std::make_pair(width_, height_);
    }

    void SetSizes(int width, int height) {
        width_ = width;
        height_ = height;
    }

    void LoadImage(std::string src) {
        try {
            std::pair<int, int> res = LoadImageFromFile(src, data_);
            width_ = res.first;
            height_ = res.second;
        } catch (const char* c) {
            std::cerr << c << std::endl;
            width_ = -1;
            height_ = -1;
        }
    };

    void ExportImage(std::string src) const {
        try {
            ExportPixelDataToBMPFile(src, data_, width_, height_);
        } catch (const char* c) {
            std::cerr << c << std::endl;
        }
    };

private:
    int width_ = -1;
    int height_ = -1;
    std::string src_;
    std::vector<Color> data_;
};
