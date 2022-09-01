#pragma once

#include <QObject>

class QNetworkReply;
class CheckVersion : public QObject
{
	Q_OBJECT
public:

	CheckVersion(QObject* parent = 0);
	~CheckVersion();

signals:

	/*!
		���������� ���� ���� ����������
	*/
	void update();
	
	/*!
		���������� ���� ��� ����������
	*/
	void notupdate(QString);

	/*!
		�������� � ���������� ������
	*/
	void finished();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		��������� �������
	*/
	void start();

private slots:

	/*!
		�������� ����� �� �������
	*/
	void replyFinished(QNetworkReply*);

};
