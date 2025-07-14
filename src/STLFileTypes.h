#ifndef STLREPAIR_STLFILETYPES__H_
#define STLREPAIR_STLFILETYPES__H_

#include <string>
#include <cstdint>

constexpr const int BINARY_STL_HEADER_SIZE_IN_BYTES = 80;
constexpr const int BINARY_STL_TRIANGLE_COUNT_IN_BYTES = 4;
constexpr const int BINARY_STL_TRIANGLE_SIZE_IN_BYTES = 48;
constexpr const int BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES = 2;

// This assumes a valid STL has at least ONE triangle.
constexpr const int MINIMUM_BINARY_STL_SIZE_IN_BYTES =
    BINARY_STL_HEADER_SIZE_IN_BYTES +
    BINARY_STL_TRIANGLE_COUNT_IN_BYTES +
    BINARY_STL_TRIANGLE_SIZE_IN_BYTES +
    BINARY_STL_TRIANGLE_ATTRIBUTE_BYTE_COUNT_IN_BYTES;

enum class STLFileType
{
    UNKNOWN,
    ASCII,
    BINARY
};

using STLBinaryHeader = std::array<uint8_t, BINARY_STL_HEADER_SIZE_IN_BYTES>;
using STLBinaryTriangleData = std::array<uint8_t, BINARY_STL_TRIANGLE_SIZE_IN_BYTES>;

/**
 * Returns a string representatio of STLFileType.
 */
std::string toString(const STLFileType fileType);

/**
 * Attempts to heuristically determine an STL file's type.
 *
 * @throws std::runtime_error
 */
STLFileType determineFileType(const std::string& stlFilePath);

#endif
