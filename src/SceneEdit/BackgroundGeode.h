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

namespace SceneEdit {

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
	BackgroundMode	getBackgroundMode() const { return backgroundMode; }

	void			setBackgroundColor(osg::Vec4d topColor_, osg::Vec4d bottomColor_);
	osg::Vec4d		getBackgroundTopColor() const { return topColor; }
	osg::Vec4d		getBackgroundBottomColor() const { return bottomColor; }

	void			setBackgroundImage(const std::string& fileName);


private:

	osg::Vec4d						topColor;
	osg::Vec4d						bottomColor;
	std::string						imageFileName;
	BackgroundMode					backgroundMode;
	osg::ref_ptr<osg::Geometry>		geom;
	osg::ref_ptr<osg::Vec3Array>	vertices;
	osg::ref_ptr<osg::Vec4Array>	colors;
	osg::ref_ptr<osg::Texture2D>	texture;
	osg::ref_ptr<osg::StateSet>		state;

	//protected:

	//	//! assigne les deux couleurs de fond pour le dégardé hortizontal/ne réaffecte pas les couleurs membres
	//	/*! cette fonction ne change pas le mode d'affichage elle assigne juste les deux couleur
	//	 *		\param topC    --> couleur du haut
	//	 *		\param bottomC --> couleur du bas
	//	 */
	//	void				setInternalBackgroundColor(const QColor& topC, const QColor& bottomC);


	//public:

	//	//! assigne l'image de fond
	//	/*! cette fonction ne change pas le mode elle assigne juste l'image de fond
	//	 *		\fileName    --> nom de l'image (chemain complet)
	//	 *		\return true si l'image a bien été chargée
	//	 */
	//	bool				setBackgroundImage(const QString& fileName);

	//	//! renvoie le nom complet de l'image de fond
	//	QString				getImageFileName() const;
};

//==============================================================================
//  Namespace
//==============================================================================

} // namespace SceneEdit

//==============================================================================
