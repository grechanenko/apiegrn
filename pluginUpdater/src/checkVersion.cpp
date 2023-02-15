#include <QtXml>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTextCodec>

#include "checkVersion.h"

// -------------------------------------------------
struct CheckVersion::SDATA
{	
	QTextCodec* codec = Q_NULLPTR;
	QNetworkAccessManager* manager = Q_NULLPTR;
	QString versionApp = QString();
	QString versionPack = QString();
};
// -------------------------------------------------
CheckVersion::CheckVersion(QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->manager = new QNetworkAccessManager(this);
	connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));	
}
// -------------------------------------------------
CheckVersion::~CheckVersion()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		if (m_data->manager != Q_NULLPTR)
		{
			delete m_data->manager;
			m_data->manager = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// -------------------------------------------------
void CheckVersion::start()
{
	qInfo() << "Start checking version program";

	if (m_data == Q_NULLPTR)
	{
		qCritical() << "m_data is null";
		emit notupdate(m_data->codec->toUnicode("Система не может запустить процесс проверки обновления"));
		emit finished();		
		return;
	}

	
	QString path = qApp->applicationDirPath() + QString("/components.xml");

	QFile f(path);
	if (!f.open(QIODevice::ReadOnly))
	{		
		emit notupdate(m_data->codec->toUnicode("Нарушена файловая структура приложения"));
		emit finished();
		return;
	}
	
	QXmlStreamReader xmlReader;
	xmlReader.setDevice(&f);
	xmlReader.readNext();   // Переходит к первому элементу в файле
	
	while (!xmlReader.atEnd())
	{
		/* 
		*	Проверяем, является ли элемент началом тега
		*/
		if (xmlReader.isStartElement())
		{
			if (xmlReader.name() == "ApplicationVersion")
			{
				m_data->versionApp = xmlReader.readElementText();
			}

			if (xmlReader.name() == "Version")
			{
				m_data->versionPack = xmlReader.readElementText();
			}
		}
		xmlReader.readNext(); // Переходим к следующему элементу файла
	}
	f.close(); // Закрываем файл


	QString request = QString("https://desktop.apiservis.ru/version.php?version=pack");
	
	
	
	if (m_data->manager->get(QNetworkRequest(QUrl(request))) == Q_NULLPTR)
	{
		emit notupdate(m_data->codec->toUnicode("Сервер обновлений не доступен"));
		emit finished();	
	}	
}
// -------------------------------------------------
void CheckVersion::replyFinished(QNetworkReply* reply)
{
	qDebug() << "Received a message from the update server";

	if (reply != Q_NULLPTR)
	{
		if (reply->error() == QNetworkReply::NoError)
		{
			QString html = QString(reply->readAll());
			if (html.length() > 0)
			{					
				if (m_data->versionPack.toLower() != html.toLower())
				{
					qInfo() << "Received update notification message";
					emit update();
					emit finished();
					return;
				}
				else
				{
					qInfo() << "Received no update message";
					emit notupdate(m_data->codec->toUnicode("У Вас установлена последняя версия программы"));
					emit finished();
					return;
				}
			}
		}
		else
		{
			qDebug() << "Reply error = " << reply->error();
		}
		reply->deleteLater();
	}
	
	qDebug() << "Reply is null";
	emit notupdate(m_data->codec->toUnicode("Во время проверки наличия обновлений произошел сбой"));
	emit finished();
}
// -------------------------------------------------