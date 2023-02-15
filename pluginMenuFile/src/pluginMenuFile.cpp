#include <QTextCodec>
#include <QAction>
#include <QDebug>
#include <QFileDialog>

#include "QtnRibbonSystemPopupBar.h"

#include "pluginMenuFile.h"
#include "i_wsCore.h"
#include "i_wsProjects.h"

#include "wgtMenu.h"

IMPLEMENT_SMARTPOINTER(PluginMenuFile)
// ----------------------------------------------------------------
struct PluginMenuFile::SDATA
{
	QTextCodec* codec = Q_NULLPTR;	
	i_wsCore* wsCore = Q_NULLPTR;
	WGTMenu* menu = Q_NULLPTR;	
};
// ----------------------------------------------------------------
PluginMenuFile::PluginMenuFile()
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ----------------------------------------------------------------
PluginMenuFile::~PluginMenuFile()
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
bool PluginMenuFile::initialize(QObject* wsCore)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	m_data->wsCore = qobject_cast<i_wsCore*>(wsCore);

	m_data->menu = new WGTMenu(m_data->wsCore);

	QObject* objAPIRosreestr = m_data->wsCore->getAPIRosreestr();
	connect(objAPIRosreestr, SIGNAL(connected(bool)), SLOT(slt_conneted(bool)));
	connect(objAPIRosreestr, SIGNAL(sig_disconnect()), SLOT(slt_disconneted()));
	
	return true;
}
// ----------------------------------------------------------------
QString PluginMenuFile::getName() const
{
	return QString("pluginMenuFile");
}
// ----------------------------------------------------------------
QStringList PluginMenuFile::references() const
{
	return QStringList();
}
// ----------------------------------------------------------------
QIcon PluginMenuFile::getIcon() const
{
	return QIcon();
}
// ----------------------------------------------------------------
QString PluginMenuFile::about() const
{
	return QString(m_data->codec->toUnicode("Модуль раздела меню \"Файл\""));
}
// ----------------------------------------------------------------
void PluginMenuFile::slt_conneted(bool flag)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->menu != Q_NULLPTR)
		{
			m_data->menu->setEnabledForConnection(flag);
		}
		else
			qDebug() << "WGTMenu is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------
void PluginMenuFile::slt_disconneted()
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
