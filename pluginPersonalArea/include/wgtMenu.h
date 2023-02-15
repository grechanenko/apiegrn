#pragma once

#include <QObject>
class i_wsCore;
class WGTMenu : public QObject
{
	Q_OBJECT
public:

	WGTMenu(i_wsCore* wsCore, QObject* parent = 0);
	~WGTMenu();
	
private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Изменить активность кнопок меню в зависимости от подключенного состояния
	*/
	void setEnabledForConnection(bool);

private slots:

	/*!
		Войти в аккаунт
	*/
	void slt_connect();


	/*!
		Выйти из аккаунта
	*/
	void slt_disconnect();


	/*!
		Вызывается при выходе из учетной записи
		по сигналу disconnect() от i_wsAPIRosreestr.
		Устанавливает соответствующую активность кнопок
	*/
	void slt_serverDisconnect();
};