#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkCookieJar>
#include <QNetworkCookie>
#include <QJsonDocument>
#include <QJsonObject>
#include <QList>

#include "wsAuthServer.h"
#include "i_wsSettingsCore.h"

struct WSAuthServer::SDATA
{	
	QNetworkAccessManager* manager = Q_NULLPTR;
	QString log = QString();
	QString pas = QString();
	i_wsSettingsCore* settings = Q_NULLPTR;
};
//--------------------------------------------
WSAuthServer::WSAuthServer(QString log, QString pas, i_wsSettingsCore* settings, QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	
	m_data->log = log;
	m_data->pas = pas;
	m_data->settings = settings;

	m_data->manager = new QNetworkAccessManager(this);
	connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
//--------------------------------------------
WSAuthServer::~WSAuthServer()
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
void WSAuthServer::start()
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
		emit s_connectid(false);
		emit finished();
	}
}
//--------------------------------------------
void WSAuthServer::replyFinished(QNetworkReply* reply)
{	
	QList<QNetworkCookie> cooke = qvariant_cast<QList<QNetworkCookie>>(reply->header(QNetworkRequest::SetCookieHeader));
	
	delete reply;
	reply = Q_NULLPTR;
		
	if ( (m_data != Q_NULLPTR) && (m_data->settings != Q_NULLPTR) && (cooke.size() >= 3) )
	{					
		m_data->settings->setSetting("auth/sessionid", QString(cooke[0].value()));
		m_data->settings->setSetting("auth/userid", QString(cooke[1].value()));
		m_data->settings->setSetting("auth/autologin", QString(cooke[2].value()));
				
		emit s_connectid(true);
		emit finished();
		return;
	}


	emit s_connectid(false);
	emit finished();
}
//--------------------------------------------