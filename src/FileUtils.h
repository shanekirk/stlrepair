#ifndef STLREPAIR_FILEUTILS__H_
#define STLREPAIR_FILEUTILS__H_

#include <cstdint>
#include <string>

/**
 * General purpose file utility functions.
 */
namespace FileUtils
{

/**
 * Returns the size of the specified file.
 */
uint32_t getFileSize(const std::string& pathToFile);

/**
 * Returns true if the specified file exists.
 */
bool fileExists(const std::string& pathToFile);

/**
 * Returns the character to use as the platform path separator.
 */
char getPathSeparatorChar();

/**
 * Splits a filepath into separate components.
 */
void splitPath(const std::string& fullpath, std::string& dir, 
    std::string& basename, std::string& extension);
}

#endif
