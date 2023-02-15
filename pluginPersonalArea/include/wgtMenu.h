#pragma once

#include <QObject>
class i_wsCore;
class WGTMenu : public QObject
{
	Q_OBJECT
public:

	WGTMenu(i_wsCore* wsCore, QObject* parent = 0);
	~WGTMenu();
	
private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		�������� ���������� ������ ���� � ����������� �� ������������� ���������
	*/
	void setEnabledForConnection(bool);

private slots:

	/*!
		����� � �������
	*/
	void slt_connect();


	/*!
		����� �� ��������
	*/
	void slt_disconnect();


	/*!
		���������� ��� ������ �� ������� ������
		�� ������� disconnect() �� i_wsAPIRosreestr.
		������������� ��������������� ���������� ������
	*/
	void slt_serverDisconnect();
};