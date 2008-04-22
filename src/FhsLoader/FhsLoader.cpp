/*
 *  FhsLoader.cpp
 *  SceneMaker
 *
 *  Created by Andre Normann on 23.11.06.
 *  Copyright 2006 Andre Normann. All rights reserved.
 *
 */

#include "FhsLoader.h"
#include <osg/Group>
#include <osg/Switch>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Material>
#include <osg/LineWidth>
#include <osg/BlendColor>
#include <osg/TexGen>
#include <osg/LightModel>
#include <osg/LOD>
#include <osg/Notify>
#include <osg/TextureCubeMap>
#include <osgDB/FileUtils>

#include <osgUtil/TriStripVisitor>
#include <osgUtil/SmoothingVisitor>
//#include "ANSmoothingVisitor.h"
#include <osgUtil/Tessellator>

#include <osgDB/ReadFile>

FhsLoader::FhsLoader()
: m_version(0.0f)
, m_pointOrder(CCW)
, m_useVertexNormals(false)
, m_useVertexColors(false)
, m_useTexCoords2D(false)
, m_useTexCoords3D(false)
, m_unlit(false)
{
//    osg::setNotifyLevel(osg::DEBUG_FP);

    m_materialCallbacks["UID"]          = &FhsLoader::matx_UID;
    m_materialCallbacks["DEPTHTEST"]    = &FhsLoader::matx_DEPTHTEST;
    m_materialCallbacks["DEPTHMASK"]    = &FhsLoader::matx_DEPTHMASK;
    m_materialCallbacks["DEPTHFUNC"]    = &FhsLoader::matx_DEPTHFUNC;
    m_materialCallbacks["LINEWIDTH"]    = &FhsLoader::matx_LINEWIDTH;
}

FhsLoader::~FhsLoader()
{
    m_materialCallbacks.clear();
}

osg::Node* FhsLoader::getRootNode()
{
    return m_rootNode.get();
}

void FhsLoader::beginModel(const char* modelName)
{
    m_modelName = modelName;
    
    m_rootNode = new osg::Group;
    m_rootNode->setName(m_modelName);
    m_assemblies.push(m_rootNode);
}

void FhsLoader::endModel()
{
    m_assemblies.pop();    
}

void FhsLoader::setVersion(float version)
{
    m_version = version;
}

void FhsLoader::setPointOrder(POINT_ORDER po)
{
    m_pointOrder = po;
}

void FhsLoader::beginEnvironment(const char* name)
{
    printf("\tEnvironment: '%s'\n", name);
}

void FhsLoader::endEnvironment()
{
    printf("\tEndOfEnvironment\n");
}

void FhsLoader::setEnvAmbient(float r, float g, float b)
{
    printf("\t\tAmbient: %g %g %g\n", r, g, b);
}

void FhsLoader::setEnvAmbient(const char* name)
{
}

void FhsLoader::setEnvBackground(float r, float g, float b)
{
    printf("\t\tBackground: %g %g %g\n", r, g, b);
}

void FhsLoader::setEnvBackground(const char* name)
{
}

void FhsLoader::beginAssembly(const char* name)
{
    osg::ref_ptr<osg::Group> ass = new osg::Group;
    ass->setName(name);
    m_assemblies.top()->addChild(ass.get());
    m_assemblies.push(ass);
}

void FhsLoader::endAssembly()
{
    m_assemblies.pop();
}

void FhsLoader::beginLOD(const char* name)
{
    osg::ref_ptr<osg::LOD> lod = new osg::LOD;
    lod->setName(name);
    m_assemblies.top()->addChild(lod.get());
    m_assemblies.push(lod.get());
    
    m_currentLOD = lod;
}

void FhsLoader::endLOD()
{
    for (int i=0; i<m_lodRanges.size()-1; ++i)
    {
        m_currentLOD->setRange(i, m_lodRanges[i], m_lodRanges[i+1]);
    }
    
    m_currentLOD->setRange(m_lodRanges.size()-1, m_lodRanges[m_lodRanges.size()-1], FLT_MAX);
    
    m_currentLOD = NULL;
    m_lodRanges.clear();
}

