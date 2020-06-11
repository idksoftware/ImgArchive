#include "PrimaryMirror.h"
#include "CSVDBFile.h"
//#include "AppConfig.h"

namespace simplearchive {

    PrimaryMirror::PrimaryMirror() : m_primaryIndex(0)
    {};

    bool PrimaryMirror::process() {


        CSVDBFile csvDBFile;
        csvDBFile.setPath(m_primaryIndexPath.c_str());
            //"C:\ProgramData\IDK-Software\ImgArchive\pi\index";
        uint64_t index = csvDBFile.getMaxIndex();

        std::unique_ptr<ImageInfo> primaryItem = csvDBFile.getItemAt(index);

        return true;
    }

    PrimaryMirror PrimaryMirror::get()
    {
        static PrimaryMirror primaryMirror;
        return primaryMirror;
    }

    void PrimaryMirror::setMasterRepositoryPath(const char* path)
    {
        m_masterRepositoryPath = path;
    }

    void PrimaryMirror::setDerivativeRepositoryPath(const char* path)
    {
        m_derivativeRepositoryPath = path;
    }

    void PrimaryMirror::setPrimaryIndexPath(const char* path)
    {
        m_primaryIndexPath = path;
    }

    void PrimaryMirror::setPrimaryMirrorPath(const char* path)
    {
        m_primaryMirrorPath = path;
    }

};