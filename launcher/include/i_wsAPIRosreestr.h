#pragma once

/*!
	signals:

	void sig_connected();							// ��������� �� �������� �������������
	void sig_disconnect();							// ��������� �� ����������
	void sig_message(QString title, QString text);	// �������������� ���������

*/

#include "i_base.h"

class i_wsProject;
class i_wsAPIRosreestr : public i_base
{
public:
	virtual ~i_wsAPIRosreestr() {}


	/*!
		��������� ���� � ������� ������
	*/
	virtual void connecting(QString login, QString password) = 0;


	/*!
		true - ���� ����������� ����� � ��������
	*/
	virtual bool isConnecting() const = 0;


	/*!
		����� �� ������� ������ 
	*/
	virtual void disconnect() const = 0;
	

	/*!
		���������� ����� ������������
	*/
	virtual QString getLogin() const = 0;


	/*!
		���������� TOKEN ������������
	*/
	virtual QString getToken() const = 0;


	/*!
		����� ������: 
		query - ����� ��� ����������� �����.
		project - ������ � ������� �������� ���������.
	*/
	virtual bool findObject(QString query, i_wsProject* project) const = 0;

};
Q_DECLARE_INTERFACE(i_wsAPIRosreestr, "launcher.i_wsAPIRosreestr/1.0")