void FhsLoader::setLODDistance(float distance)
{
    m_lodRanges.push_back(distance);
}

void FhsLoader::setTransformationByMatrix()
{
    osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform(m_matrix);
    osg::ref_ptr<osg::Group> group = m_assemblies.top();
    osg::ref_ptr<osg::Group> parent = group->getParent(0);
    
    xform->setName(group->getName());
    
    for (int i=0; i<group->getNumChildren(); ++i)
    {
        xform->addChild(group->getChild(i));
    }
    
    parent->replaceChild(group.get(), xform.get());
    
    m_assemblies.pop();
    m_assemblies.push(xform.get());
}

void FhsLoader::setTransformationByTranslate()
{
    m_matrix.makeTranslate(m_vector);
    setTransformationByMatrix();
}

void FhsLoader::setTransformationByScale()
{
    m_matrix.makeScale(m_vector);
    setTransformationByMatrix();
}

void FhsLoader::beginSwitch(const char* name)
{
    osg::ref_ptr<osg::Group> newSwitch = new osg::Switch;
    newSwitch->setName(name);
    m_assemblies.top()->addChild(newSwitch.get());
    m_assemblies.push(newSwitch);
}

void FhsLoader::endSwitch()
{
    m_assemblies.pop();
}

void FhsLoader::setWhichChild(int index)
{
    osg::ref_ptr<osg::Switch> mySwitch = dynamic_cast<osg::Switch*>(m_assemblies.top().get());
    
    if (mySwitch.valid())
    {
        switch (index)
        {
            case -1:
                mySwitch->setAllChildrenOn();
                break;
                
            case -2:
                mySwitch->setAllChildrenOff();
                break;
                
            default:
                mySwitch->setSingleChildOn(index);
                break;
        }
    }
}

void FhsLoader::beginBillboard(const char* name)
{
}

void FhsLoader::endBillboard()
{
}

void FhsLoader::beginGeometry(const char* name)
{
    osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform();
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->setName(name);
    xform->addChild(geode.get());
    m_assemblies.top()->addChild(xform.get());
    m_assemblies.push(xform.get());
    
    m_geode = geode;
    
    m_vertices = new osg::Vec3Array;
    m_normals = new osg::Vec3Array;
    m_textures2 = new osg::Vec2Array;
    m_textures3 = new osg::Vec3Array;
    m_colors = new osg::Vec4Array;
    
    m_geometry = new osg::Geometry;
    
    m_primType = -1;
}

void FhsLoader::endGeometry()
{
    if (m_vertices.valid())
    {
        m_geometry->setVertexArray(m_vertices.get());
    }
    
    if (m_normals.valid() && m_useVertexNormals)
    {
        m_geometry->setNormalArray(m_normals.get());
        m_geometry->setNormalBinding(osg::Geometry::BIND_PER_VERTEX);
    }
    else if (!m_unlit)
    {
        osgUtil::SmoothingVisitor::smooth(*m_geometry);
    }
    
    if (m_textures2.valid() && m_useTexCoords2D)
    {
        m_geometry->setTexCoordArray(0, m_textures2.get());
    }
    else if (m_textures3.valid() && m_useTexCoords3D)
    {
        m_geometry->setTexCoordArray(0, m_textures3.get());
    }
    
    if (m_colors.valid() && m_useVertexColors)
    {
        m_geometry->setColorArray(m_colors.get());
        m_geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    }
    
    m_geode->setStateSet(m_stateSet.get());
    
    /*
    osgUtil::Tesselator tesselator;
    tesselator.retesselatePolygons(*m_geometry);
    
    // tri strip polygons to improve graphics peformance
    osgUtil::TriStripVisitor tsv;
    tsv.stripify(*m_geometry);
    
    if (!m_geometry->getNormalArray() || m_geometry->getNormalArray()->getNumElements()==0)
    {
        osgUtil::SmoothingVisitor::smooth(*m_geometry);        
    }
    */
    
    m_geometry->setUseDisplayList(true);
    m_geode->addDrawable(m_geometry.get());
    
    m_geode = NULL;
    m_geometry = NULL;
    m_vertices = NULL;
    m_normals = NULL;
    m_textures2 = NULL;
    m_textures3 = NULL;
    m_colors = NULL;
    m_stateSet = NULL;
    m_primSet = NULL;
    
    m_assemblies.pop();
}

