#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QTextCodec>

#include "i_wsCore.h"
#include "i_wsAPIRosreestr.h"
#include "wgtMenu.h"
#include "wgtLogin.h"

// ----------------------------------------------------------------
struct WGTMenu::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
	QAction* actionConnect = Q_NULLPTR;
	QAction* actionDisconnect = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTMenu::WGTMenu(i_wsCore* wsCore, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}
	m_data->wsCore = wsCore;

	m_data->wsCore->createSetting("auth/remember", false, false);
	m_data->wsCore->createSetting("auth/login", QString(""), false);
	m_data->wsCore->createSetting("auth/pass", QString(""), false);


	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	bool flagConnect = false;
	QObject* objAPIRosreestr = Q_NULLPTR;	
	objAPIRosreestr = m_data->wsCore->getAPIRosreestr();
	if (objAPIRosreestr != Q_NULLPTR)
	{
		i_wsAPIRosreestr* wsAPI = Q_NULLPTR;
		wsAPI = qobject_cast<i_wsAPIRosreestr*>(objAPIRosreestr);
		if (wsAPI != Q_NULLPTR)
		{
			flagConnect = wsAPI->isConnecting();
		}
		else
			qDebug() << "wsAPI is NULL";
		
		connect(objAPIRosreestr, SIGNAL(connected(bool)), SLOT(setEnabledForConnection(bool)));
		connect(objAPIRosreestr, SIGNAL(disconnect()), SLOT(slt_serverDisconnect()));
	}
	else
		qDebug() << "Object APIRosreest is NULL";


	QMenu* menu = m_data->wsCore->addMenu(m_data->codec->toUnicode("Личный кабинет"));

	m_data->actionConnect = menu->addAction(m_data->codec->toUnicode("Вход"));
	connect(m_data->actionConnect, SIGNAL(triggered()), this, SLOT(slt_connect()));
	m_data->actionConnect->setEnabled(false);

	m_data->actionDisconnect = menu->addAction(m_data->codec->toUnicode("Выход"));
	//connect(m_data->actionDisconnect, SIGNAL(triggered()), this, SLOT(slt_disconnect()));
	m_data->actionDisconnect->setEnabled(false);

	this->setEnabledForConnection(flagConnect);
}
// ----------------------------------------------------------------
WGTMenu::~WGTMenu()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->wsCore = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void WGTMenu::setEnabledForConnection(bool flag)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->actionConnect != Q_NULLPTR)
	{
		m_data->actionConnect->setEnabled(!flag);
	}
	else
		qDebug() << "Action Connect is NULL";

	if (m_data->actionDisconnect != Q_NULLPTR)
	{
		m_data->actionDisconnect->setEnabled(flag);
	}
	else
		qDebug() << "Action Disconnect is NULL";
}
// ----------------------------------------------------------------
void WGTMenu::slt_connect()
{
	WGTLogin* dialog = Q_NULLPTR;
	dialog = new WGTLogin(m_data->wsCore);
	if (dialog != Q_NULLPTR)
	{		
		if (dialog->exec() == QDialog::Accepted)
		{
		}

		delete dialog;		
	}
}
// ----------------------------------------------------------------
void WGTMenu::slt_disconnect()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "WSCore is NULL";
		return;
	}

	QObject* objAPIRosreestr = Q_NULLPTR;
	objAPIRosreestr = m_data->wsCore->getAPIRosreestr();
	if (objAPIRosreestr == Q_NULLPTR)
	{
		qDebug() << "Object APIRosreestr is NULL";
		return;
	}

	i_wsAPIRosreestr* wsAPI = Q_NULLPTR;
	wsAPI = qobject_cast<i_wsAPIRosreestr*>(objAPIRosreestr);
	if (wsAPI == Q_NULLPTR)
	{
		qDebug() << "Can not convert Object* to wsAPIRosreestr*";
		return;
	}

	wsAPI->disconnect();

	m_data->wsCore->setSetting("auth/remember", false);
	m_data->wsCore->setSetting("auth/login", QString(""));
	m_data->wsCore->setSetting("auth/pass", QString(""));
}
// ----------------------------------------------------------------
void WGTMenu::slt_serverDisconnect()
{
	this->setEnabledForConnection(false);
}
// ----------------------------------------------------------------