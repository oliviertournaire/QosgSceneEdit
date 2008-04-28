#include "SelectionManager.h"

#include <osgDB/Registry>
#include <osgDB/DynamicLibrary>
#include <osgDB/FileUtils>
#include <osgIntrospection/Value>
#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/variant_cast>


SelectionManager::SelectionManager() : osg::Referenced()
{
    //m_pViewer = NULL;
    //m_pSEH = new SelectEventHandler(this);

    // create a selection decorator for highlighting.
    m_pSelectionDecorator = new SelectionDecorator;
}

/*!
    Destructor
*/
SelectionManager::~SelectionManager()
{
}

/*!
    Create and return a singleton instance.
    @return Singleton instance pointer.
*/
SelectionManager* SelectionManager::instance()
{
    static osg::ref_ptr<SelectionManager> s_pSelectionManager = new SelectionManager;
    return s_pSelectionManager.get();
}

/*!
    Set a new viewer for selection.
    @param[in] pViewer osgProducer viewer for selection.
*/
//void SelectionManager::setViewer(osgProducer::Viewer* pViewer)
//{
//    m_pViewer = pViewer;
//
//    m_pViewer->getEventHandlerList().push_front(m_pSEH.get());
//}

/*!
    Enable or disable selection.
    @param[in] enable Selectable state.
*/
//void SelectionManager::setSelectable(bool enable)
//{
//    m_pSEH->setSelectable(enable);
//}

/*!
    Return selectable state.
    @return enable Selectable state.
*/
//bool SelectionManager::getSelectable() const
//{
//    return m_pSEH->getSelectable();
//}

void SelectionManager::clearSelection()
{
    // deselect the previous selected node
    while (m_pSelectionDecorator->getNumChildren() > 0)
        m_pSelectionDecorator->removeChild(0,1);

    if (m_pSelectionDecorator->getNumParents() > 0)
        m_pSelectionDecorator->getParent(0)->removeChild(m_pSelectionDecorator.get());

}

/*!
    Select a node in the scene.
    @param[in] pNode Node to be selected (or NULL to deselect).
    @param[in] noEvent 'True' if you don't need to call the node selected callback functions.
    @return Return 'true' if the node is selected. Return 'false' when deseleced.
*/
bool SelectionManager::select(osg::Node* pNode, bool noEvent)
{
	return false;

    // select the new node
    if (pNode)
	{
        m_pSelectionDecorator->addChild(pNode);
        
		osg::Group* pParent = pNode->getParent(0);

        if (pParent)
            pParent->addChild(m_pSelectionDecorator.get());
    }

    //if (!noEvent) {
    //    // call the registered callback functions
    //    for (std::vector<SelectionChangedFuncStr>::iterator iter=m_selectionChangedFuncs.begin(); iter!=m_selectionChangedFuncs.end(); ++iter)
    //        (*iter).m_pFunc((*iter).m_pData, pNode);
    //}

    // keep the pointer of the selected node
    m_pSelectedNode = pNode;

    return (pNode!=NULL);
}

/*!
    Select a point in screen coordinate.
    @param[in] x X position in screen coordinate.
    @param[in] y Y position in screen coordinate.
    @return Return 'true' if the point is successfully selected. Return 'false' when deseleced.
*/
//bool SelectionManager::select(float x,float y)
//{
//    osgUtil::SceneView *pSceneView = m_pViewer->getSceneHandlerList()[0]->getSceneView();
//
//    osg::Vec3 near_point,far_point;
//    if (!m_pViewer->computeNearFarPoints(x, y, 0, near_point, far_point))
//        return false;
//
//    osg::ref_ptr<osg::LineSegment> pLineSegment = new osg::LineSegment;
//    pLineSegment->set(near_point,far_point);
//
//    osgUtil::IntersectVisitor iv;
//    iv.addLineSegment(pLineSegment.get());
//
//    pSceneView->getSceneData()->accept(iv);
//
//    if (iv.hits())
//    {
//        osgUtil::IntersectVisitor::HitList& hitList = iv.getHitList(pLineSegment.get());
//        osgUtil::IntersectVisitor::HitList::iterator hitr=hitList.begin();
//        {
//            osg::Geode* pGeode = hitr->_geode.get();
//            select(pGeode);
//        }
//        return true;
//    }
//
//    select((osg::Node *)NULL);
//    return false;
//}

/*!
    Return the current selected node.
    @return Current slected node.
*/
osg::Node* SelectionManager::getSelectedNode()
{
    return m_pSelectedNode.get();
}

