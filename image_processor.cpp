#include "image.h"
#include "filters_list.h"
#include "filter_controller.h"

int main(int args, char* argv[]) {
    try {
        FiltersList list_of_filters;
        Image image;
        std::vector<char*> params_list;

        for(int i = 3; i < args; ++i) {
            params_list.push_back(argv[i]);
        }

        image.LoadImage(argv[1]);

        if (args > 2) {
            StartFilteringController(list_of_filters, image, args - 3, params_list);
        }

        std::cout << "loading start";

        image.ExportImage(argv[2]);
    } catch (const char* c) {
        std::cerr << c << std::endl;
    }

    return 0;
}
