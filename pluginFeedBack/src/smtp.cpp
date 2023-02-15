
#include <QtNetwork/QSslSocket>
#include <QString>
#include <QTextStream>
#include <QDebug>
#include <QtWidgets/QMessageBox>
#include <QByteArray>
#include <QFile>
#include <QFileInfo>
#include <QTextCodec>

#include "smtp.h"

struct Smtp::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	int timeout = 30000;
	QString message = QString();
	QTextStream *t = Q_NULLPTR;
	QSslSocket *socket = Q_NULLPTR;
	QString from = QString();
	QString rcpt = QString();
	QString response = QString();
	QString user = QString();
	QString pass = QString();
	QString host = QString();
	int port = 465;
	enum states { Tls, HandShake, Auth, User, Pass, Rcpt, Mail, Data, Init, Body, Quit, Close };
	int state = 0;
};
// -------------------------------------------------------------------
Smtp::Smtp(const QString &user, const QString &pass, const QString &host, int port, int timeout)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->socket = new QSslSocket(this);

	connect(m_data->socket, SIGNAL(readyRead()), this, SLOT(readyRead()));
	connect(m_data->socket, SIGNAL(connected()), this, SLOT(connected()));
	connect(m_data->socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorReceived(QAbstractSocket::SocketError)));
	connect(m_data->socket, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(stateChanged(QAbstractSocket::SocketState)));
	connect(m_data->socket, SIGNAL(disconnected()), this, SLOT(disconnected()));

	m_data->user = user;
	m_data->pass = pass;
	m_data->host = host;
	m_data->port = port;
	m_data->timeout = timeout;
}
// -------------------------------------------------------------------
Smtp::~Smtp()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		if (m_data->t != Q_NULLPTR)
		{
			delete m_data->t;
			m_data->t = Q_NULLPTR;
		}

		if (m_data->socket != Q_NULLPTR)
		{
			delete m_data->socket;
			m_data->socket = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// -------------------------------------------------------------------
void Smtp::sendMail(const QString &from, const QString &to, const QString &subject, const QString &body, QStringList files)
{
	if (m_data != Q_NULLPTR)
	{
		m_data->message = "To: " + to + "\n";
		m_data->message.append("From: " + from + "\n");
		m_data->message.append("Subject: " + subject + "\n");

		//Let's intitiate multipart MIME with cutting boundary "frontier"
		m_data->message.append("MIME-Version: 1.0\n");
		m_data->message.append("Content-Type: multipart/mixed; boundary=frontier\n\n");



		m_data->message.append("--frontier\n");
		//message.append( "Content-Type: text/html\n\n" );  //Uncomment this for HTML formating, coment the line below
		m_data->message.append("Content-Type: text/plain\n\n");
		m_data->message.append(body);
		m_data->message.append("\n\n");

		if (!files.isEmpty())
		{
			qDebug() << "Files to be sent: " << files.size();
			foreach(QString filePath, files)
			{
				QFile file(filePath);
				if (file.exists())
				{
					if (!file.open(QIODevice::ReadOnly))
					{
						qDebug("Couldn't open the file");
						QMessageBox::warning(0, tr("Qt Simple SMTP client"), tr("Couldn't open the file\n\n"));
						return;
					}
					QByteArray bytes = file.readAll();
					m_data->message.append("--frontier\n");
					m_data->message.append("Content-Type: application/octet-stream\nContent-Disposition: attachment; filename=" + QFileInfo(file.fileName()).fileName() + ";\nContent-Transfer-Encoding: base64\n\n");
					m_data->message.append(bytes.toBase64());
					m_data->message.append("\n");
				}
			}
		}
		else
			qDebug() << "No attachments found";


		m_data->message.append("--frontier--\n");

		m_data->message.replace(QString::fromLatin1("\n"), QString::fromLatin1("\r\n"));
		m_data->message.replace(QString::fromLatin1("\r\n.\r\n"), QString::fromLatin1("\r\n..\r\n"));


		m_data->from = from;
		m_data->rcpt = to;
		m_data->state = m_data->Init;
		m_data->socket->connectToHostEncrypted(m_data->host, m_data->port); //"smtp.gmail.com" and 465 for gmail TLS
		if (!m_data->socket->waitForConnected(m_data->timeout)) {
			qDebug() << m_data->socket->errorString();
		}

		m_data->t = new QTextStream(m_data->socket);


	}
}
// -------------------------------------------------------------------
void Smtp::stateChanged(QAbstractSocket::SocketState socketState)
{
	qDebug() << "stateChanged " << socketState;
}
// -------------------------------------------------------------------
void Smtp::errorReceived(QAbstractSocket::SocketError socketError)
{
	qDebug() << "error " << socketError;
}
// -------------------------------------------------------------------
void Smtp::disconnected()
{
	qDebug() << "disconneted";

	if (m_data != Q_NULLPTR)
		if (m_data->socket != Q_NULLPTR)
			qCritical() << "error " << m_data->socket->errorString();
}
// -------------------------------------------------------------------
void Smtp::connected()
{
	qDebug() << "Connected ";
}
// -------------------------------------------------------------------
void Smtp::readyRead()
{
	qDebug() << "readyRead";
	// SMTP is line-oriented

	if (m_data == Q_NULLPTR)
		return;

	if (m_data->socket == Q_NULLPTR)
		return;
	
	QString responseLine;
	do
	{
		responseLine = m_data->socket->readLine();
		m_data->response += responseLine;
	} while (m_data->socket->canReadLine() && responseLine[3] != ' ');

	responseLine.truncate(3);

	qDebug() << "Server response code:" << responseLine;
	qDebug() << "Server response: " << m_data->response;

	if (m_data->state == m_data->Init && responseLine == "220")
	{
		// banner was okay, let's go on
		*m_data->t << "EHLO localhost" << "\r\n";
		m_data->t->flush();

		m_data->state = m_data->HandShake;
	}
	//No need, because I'm using socket->startClienEncryption() which makes the SSL handshake for you
	/*else if (state == Tls && responseLine == "250")
	{
		// Trying AUTH
		qDebug() << "STarting Tls";
		*t << "STARTTLS" << "\r\n";
		t->flush();
		state = HandShake;
	}*/
	else if (m_data->state == m_data->HandShake && responseLine == "250")
	{
		m_data->socket->startClientEncryption();
		if (!m_data->socket->waitForEncrypted(m_data->timeout))
		{
			qDebug() << m_data->socket->errorString();
			m_data->state = m_data->Close;
		}


		//Send EHLO once again but now encrypted

		*m_data->t << "EHLO localhost" << "\r\n";
		m_data->t->flush();
		m_data->state = m_data->Auth;
	}
	else if (m_data->state == m_data->Auth && responseLine == "250")
	{
		// Trying AUTH
		qDebug() << "Auth";
		*m_data->t << "AUTH LOGIN" << "\r\n";
		m_data->t->flush();
		m_data->state = m_data->User;
	}
	else if (m_data->state == m_data->User && responseLine == "334")
	{
		//Trying User        
		qDebug() << "Username";
		//GMAIL is using XOAUTH2 protocol, which basically means that password and username has to be sent in base64 coding
		//https://developers.google.com/gmail/xoauth2_protocol
		*m_data->t << QByteArray().append(m_data->user).toBase64() << "\r\n";
		m_data->t->flush();

		m_data->state = m_data->Pass;
	}
	else if (m_data->state == m_data->Pass && responseLine == "334")
	{
		//Trying pass
		qDebug() << "Pass";
		*m_data->t << QByteArray().append(m_data->pass).toBase64() << "\r\n";
		m_data->t->flush();

		m_data->state = m_data->Mail;
	}
	else if (m_data->state == m_data->Mail && responseLine == "235")
	{
		// HELO response was okay (well, it has to be)

		//Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
		qDebug() << "MAIL FROM:<" << m_data->from << ">";
		*m_data->t << "MAIL FROM:<" << m_data->from << ">\r\n";
		m_data->t->flush();
		m_data->state = m_data->Rcpt;
	}
	else if (m_data->state == m_data->Rcpt && responseLine == "250")
	{
		//Apperantly for Google it is mandatory to have MAIL FROM and RCPT email formated the following way -> <email@gmail.com>
		*m_data->t << "RCPT TO:<" << m_data->rcpt << ">\r\n"; //r
		m_data->t->flush();
		m_data->state = m_data->Data;
	}
	else if (m_data->state == m_data->Data && responseLine == "250")
	{

		*m_data->t << "DATA\r\n";
		m_data->t->flush();
		m_data->state = m_data->Body;
	}
	else if (m_data->state == m_data->Body && responseLine == "354")
	{

		*m_data->t << m_data->message << "\r\n.\r\n";
		m_data->t->flush();
		m_data->state = m_data->Quit;
	}
	else if (m_data->state == m_data->Quit && responseLine == "250")
	{

		*m_data->t << "QUIT\r\n";
		m_data->t->flush();
		// here, we just close.
		m_data->state = m_data->Close;
		emit status(m_data->codec->toUnicode("Сообщение отправлено"));
	}
	else if (m_data->state == m_data->Close)
	{
		//deleteLater();
		return;
	}
	else
	{
		// something broke.
		//QMessageBox::warning(0, tr("Qt Simple SMTP client"), tr("Unexpected reply from SMTP server:\n\n") + m_data->response);
		qCritical() << m_data->codec->toUnicode("Ошибка при отправке отзыва с SMTP: ") << m_data->response;
		m_data->state = m_data->Close;
		emit status(m_data->codec->toUnicode("При отправке сообщения произошла ошибка"));
	}
	m_data->response = "";
}
// -------------------------------------------------------------------