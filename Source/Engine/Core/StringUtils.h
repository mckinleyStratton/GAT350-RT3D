#pragma once

#include <string>

namespace nc
{
    class StringUtils
    {
    public:
        static std::string ToUpper(const std::string& str);
        static std::string ToLower(const std::string& str);
        static bool IsEqualIgnoreCase(const std::string& str1, const std::string& str2);
        static std::string CreateUnique(const std::string& str);

    private:
        static uint32_t unique;
    };
}
