#pragma once

/*!

signals:

	void sig_connect();									// Сообщает об установлении соединения с сервером
	void sig_disconnect();								// Сообщает о потери связи с сервером
	void sig_messageServer(QJsonDocument);				// Высылает полученные данные от сервера
	void sig_message(QString title, QString message);	// Высылает информационное сообщение

*/

#include "i_base.h"


class i_wsConnectSocket : public i_base
{
public:

	virtual ~i_wsConnectSocket() {}


	/*!
		Подключиться
	*/
	virtual void connectServer(QString url, int port) = 0;


	/*!
		Отключиться
	*/
	virtual void disconnectServer() const = 0;


	/*!
		true - если подключен
	*/
	virtual bool isConnected() const = 0;
};

Q_DECLARE_INTERFACE(i_wsConnectSocket, "launcher.i_wsConnectSocket/1.0")