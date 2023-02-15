#pragma once

/*!

signals:
	

*/

#include "i_base.h"


class i_wsConnectHttp : public i_base
{
public:
	virtual ~i_wsConnectHttp() {}


	/*!
		������������
	*/
	virtual void connectServer(QString login, QString password) const = 0;


	/*!
		�����������
	*/
	virtual void disconnectServer() const = 0;


	/*!
		true - ���� ���������
	*/
	virtual bool isConnected() const = 0;
};

Q_DECLARE_INTERFACE(i_wsConnectHttp, "launcher.i_wsConnectHttp/1.0")