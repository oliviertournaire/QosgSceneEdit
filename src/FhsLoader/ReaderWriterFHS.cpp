// -*-c++-*-

#if defined(_MSC_VER)
    #pragma warning( disable : 4786 )
#endif

#include <string>

#include <osg/Notify>
#include <osg/Node>
#include <osg/MatrixTransform>
#include <osg/Geode>

#include <osg/Geometry>
#include <osg/StateSet>
#include <osg/Material>
#include <osg/Texture2D>
#include <osg/TexGen>
#include <osg/TexMat>

#include <osgDB/Registry>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>

#include <osgUtil/TriStripVisitor>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Tessellator>

#include "FhsLoader.h"


class ReaderWriterFHS : public osgDB::ReaderWriter
{
public:
    
    ReaderWriterFHS() {}

    virtual const char* className() const { return "Fraunhofer FHS Format"; }
    virtual bool acceptsExtension(const std::string& extension) const 
    {
        return osgDB::equalCaseInsensitive(extension,"fhs");
    }

    virtual ReadResult readNode(const std::string& fileName, const osgDB::ReaderWriter::Options* options) const;
    virtual ReadResult readNode(std::istream& fin, const Options* options) const;
};

// register with Registry to instantiate the above reader/writer.
osgDB::RegisterReaderWriterProxy<ReaderWriterFHS> g_fhsReaderWriterProxy;

// read file and convert to OSG.
osgDB::ReaderWriter::ReadResult ReaderWriterFHS::readNode(const std::string& file, const osgDB::ReaderWriter::Options* options) const
{
    std::string ext = osgDB::getLowerCaseFileExtension(file);
    if (!acceptsExtension(ext)) 
        return ReadResult::FILE_NOT_HANDLED;

    std::string fileName = osgDB::findDataFile(file, options);
    if (fileName.empty()) 
        return ReadResult::FILE_NOT_FOUND;
    
    FhsLoader fhsLoader;
    if (fhsLoader.readFile(fileName))
    {
        osg::Node* node = fhsLoader.getRootNode();
        return node;
    }
    
    return ReadResult::FILE_NOT_HANDLED;
}

osgDB::ReaderWriter::ReadResult ReaderWriterFHS::readNode(std::istream& fin, const Options* options) const
{
    return ReadResult::FILE_NOT_HANDLED;
}
