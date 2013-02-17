//=======================================================================================
//  Headerfiles
//=======================================================================================

#include "BackgroundGeode.h"

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

//=======================================================================================
//  Namespace
//=======================================================================================

namespace QosgSceneEdit {

//=======================================================================================

BackgroundGeode::BackgroundGeode(osg::Vec4d topColor_, osg::Vec4d bottomColor_) 
: osg::Geode()
, _topColor(topColor_)
, _bottomColor(bottomColor_)
, _imageFileName("")
, _backgroundMode(GRADIENT_COLOR)
, _geom(new osg::Geometry())
, _vertices(new osg::Vec3Array())
, _colors(new osg::Vec4Array())
{
	_vertices->push_back(osg::Vec3(-1, -1, 0));
	_vertices->push_back(osg::Vec3(1, -1, 0));
	_vertices->push_back(osg::Vec3(1, 1, 0));
	_vertices->push_back(osg::Vec3(-1, 1, 0));

	_colors->push_back(_bottomColor);
	_colors->push_back(_bottomColor);
	_colors->push_back(_topColor);
	_colors->push_back(_topColor);
	
	_geom->setVertexArray(_vertices.get());
	_geom->setColorArray(_colors.get());
	_geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	_geom->setDataVariance(osg::Object::STATIC);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(0.0f, 0.0f);
	(*tcoords)[1].set(1.0f, 0.0f);
	(*tcoords)[2].set(1.0f, 1.0f);
	(*tcoords)[3].set(0.0f, 1.0f);
	_geom->setTexCoordArray(0, tcoords);

	_geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet:: QUADS, 0, 4));
	
	addDrawable(_geom.get());

	_state = getOrCreateStateSet();

	_texture = new osg::Texture2D();
	_texture->setImage(0);
	_state->setTextureAttributeAndModes(0, _texture.get(), osg::StateAttribute::OFF);

	_state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	_state->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	_state->setRenderBinDetails(-10, "RenderBin");

	setStateSet(_state.get());
}

//=======================================================================================

BackgroundGeode::~BackgroundGeode() 
{
}

//=======================================================================================

void BackgroundGeode::setBackgroundColor(osg::Vec4d topColor_, osg::Vec4d bottomColor_)
{
	_topColor = topColor_;
	_bottomColor = bottomColor_;
}

//=======================================================================================

void BackgroundGeode::setBackgroundMode(BackgroundMode mode)
{
	_backgroundMode = mode;

	switch (mode)
	{
	case MONO_COLOR:
		_colors->clear();
		_colors->push_back(_topColor);
		_colors->push_back(_topColor);
		_colors->push_back(_topColor);
		_colors->push_back(_topColor);
		_geom->setColorArray(_colors.get());
		_geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		_state->setTextureAttributeAndModes(0, _texture.get(), osg::StateAttribute::OFF);
		break;

	case GRADIENT_COLOR:
		_colors->clear();
		_colors->push_back(_bottomColor);
		_colors->push_back(_bottomColor);
		_colors->push_back(_topColor);
		_colors->push_back(_topColor);
		_geom->setColorArray(_colors.get());
		_geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		_state->setTextureAttributeAndModes(0, _texture.get(), osg::StateAttribute::OFF);
		break;

	case IMAGE:
		_geom->setColorBinding(osg::Geometry::BIND_OFF);
		_state->setTextureAttributeAndModes(0, _texture.get(), osg::StateAttribute::ON);
		break;

	case IMAGE_MONO_COLOR:
		break;

	case IMAGE_GRADIENT_COLOR:
		break;
	}
}

//=======================================================================================

void BackgroundGeode::setBackgroundImage(const std::string& fileName)
{
	osg::Image *image = osgDB::readImageFile(fileName);

	if (image)
	{
		_texture->setImage(image);
	}
}

//=======================================================================================
//  Namespace
//=======================================================================================

} // namespace QosgSceneEdit

//=======================================================================================