/*!
    Return the current selected node.
    @return Current slected node.
*/
const osg::Node* SelectionManager::getSelectedNode() const
{
    return m_pSelectedNode.get();
}

/*!
    Register a callback function for node selection.
    @param[in] pFunc Callback function.
    @param[in] pData User data.
*/
//void SelectionManager::addSelectionChangedFunc(void(*pFunc)(void*, osg::Node *), void* pData)
//{
//    SelectionChangedFuncStr sf;
//    sf.m_pFunc = pFunc;
//    sf.m_pData = pData;
//    m_selectionChangedFuncs.push_back(sf);
//}

/*!
    Unregister the callback function for node selection.
    @param[in] pFunc Callback function.
*/
//void SelectionManager::removeSelectionChangedFunc(void(*pFunc)(void*, osg::Node *))
//{
//    for (std::vector<SelectionChangedFuncStr>::iterator iter=m_selectionChangedFuncs.begin(); iter!=m_selectionChangedFuncs.end(); ++iter) {
//        if (iter->m_pFunc == pFunc)
//            m_selectionChangedFuncs.erase(iter);
//    }
//}

/*!
    Replace the selection decorator for highlighting.
    @param[in] pDecorator New selection decorator.
*/
void SelectionManager::setSelectionDecorator(SelectionDecorator* pDecorator)
{
    if (!pDecorator) 
		return;

    m_pSelectionDecorator = pDecorator;
    select(m_pSelectedNode.get());
}

/*!
    Return the selection decorator.
    @return Current selection decorator.
*/
SelectionDecorator* SelectionManager::getSelectionDecorator()
{
    return m_pSelectionDecorator.get();
}

/*!
    Return the selection decorator.
    @return Current selection decorator.
*/
const SelectionDecorator* SelectionManager::getSelectionDecorator() const
{
    return m_pSelectionDecorator.get();
}

/*!
    Read the current state from a stream.
    @param[in,out] fr Input stream.
    @return Return 'true' if SelectionManager data found in the current stream position.
*/
//bool SelectionManager::read(osgDB::Input& fr)
//{
//    if (!fr[0].matchWord("orh::SelectionManager"))
//        return false;
//
//    fr+=2;
//
//    if (fr[0].matchWord("UniqueID"))
//    {
//        // ignore UniqueID
//        fr += 2;
//    }
//
//    if (fr.matchSequence("selectable %w"))
//    {
//        std::string w(fr[1].getStr());
//        setSelectable(w == "TRUE");
//        fr += 2;
//    }
//
//    while (!fr[0].isCloseBracket())
//    {
//        bool itAdvanced = false;
//        if (!itAdvanced) itAdvanced = m_pSelectionDecorator->read(fr);
//        if (!itAdvanced) fr.advanceOverCurrentFieldOrBlock();
//    }
//    ++fr;  // skip the close blacket
//
//    return true;
//}

/*!
    Write the current state to a strem.
    @param[out] fw Output stream.
    @return Return 'true' if wrote the current state successfully.
*/
//bool SelectionManager::write(osgDB::Output& fw)
//{
//    fw.indent() << "orh::SelectionManager {" << std::endl;
//    fw.moveIn();
//
//    if (getSelectable())
//        fw.indent() << "selectable TRUE" << std::endl;
//    else
//        fw.indent() << "selectable FALSE" << std::endl;
//
//    m_pSelectionDecorator->write(fw);
//
//    fw.moveOut();
//    fw.indent() << "}" << std::endl;
//
//    return true;
//}







