#pragma once

#include <QObject>
#include "i_wsAPIRosreestr.h"
#include "wsconnect_global.h"

#include "wsConnectAPIRosreestrServer.h"

class i_wsCore;
class i_wsProject;
class JsonParser;
class WSCONNECT_EXPORT WSAPIRosreestr : public QObject, public virtual i_wsAPIRosreestr
{
	Q_OBJECT
	Q_INTERFACES(i_wsAPIRosreestr)
	DECLARE_SMARTPOINTER

public:
	WSAPIRosreestr(i_wsCore* wsCore, QObject* parent = 0);
	~WSAPIRosreestr();

	/*!
		Выполнить вход в учетную запись
	*/
	virtual void connecting(QString login, QString password);


	/*!
		true - если установлена связь с сервером
	*/
	virtual bool isConnecting() const;


	/*!
		Выйти из учетной записи
	*/
	virtual void disconnect() const;


	/*!
		Возвращает логин пользователя
	*/
	virtual QString getLogin() const;


	/*!
		Возвращает TOKEN пользователя
	*/
	virtual QString getToken() const;


	/*!
		Найти объект:
		query - адрес или кадастровый номер.
		project - проект в который записать результат.
	*/
	virtual bool findObject(QString query, i_wsProject* project) const;


	/*!
		Создать
	*/
	static WSAPIRosreestr* create(i_wsCore* wsCore);


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылается при подключении к серверу
	*/
	void connected(bool);


	/*!
		Высылается при отключении от сервера
	*/
	void disconnected();


	/*!
		Сообщает о том, что найден новый объект
		flag: true - есть данные
			  false - нет данных

		request - запрос серверу
	*/
	void sig_search(bool flag/*, QString request*/);


	/*!
		Высылает информационное сообщение
	*/
	void sig_message(QString, QString);


private slots:

	/*!
		Получить данные по соединению с сервером
	*/
	void slt_connected(WS_CONNECT_SERVER_INFO);


	/*!
		Получить результат поиска
		data - данные от сервера (i_wsJsonParser)
		project - в этот проект записать полученные данные
	*/
	void slt_cadSearch(QObject* data, i_wsProject* project);
};
Q_DECLARE_METATYPE(WS_CONNECT_SERVER_INFO)