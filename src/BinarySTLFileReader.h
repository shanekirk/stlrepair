#ifndef STLREPAIR_BINARYSTLFILEREADER__H_
#define STLREPAIR_BINARYSTLFILEREADER__H_

#include <string>
#include <cstdio>

/**
 * Callback interface for anything wishing to consume parsed data from the
 * BinarySTLFileReader.
 * 
 * Implementers should NOT throw.
 */
class BinarySTLFileReaderListener
{
public:

    //! Destructor.
    virtual ~BinarySTLFileReaderListener() {}

    //! Called whenever parsing begins.
    virtual void onReadBegin() {}

    //! Called whenever parsing ends. Guaranteed to be called even in the event of errors.
    virtual void onReadEnd() {}

    //! Called whenever the file header is parsed.
    virtual void onReadFileHeader(uint8_t* pBytes, size_t byteCount) {}
};

/**
 * Basic binary STL file reader.
 */
class BinarySTLFileReader
{
public:

    /**
     * Constructor.
     * 
     * @throws std::runtime_error
     */
    BinarySTLFileReader(const std::string& filepath);

    /**
     * Destructor.
     */
    ~BinarySTLFileReader();

    /**
     * Attempts to parse the file. Parsed data will be provided to the
     * given listener.
     * 
     * @throws std::runtime_error
     */
    void readFile(BinarySTLFileReaderListener& listener);

private:

    void readFileHeader(BinarySTLFileReaderListener& listener);

    FILE *m_pFile;
};

#endif
