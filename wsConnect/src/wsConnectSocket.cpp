#include <QDebug>
#include <QTextCodec>
#include <QSslSocket>
#include <QUrl>
#include <QDataStream>


#include "wsConnectSocket.h"

IMPLEMENT_SMARTPOINTER(WSConnectSocket)
// ---------------------------------------------------------
struct WSConnectSocket::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QSslSocket* socket = Q_NULLPTR;

	quint16 nextBlockSize = 0;
};
// ---------------------------------------------------------
WSConnectSocket::WSConnectSocket(QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->socket = new QSslSocket(this);
	connect(m_data->socket, SIGNAL(encrypted()), SLOT(slt_ready()));

	connect(m_data->socket, SIGNAL(connected()), SIGNAL(sig_connect()));	
	connect(m_data->socket, SIGNAL(disconnected()), SIGNAL(sig_disconnect()));
	connect(m_data->socket, SIGNAL(error(int)), SLOT(slt_error(int)));
	connect(m_data->socket, SIGNAL(readyRead()), SLOT(slt_readyRead()));

	connect(m_data->socket, SIGNAL(connected()), this, SLOT(slt_connected()));
}
// ---------------------------------------------------------
WSConnectSocket::~WSConnectSocket()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		if (m_data->socket != Q_NULLPTR)
		{
			delete m_data->socket;
			m_data->socket = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ---------------------------------------------------------
void WSConnectSocket::connectServer(QString url, int port)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->socket == Q_NULLPTR)
	{
		qDebug() << "TcpSocket is NULL";
		return;
	}

	if (url.isEmpty() == true)
	{
		qDebug() << "url is empty";
	}

	//m_data->socket->connectToHost(url, port, QIODevice::ReadOnly);		
	m_data->socket->connectToHostEncrypted(url, port);
}
// ---------------------------------------------------------
void WSConnectSocket::disconnectServer() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->socket == Q_NULLPTR)
	{
		qDebug() << "TcpSocket is NULL";
		return;
	}

	if (m_data->socket->disconnect() == false)
		qDebug() << "Failed disconnect TcpServer";
}
// ---------------------------------------------------------
bool WSConnectSocket::isConnected() const
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	if (m_data->socket == Q_NULLPTR)
	{
		qDebug() << "TcpSocket is NULL";
		return false;
	}

	bool flag = false;
	flag = m_data->socket->isOpen();

	return flag;
}
// ---------------------------------------------------------
WSConnectSocket* WSConnectSocket::create()
{
	return(new WSConnectSocket());
}
// ---------------------------------------------------------
void WSConnectSocket::slt_error(int index)
{
	qDebug() << "Error tcp connection = " << index;

	emit sig_message(m_data->codec->toUnicode("Менеджер уведомлений"), m_data->codec->toUnicode("Сервер уведомлений не доступен"));
}
// ---------------------------------------------------------
void WSConnectSocket::slt_readyRead()
{
	emit sig_message(m_data->codec->toUnicode("wsConnectSocked"), m_data->codec->toUnicode("Читаю данные"));
	QJsonDocument doc;
	QStringList list;
	QDataStream in(m_data->socket);
	in.setVersion(QDataStream::Qt_5_12);
	for (;;) {
		if (!m_data->nextBlockSize) {
			if (m_data->socket->bytesAvailable() < sizeof(quint16)) {
				break;
			}
			in >> m_data->nextBlockSize;
		}

		if (m_data->socket->bytesAvailable() < m_data->nextBlockSize) {
			break;
		}
		
		QString str;
		in >> str;

		//m_ptxtInfo->append(time.toString() + " " + str);
		list.append(str);
		m_data->nextBlockSize = 0;
	}

	emit sig_messageServer(doc);
}
// ---------------------------------------------------------
void WSConnectSocket::slt_connected()
{

	bool flagOpen = m_data->socket->isOpen();
	bool flagReadable = m_data->socket->isReadable();

	qDebug() << "Connected to server";
}
// ---------------------------------------------------------
void WSConnectSocket::slt_ready()
{
	qDebug() << "SSL Ready";
	emit sig_ready();
}
// ---------------------------------------------------------
