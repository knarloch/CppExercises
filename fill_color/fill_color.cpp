#include <vector>
#include <cstdint>
#include <stack>
#include <limits>
#include <iostream>

using namespace std;

struct Image {
public:
    Image(size_t w, size_t h) : width{w}, height{h} {};

    uint32_t &at(size_t x, size_t y) {
        return data[y * width + x];
    }

    const uint32_t &at(size_t x, size_t y) const {
        return data[y * width + x];
    }


    uint32_t &at(pair<size_t, size_t> point) {
        auto [x, y] = point;
        return data[y * width + x];
    }

    size_t GetHeight() const {
        return height;
    }

    size_t GetWidth() const {
        return width;
    }

    void Print(ostream &str) const {
        for (auto h = 0u; h < height; h++) {
            for (auto w = 0u; w < width; w++) {
                std::cout << static_cast<char>(this->at(w, h));
            }
            str << "\n";
        }
    }

private:
    size_t width{};
    size_t height{};
    std::vector<uint32_t> data = std::vector<uint32_t>(width * height, '.');

};

void FillColour(Image &image, size_t x, size_t y, uint32_t new_color) {
    if(x >= image.GetWidth() or y >= image.GetHeight()){
        return;
    }
    auto old_color = image.at(x, y);
    std::stack<std::pair<size_t, size_t>> to_process;
    to_process.push({x, y});
    while (not to_process.empty()) {
        auto point = to_process.top();
        to_process.pop();
        if (image.at(point) == old_color) {
            image.at(point) = new_color;
            if (point.second > 0) {
                to_process.push({point.first, point.second - 1});
            }
            if (point.second < image.GetHeight()-1) {
                to_process.push({point.first, point.second + 1});
            }
            if (point.first > 0) {
                to_process.push({point.first - 1, point.second});
            }
            if (point.first < image.GetWidth()-1) {
                to_process.push({point.first + 1, point.second});
            }
        }
    }
}

int main() {
    {
        std::cout<<"image 0\n";
        Image img(0, 0);
        img.Print(std::cout);

        FillColour(img, 0,0, 'x');
        img.Print(std::cout);

    }
    {
        std::cout<<"image 1\n";
        Image img(100, 20);

        img.Print(std::cout);

        img.at(3, 2) = '#';
        img.at(4, 2) = '#';
        img.at(5, 2) = '#';
        img.at(6, 2) = '#';

        img.at(3, 7) = '#';
        img.at(4, 7) = '#';
        img.at(5, 7) = '#';
        img.at(6, 7) = '#';


        img.at(3, 2) = '#';
        img.at(3, 3) = '#';
        img.at(3, 4) = '#';
        img.at(3, 5) = '#';
        img.at(3, 6) = '#';
        img.at(3, 7) = '#';

        img.at(6, 2) = '#';
        img.at(6, 3) = '#';
        img.at(6, 4) = '#';
        img.at(6, 5) = '#';
        img.at(6, 6) = '#';
        img.at(6, 7) = '#';

        std::cout << "frame\n";
        img.Print(std::cout);
        FillColour(img, 5, 5, '_');
        std::cout << "filled\n";
        img.Print(std::cout);

        FillColour(img, 3, 2, '@');
        std::cout << "filled_frame\n";
        img.Print(std::cout);
    }

    {
        std::cout<<"image 2\n";

        Image img(10, 2);

        img.Print(std::cout);

        img.at(0, 1) = '#';
        img.at(1, 1) = '#';
        img.at(2, 1) = '#';

        img.at(2, 0) = '*';

        std::cout << "frame\n";
        img.Print(std::cout);
        FillColour(img, 0, 0, '_');
        std::cout << "filled\n";
        img.Print(std::cout);

        FillColour(img, 2, 0, '@');
        std::cout << "filled_frame\n";
        img.Print(std::cout);
    }

    {
        std::cout<<"image 3\n";

        Image img(10, 10);

        img.Print(std::cout);

        FillColour(img, 0, 0, '_');
        std::cout << "filled\n";
        img.Print(std::cout);
    }

    {
        std::cout<<"image 4\n";

        Image img(10, 10);

        img.Print(std::cout);

        img.at(4, 5) = '#';
        img.at(4, 6) = '#';
        img.at(5, 4) = '#';
        img.at(6, 4) = '#';

        img.at(2, 0) = '*';

        std::cout << "frame\n";
        img.Print(std::cout);
        FillColour(img, 0, 0, '_');
        std::cout << "filled\n";
        img.Print(std::cout);

        FillColour(img, 2, 0, '@');
        std::cout << "filled_frame\n";
        img.Print(std::cout);
    }

    {
        std::cout<<"image 5\n";

        Image img(10, 10);

        img.Print(std::cout);

        img.at(4, 5) = '#';
        img.at(6, 5) = '#';
        img.at(5, 4) = '#';
        img.at(5, 6) = '#';


        std::cout << "frame\n";
        img.Print(std::cout);
        FillColour(img, 5, 5, '_');
        std::cout << "filled\n";
        img.Print(std::cout);
    }

    return 0;
}
