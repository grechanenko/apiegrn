#pragma once

#include <QObject>
#include <QJsonDocument>

#include "i_wsConnectSocket.h"
#include "wsconnect_global.h"

class QJsonDocument;
class WSCONNECT_EXPORT WSConnectSocket : public QObject, public virtual i_wsConnectSocket
{
	Q_OBJECT
	Q_INTERFACES(i_wsConnectSocket)
	DECLARE_SMARTPOINTER

public:
	WSConnectSocket(QObject* parent = 0);
	~WSConnectSocket();


	/*!
		Подключиться
	*/
	virtual void connectServer(QString url, int port);


	/*!
		Отключиться
	*/
	virtual void disconnectServer() const;


	/*!
		true - если подключен
	*/
	virtual bool isConnected() const;


	/*!
		Создает себя
	*/
	static WSConnectSocket* create();


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Сообщает об установлении соединения с сервером
	*/
	void sig_connect();


	/*!
	*/
	void sig_ready();


	/*!
		Сообщает о потери связи с сервером
	*/
	void sig_disconnect();				


	/*!
		Высылает полученные данные от сервера
	*/
	void sig_messageServer(QJsonDocument);	


	/*!
		Высылает информационное сообщение
	*/
	void sig_message(QString title, QString message);


private slots:

	/*!
		Сообщает об ошибке
	*/
	void slt_error(int);

	/*!
		Получить данные от сервера
	*/
	void slt_readyRead();


	/*!
		Вызывается при подключение к серверу
	*/
	void slt_connected();


	/*!

	*/
	void slt_ready();
};
