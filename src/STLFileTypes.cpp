#include "STLFileTypes.h"
#include "FileUtils.h"
#include "CallGuard.h"

#include <stdexcept>

constexpr const int BINARY_STL_HEADER_SIZE_IN_BYTES = 80;
constexpr const int BINARY_STL_TRIANGLE_COUNT_IN_BYTES = 4;
constexpr const int BINARY_STL_TRIANGLE_SIZE_IN_BYTES = 50;

constexpr const int MINIMUM_BINARY_STL_SIZE_IN_BYTES =
    BINARY_STL_HEADER_SIZE_IN_BYTES +
    BINARY_STL_TRIANGLE_COUNT_IN_BYTES +
    BINARY_STL_TRIANGLE_SIZE_IN_BYTES;

/**
 * @since 2024 Jan 21
 */
std::string toString(const STLFileType fileType)
{
    switch (fileType)
    {
    case STLFileType::ASCII:
        return "ASCII";
    case STLFileType::BINARY:
        return "BINARY";
    default:
        return "UNKNOWN";
    }
}

/**
 * @since 2024 Jan 21
 */
STLFileType determineFileType(const std::string& stlFilePath)
{
    if (stlFilePath.empty())
        throw std::runtime_error("STL path cannot be empty.");

    if (!FileUtils::fileExists(stlFilePath))
        throw std::runtime_error("Specified STL file does not exist - " + stlFilePath);

    auto fileSize = FileUtils::getFileSize(stlFilePath);

    FILE *pFile = fopen(stlFilePath.c_str(), "rb");
    if (!pFile)
        throw std::runtime_error("Unknown error when opening " + stlFilePath);

    auto closeGuard = makeCallGuard([&]() { fclose(pFile); });

    char buffer[6];
    memset(&buffer[0], 0, sizeof(buffer));
    size_t bytesRead = fread(&buffer[0], 1, sizeof(buffer) - 1, pFile);

    if (bytesRead != (sizeof(buffer) - 1))
        return STLFileType::UNKNOWN; // Way too small to be an STL file.

    if (strcmp(buffer, "solid") == 0)
        return STLFileType::ASCII;   // Most likely candidate, unless the exporter did something weird.

    if (fileSize < MINIMUM_BINARY_STL_SIZE_IN_BYTES)
        return STLFileType::UNKNOWN; // Too small to be a binary STL file.

    return STLFileType::BINARY;      // Only option left.
}
