#pragma once

/*!

signals:

	void sig_connect();									// �������� �� ������������ ���������� � ��������
	void sig_disconnect();								// �������� � ������ ����� � ��������
	void sig_messageServer(QJsonDocument);				// �������� ���������� ������ �� �������
	void sig_message(QString title, QString message);	// �������� �������������� ���������

*/

#include "i_base.h"


class i_wsConnectSocket : public i_base
{
public:

	virtual ~i_wsConnectSocket() {}


	/*!
		������������
	*/
	virtual void connectServer(QString url, int port) = 0;


	/*!
		�����������
	*/
	virtual void disconnectServer() const = 0;


	/*!
		true - ���� ���������
	*/
	virtual bool isConnected() const = 0;
};

Q_DECLARE_INTERFACE(i_wsConnectSocket, "launcher.i_wsConnectSocket/1.0")