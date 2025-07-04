#pragma once
#include <utility/array_2d.hpp>
#include <istream>
#include <optional>
#include <filesystem>
#include <fstream>
#include <string>
#include <functional>

namespace TextHelpers
{

std::optional<std::ifstream> OpenFileReadStream(
    const std::filesystem::path& path, std::ios_base::openmode flags = {});

std::vector<std::string> SplitStreamIntoLines(
    std::istream& stream, char delimiter = '\n');

std::string StreamToString(
    std::istream& stream);

std::vector<std::string> IntoWords(const std::string& str);

std::string ReplaceAll(std::string str, const std::string& search, const std::string& replace);

void ForeachLine(
    std::istream& stream, std::function<void(const std::string&)> fn, char delimiter = '\n');

std::vector<std::string> SplitString(
    std::string_view str, std::string_view delimiter = " ");

template <typename T>
std::optional<std::pair<T, ptrdiff_t>> ParseNumber(std::string_view str);

template <typename T>
std::vector<T> ParseAllNumbers(const std::string& str);

Array2D<char> StreamToGrid(std::istream& stream);
}