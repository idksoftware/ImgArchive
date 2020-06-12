#include "MirrorManager.h"
#include "CSVDBFile.h"
#include "CSVStatusFile.h"
#include "MirrorList.h"
//#include "AppConfig.h"

namespace simplearchive {

    MirrorManager::MirrorManager() : m_primaryIndex(1), m_mirrorList(std::make_shared<MirrorList>())
    {};

    bool MirrorManager::process() {


        CSVDBFile csvDBFile;
        CSVStatusFile csvStatusFile;
        csvDBFile.setPath(m_primaryIndexPath.c_str());
        csvStatusFile.setPath("C:\\ProgramData\\IDK-Software\\ImgArchive\\mirror");
        m_primaryIndex = csvStatusFile.getMaxIndex();
            //"C:\ProgramData\IDK-Software\ImgArchive\pi\index";
        uint64_t index = csvDBFile.getMaxIndex();
        for (m_primaryIndex; m_primaryIndex <= index; m_primaryIndex++) {
            std::unique_ptr<ImageInfo> primaryItem = csvDBFile.getItemAt(m_primaryIndex);
            ExifDate& date = (ExifDate &)primaryItem->getDate();
            csvStatusFile.insert((uint64_t)primaryItem->getIdx(), primaryItem->getShortImagePath().c_str(), primaryItem->getName().c_str(), primaryItem->getSize(), (uint64_t)primaryItem->getCrc(),
                                primaryItem->getMd5().c_str(), primaryItem->getUuid().c_str(), primaryItem->getVersion(), date, (uint64_t)primaryItem->getDBIdx());
            // new images added
            //m_mirrorList
        }
        

        return true;
    }

    MirrorManager MirrorManager::get()
    {
        static MirrorManager primaryMirror;
        return primaryMirror;
    }

    void MirrorManager::setMasterRepositoryPath(const char* path)
    {
        m_masterRepositoryPath = path;
    }

    void MirrorManager::setDerivativeRepositoryPath(const char* path)
    {
        m_derivativeRepositoryPath = path;
    }

    void MirrorManager::setPrimaryIndexPath(const char* path)
    {
        m_primaryIndexPath = path;
    }

    void MirrorManager::setPrimaryMirrorPath(const char* path)
    {
        m_primaryMirrorPath = path;
    }

};