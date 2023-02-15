#pragma once

#include <QObject>

class QNetworkReply;
class i_wsSettingsCore;

class WSAuthServer : public QObject
{
	Q_OBJECT
public: 

	WSAuthServer(QString log, QString pas, i_wsSettingsCore* settings, QObject* parent = 0);
	~WSAuthServer();		

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!	
		Высылается при подключении к серверу
	*/
	void s_connectid(bool);


	/*!
		Высылается по окончанию работы
	*/
	void finished();


public slots:

	/*!
		Подключить к АПИРосреестру
	*/
	void start();

	/*!
		Получить ответ от сервера
	*/
	void replyFinished(QNetworkReply*);
};