/*!
    Constructor
*/
SelectionDecorator::SelectionDecorator() : osg::Group()
{
    // create highlighting state
    m_pStateSet = new osg::StateSet;
    m_pPolyOffset = new osg::PolygonOffset;
    m_pPolyOffset->setFactor(-1.0f);
    m_pPolyOffset->setUnits(-1.0f);
    m_pPolyMode = new osg::PolygonMode;
    m_pPolyMode ->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
    m_pStateSet->setAttributeAndModes(m_pPolyOffset.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    m_pStateSet->setAttributeAndModes(m_pPolyMode.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    m_pLineWidth = new osg::LineWidth;
    m_pStateSet->setAttributeAndModes(m_pLineWidth.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    m_pMaterial = new osg::Material;
    m_pMaterial->setDiffuse(osg::Material::FRONT_AND_BACK,osg::Vec4(1.0, 0.0, 0.0, 1.0));
	m_pMaterial->setColorMode(osg::Material::DIFFUSE);
    m_pStateSet->setAttributeAndModes(m_pMaterial.get(), osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
    m_pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
    m_pStateSet->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OVERRIDE|osg::StateAttribute::OFF);
    setStateSet(m_pStateSet.get());

    setEnable(true);
}

/*!
    Destructor
*/
SelectionDecorator::~SelectionDecorator()
{
}

/*!
    Enable or disable selection highlighting.
    @param[in] enable Enable state.
*/
void SelectionDecorator::setEnable(bool enable)
{
    m_bEnable = enable;

    if (m_bEnable)
        setNodeMask(-1);
    else
        setNodeMask(0);
}

/*!
    Return 'true' if selection highlighting is enabled.
    @return Enable state.
*/
bool SelectionDecorator::getEnable() const
{
    return m_bEnable;
}

/*!
    Set offset factor of polygon offset attribute.
    @param[in] factor Offset factor of polygon offset attribute.
*/
void SelectionDecorator::setOffsetFactor(float factor)
{
    m_pPolyOffset->setFactor(factor);
}

/*!
    Return offset factor of polygon offset attribute.
    @return Offset factor of polygon offset attribute.
*/
float SelectionDecorator::getOffsetFactor() const
{
    return m_pPolyOffset->getFactor();
}

/*!
    Set offset units of polygon offset attribute.
    @param[in] units Offset units of polygon offset attribute.
*/
void SelectionDecorator::setOffsetUnits(float units)
{
    m_pPolyOffset->setUnits(units);
}

/*!
    Return offset units of polygon offset attribute.
    @return Offset units of polygon offset attribute.
*/
float SelectionDecorator::getOffsetUnits() const
{
    return m_pPolyOffset->getUnits();
}

/*!
    Set polygon mode attribute for highlight rendering.
    @param[in] mode Polygon mode.
*/
void SelectionDecorator::setPolygonMode(osg::PolygonMode::Mode mode)
{
    m_pPolyMode->setMode(osg::PolygonMode::FRONT_AND_BACK, mode);
}

/*!
    Return polygon mode attribute for highlight rendering.
    @return Polygon mode.
*/
osg::PolygonMode::Mode SelectionDecorator::getPolygonMode() const
{
    return m_pPolyMode->getMode(osg::PolygonMode::FRONT_AND_BACK);
}

/*!
    Set line width attribute for highlight rendering.
    @param[in] width Line width.
*/
void SelectionDecorator::setLineWidth(float width)
{
    m_pLineWidth->setWidth(width);
}

/*!
    Return line width attribute for highlight rendering.
    @return Line width.
*/
float SelectionDecorator::getLineWidth() const
{
    return m_pLineWidth->getWidth();
}

/*!
    Set highlight color.
    @param[in] color Highlight color.
*/
void SelectionDecorator::setEmissionColor(osg::Vec4 color)
{
    m_pMaterial->setEmission(osg::Material::FRONT_AND_BACK, color);
}

/*!
    Return highlight color.
    @return Highlight color.
*/
osg::Vec4 SelectionDecorator::getEmissionColor() const
{
    return m_pMaterial->getEmission(osg::Material::FRONT_AND_BACK);
}

/*!
    Enable or disable textureing for highlight rendering.
    @param[in] texturing Texturing state.
*/
void SelectionDecorator::setTexturing(bool texturing)
{
    m_pStateSet->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::OVERRIDE|(texturing?(osg::StateAttribute::ON):(osg::StateAttribute::OFF)));
}

/*!
    Return textureing state for highlight rendering.
    @return Texturing state.
*/
bool SelectionDecorator::getTexturing() const
{
    return m_pStateSet->getTextureMode(0, GL_TEXTURE_2D) & osg::StateAttribute::ON;
}

/*!
    Enable or disable lighting for highlight rendering.
    @param[in] lighting Lighting state.
*/
void SelectionDecorator::setLighting(bool lighting)
{
    m_pStateSet->setMode(GL_LIGHTING, osg::StateAttribute::OVERRIDE|(lighting?(osg::StateAttribute::ON):(osg::StateAttribute::OFF)));
}

/*!
    Return lighting state for highlight rendering.
    @return Lighting state.
*/
bool SelectionDecorator::getLighting() const
{
    return m_pStateSet->getMode(GL_LIGHTING) & osg::StateAttribute::ON;
}
