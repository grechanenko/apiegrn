#pragma once

#include <QObject>
#include <QJsonDocument>

#include "i_wsConnectSocket.h"
#include "wsconnect_global.h"

class QJsonDocument;
class WSCONNECT_EXPORT WSConnectSocket : public QObject, public virtual i_wsConnectSocket
{
	Q_OBJECT
	Q_INTERFACES(i_wsConnectSocket)
	DECLARE_SMARTPOINTER

public:
	WSConnectSocket(QObject* parent = 0);
	~WSConnectSocket();


	/*!
		������������
	*/
	virtual void connectServer(QString url, int port);


	/*!
		�����������
	*/
	virtual void disconnectServer() const;


	/*!
		true - ���� ���������
	*/
	virtual bool isConnected() const;


	/*!
		������� ����
	*/
	static WSConnectSocket* create();


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		�������� �� ������������ ���������� � ��������
	*/
	void sig_connect();


	/*!
	*/
	void sig_ready();


	/*!
		�������� � ������ ����� � ��������
	*/
	void sig_disconnect();				


	/*!
		�������� ���������� ������ �� �������
	*/
	void sig_messageServer(QJsonDocument);	


	/*!
		�������� �������������� ���������
	*/
	void sig_message(QString title, QString message);


private slots:

	/*!
		�������� �� ������
	*/
	void slt_error(int);

	/*!
		�������� ������ �� �������
	*/
	void slt_readyRead();


	/*!
		���������� ��� ����������� � �������
	*/
	void slt_connected();


	/*!

	*/
	void slt_ready();
};
