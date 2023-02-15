#pragma once

#include <QObject>

class QNetworkReply;
class i_wsProject;
class WSCadSearch : public QObject
{
	Q_OBJECT
public:
	WSCadSearch(QString query, QString token, i_wsProject* project, QObject* parent = 0);
	~WSCadSearch();

private:
	struct SDATA;
	SDATA* m_data;

signals:


	/*!
		�������� ���������� ������ �� �������
		QObject - ��� i_wsJsonParser
	*/
	void result(QObject*, i_wsProject*);


	/*!
		����� �������
	*/
	void request(bool);


	/*!
		��������� � ���������� ������
	*/
	void finished();
	

	/*!
		���������
	*/
	void sig_message(QString title, QString text);


public slots:

	/*!
		��������� �������
	*/
	void start();


private slots:


	/*!
		�������� ������ �� �������
	*/
	void replyFinished(QNetworkReply*);
};