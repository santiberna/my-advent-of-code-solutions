#pragma once
#include <iostream>
#include <format>
#include <string>
#include <utility>

namespace Log
{

template <typename... Args>
std::string Format(std::string_view str, Args&&... args)
{
    return std::vformat(str, std::make_format_args(args...));
}

template <typename... Args>
void Debug(std::string_view str, Args&&... args)
{
    std::cerr << Format(str, std::forward<Args>(args)...) << std::endl;
}

}