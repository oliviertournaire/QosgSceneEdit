#pragma once

//==============================================================================
//  Headerfiles
//==============================================================================

#include <osg/Vec4>
#include <osg/Geode>

#include <string>

//==============================================================================
//  Forward declarations
//==============================================================================

namespace osg {
	class Texture2D;
}

//==============================================================================
//  Namespace
//==============================================================================

namespace QosgSceneEdit {

//==============================================================================
//  Class Declaration
//==============================================================================

class BackgroundGeode : public osg::Geode
{
public:

	enum BackgroundMode 
	{
		MONO_COLOR,
		GRADIENT_COLOR,
		IMAGE,
		IMAGE_MONO_COLOR,
		IMAGE_GRADIENT_COLOR
	};

					BackgroundGeode(osg::Vec4d topColor_ = osg::Vec4d(0.1765, 0.5, 1.0, 1.0),
									osg::Vec4d bottomColor_ = osg::Vec4d(1.0, 1.0, 1.0, 1.0));

	virtual			~BackgroundGeode();

	void			setBackgroundMode(BackgroundMode mode);
	BackgroundMode	getBackgroundMode() const { return _backgroundMode; }

	void			setBackgroundColor(osg::Vec4d topColor_, osg::Vec4d bottomColor_);
	osg::Vec4d		getBackgroundTopColor() const { return _topColor; }
	osg::Vec4d		getBackgroundBottomColor() const { return _bottomColor; }

	void			setBackgroundImage(const std::string& fileName);


private:

	osg::Vec4d						_topColor;
	osg::Vec4d						_bottomColor;
	std::string						_imageFileName;
	BackgroundMode					_backgroundMode;
	osg::ref_ptr<osg::Geometry>		_geom;
	osg::ref_ptr<osg::Vec3Array>	_vertices;
	osg::ref_ptr<osg::Vec4Array>	_colors;
	osg::ref_ptr<osg::Texture2D>	_texture;
	osg::ref_ptr<osg::StateSet>		_state;
};

//==============================================================================
//  Namespace
//==============================================================================

} // namespace QosgSceneEdit

//==============================================================================
