//=======================================================================================
//  Headerfiles
//=======================================================================================

#include "ComputeNodeInfoVisitor.h"

#include <osg/Geode>
#include <osg/Drawable>
#include <osg/Geometry>
#include <osg/PrimitiveSet>

//=======================================================================================
//  Namespace
//=======================================================================================

namespace SceneEdit {

//=======================================================================================

ComputeNodeInfoVisitor::ComputeNodeInfoVisitor(TraversalMode traversalMode)
: osg::NodeVisitor(traversalMode)
, _numTriangles(0)
{
}

//=======================================================================================

void ComputeNodeInfoVisitor::apply(osg::Geode& geode)
{
	for (unsigned int i=0; i<geode.getNumDrawables(); ++i)
	{
		osg::ref_ptr<osg::Drawable> drawable = geode.getDrawable(i);

		if (drawable.valid())
		{
			osg::ref_ptr<osg::Geometry> geometry = drawable->asGeometry();

			if (geometry.valid())
			{
				for (unsigned int j=0; j<geometry->getNumPrimitiveSets(); ++j)
				{
					osg::ref_ptr<osg::PrimitiveSet> ps = geometry->getPrimitiveSet(j);

					if (ps.valid())
					{
						switch (ps->getMode())
						{
						case osg::PrimitiveSet::TRIANGLES:
							_numTriangles += ps->getNumIndices() / 3;
							break;

						case osg::PrimitiveSet::TRIANGLE_STRIP:
							_numTriangles += ps->getNumIndices() - 2;
							break;

						case osg::PrimitiveSet::TRIANGLE_FAN:
							_numTriangles += ps->getNumIndices() - 2;
							break;

						case osg::PrimitiveSet::QUADS:
							_numTriangles += ps->getNumIndices() / 2;
							break;

						case osg::PrimitiveSet::QUAD_STRIP:
							_numTriangles += ps->getNumIndices() - 2;
							break;

						case osg::PrimitiveSet::POLYGON:
							_numTriangles += ps->getNumIndices() - 2;
							break;
						}
					}
				}
			}
		}
	}
}

//=======================================================================================
//  Namespace
//=======================================================================================

} // namespace SceneEdit

//=======================================================================================
