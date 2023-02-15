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
		�������� ������ ��, ���� ������� ����� � ������
	*/
	void slt_enableOkButton(const QString &);


	/*!
		�������� �������� ���
	*/
	void slt_showProgressBar();


	/*!
		������ �������� ���
	*/
	void slt_hideProgressBar();


	/*!
		�������� ��������� ���������������
	*/
	void slt_connected(bool flag);
};