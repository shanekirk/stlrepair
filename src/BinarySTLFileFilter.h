#ifndef STLREPAIR_BINARYSTLFILEFILTER__H_
#define STLREPAIR_BINARYSTLFILEFILTER__H_

#include "BinarySTLFileReader.h"
#include "BinarySTLFileWriter.h"

#include <string>
#include <memory>
#include <vector>

/**
 * This class serves to filter/modify STL data produced from a 
 * BinarySTLFileReader and write it back out to disk.
 *
 * At this moment, this works as a monolithic filter class. But
 * in the future, as needs get more sophisticated, I could 
 * envision breaking it up into a more traditional pipe-filter
 * oriented design.
 */
class BinarySTLFileFilter : public BinarySTLFileReaderListener
{
public:

    //! Constructor.
    BinarySTLFileFilter(const std::string& outputFilePath);

    //! Called whenever parsing ends. Guaranteed to be called even in the event of errors.
    void onReadEnd() override;

    //! Called whenever the file header is parsed.
    bool onReadFileHeader(const STLBinaryHeader &header) override;

    //! Called whenever the total triangle count has been parsed.
    bool onReadTriangleCount(const uint32_t triangleCount) override;

    /**
     * Called whenever a triangle has been read.
     * Note that we don't currently need to tease apart the triangle data for
     * this app. We only need to treat it as a binary blob. This may change
     * in the future.
     */
    bool onReadTriangle(const STLBinaryTriangleData& triangleData, 
        const uint16_t attributeByteCount) override;

    /**
     * Called whenever a blob of unknown data is encountered. This usually
     * happens near the end of a file which has truncated triangle data or
     * an exporter has tacked something else on to the end.
     *
     * Regardless of why, it basically indicates the data isn't the same size as
     * triangle data.
     */
    bool onReadUnknownData(const uint8_t* const pData, const size_t dataSize) override;

    bool m_zeroOutHeader;
    bool m_updateTriangleCount;
    bool m_zeroAttributeByteCounts;
    bool m_clearExtraFileData;
    uint32_t m_triangleLimit;

private:

    std::string m_outputFilePath;
    STLBinaryHeader m_header;
    std::unique_ptr<BinarySTLFileWriter> m_spWriter;
    uint32_t m_readTriangleCount;
    uint32_t m_actualTriangleCount;    

    std::vector<char> m_xtraData;
};

#endif
