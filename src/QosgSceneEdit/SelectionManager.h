#pragma once

#include <osg/Group>
#include <osg/StateSet>
#include <osg/PolygonOffset>
#include <osg/PolygonMode>
#include <osg/LineWidth>
#include <osg/Material>
#include <osgDB/Input>
#include <osgDB/Output>


class SelectionDecorator;

class SelectionManager : public osg::Referenced
{
public:

    // Initialization
    static SelectionManager* instance();            //!< Create and return a singleton instance.

    //void setViewer(osgProducer::Viewer* pViewer);   //!< Set a new viewer for selection.

    // Selectable state
    //void setSelectable(bool enable);    //!< Enable or disable selection.
    //bool getSelectable() const;         //!< Return selectable state.

    // Select operations
	void clearSelection();
    bool select(osg::Node *pNode, bool noEvent=false);  //!< Select a node in the scene.
    //bool select(float x, float y);                      //!< Select a point in screen coordinate.
    osg::Node* getSelectedNode();                       //!< Return the current selected node.
    const osg::Node* getSelectedNode() const;           //!< Return the current selected node.

    // Callback registration
    //void addSelectionChangedFunc(void(*pFunc)(void*, osg::Node *), void* pData);    //!< Register a callback function for node selection.
    //void removeSelectionChangedFunc(void(*pFunc)(void*, osg::Node *));          //!< Unregister the callback function for node selection.

    // Replace and get the selection decorator
    void setSelectionDecorator(SelectionDecorator* pDecorator);         //!< Replace the selection decorator for highlighting.
    SelectionDecorator* getSelectionDecorator();                        //!< Return the selection decorator.
    const SelectionDecorator* getSelectionDecorator() const;            //!< Return the selection decorator.

    // Store/restore the state
    //bool read(osgDB::Input &fr);    //!< Read the current state from a stream.
    //bool write(osgDB::Output &fr);  //!< Write the current state to a strem.

protected:

    SelectionManager();             //!< Constructor
    virtual ~SelectionManager();    //!< Destructor

protected:

    //osgProducer::Viewer* m_pViewer;             //!< osgProducer viewer
    //osg::ref_ptr<SelectEventHandler> m_pSEH;    //!< SeekEventHandler

    osg::ref_ptr<osg::Node> m_pSelectedNode;    //!< Selected node

    osg::ref_ptr<SelectionDecorator> m_pSelectionDecorator;    //!< Decorator node for hilighting

    //std::vector<SelectionChangedFuncStr> m_selectionChangedFuncs;   //!< Node selection changed callback functions
};

/*!
    @brief Selection decorator class for selection highlighting.
*/
class SelectionDecorator : public osg::Group
{
public:

    // Initialization
    SelectionDecorator();           //!< Constructor
    virtual ~SelectionDecorator();  //!< Destructor

    // Highlighting states
    void setEnable(bool enable);    //!< Enable or disable selection highlighting.
    bool getEnable() const;         //!< Return 'true' if selection highlighting is enabled.

    void setOffsetFactor(float factor); //!< Set offset factor of polygon offset attribute.
    float getOffsetFactor() const;      //!< Return offset factor of polygon offset attribute.

    void setOffsetUnits(float units);   //!< Set offset units of polygon offset attribute.
    float getOffsetUnits() const;       //!< Return offset units of polygon offset attribute.

    void setPolygonMode(osg::PolygonMode::Mode mode);   //!< Set polygon mode attribute for highlight rendering.
    osg::PolygonMode::Mode getPolygonMode() const;      //!< Return polygon mode attribute for highlight rendering.

    void setLineWidth(float width);     //!< Set line width attribute for highlight rendering.
    float getLineWidth() const;         //!< Return line width attribute for highlight rendering.

    void setEmissionColor(osg::Vec4 color); //!< Set highlight color.
    osg::Vec4 getEmissionColor() const;     //!< Return highlight color.

    void setTexturing(bool texturing);  //!< Enable or disable textureing for highlight rendering.
    bool getTexturing() const;          //!< Return textureing state for highlight rendering.

    void setLighting(bool lighting);    //!< Enable or disable lighting for highlight rendering.
    bool getLighting() const;           //!< Return lighting state for highlight rendering.


protected:

    bool m_bEnable;         //!< Enable state
    osg::ref_ptr<osg::StateSet> m_pStateSet;        //!< StateSet for highlight rendering.
    osg::ref_ptr<osg::PolygonOffset> m_pPolyOffset; //!< PolygonOffset attribute for highlighting.
    osg::ref_ptr<osg::PolygonMode> m_pPolyMode;     //!< PolygonMode attribute for highlighting.
    osg::ref_ptr<osg::LineWidth> m_pLineWidth;      //!< LineWidth attribute for highlighting.
    osg::ref_ptr<osg::Material> m_pMaterial;        //!< Material attribute for highlighting.
};
