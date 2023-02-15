#pragma once

#include <QDialog>
#include "wsauth_global.h"

class i_wsSettingsCore;
class WSAUTH_EXPORT WSAuthDialog : public QDialog
{
	Q_OBJECT

public:
	WSAuthDialog(i_wsSettingsCore* settings, QDialog* parent = 0);
	~WSAuthDialog();

	/*!
		true - если пользователь авторизован
	*/
	static bool isAuth(i_wsSettingsCore* settings);


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
	void slt_connectid(bool flag);

};