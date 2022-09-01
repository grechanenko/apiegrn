#pragma once

#include <QObject>

class QNetworkReply;
class AutoCheckVersion : public QObject
{
	Q_OBJECT
public:

	AutoCheckVersion(int interval, QObject* parent = 0);
	~AutoCheckVersion();

	/*!
		��������� ������� ����������
	*/
	void checkVersion();

signals:

	/*!
		���������� ���� ���� ����������
	*/
	void update();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		��������� �������
	*/
	void start();


private slots:

	/*
	*/
	void slot_update();
};
