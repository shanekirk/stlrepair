#include "FileUtils.h"
#include "Contracts.h"
#include "CallGuard.h"

#include <filesystem>

namespace FileUtils
{

/**
 * @since 2024 Jan 22
 */
void splitPath(const std::string& fullpath, std::string& dir,
    std::string& basename, std::string& extension)
{
    precondition_ret(!fullpath.empty(), );

    auto separatorPos = fullpath.find_last_of("\\/:");
    std::string filename;
    if (separatorPos != std::string::npos)
    {
        dir = fullpath.substr(0, separatorPos + 1);
        filename = fullpath.substr(separatorPos + 1);
    }
    else
    {
        dir = "";
        filename = fullpath;
    }

    auto dotPos = filename.find_last_of('.');
    if (dotPos != std::string::npos)
    {
        basename = filename.substr(0, dotPos);
        extension = filename.substr(dotPos + 1);
    }
    else
    {
        basename = filename;
        extension = "";
    }
}

/**
 * @since 2024 Jan 30
 */
std::string generateUniqueFilePath(const std::string& filepathTemplate)
{
    precondition_ret(!filepathTemplate.empty(), "");

    if (!fileExists(filepathTemplate))
        return filepathTemplate; // Job well done!
    
    std::string dir, base, ext;
    splitPath(filepathTemplate, dir, base, ext);

    int count = 1;
    std::string newFilePath;
    do
    {
        newFilePath = dir + base + "(" + std::to_string(count) + ")." + ext;
        ++count;

    } while (fileExists(newFilePath));

    return newFilePath;
}

/**
 * @since 2024 Feb 08
 */
bool areFilesEqual(const std::string& filepath1, const std::string& filepath2,
    std::uintmax_t startByte, std::uintmax_t endByte)
{
    if (filepath1.empty() || filepath2.empty())
        return false;

    if (endByte < startByte)
        return false;

    FILE* pFile1 = fopen(filepath1.c_str(), "rb");
    if (!pFile1)
        return false;
    auto file1Guard = makeCallGuard([&]() { fclose(pFile1); });

    FILE* pFile2 = fopen(filepath2.c_str(), "rb");
    if (!pFile2)
        return false;
    auto file2Guard = makeCallGuard([&]() { fclose(pFile2); });

    fseek(pFile1, static_cast<long>(startByte), SEEK_SET);
    fseek(pFile2, static_cast<long>(startByte), SEEK_SET);

    std::uintmax_t totalBytesToCompare = endByte - startByte;

    if (endByte == startByte)
        totalBytesToCompare = 1;

    do
    {
        char buffer1[1024] = { 0 };
        char buffer2[1024] = { 0 };

        auto bytesRead1 = fread(&buffer1[0], 1, std::min(sizeof(buffer1), totalBytesToCompare), pFile1);
        auto bytesRead2 = fread(&buffer2[0], 1, std::min(sizeof(buffer2), totalBytesToCompare), pFile2);

        if (bytesRead1 != bytesRead2)
            return false;

        if (bytesRead1 == 0)
            return true; // Nothing left to read. Must be equal.

        if (memcmp(&buffer1[0], &buffer2[0], bytesRead1) != 0)
            return false;

        totalBytesToCompare -= bytesRead1;

    } while (totalBytesToCompare > 0);

    return true;
}

/**
 * @since 2024 Feb 08
 */
bool areFilesEqual(const std::string& filepath1, const std::string& filepath2)
{
    if (filepath1.empty() || filepath2.empty())
        return false;

    std::filesystem::path p1(filepath1);
    std::filesystem::path p2(filepath2);

    if (!std::filesystem::exists(p1) || !std::filesystem::exists(p2))
        return false;

    auto fileSize1 = std::filesystem::file_size(p1);
    auto fileSize2 = std::filesystem::file_size(p2);

    if (fileSize1 != fileSize2)
        return false;

    return areFilesEqual(filepath1, filepath2, 0, fileSize1);
}

}
