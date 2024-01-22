#ifndef STLREPAIR_STLFILETYPES__H_
#define STLREPAIR_STLFILETYPES__H_

#include <string>

enum class STLFileType
{
    UNKNOWN,
    ASCII,
    BINARY
};

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
