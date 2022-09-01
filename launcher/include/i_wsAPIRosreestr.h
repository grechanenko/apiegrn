#pragma once

/*!
	signals:

	void sig_connected();							// Сообщение об успешной идентификации
	void sig_disconnect();							// Сообщение об отключении
	void sig_message(QString title, QString text);	// Информационное сообщение

*/

#include "i_base.h"

class i_wsProject;
class i_wsAPIRosreestr : public i_base
{
public:
	virtual ~i_wsAPIRosreestr() {}


	/*!
		Выполнить вход в учетную запись
	*/
	virtual void connecting(QString login, QString password) = 0;


	/*!
		true - если установлена связь с сервером
	*/
	virtual bool isConnecting() const = 0;


	/*!
		Выйти из учетной записи 
	*/
	virtual void disconnect() const = 0;
	

	/*!
		Возвращает логин пользователя
	*/
	virtual QString getLogin() const = 0;


	/*!
		Возвращает TOKEN пользователя
	*/
	virtual QString getToken() const = 0;


	/*!
		Найти объект: 
		query - адрес или кадастровый номер.
		project - проект в который записать результат.
	*/
	virtual bool findObject(QString query, i_wsProject* project) const = 0;

};
Q_DECLARE_INTERFACE(i_wsAPIRosreestr, "launcher.i_wsAPIRosreestr/1.0")