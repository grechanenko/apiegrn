#pragma once

#include <QObject>
#include <QtNetwork/QAbstractSocket>

class Smtp : public QObject
{
	Q_OBJECT

public:
	Smtp(const QString &user, const QString &pass, const QString &host, int port = 465, int timeout = 30000);
	~Smtp();

	/*!
		Отправить письмо
	*/
	void sendMail(const QString &from, const QString &to, const QString &subject, const QString &body, QStringList files = QStringList());

signals:

	/*!
		Высылает статус отправки письма
	*/
	void status(const QString &);

private slots:

	void stateChanged(QAbstractSocket::SocketState socketState);
	void errorReceived(QAbstractSocket::SocketError socketError);
	void disconnected();
	void connected();
	void readyRead();

private:

	struct SDATA;
	SDATA* m_data;
};