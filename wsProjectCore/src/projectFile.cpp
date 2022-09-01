#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QCryptographicHash>
#include <QDate>

#include "i_wsAPIRosreestr.h"
#include "i_wsSettingsCore.h"
#include "projectFile.h"

// -------------------------------------------------
struct ProjectFile::SDATA
{
	QString filePath = QString();
	QFile* file = Q_NULLPTR;
};
// -------------------------------------------------
ProjectFile::ProjectFile(QString filePath, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	if (filePath.isEmpty() == true)
	{
		qDebug() << "File path is empty";
		return;
	}
	
	m_data->filePath = filePath;	
	m_data->file = new QFile(m_data->filePath);
	bool flag = m_data->file->open(QIODevice::ReadWrite);
	if (flag == false)
		qDebug() << "Can not open project file";
}
// -------------------------------------------------
ProjectFile::~ProjectFile()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->file != Q_NULLPTR)
		{
			m_data->file->close();
			delete m_data->file;
			m_data->file = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// -------------------------------------------------
bool ProjectFile::addObject(JsonParser* parser)
{
	if (parser == Q_NULLPTR)
	{
		qDebug() << "Parser object is NULL";
		return false;
	}


}
// -------------------------------------------------
bool ProjectFile::create(QString filePath, i_wsAPIRosreestr* api)
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

	QFile file(filePath);
	file.open(QIODevice::WriteOnly);

	QXmlStreamWriter xmlWriter(&file);
	xmlWriter.setAutoFormatting(true);		// Устанавливаем автоформатирование текста	
	xmlWriter.writeStartDocument();			// Запускаем запись в документ
	// version file project
	xmlWriter.writeStartElement("version"); // Записываем первый элемент с его именем
	xmlWriter.writeCharacters("1.0");		// Записываем версию файла проекта
	xmlWriter.writeEndElement();			// Закрываем тег 
	// user
	xmlWriter.writeStartElement("user");    // Записываем первый элемент с его именем
	xmlWriter.writeCharacters(loginHex);	// Записываем HASH в тело этого элемента
	xmlWriter.writeEndElement();			// Закрываем тег 
	// data
	xmlWriter.writeStartElement("data");    // Записываем первый элемент с его именем
	xmlWriter.writeCharacters(QDate::currentDate().toString());	// Записываем HASH в тело этого элемента
	xmlWriter.writeEndElement();			// Закрываем тег 


	xmlWriter.writeEndDocument();			// Завершаем запись в документ
	file.close();							// Закрываем файл	

	return true;
}
// -------------------------------------------------
bool ProjectFile::isValid(QString filePath)
{
	/*if (filePath.isEmpty() == true)
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
	file.open(QIODevice::ReadOnly);

	QXmlStreamReader xmlReader(&file);
	
	bool flag = false;
	QString ver = xmlReader.readElementText();
	if (ver == "1.0");
	{
		flag = true;
	}

	file.close();

	return flag;*/

	return true;
}
// -------------------------------------------------
bool ProjectFile::isValidUser(QString filePath)
{

	return false;
}
// -------------------------------------------------