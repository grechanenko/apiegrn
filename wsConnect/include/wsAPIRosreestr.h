#pragma once

#include <QObject>
#include "i_wsAPIRosreestr.h"
#include "wsconnect_global.h"

#include "wsConnectAPIRosreestrServer.h"

class i_wsCore;
class i_wsProject;
class JsonParser;
class WSCONNECT_EXPORT WSAPIRosreestr : public QObject, public virtual i_wsAPIRosreestr
{
	Q_OBJECT
	Q_INTERFACES(i_wsAPIRosreestr)
	DECLARE_SMARTPOINTER

public:
	WSAPIRosreestr(i_wsCore* wsCore, QObject* parent = 0);
	~WSAPIRosreestr();

	/*!
		��������� ���� � ������� ������
	*/
	virtual void connecting(QString login, QString password);


	/*!
		true - ���� ����������� ����� � ��������
	*/
	virtual bool isConnecting() const;


	/*!
		����� �� ������� ������
	*/
	virtual void disconnect() const;


	/*!
		���������� ����� ������������
	*/
	virtual QString getLogin() const;


	/*!
		���������� TOKEN ������������
	*/
	virtual QString getToken() const;


	/*!
		����� ������:
		query - ����� ��� ����������� �����.
		project - ������ � ������� �������� ���������.
	*/
	virtual bool findObject(QString query, i_wsProject* project) const;


	/*!
		�������
	*/
	static WSAPIRosreestr* create(i_wsCore* wsCore);


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		���������� ��� ����������� � �������
	*/
	void connected(bool);


	/*!
		���������� ��� ���������� �� �������
	*/
	void disconnected();


	/*!
		�������� � ���, ��� ������ ����� ������
		flag: true - ���� ������
			  false - ��� ������

		request - ������ �������
	*/
	void sig_search(bool flag/*, QString request*/);


	/*!
		�������� �������������� ���������
	*/
	void sig_message(QString, QString);


private slots:

	/*!
		�������� ������ �� ���������� � ��������
	*/
	void slt_connected(WS_CONNECT_SERVER_INFO);


	/*!
		�������� ��������� ������
		data - ������ �� ������� (i_wsJsonParser)
		project - � ���� ������ �������� ���������� ������
	*/
	void slt_cadSearch(QObject* data, i_wsProject* project);
};
Q_DECLARE_METATYPE(WS_CONNECT_SERVER_INFO)