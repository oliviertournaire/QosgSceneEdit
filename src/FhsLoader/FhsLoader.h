/*
 *  FhsLoader.h
 *  SceneMaker
 *
 *  Created by Andre Normann on 23.11.06.
 *  Copyright 2006 Andre Normann. All rights reserved.
 *
 */

#ifndef FHSLOADER_H
#define FHSLOADER_H

// C++ Library
#include <string>
#include <stack>
#include <map>

// OpenSceneGraph
#include <osg/Vec3f>
#include <osg/Matrixf>
#include <osg/ref_ptr>
#include <osg/Array>
#include <osg/Texture>

namespace osg
{
    class Group;
    class Node;
    class Geode;
    class Material;
    class Geometry;
    class DrawElementsUInt;
    class LOD;
    class Program;
}

class FhsLoader
{
public:
    
    typedef std::vector< int > IndexList;
    typedef std::map< std::string, 
                      bool (FhsLoader::*)(const char*) > MaterialCallbackMap;
    
    
    enum POINT_ORDER {
        CW,
        CCW
    };
    
            FhsLoader();
    virtual ~FhsLoader();
    
    bool        readFile(const std::string& fileName);
    osg::Node*  getRootNode();
    
    // Model
    void    beginModel(const char* modelName);
    void    endModel();    
    void    setVersion(float version);
    void    setPointOrder(POINT_ORDER po);

    // Assembly
    void    beginAssembly(const char* name);
    void    endAssembly();
    void    setTransformationByMatrix();
    void    setTransformationByTranslate();
    void    setTransformationByScale();
    
    // LevelOfDetail
    void    beginLOD(const char* name);
    void    endLOD();
    void    setLODDistance(float distance);
    
    // Billboard
    void    beginBillboard(const char* name);
    void    endBillboard();
    
    // Switch
    void    beginSwitch(const char* name);
    void    endSwitch();
    void    setWhichChild(int index);
    
    // Geometry handling
    void    beginGeometry(const char* name);
    void    endGeometry();
    void    beginPolypool();
    void    endPolypool();
    
    void    beginNewFace(int type);
    void    beginPolygon();
    void    beginPoints();
    void    beginLine();
    void    beginLines();
    void    beginClosedLine();
    void    beginTriangles();
    void    beginTriFan();
    void    beginTriStrip();
    void    beginQuads();
    void    beginQuadStrip();
    void    endPrimitive();
    
    void    addPos();
    void    addNormal();
    void    addTexture2();
    void    addTexture3();
    void    addColor();
    void    addColor4();
    void    setUseVertexNormals(bool flag);
    void    setUseVertexColors(bool flag);
    void    setUseTexCoords2D(bool flag);
    void    setUseTexCoords3D(bool flag);
    
    void    addIndex(int idx);
    void    addNormalIndex(int idx);
    void    addTextureIndex(int idx);
    void    addTexture3Index(int idx);
    void    addTextureNormalIndex(int idx);
    void    addTexture3NormalIndex(int idx);
    void    addColorIndex(int idx);
    void    addColorNormalIndex(int idx);
    void    addColorTextureIndex(int idx);
    void    addColorTexture3Index(int idx);
    void    addColorTextureNormalIndex(int idx);
    void    addColorTexture3NormalIndex(int idx);
    
    // Environment handling
    void    beginEnvironment(const char* name);
    void    endEnvironment();
    void    setEnvAmbient(float r, float g, float b);
    void    setEnvAmbient(const char* name);
    void    setEnvBackground(float r, float g, float b);
    void    setEnvBackground(const char* name);
    
    // Material handling
    void    beginMaterial(const char* name);
    void    endMaterial();
    void    setMaterial(const char* name);
    void    setFromMaterial(const char* name);
    void    setColor(float r, float g, float b);
    void    setDiffuse(float r, float g, float b);
    void    setDiffuse(float mult);
    void    setAmbient(float r, float g, float b);
    void    setAmbient(float mult);
    void    setSpecular(float r, float g, float b);
    void    setSpecular(float mult);
    void    setEmission(float r, float g, float b);
    void    setEmission(float mult);
    void    setTransparency(float tr);
    void    setShininess(float s);
    void    setTexture(const char* name);
    void    setTexWrap(int u, int v);
    void    setTexFilter(int minFilter, int magFilter);
    void    setUnlit(bool unlit);
    void    setAlpha(float alpha);
    void    setLineWidth(float lw);
    void    callMaterialExtension(const char* ext);
    
    void    setVector(float x, float y, float z);
    void    setVector4(float x, float y, float z, float w);
    void    setVector2(float x, float y);
    void    setMatrix(float a00, float a01, float a02, float a03,
                      float a10, float a11, float a12, float a13,
                      float a20, float a21, float a22, float a23,
                      float a30, float a31, float a32, float a33);
    
    
protected:

    bool    matx_UID(const char* ext);
    bool    matx_DEPTHTEST(const char *ext);
    bool    matx_DEPTHMASK(const char *ext);
    bool    matx_DEPTHFUNC(const char *ext);
    bool    matx_LINEWIDTH(const char *ext);
        
private:
        
    std::string     m_modelName;
    float           m_version;
    POINT_ORDER     m_pointOrder;
    osg::Vec3f      m_vector;
    osg::Vec2f      m_vector2;
    osg::Vec4f      m_vector4;
    bool            m_useVertexNormals;
    bool            m_useVertexColors;
    bool            m_useTexCoords2D;
    bool            m_useTexCoords3D;
    osg::Matrixf    m_matrix;
    
    MaterialCallbackMap                     m_materialCallbacks;
    
    osg::ref_ptr<osg::Group>                m_rootNode;
    std::stack<osg::ref_ptr<osg::Group> >   m_assemblies;
    osg::ref_ptr<osg::Geode>                m_geode;
    osg::ref_ptr<osg::Geometry>             m_geometry;
    osg::ref_ptr<osg::Vec3Array>            m_vertices;
    osg::ref_ptr<osg::Vec3Array>            m_normals;
    osg::ref_ptr<osg::Vec2Array>            m_textures2;
    osg::ref_ptr<osg::Vec3Array>            m_textures3;
    osg::ref_ptr<osg::Vec4Array>            m_colors;
    osg::ref_ptr<osg::StateSet>             m_stateSet;
    std::map<std::string, 
             osg::ref_ptr<osg::StateSet> >  m_nameToStateSetMap;
    osg::ref_ptr<osg::Material>             m_material;
    osg::ref_ptr<osg::DrawElementsUInt>     m_primSet;
    int                                     m_primType;
    osg::Vec4f                              m_baseColor;
    bool                                    m_unlit;
    osg::ref_ptr<osg::Texture>              m_texture;
    osg::ref_ptr<osg::LOD>                  m_currentLOD;
    std::vector<float>                      m_lodRanges;
    osg::ref_ptr<osg::Program>              m_program;
    osg::Texture::WrapMode                  m_wrapS;
    osg::Texture::WrapMode                  m_wrapT;
    osg::Texture::WrapMode                  m_wrapR;
    osg::Texture::FilterMode                m_minFilter;
    osg::Texture::FilterMode                m_magFilter;
};

#endif
