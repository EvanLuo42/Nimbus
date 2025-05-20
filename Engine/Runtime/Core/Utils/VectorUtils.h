#pragma once
#include <sstream>
#include <string>

template <typename T>
std::string JoinVector(const std::vector<T> &vec, const std::string& sep = ", ") {
    std::ostringstream oss;
    for (size_t i = 0; i < vec.size(); ++i) {
        oss << vec[i];
        if (i + 1 < vec.size()) oss << sep;
    }
    return oss.str();
}
