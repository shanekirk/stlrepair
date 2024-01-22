#include "FileUtils.h"
#include "CallGuard.h"

#include <shlwapi.h>

namespace FileUtils
{

/**
 * @since 2024 Jan 21
 */
uint32_t getFileSize(const std::string& pathToFile)
{
    if (pathToFile.empty())
        return 0;

    HANDLE hFile = ::CreateFileA(pathToFile.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, 
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

    if (hFile == INVALID_HANDLE_VALUE)
        return 0;

    auto closeGuard = makeCallGuard([&]() { ::CloseHandle(hFile); });
    uint32_t fileSize = GetFileSize(hFile, NULL);
    return fileSize;
}

/**
 * @since 2024 Jan 21
 */
bool fileExists(const std::string& pathToFile)
{
    if (pathToFile.empty())
        return false;

    return ::PathFileExistsA(pathToFile.c_str()) != FALSE;
}

/**
 * @since 2024 Jan 22
 */
char getPathSeparatorChar()
{
    return '\\';
}

}