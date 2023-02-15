#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDebug>
#include <QTextCodec>

#include "wgtMenu.h"
#include "pluginPersonalArea.h"
#include "i_wsCore.h"
#include "i_wsConnectSocket.h"
#include "i_wsAPIRosreestr.h"

IMPLEMENT_SMARTPOINTER(PluginPersonalArea)
// ----------------------------------------------------------------
struct PluginPersonalArea::SDATA
{
	QTextCodec* codec = Q_NULLPTR;	
	i_wsCore* ws = Q_NULLPTR;	
	WGTMenu* menu = Q_NULLPTR;
};
// ----------------------------------------------------------------
PluginPersonalArea::PluginPersonalArea()
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);		
}
// ----------------------------------------------------------------
PluginPersonalArea::~PluginPersonalArea()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->ws = Q_NULLPTR;

		if (m_data->menu != Q_NULLPTR)
		{
			delete m_data->menu;
			m_data->menu = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
bool PluginPersonalArea::initialize(QObject* wsCore)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}
	
	m_data->ws = qobject_cast<i_wsCore*>(wsCore);
	if (m_data->ws == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return false;
	}
		
	m_data->menu = new WGTMenu(m_data->ws);

	QObject* objAPIRosreestr = m_data->ws->getAPIRosreestr();
	connect(objAPIRosreestr, SIGNAL(sig_connected()), SLOT(slt_conneted()));
	connect(objAPIRosreestr, SIGNAL(sig_disconnect()), SLOT(slt_disconneted()));


	if (m_data->ws->getSetting("auth/remember") == true)
	{
		QString login = m_data->ws->getSetting("auth/login").toString();
		QString pass = m_data->ws->getSetting("auth/pass").toString();

		if ((login.isEmpty() == false) && (pass.isEmpty() == false))
		{
			QObject* objAPI = Q_NULLPTR;
			objAPI = m_data->ws->getAPIRosreestr();
			if (objAPI != Q_NULLPTR)
			{
				i_wsAPIRosreestr* api = Q_NULLPTR;
				api = qobject_cast<i_wsAPIRosreestr*>(objAPI);
				if (api != Q_NULLPTR)
				{
					api->connecting(login, pass);
				}
				else
					qDebug() << "Can not convert QObject* to i_wsAPIRosreestr";
			}
			else
				qDebug() << "Object APIRosreestr is NULL";
		}
		else
			qDebug() << "Can not auto login. Login or password is not correct";
	}
	
	return true;
}
// ----------------------------------------------------------------
QString PluginPersonalArea::getName() const
{
	return QString("pluginPersonalArea");
}
// ----------------------------------------------------------------
QStringList PluginPersonalArea::references() const
{
	return QStringList();
}
// ----------------------------------------------------------------
QIcon PluginPersonalArea::getIcon() const
{
	return QIcon();
}
// ----------------------------------------------------------------
QString PluginPersonalArea::about() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->codec != Q_NULLPTR)
			return QString(m_data->codec->toUnicode("Модуль предоставляет интерфейс личного кабинета"));
	return QString("Plugin personal area");
}
// ----------------------------------------------------------------
void PluginPersonalArea::slt_conneted()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->menu != Q_NULLPTR)
		{
			m_data->menu->setEnabledForConnection(true);
		}
		else
			qDebug() << "WGTMenu is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------
void PluginPersonalArea::slt_disconneted()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->menu != Q_NULLPTR)
		{
			m_data->menu->setEnabledForConnection(false);
		}
		else
			qDebug() << "WGTMenu is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------