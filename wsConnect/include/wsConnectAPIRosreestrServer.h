#pragma once

#include <QObject>

struct WS_CONNECT_SERVER_INFO
{
	bool connected;
	QString login;
	QString pass;
	
	QString sessionid;
	QString userid;
	QString autologin;
};

class QNetworkReply;
class WSConnectAPIRosreestrServer : public QObject
{
	Q_OBJECT
public: 

	WSConnectAPIRosreestrServer(QString log, QString pas, QObject* parent = 0);
	~WSConnectAPIRosreestrServer();

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!	
		Высылается при подключении к серверу
	*/
	void connected(WS_CONNECT_SERVER_INFO);


	/*!
		Сообщение о завершении работы
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
