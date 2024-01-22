#ifndef STLREPAIR_VERSION__H
#define STLREPAIR_VERSION__H

#include <string>

constexpr const int VERSION_MAJOR = 0;
constexpr const int VERSION_MINOR = 1;
constexpr const int VERSION_BUILD = 0;

inline std::string getVersionString()
{
    return std::to_string(VERSION_MAJOR) + "." + 
           std::to_string(VERSION_MINOR) + "." + 
           std::to_string(VERSION_BUILD);
}

#endif