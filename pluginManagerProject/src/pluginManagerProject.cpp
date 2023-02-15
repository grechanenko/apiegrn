#include <QIcon>
#include <QDebug>
#include <QTextCodec>

#include "pluginManagerProject.h"
#include "i_wsCore.h"
//#include "i_wsProjects.h"
#include "wgtManager.h"

IMPLEMENT_SMARTPOINTER(PluginManagerProject)
// ------------------------------------------------------------------
struct PluginManagerProject::SDATA
{
	QTextCodec* codec = Q_NULLPTR;	
	i_wsCore* ws = Q_NULLPTR;
};
// ------------------------------------------------------------------
PluginManagerProject::PluginManagerProject(QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ------------------------------------------------------------------
PluginManagerProject::~PluginManagerProject()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->ws = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ------------------------------------------------------------------
bool PluginManagerProject::initialize(QObject* wsCore)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}
	
	m_data->ws = qobject_cast<i_wsCore*>(wsCore);	

	WGTManager* manager = Q_NULLPTR;
	manager = new WGTManager(m_data->ws->getProjects());

	m_data->ws->addDockWidget(Qt::LeftDockWidgetArea, manager);

	return true;
}
// ------------------------------------------------------------------
QString PluginManagerProject::getName() const
{
	return QString("pluginManagerProject");
}
// ----------------------------------------------------------------
QStringList PluginManagerProject::references() const
{
	return QStringList();
}
// ----------------------------------------------------------------
QIcon PluginManagerProject::getIcon() const
{
	return QIcon();
}
// ----------------------------------------------------------------
QString PluginManagerProject::about() const
{
	return QString(tr("Plugin manager project files"));
}