void FhsLoader::beginNewFace(int type)
{
    if (m_primType != type                          ||
        type == osg::PrimitiveSet::QUAD_STRIP       ||
        type == osg::PrimitiveSet::TRIANGLE_STRIP   ||
        type == osg::PrimitiveSet::TRIANGLE_FAN     ||
        type == osg::PrimitiveSet::LINE_LOOP        ||
        type == osg::PrimitiveSet::LINE_STRIP       ||
        type == osg::PrimitiveSet::POLYGON)
    {
        m_primSet = new osg::DrawElementsUInt(type, 0);
        m_geometry->addPrimitiveSet(m_primSet.get());
    }
    
    m_primType = type;
}

void FhsLoader::beginPolypool()
{
    setUseVertexNormals(false);
    setUseVertexColors(false);
    setUseTexCoords2D(false);
    setUseTexCoords3D(false);
}

void FhsLoader::endPolypool()
{
}

void FhsLoader::beginPolygon()
{
    beginNewFace(osg::PrimitiveSet::POLYGON);
}

void FhsLoader::beginQuads()
{
    beginNewFace(osg::PrimitiveSet::QUADS);
}

void FhsLoader::beginQuadStrip()
{
    beginNewFace(osg::PrimitiveSet::QUAD_STRIP);
}

void FhsLoader::beginTriangles()
{
    beginNewFace(osg::PrimitiveSet::TRIANGLES);
}

void FhsLoader::beginTriStrip()
{
    beginNewFace(osg::PrimitiveSet::TRIANGLE_STRIP);
}

void FhsLoader::beginTriFan()
{
    beginNewFace(osg::PrimitiveSet::TRIANGLE_FAN);
}

void FhsLoader::beginPoints()
{
    beginNewFace(osg::PrimitiveSet::POINTS);
}

void FhsLoader::beginLine()
{
    beginNewFace(osg::PrimitiveSet::LINE_STRIP);
}

void FhsLoader::beginLines()
{
    beginNewFace(osg::PrimitiveSet::LINES);
}

void FhsLoader::beginClosedLine()
{
    beginNewFace(osg::PrimitiveSet::LINE_LOOP);
}

void FhsLoader::endPrimitive()
{
}

void FhsLoader::addPos()
{
    m_vertices->push_back(m_vector);
}

void FhsLoader::addNormal()
{
    m_normals->push_back(m_vector);
}

void FhsLoader::addTexture2()
{
    m_textures2->push_back(m_vector2);
}

void FhsLoader::addTexture3()
{
    m_textures3->push_back(m_vector);
}

void FhsLoader::addColor()
{
    m_colors->push_back(osg::Vec4f(m_vector[0], m_vector[1], m_vector[2], 1.0f));
}

void FhsLoader::addColor4()
{
    m_colors->push_back(m_vector4);
}

void FhsLoader::addIndex(int idx)
{
    m_primSet->push_back(idx);
}

void FhsLoader::addNormalIndex(int idx)
{
    addIndex(idx);
    setUseVertexNormals(true);
}

void FhsLoader::addTextureIndex(int idx)
{
    addIndex(idx);
    setUseTexCoords2D(true);
}

void FhsLoader::addTexture3Index(int idx)
{
    addIndex(idx);
    setUseTexCoords3D(true);
}

void FhsLoader::addTextureNormalIndex(int idx)
{
    addIndex(idx);
    setUseTexCoords2D(true);
    setUseVertexNormals(true);
}

