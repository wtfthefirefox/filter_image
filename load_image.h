#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class ReadBPMFile {
public:
    std::pair<int, int> ReadFile(std::string src, std::vector<Color>& data) {
        std::ifstream file(src, std::ios::in | std::ios::binary);

        if (!file.is_open()) {
            throw "The file is not founded";
        }

        std::cout << "file opened" << std::endl;

        const int file_header_size = 14;
        const int information_header_size = 40;

        unsigned char file_header[file_header_size];
        file.read(reinterpret_cast<char*>(file_header), file_header_size);

        if (file_header[0] != 'B' || file_header[1] != 'M') {
            file.close();
            throw "The file is not a bitmap image";
        }

        int file_size = file_header[10];

        if (file_size != 54) {
            throw "BMP file header is not BITMAPINFOHEADER";
        }

        unsigned char information_header[information_header_size];
        file.read(reinterpret_cast<char*>(information_header), information_header_size);

        width_ = information_header[4] + (information_header[5] << 8) + (information_header[6] << 16) + (information_header[7] << 24);
        height_ = information_header[8] + (information_header[9] << 8) + (information_header[10] << 16) + (information_header[11] << 24);

        data.resize(height_ * width_);

        const int padding_amount = ((4 - (width_ * 3) % 4) % 4);

        for(int y = 0; y < height_; ++y) {
            for(int x = 0; x < width_; ++x) {
                unsigned char color[3];
                file.read(reinterpret_cast<char*>(color), 3);
                data[y * width_ + x].red = static_cast<float>(color[2]) / 255.0f;
                data[y * width_ + x].green = static_cast<float>(color[1]) / 255.0f;
                data[y * width_ + x].blue = static_cast<float>(color[0]) / 255.0f;
            }

            file.ignore(padding_amount);
        }

        file.close();

        std::cout << "file readed" << std::endl;

        return std::make_pair(width_, height_);
    };

private:
    int width_;
    int height_;
};

std::pair<int, int> LoadImageFromFile(std::string src, std::vector<Color>& data) {
    ReadBPMFile file;
    return file.ReadFile(src, data);
}