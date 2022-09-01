#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

#include "wsConnectAPIRosreestrServer.h"

struct WSConnectAPIRosreestrServer::SDATA
{	
	QNetworkAccessManager* manager = Q_NULLPTR;
	QString log = QString();
	QString pas = QString();
};
//--------------------------------------------
WSConnectAPIRosreestrServer::WSConnectAPIRosreestrServer(QString log, QString pas, QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	
	m_data->log = log;
	m_data->pas = pas;
	
	m_data->manager = new QNetworkAccessManager(this);
	connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
//--------------------------------------------
WSConnectAPIRosreestrServer::~WSConnectAPIRosreestrServer()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->manager != Q_NULLPTR)
		{
			delete m_data->manager;
			m_data->manager = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
//--------------------------------------------
void WSConnectAPIRosreestrServer::start()
{
	if (m_data == Q_NULLPTR)
	{
		emit finished();
		return;
	}

	QJsonObject obj;
	obj.insert("phone", m_data->log);
	obj.insert("password", m_data->pas);
	
	QJsonDocument doc(obj);

	QNetworkRequest request(QUrl("https://apirosreestr.ru/api/sign/in"));	
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	//request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/74.0.3729.169 Safari/537.36");
	//request.setRawHeader("accept", "application/json, */*; q=0.01");
	//request.setRawHeader("accept-encoding:", "gzip, deflate, br");
	//request.setRawHeader("accept-language", "ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7");
	//request.setRawHeader("content-length", "38");
	//request.setRawHeader("content-type", "application/x-www-form-urlencoded; charset=UTF-8");
	//request.setRawHeader("cookie", "_ga=GA1.2.1853538772.1558337601; _ym_uid=1558337601140667089; _ym_d=1558337601; _gid=GA1.2.497987379.1559810433; _ym_isad=1; PHPSESSID=39787880c8c9d2e92e67e3821d738a56; user_id2=2245");
	
	
	//request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	//m_data->manager = new QNetworkAccessManager(this);
	//connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

	if (m_data->manager->post(request, doc.toJson()) == Q_NULLPTR)
	{
		qDebug() << "Can not send POST message to server";

		WS_CONNECT_SERVER_INFO si;
		si.connected = false;

		emit connected(si);
		emit finished();
	}
}
//--------------------------------------------
void WSConnectAPIRosreestrServer::replyFinished(QNetworkReply* reply)
{	
	QList<QNetworkCookie> cooke = qvariant_cast<QList<QNetworkCookie>>(reply->header(QNetworkRequest::SetCookieHeader));
	
	delete reply;
	reply = Q_NULLPTR;
		
	WS_CONNECT_SERVER_INFO si;
	si.connected = false;

	if ( (m_data != Q_NULLPTR) && (cooke.size() >= 3) )
	{			
		si.connected = true;
		si.login = m_data->log;
		si.pass = m_data->pas;
		si.sessionid = cooke[0].value();
		si.userid = cooke[1].value();
		si.autologin = cooke[2].value();

		qInfo() << "Connect to APIRosreestr server";
		emit connected(si);
		emit finished();
		return;
	}
	   	
	qDebug() << "Cookie or m_data failed";
	emit connected(si);
	emit finished();
}
//--------------------------------------------