#pragma once

class FilterController {
public:
    FilterController(FiltersList filter, Image& img, int args, std::vector<char*> argv) : filter_list_(filter), img_(img), count_(args), filters_params_(argv) {};
    
    void StartFilter() {
        try {
            for(int i = 0; i < count_; ++i) {
                if (filters_params_[i][0] == '-') {
                    std::string s = "";
                    int g = 1;

                    while(filters_params_[i][g] != '\0') {
                        s += filters_params_[i][g];
                        ++g;
                    }

                    if (s == "crop") {
                        if (i + 2 < count_) {
                            int cropping_width = std::atoi(filters_params_[i + 1]);
                            int cropping_height = std::atoi(filters_params_[i + 2]);

                            if ((cropping_width == 0 && filters_params_[i + 2][0] != '0') || (cropping_height == 0 && filters_params_[i + 1][0] != '0')) {
                                throw "Sizes of cropped img_ must be numbers";
                            }

                            filter_list_.CroppingImage(img_, cropping_width, cropping_height);

                            i += 2;
                        } else {
                            throw "Not enough arguments to call -crop";
                        }
                    } else if (s == "gs") {
                        filter_list_.GrayScaleImage(img_);
                    } else if (s == "neg") {
                        filter_list_.Negative(img_);
                    } else if (s == "sharp") {
                        filter_list_.Sharpening(img_);
                    } else if (s == "edge") {
                        if (i + 1 < count_) {
                            float threshold = std::atof(filters_params_[i + 1]);
                            filter_list_.EdgeDetection(img_, threshold);
                        } else {
                            throw "Not enough arguments to use Edge detecion";
                        }
                    } else if (s == "blur") {
                        filter_list_.Blur(img_);
                    }
                }
            }
        } catch (const char* c) {
            std::cerr << c << std::endl;
        }
    };

private:
    FiltersList filter_list_;
    Image& img_;
    int count_;
    std::vector<char*> filters_params_;
};

void StartFilteringController(FiltersList filter, Image& img, int args, std::vector<char*> argv) {
    FilterController filter_controller(filter, img, args, argv);
    filter_controller.StartFilter();
}