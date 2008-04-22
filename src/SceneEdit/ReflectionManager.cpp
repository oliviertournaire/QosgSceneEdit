#include "ReflectionManager.h"
#include "QPropertyEditor.h"
#include "TreeViewItem.h"

#include <QTreeWidget>
#include <QTextStream>

#include <osg/Node>
#include <osgDB/Registry>
#include <osgDB/DynamicLibrary>
#include <osgDB/FileUtils>
#include <osgDB/Version>
#include <osgIntrospection/Value>
#include <osgIntrospection/Reflection>
#include <osgIntrospection/MethodInfo>
#include <osgIntrospection/PropertyInfo>
#include <osgIntrospection/variant_cast>

using osgIntrospection::variant_cast;

//=================================================================================================

ReflectionManager::ReflectionManager(QTreeWidget *treeWidget, QPropertyEditor *pe)
: _treeWidget(treeWidget)
, _propertyEditor(pe)
, _collection(0L)
{
    osgDB::FilePathList dirPathList = osgDB::getLibraryFilePathList();
    dirPathList.push_back(".");

    for (osgDB::FilePathList::iterator dirIter=dirPathList.begin(); dirIter!=dirPathList.end(); ++dirIter) 
	{
        std::string dirName = *dirIter;
		std::string pluginDir = std::string("/osgPlugins-") + osgDBGetVersion();
        osgDB::DirectoryContents filePathList = osgDB::getDirectoryContents(dirName + pluginDir);

        for (osgDB::DirectoryContents::iterator fileIter=filePathList.begin(); fileIter!=filePathList.end(); ++fileIter) 
		{
            std::string fileName = *fileIter;

#if _DEBUG
			if (fileName == "osgwrapper_osgd.dll")
#else
			if (fileName == "osgwrapper_osg.dll)
#endif
				osgDB::DynamicLibrary::loadLibrary(dirName + pluginDir + "/" + fileName);
        }
    }

	connect(_treeWidget, SIGNAL(itemSelectionChanged()), this, SLOT(treeItemSelectionChanged()));
}

//=================================================================================================

ReflectionManager::~ReflectionManager()
{
}

//=================================================================================================

void ReflectionManager::setNode(osg::Referenced *obj)
{
	if (!obj)
	{
		_propertyEditor->setInitialInput(0L);
	}
	else
	{
		osgIntrospection::ExtendedTypeInfo eti(typeid(*obj), false, false);
		const osgIntrospection::Type& classType = osgIntrospection::Reflection::getType(eti);

		if (_collection)
		{
			delete _collection;
			_collection = 0L;
		}

		_collection = new PropertyCollection("<root>");

		if (classType.isDefined())
		{
			bool objectParsed = false;

			parseClass(classType, obj);

			if (classType.getNumBaseTypes() > 0)
			{
				for (int i=0; i<classType.getNumBaseTypes(); ++i)
				{
					const osgIntrospection::Type& baseClassType = classType.getBaseType(i);

					parseClass(baseClassType, obj);

					if (baseClassType.getQualifiedName() == "osg::Object")
						objectParsed = true;
				}
			}

			if (dynamic_cast<osg::Object*>(obj) && !objectParsed)
			{
				parseClass(osgIntrospection::Reflection::getType(osgIntrospection::ExtendedTypeInfo(typeid(osg::Object), false, false)), 
					       obj);
			}
		}

		_propertyEditor->setInitialInput(_collection);
	}
}

//=================================================================================================

void ReflectionManager::treeItemSelectionChanged()
{
	QList<QTreeWidgetItem*> items = _treeWidget->selectedItems();

	if (items.size() == 1)
	{
		TreeViewItem *item = dynamic_cast<TreeViewItem*>(items.front());

		if (item)
		{
			osg::ref_ptr<osg::Node> node = item->getOsgNode();

			setNode(node.get());
		}
	}
	else
		setNode(0L);
}

//=================================================================================================

void ReflectionManager::parseClass(const osgIntrospection::Type& classType, osg::Referenced *obj)
{
	QString className("[undefined type]");
	const osgIntrospection::PropertyInfoList& pil = classType.getProperties();

	if (classType.isDefined())
		className = classType.getQualifiedName().c_str();

	if (classType.isAbstract())
		className += " [abstract]";

	if (classType.isAtomic())
		className += " [atomic]";
	
	_collection->addProperty(new SeparatorProperty(QString(), className));

	if (!pil.empty())
	{
		int count = 0;
		for (osgIntrospection::PropertyInfoList::const_iterator piItr = pil.begin(); piItr != pil.end(); piItr++)
		{
			const osgIntrospection::PropertyInfo *propInfo = *piItr;
			IProperty *p = parseProperties(propInfo, obj);

			if (p)
				_collection->addProperty(p);
		}
	}
}

//=================================================================================================

IProperty* ReflectionManager::parseProperties(const osgIntrospection::PropertyInfo* pPropInfo, 
		                                      const osgIntrospection::Value& instance)
{
	IProperty *property = 0L;
	QString propName = pPropInfo->getName().c_str();
	const osgIntrospection::Type& propType = pPropInfo->getPropertyType();

	if (!propType.isDefined())
		return 0L;

	if (pPropInfo->canGet())
	{
		if (pPropInfo->isSimple())
		{
			osgIntrospection::Value propValue = pPropInfo->getValue(instance);
			property = parseSimpleProperty(propType, propName, propValue, pPropInfo);
		}
	}

	return property;
}

//=================================================================================================

IProperty* ReflectionManager::parseSimpleProperty(const osgIntrospection::Type& propType, const QString& propName, const osgIntrospection::Value& propValue, const osgIntrospection::PropertyInfo* parentPropInfo)
{
	IProperty *p = 0L;
	const type_info& tid = propType.getStdTypeInfo();
	
	if (tid == typeid(std::string))
	{
		QString value = variant_cast<std::string>(propValue).c_str();
		p = new StringProperty(qvariant_cast<QString>(value), propName);
	}
	else if (tid == typeid(osg::BoundingBox))
	{
		osg::BoundingBox bb = variant_cast<osg::BoundingBox>(propValue);
		p = new OsgVec3fProperty(bb._max, propName);
	}
	else if (tid == typeid(osg::BoundingSphere))
	{
		const osg::BoundingSphere& bs = variant_cast<const osg::BoundingSphere&>(propValue);
		p = new OsgBoundingSphereProperty(bs, propName);
	}
	else if (tid == typeid(bool))
	{
		p = new BoolProperty(variant_cast<bool>(propValue), propName);
		p->setHasReset(true);
	}
	else if (tid == typeid(unsigned int) ||
		     tid == typeid(GLenum) ||
			 tid == typeid(GLbitfield) ||
			 tid == typeid(GLuint))
	{
		IntProperty *intProp = new IntProperty(variant_cast<unsigned int>(propValue), propName);
		p = intProp;
	}
	else if (tid == typeid(std::vector<std::string>))
	{
		QStringList stringList;
		const std::vector<std::string>& descriptions = variant_cast<const std::vector<std::string>&>(propValue);

		for(std::vector<std::string>::const_iterator it = descriptions.begin(); 
			it != descriptions.end(); 
			++it)
		{
			stringList += it->c_str();
		}

		p = new StringListProperty(stringList, propName);
	}
	else if (propType.isEnum())
	{
		const osgIntrospection::EnumLabelMap& emap = propType.getEnumLabels();
		QStringList labels;
		
		for (osgIntrospection::EnumLabelMap::const_iterator it = emap.begin(); 
			 it != emap.end(); 
			 ++it)
		{
			labels += it->second.c_str();
		}

		p = new ListProperty(labels, variant_cast<int>(propValue), propName);
	}
	else if (propType.isPointer())
	{
		QString str;
		QTextStream(&str) << "0x" << propValue.toString().c_str();
		p = new StringProperty(str, propName);
	}

	if (!p)
		p = new StringProperty(propType.getQualifiedName().c_str(), propName);

	return p;
}