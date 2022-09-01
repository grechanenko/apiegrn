#pragma once

#include <QObject>

class QNetworkReply;
class i_wsSettingsCore;

class WSAuthServer : public QObject
{
	Q_OBJECT
public: 

	WSAuthServer(QString log, QString pas, i_wsSettingsCore* settings, QObject* parent = 0);
	~WSAuthServer();		

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!	
		���������� ��� ����������� � �������
	*/
	void s_connectid(bool);


	/*!
		���������� �� ��������� ������
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