void FhsLoader::addTexture3NormalIndex(int idx)
{
    addIndex(idx);
    setUseTexCoords3D(true);
    setUseVertexNormals(true);
}

void FhsLoader::addColorIndex(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
}

void FhsLoader::addColorNormalIndex(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
    setUseVertexNormals(true);
}

void FhsLoader::addColorTextureIndex(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
    setUseTexCoords2D(true);
}

void FhsLoader::addColorTexture3Index(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
    setUseTexCoords3D(true);
}

void FhsLoader::addColorTextureNormalIndex(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
    setUseTexCoords2D(true);
    setUseVertexNormals(true);
}

void FhsLoader::addColorTexture3NormalIndex(int idx)
{
    addIndex(idx);
    setUseVertexColors(true);
    setUseTexCoords3D(true);
    setUseVertexNormals(true);
}

void FhsLoader::setUseVertexNormals(bool flag)
{
    m_useVertexNormals = flag;
}

void FhsLoader::setUseVertexColors(bool flag)
{
    m_useVertexColors = flag;
}

void FhsLoader::setUseTexCoords2D(bool flag)
{
    m_useTexCoords2D = flag;
}

void FhsLoader::setUseTexCoords3D(bool flag)
{
    m_useTexCoords3D = flag;
}

void FhsLoader::beginMaterial(const char* name)
{
    m_stateSet = new osg::StateSet;
    m_nameToStateSetMap[name] = m_stateSet;
    
    m_material = new osg::Material;
    m_stateSet->setAttribute(m_material.get());

    m_minFilter = osg::Texture::LINEAR;
    m_magFilter = osg::Texture::LINEAR;
    
/*    osg::LightModel* lightModel = new osg::LightModel;
    lightModel->setTwoSided(true);
    m_stateSet->setAttributeAndModes(lightModel, osg::StateAttribute::ON);*/
}

void FhsLoader::endMaterial()
{
    if (m_texture.valid())
    {
        m_texture->setWrap(osg::Texture::WRAP_S, m_wrapS);
        m_texture->setWrap(osg::Texture::WRAP_T, m_wrapT);
        m_texture->setFilter(osg::Texture::MIN_FILTER, m_minFilter);
        m_texture->setFilter(osg::Texture::MAG_FILTER, m_magFilter);
    }
    
    m_stateSet->setMode(GL_LIGHTING, m_unlit ? osg::StateAttribute::OFF : osg::StateAttribute::ON);
    
    m_program = NULL;
    m_material = NULL;
    m_stateSet = NULL;
    m_texture = 0L;
    m_unlit = false;
}

void FhsLoader::setMaterial(const char* name)
{
    m_stateSet = m_nameToStateSetMap[name];
}

void FhsLoader::setFromMaterial(const char* name)
{
    printf("\tUseMatFrom: '%s'\n", name);
}

