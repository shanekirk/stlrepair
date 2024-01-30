#include "FileUtils.h"
#include "Contracts.h"

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

}
