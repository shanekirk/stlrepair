#include "FileUtils.h"
#include "Version.h"
#include "RepairOptionPrompts.h"
#include "STLFileTypes.h"
#include "BinarySTLFileReader.h"
#include "BinarySTLFileFilter.h"

#include <iostream>

/**
 * Prints out command line options.
 */
void printHelp()
{
    std::cout << "usage: stlrepair <file.stl>" << std::endl;
}

/**
 * Options for the repair. We prompt the user for each one. At some point
 * it might be useful to turn these into command line options.
 */
struct RepairOpts
{
    bool clearHeader;
    bool zeroOutFacetAttributeCount;

    enum class FacetMismatchAction
    {
        TruncateFacets,
        UpdateFileFacetCount,
        DoNothing
    };

    FacetMismatchAction mismatchFacetAction;

    // Ctor
    RepairOpts() : clearHeader(false),
        zeroOutFacetAttributeCount(false),
        mismatchFacetAction(FacetMismatchAction::DoNothing)
    {
    }
};

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
        printHelp();
        return 1;
    }

    if (!FileUtils::fileExists(argv[1]))
    {
        std::cerr << "Specified file (" << argv[1] << ") does not exist.\n";
        return 1;
    }

    try
    {
        if (determineFileType(argv[1]) == STLFileType::ASCII)
        {
            if (!promptShouldTreatASCIIModeAsBinary())
            {
                std::cout << "Exiting";
                return 0;
            }
        }

        std::string newFile = FileUtils::generateUniqueFilePath(argv[1]);
        BinarySTLFileFilter filter(newFile);
        BinarySTLFileReader reader(argv[1]);
        reader.readFile(filter);


//        RepairOpts repairOpts;
//        repairOpts.clearHeader = promptClearFileHeader();
//        repairOpts.zeroOutFacetAttributeCount = promptClearFacetAttributeCounts();

//        STLFileReader::parseFile(argv[1]);
    }
    catch (const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
