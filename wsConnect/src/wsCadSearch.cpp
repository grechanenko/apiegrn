#include <QObject>
#include <QTextCodec>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QJsonDocument>
#include <QJsonObject>

#include "wsCadSearch.h"
#include "wsJsonParser.h"
#include "i_wsProject.h"
// ---------------------------------------------------------
struct WSCadSearch::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QString query = QString();
	QString token = QString();
	i_wsProject* project = Q_NULLPTR;

	QNetworkAccessManager* manager = Q_NULLPTR;
};
// ---------------------------------------------------------
WSCadSearch::WSCadSearch(QString query, QString token, i_wsProject* project, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);	

	m_data->query = query;
	m_data->token = token;
	m_data->project = project;

	m_data->manager = new QNetworkAccessManager(this);
	connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
// ---------------------------------------------------------
WSCadSearch::~WSCadSearch()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->project = Q_NULLPTR;

		if (m_data->manager != Q_NULLPTR)
		{
			delete m_data->manager;
			m_data->manager = Q_NULLPTR;
		}
				
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ---------------------------------------------------------
void WSCadSearch::start()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		emit sig_message("Finder", "Find process error");
		emit finished();
		return;
	}

	QByteArray token;
	token.append(m_data->token);

	QJsonObject obj;
	obj.insert("query", m_data->query);
	obj.insert("mode", "normal");
	obj.insert("grouped", 0);
	QJsonDocument doc(obj);

	QNetworkRequest request(QUrl("https://apirosreestr.ru/api/cadaster/search"));
	request.setRawHeader("Token", token);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

	if (m_data->manager->post(request, doc.toJson()) == Q_NULLPTR)
	{
		qDebug() << "Server renurn error";
		emit sig_message(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Сервер не отвечает"));
		emit finished();
	}
}
// ---------------------------------------------------------
void WSCadSearch::replyFinished(QNetworkReply* reply)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		emit request(false);
		emit finished();
		return;
	}

	if (reply->error() != QNetworkReply::NoError)
	{
		qDebug() << "Server not request. Error = " << reply->error();
		emit sig_message("Finder", "Server not request");
		emit request(false);
		emit finished();
		return;
	}

	if (reply != Q_NULLPTR)
	{
		WSJsonParser* jsParser = Q_NULLPTR;
		QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
		jsParser = new WSJsonParser(doc, DOC_TYPE::SEARCH);
		reply->deleteLater();

		if (jsParser != Q_NULLPTR)
		{		

			QVector<S_OBJECT> objects = jsParser->objects();
			if (objects.size() < 1)
			{
				qDebug() << "Count object < 1";
				qDebug() << "Json = " << doc;

				QString text = m_data->codec->toUnicode("По Вашему запросу: </br> ") + m_data->query + " </br> " +
					m_data->codec->toUnicode("ничего не найдено. </br>") +
					m_data->codec->toUnicode("Повторите запрос через некоторое время.");

				emit sig_message(m_data->codec->toUnicode("Поиск объектов"), text);
				
			}

			emit result(jsParser, m_data->project);
			emit request(true);
			emit finished();
			return;
		}
				
		qDebug() << "Can not create JsonParser object";
		emit request(false);
		emit finished();
		return;
		
	}

	qDebug() << "Server sending empty request";	

	if (m_data->codec != Q_NULLPTR)
		emit sig_message(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("От сервера пришел пустой ответ"));
	else
		emit sig_message("Finder", "Server sending empty request");

	emit request(false);
	emit finished();
}
// ---------------------------------------------------------