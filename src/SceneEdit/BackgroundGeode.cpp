// ======================================================================================

#include "BackgroundGeode.h"

#include <osg/Geometry>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

// ======================================================================================

BackgroundGeode::BackgroundGeode(osg::Vec4d topColor_, osg::Vec4d bottomColor_) 
: osg::Geode()
, topColor(topColor_)
, bottomColor(bottomColor_)
, imageFileName("")
, backgroundMode(GRADIENT_COLOR)
, geom(new osg::Geometry())
, vertices(new osg::Vec3Array())
, colors(new osg::Vec4Array())
{
	vertices->push_back(osg::Vec3(-1, -1, 0));
	vertices->push_back(osg::Vec3(1, -1, 0));
	vertices->push_back(osg::Vec3(1, 1, 0));
	vertices->push_back(osg::Vec3(-1, 1, 0));

	colors->push_back(bottomColor);
	colors->push_back(bottomColor);
	colors->push_back(topColor);
	colors->push_back(topColor);
	
	geom->setVertexArray(vertices.get());
	geom->setColorArray(colors.get());
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
	geom->setDataVariance(osg::Object::STATIC);

	osg::Vec2Array* tcoords = new osg::Vec2Array(4);
	(*tcoords)[0].set(0.0f, 0.0f);
	(*tcoords)[1].set(1.0f, 0.0f);
	(*tcoords)[2].set(1.0f, 1.0f);
	(*tcoords)[3].set(0.0f, 1.0f);
	geom->setTexCoordArray(0, tcoords);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet:: QUADS, 0, 4));
	
	addDrawable(geom.get());

	state = getOrCreateStateSet();

	texture = new osg::Texture2D();
	texture->setImage(0);
	state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::OFF);

	state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	state->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);

	state->setRenderBinDetails(-10, "RenderBin");

	setStateSet(state.get());
}

// ======================================================================================

BackgroundGeode::~BackgroundGeode() 
{
}

// ======================================================================================

void BackgroundGeode::setBackgroundColor(osg::Vec4d topColor_, osg::Vec4d bottomColor_)
{
	topColor = topColor_;
	bottomColor = bottomColor_;
}

// ======================================================================================

void BackgroundGeode::setBackgroundMode(BackgroundMode mode)
{
	backgroundMode = mode;

	switch (mode)
	{
	case MONO_COLOR:
		colors->clear();
		colors->push_back(topColor);
		colors->push_back(topColor);
		colors->push_back(topColor);
		colors->push_back(topColor);
		geom->setColorArray(colors.get());
		geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::OFF);
		break;

	case GRADIENT_COLOR:
		colors->clear();
		colors->push_back(bottomColor);
		colors->push_back(bottomColor);
		colors->push_back(topColor);
		colors->push_back(topColor);
		geom->setColorArray(colors.get());
		geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
		state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::OFF);
		break;

	case IMAGE:
		geom->setColorBinding(osg::Geometry::BIND_OFF);
		state->setTextureAttributeAndModes(0, texture.get(), osg::StateAttribute::ON);
		break;

	case IMAGE_MONO_COLOR:
		break;

	case IMAGE_GRADIENT_COLOR:
		break;
	}
}

// ======================================================================================

void BackgroundGeode::setBackgroundImage(const std::string& fileName)
{
	osg::Image *image = osgDB::readImageFile(fileName);

	if (image)
	{
		texture->setImage(image);
	}
}

// ======================================================================================
