#ifndef STLREPAIR_BINARYSTLFILEREADER__H_
#define STLREPAIR_BINARYSTLFILEREADER__H_

#include <string>
#include <cstdio>

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

    //! Destructor.
    virtual ~BinarySTLFileReader();

private:

    FILE *m_pFile;
};

#endif
