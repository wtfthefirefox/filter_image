#pragma once

#include <cmath>

class FiltersList {
public:
    void CroppingImage(Image& img, int new_width, int new_height) {
        auto [width_image, height_image] = img.GetSize();
        
        if (new_width > width_image || new_height > height_image || new_height <= 0 || new_width <= 0) {
            throw "Sizes of cropped image can not be setted";
        };

        if (new_width == width_image && new_height == height_image) {
            throw "New size is equal to current size";
        }

        std::vector<Color>& img_data = img.GetAllPixels();
        std::vector<Color> new_data(new_width * new_height);

        std::cout << "Image start cropping" << std::endl;

        for(int y = 0; y < new_height; ++y) {
            for(int x = 0; x < new_width; ++x) {
                new_data[y * new_width + x] = img_data[(y + height_image - new_height) * width_image + x];
            }
        }

        img_data = std::move(new_data);

        std::cout << "Image was cropped" << std::endl;

        img.SetSizes(new_width, new_height);
    };

    void GrayScaleImage(Image& img) {
        auto [width_image, height_image] = img.GetSize();

        for(int y = 0; y < height_image; ++y) {
            for(int x = 0; x < width_image; ++x) {
                Color cur_px = img.GetColor(x, y);
                float new_value = cur_px.red * 0.299 + cur_px.green * 0.587 + cur_px.blue * 0.114;
                img.SetColor(Color(new_value, new_value, new_value), x, y);
            }
        }
    };

    void Negative(Image& img) {
        auto [width_image, height_image] = img.GetSize();

        for(int y = 0; y < height_image; ++y) {
            for(int x = 0; x < width_image; ++x) {
                Color cur_px = img.GetColor(x, y);
                img.SetColor(Color(1 - cur_px.red, 1 - cur_px.green, 1 - cur_px.blue), x, y);
            }
        }
    };

    void Sharpening(Image& img) {
        std::cout << "Image start sharpening" << std::endl;
        SetPixelsFromMatrix_(img, sharpening_matr_);
        std::cout << "Image was sharped" << std::endl;
    };

    void EdgeDetection(Image& img, float threshold) {
        std::cout << "Image start edge detection" << std::endl;
        GrayScaleImage(img);
        SetPixelsFromMatrix_(img, edge_detection_matr_);

        auto [width_image, height_image] = img.GetSize();

        for(int y = 0; y < height_image; ++y) {
            for(int x = 0; x < width_image; ++x) {
                Color cur_px = img.GetColor(x, y);
                if ((cur_px.red + cur_px.green + cur_px.blue) / 3 > threshold) {
                    img.SetColor(Color(1, 1, 1), x, y);
                } else {
                    img.SetColor(Color(0, 0, 0), x, y);
                }

            }
        }

        std::cout << "Image was edge detected" << std::endl;
    };

    void Blur(Image& img) {
        std::cout << "Image start blur" << std::endl;

        std::cout << "Image was blured" << std::endl;
    }

private:
    std::vector<std::vector<int>> sharpening_matr_ = {
        {0, -1, 0},
        {-1, 5, -1},
        {0, -1, 0}
    };

    std::vector<std::vector<int>> edge_detection_matr_ = {
        {0, -1, 0},
        {-1, 4, -1},
        {0, -1, 0}
    };