void FhsLoader::setDiffuse(float r, float g, float b)
{
    osg::Vec4f color(r, g, b, 1.0f);    
    m_material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setDiffuse(float mult)
{
    osg::Vec4f color = m_baseColor * mult;
    m_material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setAmbient(float r, float g, float b)
{
    osg::Vec4f color(r, g, b, 1.0f);    
    m_material->setAmbient(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setAmbient(float mult)
{
    osg::Vec4f color = m_baseColor * mult;
    m_material->setAmbient(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setEmission(float r, float g, float b)
{
    osg::Vec4f color(r, g, b, 1.0f);    
    m_material->setEmission(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setEmission(float mult)
{
    osg::Vec4f color = m_baseColor * mult;
    m_material->setEmission(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setSpecular(float r, float g, float b)
{
    osg::Vec4f color(r, g, b, 1.0f);    
    m_material->setSpecular(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setSpecular(float mult)
{
    osg::Vec4f color = m_baseColor * mult;
    m_material->setSpecular(osg::Material::FRONT_AND_BACK, color);
}

void FhsLoader::setColor(float r, float g, float b)
{
    m_baseColor.set(r, g, b, 1.0);
}

void FhsLoader::setTransparency(float tr)
{
    osg::Vec4f color = m_material->getDiffuse(osg::Material::FRONT_AND_BACK);
    color[3] = 1.0f - tr;
    m_material->setDiffuse(osg::Material::FRONT_AND_BACK, color);
    
    if (tr > 0.0f)
    {
        m_stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
        m_stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);    
    }
}

void FhsLoader::setShininess(float s)
{
    m_material->setShininess(osg::Material::FRONT_AND_BACK, s);
}

void FhsLoader::setTexture(const char* name)
{
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(name);
    
    if (image.valid())
    {
        m_texture = new osg::Texture2D(image.get());
        m_stateSet->setTextureAttributeAndModes(0, m_texture.get(), osg::StateAttribute::ON);
    }
}

void FhsLoader::setTexWrap(int wrapS, int wrapT)
{
    m_wrapS = (osg::Texture::WrapMode) wrapS;
    m_wrapT = (osg::Texture::WrapMode) wrapT;
}

void FhsLoader::setTexFilter(int minFilter, int magFilter)
{
    m_minFilter = (osg::Texture::FilterMode) minFilter;
    m_magFilter = (osg::Texture::FilterMode) magFilter;
}

void FhsLoader::setUnlit(bool unlit)
{
    m_unlit = unlit;
}

void FhsLoader::setLineWidth(float lw)
{
    osg::ref_ptr<osg::LineWidth> lineWidth = new osg::LineWidth;
    lineWidth->setWidth(lw);
    m_stateSet->setAttribute(lineWidth.get());
}

void FhsLoader::setAlpha(float alpha)
{
    osg::ref_ptr<osg::BlendColor> blendColor = new osg::BlendColor(osg::Vec4f(0.0f, 0.0f, 0.0f, alpha));
    m_stateSet->setAttribute(blendColor.get());
    m_stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    m_stateSet->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);    
}

bool FhsLoader::matx_UID(const char* ext)
{
    long long uid = 0;
    
    if (sscanf(ext, "%llx", &uid) == 1)
    {
        printf("Ignoring UID '%llx'\n", uid);
        return true;
    }
    
    return false;
}

bool FhsLoader::matx_DEPTHTEST(const char *ext)
{
    int value;
    
    if (sscanf(ext, "%d", &value) == 1)
    {
        return true;
    }
    
    return false;
}

bool FhsLoader::matx_DEPTHMASK(const char *ext)
{
    int value;
    
    if (sscanf(ext, "%d", &value) == 1)
    {
        return true;
    }
    
    return false;
}

bool FhsLoader::matx_DEPTHFUNC(const char *ext)
{
    int value;
    
    if (sscanf(ext, "%d", &value) == 1)
    {
        return true;
    }
    
    return false;
}

bool FhsLoader::matx_LINEWIDTH(const char *ext)
{
    float lineWidth;
    
    if (sscanf(ext, "%f", &lineWidth) == 1)
    {
        setLineWidth(lineWidth);
        return true;
    }
    
    return false;
}

void FhsLoader::callMaterialExtension(const char* ext)
{
    char extName[128];
    sscanf(ext, "%s", extName);
    
    MaterialCallbackMap::iterator it = m_materialCallbacks.find(extName);
    if (it == m_materialCallbacks.end())
    {
        fprintf(stderr, "Skipping unknown extension \"%s\"\n", extName);
    }
    else
    {
        if (!(this->*(it->second))(ext+strlen(extName)))
        {
            // TODO: Add error handling
        }
    }
    
    /*
    else if (!strcmp("GENFUNC", extName))
    {
        char c, mode[80];
        
        if (sscanf(ext, "%s %c %s", extName, &c, mode) == 3)
        {
            if (!strcmp("Sphere", mode))
            {
                osg::ref_ptr<osg::TexGen> texGen = new osg::TexGen;
                texGen->setMode(osg::TexGen::SPHERE_MAP);
                m_stateSet->setTextureAttribute(0, texGen.get());
                
                switch (c)
                {
                    case 's':
                        m_stateSet->setTextureMode(0, GL_TEXTURE_GEN_S, osg::StateAttribute::ON);
                        break;
                        
                    case 't':
                        m_stateSet->setTextureMode(0, GL_TEXTURE_GEN_T, osg::StateAttribute::ON);
                        break;
                }
            }
        }
    }
    else if (!strcmp("SHADERPROG", extName))
    {
        char shaderType[80], shaderTarget[80], fileName[256];
        
        if (sscanf(ext, "%s %s %s '%s'", extName, shaderType, shaderTarget, fileName) == 4)
        {
            fileName[strlen(fileName)-1] = '\0';
            
            std::string filename = osgDB::findDataFile(fileName);
            
            if (!strcmp("GLSL", shaderType))
            {
                if (!m_program.valid())
                {
                    m_program = new osg::Program;
                    m_program->setName(m_material->getName());
                }
                
                osg::ref_ptr<osg::Shader> shader;
                
                if (!strcmp("VS", shaderTarget))
                {
                    shader = new osg::Shader(osg::Shader::VERTEX);
                }
                else
                {
                    shader = new osg::Shader(osg::Shader::FRAGMENT);
                }
                
                shader->setName(fileName);
                shader->loadShaderSourceFromFile(filename);
                m_program->addShader(shader.get());
                m_stateSet->setAttributeAndModes(m_program.get(), osg::StateAttribute::ON);
            }
        }
    }
    else if (!strcmp("SHADERPARAM", extName))
    {
        char shaderType[80], paramName[256], paramType[256];
        
        if (sscanf(ext, "%s %s %s %s", extName, shaderType, paramName, paramType) == 4)
        {
            if (!strcmp(paramType, "SAMPLERCUBE"))
            {
                char imageFile[256];
                
                if (sscanf(ext, "%s %s %s %s '%s'", extName, shaderType, paramName, paramType, imageFile) == 5)
                {
                    imageFile[strlen(imageFile)-1] = '\0';
                    
                    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(imageFile);
                    if (image.valid())
                    {
                        osg::ref_ptr<osg::TextureCubeMap> texture = new osg::TextureCubeMap();
                        m_stateSet->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
                    }                
                    
                    m_stateSet->addUniform(new osg::Uniform(paramName, 0));
                }
                
            }
            else if (!strcmp(paramType, "FLOAT"))
            {
                float floatValue;
                
                if (sscanf(ext, "%s %s %s %s %f", extName, shaderType, paramName, paramType, &floatValue) == 5)
                {
                    m_stateSet->addUniform(new osg::Uniform(paramName, floatValue));
                }
            }
            else if (!strcmp(paramType, "FLOAT3"))
            {
                float f1, f2, f3;
                
                if (sscanf(ext, "%s %s %s %s %f %f %f", extName, shaderType, paramName, paramType, &f1, &f2, &f3) == 7)
                {
                    m_stateSet->addUniform(new osg::Uniform(paramName, osg::Vec3(f1,f2,f3)));
                }
            }
        }
    }
    */
}

void FhsLoader::setVector(float x, float y, float z)
{
    m_vector[0] = x;
    m_vector[1] = y;
    m_vector[2] = z;
}

void FhsLoader::setVector4(float x, float y, float z, float w)
{
    m_vector[0] = x;
    m_vector[1] = y;
    m_vector[2] = z;
    m_vector[3] = w;
}

void FhsLoader::setVector2(float x, float y)
{
    m_vector2[0] = x;
    m_vector2[1] = y;
}

void FhsLoader::setMatrix(float a00, float a01, float a02, float a03,
                          float a10, float a11, float a12, float a13,
                          float a20, float a21, float a22, float a23,
                          float a30, float a31, float a32, float a33)
{
    m_matrix.set(a00, a01, a02, a03,
                 a10, a11, a12, a13,
                 a20, a21, a22, a23,
                 a30, a31, a32, a33);
}
