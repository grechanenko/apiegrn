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
		Проверить наличие обновления
	*/
	void checkVersion();

signals:

	/*!
		Высылается если есть обновление
	*/
	void update();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Запустить процесс
	*/
	void start();


private slots:

	/*
	*/
	void slot_update();
};