    template <class T>
    void SetPixelsFromMatrix_(Image& img, std::vector<std::vector<T>>& matrix) {
        auto [width, height] = img.GetSize();

        Image img_copy = Image(img);
        std::vector<Color>& copy_data = img_copy.GetAllPixels();

        for(int y = 0; y < height; ++y) {

            // ... left bound

            if (y == 0) {
                Color cur_color = img_copy.GetColor(0, y);
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = img_copy.GetColor(0, 1);
                part[0][1] = img_copy.GetColor(0, 1);
                part[0][2] = img_copy.GetColor(1, 1);
                part[1][0] = cur_color;
                part[1][1] = cur_color;
                part[1][2] = img_copy.GetColor(1, 0);
                part[2][0] = cur_color;
                part[2][1] = cur_color;
                part[2][2] = img_copy.GetColor(1, 0);

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), 0, 0);
            } else if (y == height - 1) {
                Color cur_color = img_copy.GetColor(0, y);
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = cur_color;
                part[0][1] = cur_color;
                part[0][2] = img_copy.GetColor(1, y);
                part[1][0] = cur_color;
                part[1][1] = cur_color;
                part[1][2] = img_copy.GetColor(1, y);
                part[2][0] = img_copy.GetColor(0, y - 1);
                part[2][1] = img_copy.GetColor(0, y - 1);
                part[2][2] = img_copy.GetColor(1, y - 1);

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), 0, 0);
            } else {
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = img_copy.GetColor(0, y + 1);
                part[0][1] = img_copy.GetColor(0, y + 1);
                part[0][2] = img_copy.GetColor(1, y + 1);
                part[1][0] = img_copy.GetColor(0, y);
                part[1][1] = img_copy.GetColor(0, y);
                part[1][2] = img_copy.GetColor(1, y);
                part[2][0] = img_copy.GetColor(0, y - 1);
                part[2][1] = img_copy.GetColor(0, y - 1);
                part[2][2] = img_copy.GetColor(1, y - 1);

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), 0, 0);
            }

            if (y != 0 && y != height - 1) {
                for (int x = 1; x < width - 2; ++x) {
                    img_copy.SetColor(CountColorFromTwoMatrix_(img, x - 1, y - 1, x + 1, y + 1, matrix), x, y);  // need to write
                }
            }

            // ... right bound

            if (y == 0) {
                Color cur_color = img_copy.GetColor(width - 1, 0);
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = img_copy.GetColor(width - 2, 1);
                part[0][1] = img_copy.GetColor(width - 1, 1);
                part[0][2] = img_copy.GetColor(width - 1, 1);
                part[1][0] = img_copy.GetColor(width - 2, 0);
                part[1][1] = cur_color;
                part[1][2] = cur_color;
                part[2][0] = img_copy.GetColor(width - 2, 0);
                part[2][1] = cur_color;
                part[2][2] = cur_color;

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), width - 1, 0);
            } else if (y == height - 1) {
                Color cur_color = img_copy.GetColor(width - 1, y);
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = img_copy.GetColor(width - 2, y);
                part[0][1] = cur_color;
                part[0][2] = cur_color;
                part[1][0] = img_copy.GetColor(width - 2, y);
                part[1][1] = cur_color;
                part[1][2] = cur_color;
                part[2][0] = img_copy.GetColor(width - 2, y - 1);
                part[2][1] = img_copy.GetColor(width - 1, y - 1);
                part[2][2] = img_copy.GetColor(width - 1, y - 1);

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), width - 1, y);
            } else {
                std::vector<std::vector<Color>> part(3,std::vector<Color>(3));

                part[0][0] = img_copy.GetColor(width - 2, y + 1);
                part[0][1] = img_copy.GetColor(width - 1, y + 1);
                part[0][2] = img_copy.GetColor(width - 1, y + 1);
                part[1][0] = img_copy.GetColor(width - 2, y);
                part[1][1] = img_copy.GetColor(width - 1, y);
                part[1][2] = img_copy.GetColor(width - 1, y);
                part[2][0] = img_copy.GetColor(width - 2, y - 1);
                part[2][1] = img_copy.GetColor(width - 1, y - 1);
                part[2][2] = img_copy.GetColor(width - 1, y - 1);

                img_copy.SetColor(CountColorFromTwoMatrix_(part, matrix), 0, 0);
            }
        }

        std::vector<Color>& img_data = img.GetAllPixels();

        img_data = std::move(copy_data);
    }

    template <class T>
    Color CountColorFromTwoMatrix_(std::vector<std::vector<Color>>& matrix_data, std::vector<std::vector<T>>& matrix_coefs) const {
        float val_red = 0;
        float val_green = 0;
        float val_blue = 0;

        for(int i = 0; i < 3; ++i) {
            for(int g = 0; g < 3; ++g) {
                val_red += matrix_data[i][g].red * matrix_coefs[i][g];
                val_green += matrix_data[i][g].green * matrix_coefs[i][g];
                val_blue += matrix_data[i][g].blue * matrix_coefs[i][g];
            }
        }

        return Color(std::min(1.0f, std::max(0.0f, val_red)), std::min(1.0f, std::max(0.0f, val_green)), std::min(1.0f, std::max(0.0f, val_blue)));
    };

    template <class T>
    Color CountColorFromTwoMatrix_(Image& img, int x1, int y1, int x2, int y2, std::vector<std::vector<T>>& matrix_coefs) {
        float val_red = 0;
        float val_green = 0;
        float val_blue = 0;

        int cur_x = 0;
        int cur_y = 0;

        try {
            for (int y = y1; y <= y2; ++y) {
                for (int x = x1; x <= x2; ++x) {
                    Color cur_color = img.GetColor(x, y);

                    val_red += cur_color.red * matrix_coefs[cur_y][cur_x];
                    val_green += cur_color.green * matrix_coefs[cur_y][cur_x];
                    val_blue += cur_color.blue * matrix_coefs[cur_y][cur_x];

                    ++cur_x;
                }
                cur_x = 0;
                ++cur_y;
            }
        } catch (const char* c) {
            std::cerr << c << std::endl;
        }

        return Color(std::min(1.0f, std::max(0.0f, val_red)), std::min(1.0f, std::max(0.0f, val_green)), std::min(1.0f, std::max(0.0f, val_blue)));
    };

    std::vector<double> GetGuassianKernel_(const int kernel_size, double sigma) {
        std::setprecision(8);
        std::vector<double> kernel(kernel_size*kernel_size,0);

        if (sigma <=0 ){
            sigma = 0.3*((kernel_size-1)*0.5 - 1) + 0.8;
        }
        double r = 0.0;
        double s = 2.0 * sigma * sigma;

        // sum is for normalization
        double sum = 0.0;

        // generating nxn kernel
        double mean = kernel_size/2;
        for (int i=0 ; i<kernel_size ; i++) {
            for (int j=0 ; j<kernel_size ; j++) {
                kernel[(i*kernel_size)+j] =exp( -0.5 * (pow((i-mean)/sigma, 2.0) + pow((j-mean)/sigma,2.0)) )
                                                / (2 * M_PI * sigma * sigma);
                sum += kernel[(i*kernel_size)+j];
            }
        }

        // normalising the Kernel
        for (int i = 0; i < kernel.size(); ++i){
            kernel[i] /= sum;
        }

        return kernel;
    };

    void blur_gaussian(std::vector<Color> input, const int kernel_size, const double sigma) {
        std::vector<double> kernel = GetGuassianKernel_(kernel_size, sigma);

        unsigned char* data_in = (unsigned char*)(input);
        unsigned char* data_out = (unsigned char*)(output.data);

        for (int row = 0; row < input.rows; row++) {
            for (int col = 0; col < (input.cols * input.channels()); col += input.channels()) {
                for (int channel_index = 0; channel_index < input.channels(); channel_index++) {

                    if (row <= kernel_size / 2 || row >= input.rows - kernel_size / 2 ||
                        (input.cols * input.channels()) <= kernel_size / 2 ||
                        col >= (input.cols * input.channels()) - kernel_size / 2) {
                        data_out[output.step * row + col + channel_index] =
                            data_in[output.step * row + col + channel_index];
                        continue;
                    }

                    int k_ind = 0;
                    double sum = 0;
                    for (int k_row = -kernel_size / 2; k_row <= kernel_size / 2; ++k_row) {
                        for (int k_col = -kernel_size / 2; k_col <= kernel_size / 2; ++k_col) {
                            // The operation should be done on images with range [0,1] so we convert the pixel value
                            // from [0-255] to [0-1] before the operation.
                            sum += kernel[k_ind] * (data_in[input.step * (row + k_row) + col +
                                                            (k_col * input.channels()) + channel_index] /
                                                    255.0);
                            k_ind++;
                        }
                    }
                    // Do not forget to convert back to [0-255] by multiplying the pixel value by 255.
                    data_out[output.step * row + col + channel_index] =
                        (unsigned int)(std::max(std::min(sum, 1.0), 0.0) * 255.0);
                }
            }
        }
    };
};