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

using namespace QosgSceneEdit;
using namespace osg;

//=======================================================================================

ComputeNodeInfoVisitor::ComputeNodeInfoVisitor(TraversalMode traversalMode)
    : NodeVisitor(traversalMode)
    , _numTriangles(0)
{
}

//=======================================================================================

void ComputeNodeInfoVisitor::apply(Geode& geode)
{
    for (unsigned int i=0; i<geode.getNumDrawables(); ++i)
    {
        ref_ptr<Drawable> drawable = geode.getDrawable(i);

        if (drawable.valid())
        {
            ref_ptr<Geometry> geometry = drawable->asGeometry();

            if (geometry.valid())
            {
                for (unsigned int j=0; j<geometry->getNumPrimitiveSets(); ++j)
                {
                    ref_ptr<PrimitiveSet> ps = geometry->getPrimitiveSet(j);

                    if (ps.valid())
                    {
                        switch (ps->getMode())
                        {
                        case PrimitiveSet::TRIANGLES:
                            _numTriangles += ps->getNumIndices() / 3;
                            break;

                        case PrimitiveSet::TRIANGLE_STRIP:
                            _numTriangles += ps->getNumIndices() - 2;
                            break;

                        case PrimitiveSet::TRIANGLE_FAN:
                            _numTriangles += ps->getNumIndices() - 2;
                            break;

                        case PrimitiveSet::QUADS:
                            _numTriangles += ps->getNumIndices() / 2;
                            break;

                        case PrimitiveSet::QUAD_STRIP:
                            _numTriangles += ps->getNumIndices() - 2;
                            break;

                        case PrimitiveSet::POLYGON:
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
