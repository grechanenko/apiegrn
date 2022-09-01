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
		Создать файл проекта
	*/
	void slt_create();


	/*!
		Открыть файл проекта
	*/
	void slt_open();
};