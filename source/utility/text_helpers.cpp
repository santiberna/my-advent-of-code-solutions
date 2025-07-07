#include <utility/text_helpers.hpp>
#include <utility/array_2D.hpp>
#include <cstddef>
#include <string_view>

std::optional<std::ifstream> TextHelpers::OpenFileReadStream(const std::filesystem::path& path, std::ios_base::openmode flags)
{
    std::ifstream s = std::ifstream(path.string(), flags);
    if (s)
        return s;
    else
        return std::nullopt;
}

std::vector<std::string> TextHelpers::SplitStreamIntoLines(std::istream& stream, char delimiter)
{
    std::vector<std::string> out {};
    std::string temp {};

    while (std::getline(stream, temp, delimiter))
    {
        out.emplace_back(std::move(temp));
    }

    return out;
}

std::vector<std::string> TextHelpers::IntoWords(const std::string& str)
{
    std::istringstream input { str };
    return SplitStreamIntoLines(input, ' ');
}

std::string TextHelpers::ReplaceAll(std::string str, const std::string& search, const std::string& replace)
{
    size_t pos = 0;
    while ((pos = str.find(search, pos)) != std::string::npos)
    {
        str.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return str;
}

void TextHelpers::ForeachLine(std::istream& stream, std::function<void(const std::string& line)> fn, char delimiter)
{
    std::string temp {};
    while (std::getline(stream, temp, delimiter))
    {
        fn(temp);
    }
}

std::string TextHelpers::StreamToString(std::istream& stream)
{
    std::string out;
    auto current = stream.tellg();

    stream.seekg(0, std::ios_base::end);
    auto size = stream.tellg() - current;
    stream.seekg(current);

    out.resize(size);
    stream.read(out.data(), size);

    return out;
}

std::vector<std::string> TextHelpers::SplitString(std::string_view str, std::string_view delimiter)
{
    std::vector<std::string> out;
    std::string temp = std::string(str);
    size_t it = 0;

    while ((it = temp.find(delimiter)) != std::string::npos)
    {
        out.emplace_back(temp.substr(0, it));
        temp.erase(0, it + delimiter.length());
    }

    out.emplace_back(std::move(temp));

    return out;
}

template <typename T>
std::optional<std::pair<T, ptrdiff_t>> TextHelpers::ParseNumber(std::string_view str)
{
    T val {};
    auto result = std::from_chars(str.data(), str.data() + str.size(), val);
    if (result.ec == std::errc {})
    {
        ptrdiff_t diff = result.ptr - str.data();
        return std::make_pair(val, diff);
    }
    else
    {
        return std::nullopt;
    }
}

Array2D<char> TextHelpers::StreamToGrid(std::istream& stream)
{
    auto lines = SplitStreamIntoLines(stream);

    size_t width = lines.front().size();
    size_t height = lines.size();

    Array2D<char> out { width, height };

    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            out.at(i, j) = lines[j][i];
        }
    }

    return out;
}

#define INSTANTIATE_PARSE_NUMBER(T) \
    template std::optional<std::pair<T, ptrdiff_t>> TextHelpers::ParseNumber<T>(std::string_view str)

INSTANTIATE_PARSE_NUMBER(double);
INSTANTIATE_PARSE_NUMBER(float);
INSTANTIATE_PARSE_NUMBER(int16_t);
INSTANTIATE_PARSE_NUMBER(uint16_t);
INSTANTIATE_PARSE_NUMBER(int32_t);
INSTANTIATE_PARSE_NUMBER(uint32_t);
INSTANTIATE_PARSE_NUMBER(int64_t);
INSTANTIATE_PARSE_NUMBER(uint64_t);

template <typename T>
std::vector<T> TextHelpers::ParseAllNumbers(const std::string& str)
{
    std::vector<T> out {};

    auto end_it = str.end();
    for (auto it = str.begin(); it != end_it; ++it)
    {
        if (std::isdigit(*it) || *it == '-')
        {
            std::string_view remaining_view = { it, end_it };
            if (auto parsed_num = ParseNumber<T>(remaining_view))
            {
                it += parsed_num.value().second;
                out.emplace_back(parsed_num.value().first);
            }
        }

        if (it == end_it)
            break;
    }

    return out;
}

#define INSTANTIATE_PARSE_ALL_NUMBERS(T) \
    template std::vector<T> TextHelpers::ParseAllNumbers<T>(const std::string& str)

INSTANTIATE_PARSE_ALL_NUMBERS(double);
INSTANTIATE_PARSE_ALL_NUMBERS(float);
INSTANTIATE_PARSE_ALL_NUMBERS(int16_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint16_t);
INSTANTIATE_PARSE_ALL_NUMBERS(int32_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint32_t);
INSTANTIATE_PARSE_ALL_NUMBERS(int64_t);
INSTANTIATE_PARSE_ALL_NUMBERS(uint64_t);