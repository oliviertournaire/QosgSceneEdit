#pragma once

#include <QObject>
#include <QHash>

#include <osg/Referenced>
#include <osgIntrospection/Reflection>

class QPropertyEditor;
class QTreeWidget;
class PropertyCollection;
class IProperty;

namespace osgIntrospection {
	class PropertyInfo;
	class Value;
}

class ReflectionManager : public QObject,
						  public osg::Referenced
{
	Q_OBJECT

public:

					ReflectionManager(QTreeWidget *treeWidget, QPropertyEditor *pe);
	virtual			~ReflectionManager();

	void			setNode(osg::Referenced *obj);


protected slots:

	void			treeItemSelectionChanged();
    void			propertyChanged(IProperty *property);
	void			resetProperty(const QString& name);


private:

	void			parseClass(const osgIntrospection::Type& classType, osg::Referenced *obj);
	IProperty*		parseProperties(const osgIntrospection::PropertyInfo* pPropInfo, 
		                            const osgIntrospection::Value& instance);
	IProperty*		parseSimpleProperty(const osgIntrospection::Type& propType, const QString& propName, const osgIntrospection::Value& propValue, const osgIntrospection::PropertyInfo* parentPropInfo);


	QPropertyEditor *_propertyEditor;
	QTreeWidget *_treeWidget;
	PropertyCollection *_collection;
	QHash<IProperty*, const osgIntrospection::PropertyInfo*> _propertyHash;
};
