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
std::uintmax_t getFileSize(const std::string& pathToFile);

/**
 * Returns true if the specified file exists.
 */
bool fileExists(const std::string& pathToFile);

/**
 * Splits a filepath into separate components.
 */
void splitPath(const std::string& fullpath, std::string& dir,
    std::string& basename, std::string& extension);

/**
 * Generates a new filename that doesn't exist, using a base filepath as a
 * template. It basically appends an increasing number to the end of the
 * filename.
 *
 * Example: Given c:\path.txt
 *
 * It will attempt to generate a filename like...
 *
 * c:\path(1).txt
 * c:\path(2).txt
 * ...
 *
 * ...until it finds a file that doesn't exist.
 */
std::string generateUniqueFilePath(const std::string& filepathTemplate);

/**
 * Returns true if the specified byte range matches between filepath1 and filepath2.
 */
bool areFilesEqual(const std::string& filepath1, const std::string& filepath2,
    std::uintmax_t startByte, std::uintmax_t endByte);

/**
 * Returns true if the specified files have identical content.
 */
bool areFilesEqual(const std::string& filepath1, const std::string& filepath2);

}

#endif
