#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QCryptographicHash>
#include <QDate>
#include <QFile>
#include <QDomDocument>
#include <QTextCodec>

#include "i_wsJsonParser.h"
#include "i_wsAPIRosreestr.h"
#include "wsProject.h"

IMPLEMENT_SMARTPOINTER(WSProject)
// ----------------------------------------------------------------
struct WSProject::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QString filePath = QString();	
	QFile* fProject = Q_NULLPTR;
	QString name = QString();
	QDomDocument* doc = Q_NULLPTR;
	bool flagChanged = false;			// true - если есть не сохраненные данные
};
// ----------------------------------------------------------------
WSProject::WSProject(QString filePath, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->filePath = filePath;	
	m_data->name = QFileInfo(m_data->filePath).baseName();
	
	m_data->fProject = new QFile(m_data->filePath);
	if (m_data->fProject->open(QIODevice::ReadOnly) == true)
	{
		m_data->doc = new QDomDocument();
		if (m_data->doc != Q_NULLPTR)
		{
			m_data->doc->setContent(m_data->fProject);

		}		
	}	else		qDebug() << "Can not open project file";
}
// ----------------------------------------------------------------
WSProject::~WSProject()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		if (m_data->doc != Q_NULLPTR)
		{
			delete m_data->doc;
			m_data->doc = Q_NULLPTR;
		}

		if (m_data->fProject != Q_NULLPTR)
		{
			if (m_data->fProject->isOpen() == true)
				m_data->fProject->close();

			delete m_data->fProject;
			m_data->fProject = Q_NULLPTR;
		}
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
bool WSProject::addObject(QObject* data)
{
	if (data == Q_NULLPTR)
	{
		qDebug() << "JsonParser is NULL";
		return false;
	}


	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}
		   
	if (m_data->fProject == Q_NULLPTR)
	{
		qDebug() << "Can not add object to project file. File is NULL";
		return false;
	}

	if (m_data->doc == Q_NULLPTR)
	{
		qDebug() << "DomDocument is NULL";
		return false;
	}
	

	if (m_data->fProject->isOpen() == false)
	{
		qDebug() << "Can not write object to project file. File is not open";
		return false;
	}
		
	i_wsJsonParser* parser = Q_NULLPTR;
	parser = qobject_cast<i_wsJsonParser*>(data);
	if (parser == Q_NULLPTR)
	{
		qDebug() << "Can not convert QObject* to i_wsJsonParser*";
		return false;
	}
	

	QVector<S_OBJECT> objects = parser->objects();
	if (objects.size() < 1)
	{
		qDebug() << "Count object < 1";
		emit message(m_data->codec->toUnicode("Поиск объектов"), m_data->codec->toUnicode("Объект не найден"));
		return false;
	}	

	for (int i = 0; i < objects.count(); i++)
	{
		S_OBJECT obj = objects.at(i);

		QDomElement domElement = m_data->doc->documentElement();
		if (domElement.isNull() == false)
		{
			QDomElement objElement = domElement.firstChildElement("objects");
			if (objElement.isNull() == false)
			{
				// Создаем объект и назначаем ему кадастровый новымер
				QDomElement domElementObject = m_data->doc->createElement("object");
				QDomAttr domAttrObject = m_data->doc->createAttribute("cadNumber");
				domAttrObject.setValue(obj.cadnomer);
				domElementObject.setAttributeNode(domAttrObject);				
				
				// Создаем остальные подъобъекты
				QDomElement domElementAddress = m_data->doc->createElement("address");
				QDomText domTextAddress = m_data->doc->createTextNode(obj.address);
				domElementAddress.appendChild(domTextAddress);

				QDomElement domElementType = m_data->doc->createElement("type");
				QDomText domTextType = m_data->doc->createTextNode(obj.type);
				domElementType.appendChild(domTextType);

				QDomElement domElementArea = m_data->doc->createElement("area");
				QDomText domTextArea = m_data->doc->createTextNode(obj.area);
				domElementArea.appendChild(domTextArea);
				
				QDomElement domElementCategory = m_data->doc->createElement("category");
				QDomText domTextCategory = m_data->doc->createTextNode(obj.category);
				domElementCategory.appendChild(domTextCategory);

				// Добавляем подобъекты 
				domElementObject.appendChild(domElementAddress);
				domElementObject.appendChild(domElementType);
				domElementObject.appendChild(domElementArea);
				domElementObject.appendChild(domElementCategory);

				objElement.appendChild(domElementObject);

				m_data->flagChanged = true;
			}
		}			
	}

	return true;
}
// ----------------------------------------------------------------
QVariant WSProject::getProperty(QString) const
{
	return QVariant();
}
// ----------------------------------------------------------------
void WSProject::setProperty(QString name, QVariant value)
{

}
// ----------------------------------------------------------------
int WSProject::getIDUser() const
{
	return 0;
}
// ----------------------------------------------------------------
bool WSProject::isUser(QString login)
{
	return false;
}
// ----------------------------------------------------------------
QDir WSProject::workingDirectory() const
{
	return QDir();
}
// ----------------------------------------------------------------
QString WSProject::filePath() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QString();
	}
		
	return m_data->filePath;
}
// ----------------------------------------------------------------
QString WSProject::name() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QString();
	}

	return m_data->name;
}
// ----------------------------------------------------------------
bool WSProject::save()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	if (m_data->doc == Q_NULLPTR)
	{
		qDebug() << "DomDocument is NULL";
		return false;
	}

	// Закрываем файл
	QFile file(m_data->filePath);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream(&file) << m_data->doc->toString();
		file.close();

		m_data->flagChanged = false;
	}
	else
	{	
		qDebug() << "Can not save project file. Can not open project file";
		return false;
	}

	qDebug() << "Save project file failed";
	return false;
}
// ----------------------------------------------------------------
bool WSProject::createProjectFile(QString filePath, i_wsAPIRosreestr* api)
{
	if (filePath.isEmpty() == true)
	{
		qDebug() << "Can not create project file. File path is empty";
		return false;
	}

	if (api == Q_NULLPTR)
	{
		qDebug() << "Can not create project file. APIRosreestr is NULL";
		return false;
	}


	QString login = api->getLogin();
	if (login.isEmpty() == true)
	{
		qDebug() << "Login user is empty";
		return false;
	}

	QByteArray loginHex = QCryptographicHash::hash(login.toUtf8(), QCryptographicHash::Sha256).toHex();


	QDomDocument doc("APIRosreestr");
	QDomElement domElement = doc.createElement("APIRosreestr");
	doc.appendChild(domElement);

	// Version
	{
		QDomElement element = doc.createElement("version");
		QDomAttr domAttr = doc.createAttribute("number");
		domAttr.setValue("1.0");
		element.setAttributeNode(domAttr);
		domElement.appendChild(element);
	}

	// user
	{
		QDomElement element = doc.createElement("user");
		QDomAttr domAttr = doc.createAttribute("name");
		domAttr.setValue(loginHex);
		element.setAttributeNode(domAttr);
		domElement.appendChild(element);
	}

	// data
	{
		QDomElement element = doc.createElement("data");
		QDomAttr domAttr = doc.createAttribute("dataCreated");
		domAttr.setValue(QDate::currentDate().toString());
		element.setAttributeNode(domAttr);
		domElement.appendChild(element);
	}

	// Раздел объекты
	{
		QDomElement element = doc.createElement("objects");		
		domElement.appendChild(element);
	}

	// Закрываем файл
	QFile file(filePath);
	if (file.open(QIODevice::WriteOnly)) {
		QTextStream(&file) << doc.toString();
		file.close();
	}						

	return true;
}
// ----------------------------------------------------------------
bool WSProject::isValidProjectFile(QString filePath)
{
	if (filePath.isEmpty() == true)
	{
		qDebug() << "Can not create project file. File path is empty";
		return false;
	}

	if (!QFileInfo(filePath).exists())
	{
		qDebug() << "File is not found";
		return false;
	}
			
	QFile file(filePath);
	if (file.open(QIODevice::ReadOnly) == true)
	{
		QDomDocument doc;
		doc.setContent(&file);
		QDomElement domElement = doc.documentElement();

		if (domElement.tagName() == "APIRosreestr")
			return true;
	}
	else
	{
		qDebug() << "Can not open for read project file";
		return false;
	}


	qDebug() << "Checkig project file failed";
	return false;
}
// ----------------------------------------------------------------
WSProject* WSProject::create(QString filePath, QObject* APIRosreestr)
{
	return (new WSProject(filePath, APIRosreestr));
}
// ----------------------------------------------------------------