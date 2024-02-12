#include "FileUtils.h"
#include "Version.h"
#include "RepairOptionPrompts.h"
#include "STLFileTypes.h"
#include "BinarySTLFileReader.h"
#include "BinarySTLFileFilter.h"

#include <iostream>

/**
 * main()
 */
int main(int argc, const char** argv)
{
    std::cout << "stlrepair " << getVersionString() << " - An STL repair tool.\n"
        "Copyright(C) 2024, Shane Kirk\n"
        "Visit http://www.shanekirk.com for more info.\n" << std::endl;

    if (argc < 2)
    {
        std::cout << "usage: stlrepair <file.stl>" << std::endl;
        return 1;
    }

    const std::string inputFile = argv[1];

    if (!FileUtils::fileExists(inputFile))
    {
        std::cerr << "Specified file (" << inputFile << ") does not exist.\n";
        return 1;
    }

    try
    {
        if (determineFileType(inputFile) == STLFileType::ASCII)
        {
            if (!promptShouldTreatASCIIModeAsBinary())
            {
                std::cout << "Exiting\n";
                return 0;
            }
        }

        if (FileUtils::getFileSize(inputFile) < MINIMUM_BINARY_STL_SIZE_IN_BYTES)
            throw std::runtime_error("Specified file too small to be a binary STL - " + inputFile);


        std::string newFile = FileUtils::generateUniqueFilePath(inputFile);
        BinarySTLFileFilter filter(newFile);

        if (promptClearFileHeader())
            filter.m_zeroOutHeader = true;

        if (promptClearFacetAttributeCounts())
            filter.m_zeroAttributeByteCounts = true;

        uint32_t triangleCountRead = readTriangleCount(inputFile);
        uint32_t triangleCountCalc = calculateTriangleCount(inputFile);

        if (triangleCountRead > triangleCountCalc)
        {
            if (promptTriangleCountTooBig())
            {
                filter.m_updateTriangleCount = true;
                filter.m_triangleLimit = triangleCountCalc;
                filter.m_clearExtraFileData = true;
            }
        }

        if (hasExtraData(inputFile))
        {
            if (promptTruncateExtraData())
                filter.m_clearExtraFileData = true;
        }

        std::cout << "Generating new STL - " << newFile << "\n";
        BinarySTLFileReader reader(inputFile);
        reader.readFile(filter);

        std::cout << "Done.\n";
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << "\n";
        return 1;
    }

    return 0;
}
