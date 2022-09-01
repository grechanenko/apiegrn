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
		Высылает полученные данные от сервера
		QObject - это i_wsJsonParser
	*/
	void result(QObject*, i_wsProject*);


	/*!
		Ответ получен
	*/
	void request(bool);


	/*!
		Сообщение о завершении работы
	*/
	void finished();
	

	/*!
		Сообщение
	*/
	void sig_message(QString title, QString text);


public slots:

	/*!
		Запустить процесс
	*/
	void start();


private slots:


	/*!
		Получить данные от сервера
	*/
	void replyFinished(QNetworkReply*);
};