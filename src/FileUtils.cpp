#include "FileUtils.h"

namespace FileUtils
{

/**
 * @since 2024 Jan 22
 */
void splitPath(const std::string& fullpath, std::string& dir,
    std::string& basename, std::string& extension)
{
    if (fullpath.empty())
        return;

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

}