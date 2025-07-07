#include <utility/array_2d.hpp>
#include <utility/challenge_runner.hpp>
#include <utility/text_helpers.hpp>
#include <string_view>

REGISTER_CHALLENGE(Y2019_C8, "input/Y2019/C8.txt")
{
    constexpr size_t WIDTH = 25;
    constexpr size_t HEIGHT = 6;

    std::vector<Array2D<char>> layers;

    if (auto stream = TextHelpers::OpenFileReadStream(input))
    {
        auto line = TextHelpers::StreamToString(stream.value());

        size_t size = WIDTH * HEIGHT;

        for (size_t i = 0; i < line.size(); i += size)
        {
            Array2D<char> layer { WIDTH, HEIGHT, -1 };
            std::memcpy(&layer.at(0, 0), &line.at(i), size);
            layers.emplace_back(std::move(layer));
        }
    }

    int64_t answer1 {};

    {
        Array2D<char>* lowest_layer = nullptr;
        size_t lowest_zero_count = -1;

        for (auto& layer : layers)
        {
            size_t zero_count = 0;
            for (auto c : layer)
            {
                if (c == '0')
                {
                    zero_count++;
                }
            }

            if (zero_count < lowest_zero_count)
            {
                lowest_zero_count = zero_count;
                lowest_layer = &layer;
            }
        }

        size_t ones = 0;
        size_t twos = 0;

        for (auto c : *lowest_layer)
        {
            if (c == '1')
                ones++;
            if (c == '2')
                twos++;
        }

        answer1 = ones * twos;
    }

    {
        Array2D<char> final_image { WIDTH, HEIGHT, 2 };

        for (auto rev_it = layers.rbegin(); rev_it != layers.rend(); ++rev_it)
        {
            auto& layer = *rev_it;

            for (auto it = layer.begin(); it != layer.end(); ++it)
            {
                auto c = *it;
                auto [x, y] = it.getPosition();

                if (c == '2')
                {
                    continue;
                }
                else
                {
                    final_image.at(x, y) = c;
                }
            }
        }

        for (auto& c : final_image)
        {
            if (c == '0')
                c = '.';
            if (c == '1')
                c = '#';
        }

        for (size_t j = 0; j < final_image.getHeight(); ++j)
        {
            std::string_view row_string = { &final_image.at(0, j), final_image.getWidth() };
            // std::cout << row_string << "\n";
        }
    }

    return { answer1, 0 };
}