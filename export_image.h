#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class ExporterPixelDataToBMPFile {
public:
    void ExportDataToImage(std::string src, const std::vector<Color>& data, int width, int height) const {
        std::ofstream file(src, std::ios::out | std::ios::binary);

        if (!file.is_open()) {
            throw "The file is not founded";
        }

        if (width <= 0 || height <= 0) {
            throw "Loaded image is empty";
        }

        std::cout << "file " << src << " is opened" << std::endl;

        unsigned char bmp_pad[3] = {0, 0, 0};

        const int padding_amount = ((4 - (width * 3) % 4) % 4);

        const int file_header_size = 14;
        const int information_header_size = 40;
        const int file_size = file_header_size + information_header_size + width * height * 3 + padding_amount * height;

        unsigned char file_header[file_header_size];

        // file type
        file_header[0] = 'B';
        file_header[1] = 'M';
        // file size
        file_header[2] = file_size;
        file_header[3] = file_size >> 8;
        file_header[4] = file_size >> 16;
        file_header[5] = file_size >> 24;
        // Reserved 1 (Not used)
        file_header[6] = 0;
        file_header[7] = 0;
        // Reserved 2 (Not used)
        file_header[8] = 0;
        file_header[9] = 0;
        // Pixel data offset
        file_header[10] = file_header_size + information_header_size;
        file_header[11] = 0;
        file_header[12] = 0;
        file_header[13] = 0;

        unsigned char information_header[information_header_size];

        // Header size
        information_header[0] = information_header_size;
        information_header[1] = 0;
        information_header[2] = 0;
        information_header[3] = 0;
        // Image width
        information_header[4] = width;
        information_header[5] = width >> 8;
        information_header[6] = width >> 16;
        information_header[7] = width >> 24;
        // Image height
        information_header[8] = height;
        information_header[9] = height >> 8;
        information_header[10] = height >> 16;
        information_header[11] = height >> 24;
        // Planes
        information_header[12] = 1;
        information_header[13] = 0;
        // Bits per pixel
        information_header[14] = 24;
        information_header[15] = 0;
        // Other bytes
        information_header[16] = 0;
        information_header[17] = 0;
        information_header[18] = 0;
        information_header[19] = 0;
        information_header[20] = 0;
        information_header[21] = 0;
        information_header[22] = 0;
        information_header[23] = 0;
        information_header[24] = 0;
        information_header[25] = 0;
        information_header[26] = 0;
        information_header[27] = 0;
        information_header[28] = 0;
        information_header[29] = 0;
        information_header[30] = 0;
        information_header[31] = 0;
        information_header[32] = 0;
        information_header[33] = 0;
        information_header[34] = 0;
        information_header[35] = 0;
        information_header[36] = 0;
        information_header[37] = 0;
        information_header[38] = 0;
        information_header[39] = 0;

        file.write(reinterpret_cast<char*>(file_header), file_header_size);
        file.write(reinterpret_cast<char*>(information_header), information_header_size);

        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                Color cur_colors = data[y * width + x];

                unsigned char red = static_cast<unsigned char>(cur_colors.red * 255.0f);
                unsigned char green = static_cast<unsigned char>(cur_colors.green * 255.0f);
                unsigned char blue = static_cast<unsigned char>(cur_colors.blue * 255.0f);

                unsigned char color[] = {blue, green, red};

                file.write(reinterpret_cast<char*>(color), 3);
            }
            file.write(reinterpret_cast<char*>(bmp_pad), padding_amount);
        }

        file.close();

        std::cout << "file loaded" << std::endl;
    }
};

void ExportPixelDataToBMPFile(std::string src, const std::vector<Color>& data, int width, int height) {
    ExporterPixelDataToBMPFile file;
    file.ExportDataToImage(src, data, width, height);
}