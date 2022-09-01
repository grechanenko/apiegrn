#pragma once

#include <QObject>

struct WS_CONNECT_SERVER_INFO
{
	bool connected;
	QString login;
	QString pass;
	
	QString sessionid;
	QString userid;
	QString autologin;
};

class QNetworkReply;
class WSConnectAPIRosreestrServer : public QObject
{
	Q_OBJECT
public: 

	WSConnectAPIRosreestrServer(QString log, QString pas, QObject* parent = 0);
	~WSConnectAPIRosreestrServer();

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!	
		���������� ��� ����������� � �������
	*/
	void connected(WS_CONNECT_SERVER_INFO);


	/*!
		��������� � ���������� ������
	*/
	void finished();


public slots:

	/*!
		���������� � �������������
	*/
	void start();

	/*!
		�������� ����� �� �������
	*/
	void replyFinished(QNetworkReply*);
};
