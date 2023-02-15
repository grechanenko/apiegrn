#pragma once

#include <QDialog>
#include "pluginpersonalarea_global.h"

class i_wsCore;
class PLUGINPERSONALAREA_EXPORT WGTLogin: public QDialog
{
	Q_OBJECT

public:
	WGTLogin(i_wsCore* wsCore, QDialog* parent = 0);
	~WGTLogin();
	
private:

	struct SDATA;
	SDATA* m_data;

public slots:


	/*!
		Called when the Ok button is pressed
	*/
	void slt_okButton();


private slots:


	/*!
		Включить кнопку ОК, если указаны логин и пароль
	*/
	void slt_enableOkButton(const QString &);


	/*!
		Показать прогресс бар
	*/
	void slt_showProgressBar();


	/*!
		Скрыть прогресс баз
	*/
	void slt_hideProgressBar();


	/*!
		Получить результат ауидентификации
	*/
	void slt_connected(bool flag);
};