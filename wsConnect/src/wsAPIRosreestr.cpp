#include <QDebug>
#include <QTextCodec>
#include <QThread>

#include "wsCadSearch.h"
#include "wsAPIRosreestr.h"
#include "i_wsCore.h"
#include "i_wsProject.h"


IMPLEMENT_SMARTPOINTER(WSAPIRosreestr)
// ----------------------------------------------------------------
struct WSAPIRosreestr::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;	
	
	bool connected = false;
	QString login = QString();
	QString pass = QString();
	QString sessionid = QString();
	QString userid = QString();
	QString autologin = QString();
	QString token = QString("T8A4-OGZ3-HBWR-T59O");
};
// ----------------------------------------------------------------
WSAPIRosreestr::WSAPIRosreestr(i_wsCore* wsCore, QObject* parent)
	:QObject(parent)
{
	qRegisterMetaType<WS_CONNECT_SERVER_INFO>("WS_CONNECT_SERVER_INFO");

	m_data = Q_NULLPTR;
	m_data = new SDATA;

	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}
	m_data->wsCore = wsCore;
}
// ----------------------------------------------------------------
WSAPIRosreestr::~WSAPIRosreestr()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->wsCore = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void WSAPIRosreestr::connecting(QString login, QString password)
{
	if ((login.isEmpty() == true) || (password.isEmpty() == true))
	{
		qDebug() << "Login or password is empty";
		emit connected(false);
		return;
	}

	WSConnectAPIRosreestrServer* connectServer = Q_NULLPTR;
	connectServer = new WSConnectAPIRosreestrServer(login, password);
	QThread* thread = new QThread;
	connectServer->moveToThread(thread);

	// Получить результат
	connect(connectServer, SIGNAL(connected(WS_CONNECT_SERVER_INFO)), SLOT(slt_connected(WS_CONNECT_SERVER_INFO)));

	// Запустить процесс
	connect(thread, SIGNAL(started()), connectServer, SLOT(start()));
	
	// Остановить поток
	connect(connectServer, SIGNAL(finished()), thread, SLOT(quit()));
	
	// Удалить класс
	connect(connectServer, SIGNAL(finished()), connectServer, SLOT(deleteLater()));

	// Удалить поток 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	// Запустить поток
	thread->start();		
}
// ----------------------------------------------------------------
bool WSAPIRosreestr::isConnecting() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	return m_data->connected;
}
// ----------------------------------------------------------------
void WSAPIRosreestr::disconnect() const
{
	if (m_data != Q_NULLPTR)
	{
		m_data->connected = false;
		m_data->autologin = QString();
		m_data->login = QString();
		m_data->pass = QString();
		m_data->sessionid = QString();
		m_data->userid = QString();
	}
	else
		qDebug() << "m_data is NULL";

	emit disconnect();
}
// ----------------------------------------------------------------
QString WSAPIRosreestr::getLogin() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QString();
	}
	return m_data->login;
}
// ----------------------------------------------------------------
QString WSAPIRosreestr::getToken() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QString();
	}
	return m_data->token;
}
// ----------------------------------------------------------------
bool WSAPIRosreestr::findObject(QString query, i_wsProject* project) const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	if (m_data->token.isEmpty() == true)
	{
		qDebug() << "Can not find object. Token is empty";
		return false;
	}	

	if (query.isEmpty() == true)
	{
		qDebug() << "Query is NULL";
		return false;
	}

	if (project == Q_NULLPTR)
	{
		qDebug() << "Project object is NULL";
		return false;
	}

	WSCadSearch* cadSearch = Q_NULLPTR;
	cadSearch = new WSCadSearch(query, m_data->token, project);
	QThread* thread = new QThread;
	cadSearch->moveToThread(thread);

	// Получить результат
	connect(cadSearch, SIGNAL(result(QObject*, i_wsProject*)), SLOT(slt_cadSearch(QObject*, i_wsProject*)));
	connect(cadSearch, SIGNAL(sig_message(QString, QString)), SIGNAL(sig_message(QString, QString)));

	// Запустить процесс
	connect(thread, SIGNAL(started()), cadSearch, SLOT(start()));

	// Остановить поток
	connect(cadSearch, SIGNAL(finished()), thread, SLOT(quit()));

	// Удалить класс
	connect(cadSearch, SIGNAL(finished()), cadSearch, SLOT(deleteLater()));

	// Удалить поток 
	connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

	// Запустить поток
	thread->start();

	return true;
}
// ----------------------------------------------------------------
WSAPIRosreestr* WSAPIRosreestr::create(i_wsCore* wsCore)
{
	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return Q_NULLPTR;
	}
	return (new WSAPIRosreestr(wsCore));
}
// ----------------------------------------------------------------
void WSAPIRosreestr::slt_connected(WS_CONNECT_SERVER_INFO si)
{
	if (si.connected == false)
	{
		qDebug() << "Connect to APIRosreestr server failed";
		emit connected(false);		
		return;
	}

	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		emit connected(false);
		return;
	}

	m_data->connected = si.connected;
	m_data->autologin = si.autologin;
	m_data->login = si.login;
	m_data->pass = si.pass;
	m_data->sessionid = si.sessionid;
	m_data->userid = si.userid;
	
	qDebug() << "Connected to APIRosreestr server";
	emit connected(true);
}
// ----------------------------------------------------------------
void WSAPIRosreestr::slt_cadSearch(QObject* data, i_wsProject* project)
{
	if (data == Q_NULLPTR)
	{
		qDebug() << "Data is NULL";
		emit sig_search(false);
		return;
	}

	if (project == Q_NULLPTR)
	{
		qDebug() << "Project is NULL";
		emit sig_search(false);
		return;
	}

	QString filePaht = project->filePath();


	if (project->addObject(data) == false)
	{
		qDebug() << "Can not add object to file project";		
	}

	project->save();

	emit sig_search(true);
}
// ----------------------------------------------------------------