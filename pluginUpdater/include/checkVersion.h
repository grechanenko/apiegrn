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
		Высылается если есть обновление
	*/
	void update();
	
	/*!
		Высылается если нет обносления
	*/
	void notupdate(QString);

	/*!
		Сообщает о завершении работы
	*/
	void finished();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Запустить процесс
	*/
	void start();

private slots:

	/*!
		Получить ответ от сервера
	*/
	void replyFinished(QNetworkReply*);

};
