#include <QDebug>
#include <QApplication>
#include <QMenu>
#include <QDockWidget>

#include "wsCore.h"
#include "wsPluginCore.h"
#include "wsProjects.h"
#include "wsConnectSocket.h"
#include "wsAPIRosreestr.h"

#include "mainWindow.h"

#include "i_docViewer.h"
#include "i_wsSettingsCore.h"
#include "i_wsProjects.h"
#include "i_wsConnectSocket.h"

IMPLEMENT_SMARTPOINTER(WSCore)
// ----------------------------------------------------------------
struct WSCore::SDATA
{	
	i_wsPluginCore* plugins = Q_NULLPTR;
	i_wsSettingsCore* settings = Q_NULLPTR;
	WSProjects* projectCore = Q_NULLPTR;
	WSConnectSocket* connectSocket = Q_NULLPTR;
	WSAPIRosreestr* apirosreestr = Q_NULLPTR;

	// GUI
	MainWindow* mainWindow = Q_NULLPTR;	
};
// ----------------------------------------------------------------
WSCore::WSCore(i_wsSettingsCore* settings)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;

	if (settings != Q_NULLPTR)
		m_data->settings = settings;

	m_data->projectCore = WSProjects::create(this);
	connect(m_data->projectCore, SIGNAL(sig_message(QString, QString)), SLOT(showMessageTaskBar(QString, QString)));

	m_data->connectSocket = WSConnectSocket::create();
	connect(m_data->connectSocket, SIGNAL(sig_message(QString, QString)), SLOT(showMessageTaskBar(QString, QString)));

	m_data->apirosreestr = WSAPIRosreestr::create(this);
	connect(m_data->apirosreestr, SIGNAL(sig_message(QString, QString)), SLOT(showMessageTaskBar(QString, QString)));

	m_data->plugins = new WSPluginCore();

	if (qobject_cast<QApplication *>(qApp) != Q_NULLPTR)
		m_data->mainWindow = new MainWindow();
}
// ----------------------------------------------------------------
WSCore::~WSCore()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
QAction* WSCore::addAction(const QIcon& icon, const QString& caption, Qt::ToolButtonStyle style, QMenu* menu)
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->addAction(icon, caption, style, menu);
	return Q_NULLPTR;
}
// ----------------------------------------------------------------v
QMenu* WSCore::addMenu(const QString& caption)
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->addMenu(caption);
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QWidget* WSCore::createTab(QString name)
{
	if (m_data == Q_NULLPTR)
		return Q_NULLPTR;

	if (m_data->mainWindow == Q_NULLPTR)
		return Q_NULLPTR;

	QWidget* page = Q_NULLPTR;
	page = m_data->mainWindow->createTab(name);
	return page;
}
// ----------------------------------------------------------------
QAction* WSCore::addSystemButton(QIcon iconLogo, QString name)
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->addSystemButton(iconLogo, name);
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QWidget* WSCore::createQuickAccessBar()
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->createQuickAccessBar();
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QStatusBar* WSCore::statusBar() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->statusBar();
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
void WSCore::setStatusBar(QStatusBar* statusBar)
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->setStatusBar(statusBar);
}
// ----------------------------------------------------------------
i_docViewer* WSCore::getDocViewer()
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			return m_data->mainWindow->getDocViewer();
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
void WSCore::showMessageTaskBar(QString title, QString text) const
{
	if (m_data != Q_NULLPTR)
		if (m_data->mainWindow != Q_NULLPTR)
			m_data->mainWindow->showMessageTaskBar(title, text);
}
// ----------------------------------------------------------------
void WSCore::addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->mainWindow != Q_NULLPTR)
		{
			m_data->mainWindow->addDockWidget(area, dockwidget);
		}
		else
			qDebug() << "MainWindow is NULL. Can't add dockWidget";
	}
	else
		qDebug() << "m_data is NULL. Can'n add dockWidget";
}
// ----------------------------------------------------------------
int WSCore::loadPlugins(QObject* ws, QObject* splash)
{
	if (m_data == Q_NULLPTR)
		return 0;

	if (m_data->plugins == Q_NULLPTR)
		return 0;

	if (ws == Q_NULLPTR)
		return 0;

	int countPlugins = m_data->plugins->loadPlugins(ws, splash);
	return countPlugins;
}
// ----------------------------------------------------------------
QStringList WSCore::getPluginNames() const
{
	if (m_data == Q_NULLPTR)
		return QStringList();

	if (m_data->plugins == Q_NULLPTR)
		return QStringList();

	return m_data->plugins->getPluginNames();
}
// ----------------------------------------------------------------
QObject* WSCore::getPlugin(QString name) const
{
	if (m_data == Q_NULLPTR)
		return Q_NULLPTR;

	if (m_data->plugins == Q_NULLPTR)
		return Q_NULLPTR;

	if (name.isEmpty() == true)
		return Q_NULLPTR;

	return m_data->plugins->getPlugin(name);
}
// ----------------------------------------------------------------
bool WSCore::registerObject(i_base* object, QString className) const
{
	if (m_data == Q_NULLPTR)
		return false;

	if (m_data->plugins == Q_NULLPTR)
		return false;

	if (object == Q_NULLPTR)
		return false;

	if (className.isEmpty() == true)
		return false;

	if (m_data->plugins->registerObject(object, className) == true)
		return true;

	return false;
}
// ----------------------------------------------------------------
QList<QString> WSCore::getRegisterObjectNames() const
{
	if (m_data == Q_NULLPTR)
		return QList<QString>();

	if (m_data->plugins == Q_NULLPTR)
		return QList<QString>();

	return m_data->plugins->getRegisterObjectNames();
}
// ----------------------------------------------------------------
i_base* WSCore::getResterObject(QString name) const
{
	if (name.isEmpty() == true)
		return Q_NULLPTR;

	if (m_data == Q_NULLPTR)
		return Q_NULLPTR;

	if (m_data->plugins == Q_NULLPTR)
		return Q_NULLPTR;

	i_base* object = m_data->plugins->getResterObject(name);

	if (object != Q_NULLPTR)
		return object;

	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QMainWindow* WSCore::getMainWindow() const
{
	if (m_data == Q_NULLPTR)
		return Q_NULLPTR;

	if (m_data->mainWindow != Q_NULLPTR)
		return m_data->mainWindow;
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
void WSCore::createSetting(const QString& key, const QVariant& value, const bool visible)
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			m_data->settings->createSetting(key, value, visible);
}
// ----------------------------------------------------------------
void WSCore::createSetting(i_wsDescriptionSetting* wsDS)
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			m_data->settings->createSetting(wsDS);
}
// ----------------------------------------------------------------
QList<i_wsDescriptionSetting*> WSCore::getSettingsList() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			return m_data->settings->getSettingsList();
	return QList<i_wsDescriptionSetting*>();
}
// ----------------------------------------------------------------
QVariant WSCore::getSetting(const QString& name) const
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			return m_data->settings->getSetting(name);
	return QVariant();
}
// ----------------------------------------------------------------
void WSCore::setSetting(const QString& name, const QVariant& value)
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			m_data->settings->setSetting(name, value);
}
// ----------------------------------------------------------------
void WSCore::addTranslator(const QString& filePath)
{
	if (m_data != Q_NULLPTR)
		if (m_data->settings != Q_NULLPTR)
			m_data->settings->addTranslator(filePath);
}
// ----------------------------------------------------------------
QObject* WSCore::getProjects()  const
{
	if (m_data != Q_NULLPTR)
		if (m_data->projectCore != Q_NULLPTR)
			return m_data->projectCore;
	qDebug() << "Failed getting projectCore";
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QObject* WSCore::getConnectSocket() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->connectSocket != Q_NULLPTR)
			return m_data->connectSocket;
	qDebug() << "Failed getting connectSocket";
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QObject* WSCore::getAPIRosreestr() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->apirosreestr != Q_NULLPTR)
			return m_data->apirosreestr;
	qDebug() << "Failed getting APIRosreestr object";
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
WSCore* WSCore::create(i_wsSettingsCore* settings)
{	
	return(new WSCore(settings));
}
// ----------------------------------------------------------------