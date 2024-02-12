#ifndef STLREPAIR_REPAIROPTIONPROMPTS__H_
#define STLREPAIR_REPAIROPTIONPROMPTS__H_

/**
 * Used for when the tool thinks this is an ASCII mode STL and we
 * want to know if the user wants to attempt a Binary mode repair.
 */
bool promptShouldTreatASCIIModeAsBinary();

/**
 * Prompts the user for clearing out the STL header.
 */
bool promptClearFileHeader();

/**
 * Prompts the user for clearing out the facet attribute counts.
 */
bool promptClearFacetAttributeCounts();

/**
 * Prompts the user to adjust the internal triangle count to match the actual number of triangles.
 */
bool promptTriangleCountTooBig();

/**
 * Prompts the user to clear out any extra data appended to end of the file.
 */
bool promptTruncateExtraData();

#endif
