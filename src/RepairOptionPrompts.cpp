#include "RepairOptionPrompts.h"

#include <iostream>
#include <string>

namespace
{
    bool promptUser(const std::string& question, const std::string& help)
    {
        std::string input;
        do
        {
            std::cout << question;
            std::getline(std::cin, input);
            if ((input == "y") || (input == "Y"))
                return true;

            if (input == "?")
            {
                std::cout << help;
            }
        } while (input == "?");

        return ((input == "y") || (input == "Y"));
    }
}

/**
 * @since 2024 Jan 21
 */
bool promptShouldTreatASCIIModeAsBinary()
{
    const char *pszHelp =
        "This file appears to be an ASCII-mode STL. ASCII-mode repairs are currently\n"
        "unsupported. But it's possible a misbehaving exporter produced a Binary-mode\n"
        "STL that merely looks like an ASCII-mode file. Would you like to treat this\n"
        "as a Binary-mode STL?\n\n"
        "Note that treating a real ASCII-mode STL as a Binary-mode STL will most\n"
        "certainly result in a file even more broken than the original.\n\n";

    return promptUser("Treat as Binary-mode STL (y/N/?)? ", pszHelp);
}

/**
 * @since 2024 Jan 21
 */
bool promptClearFileHeader()
{
    const char *pszHelp =
        "Exporters sometimes include color information and others bits and bobs within\n"
        "the file's header. Many importers ignore this data altogether. But others can\n"
        "be confused by it, depending on the content. It's usually safe to clear it\n"
        "out entirely.\n\n";

    return promptUser("Clear the header (y/N/?)? ", pszHelp);
}

/**
 * @since 2024 Jan 21
 */
bool promptClearFacetAttributeCounts()
{
    const char *pszHelp =
        "Exporters sometimes include information in an area of each facet, known as\n"
        "the attribute count. When this happens, it can bewilder some importers.\n"
        "This information is usually safe to zero out.\n\n";

    return promptUser("Clear the attribute counts (y/N/?)? ", pszHelp);
}

/**
 * @since 2024 Feb 09
 */
bool promptTriangleCountTooBig()
{
    const char* pszHelp =
        "The given file appears to have fewer triangles than that reported by\n"
        "the file. This could happen if the file was interrupted during a download.\n"
        "It could also happen that triangle count portion of the file is corrupt.\n"
        "In any case, there's a possibility of lost geometry that we simply can't\n"
        "recover.";

    return promptUser("Sync the internal triangle count (y/N/?)? ", pszHelp);
}

/**
 * @since 2024 Feb 11
 */
bool promptTruncateExtraData()
{
    const char* pszHelp =
        "The given file appears to have non-triangle data on the end. This could be\n"
        "due to a misbehaving exporter. This is a non-standard practice. Some importers\n"
        "don't know what to do with this data. It's usually safe to clear it.\n";

    return promptUser("Clear extra data (y/N/?)? ", pszHelp);
}