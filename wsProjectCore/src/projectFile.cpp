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
	xmlWriter.setAutoFormatting(true);		// ������������� ������������������ ������	
	xmlWriter.writeStartDocument();			// ��������� ������ � ��������
	// version file project
	xmlWriter.writeStartElement("version"); // ���������� ������ ������� � ��� ������
	xmlWriter.writeCharacters("1.0");		// ���������� ������ ����� �������
	xmlWriter.writeEndElement();			// ��������� ��� 
	// user
	xmlWriter.writeStartElement("user");    // ���������� ������ ������� � ��� ������
	xmlWriter.writeCharacters(loginHex);	// ���������� HASH � ���� ����� ��������
	xmlWriter.writeEndElement();			// ��������� ��� 
	// data
	xmlWriter.writeStartElement("data");    // ���������� ������ ������� � ��� ������
	xmlWriter.writeCharacters(QDate::currentDate().toString());	// ���������� HASH � ���� ����� ��������
	xmlWriter.writeEndElement();			// ��������� ��� 


	xmlWriter.writeEndDocument();			// ��������� ������ � ��������
	file.close();							// ��������� ����	

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