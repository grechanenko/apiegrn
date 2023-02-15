#pragma once

#include <QObject>
#include "i_base.h"
#include "i_wsSettingsCore.h"
#include "i_wsProject.h"

class QAction;
class QStatusBar;
class QMenu;
class QDockWidget;
class i_wsGuiPlugin;
class i_docViewer;
class i_wsProjectCore;
class i_wsConnectSocket;

class i_wsGui
{
public:

	virtual ~i_wsGui() {};


	/*!
		Create action from right bar window
	*/
	virtual QAction* addAction(const QIcon& icon, const QString& caption, Qt::ToolButtonStyle style, QMenu* menu = Q_NULLPTR) = 0;


	/*!
		Create menu from right bar window
	*/
	virtual QMenu* addMenu(const QString& caption) = 0;


	/*!
		Create tab from name

		Using in plugin:

		i_wsCore* ws - passed to plugin

		QWidget* tabWidget = ws->crateTab(tr("Tab name"));
		Qtitan::RibbonPage* tab = qobject_cast<Qtitan::RibbonPage*>(tabWidget);		
		if (tab)
		{
			QAction* actionNewButton = groupOrder->addAction(QIcon(":/icon.png"), tr("&NameButton"), Qt::ToolButtonTextUnderIcon); 
			actionNewOrder->setShortcut(QKeySequence::New);			
		}
	*/
	virtual QWidget* createTab(QString name) = 0;	


	/*!
		Create sysmen menu "File"
	*/
	virtual QAction* addSystemButton(QIcon iconLogo, QString name) = 0;


	/*!
		Create quick access bar
	*/
	virtual QWidget* createQuickAccessBar() = 0;


	/*!
		Get status bar
	*/
	virtual QStatusBar *statusBar() const = 0;


	/*!
		Set status bar
	*/
	virtual void setStatusBar(QStatusBar *statusbar) = 0;

	
	/*!
		Get document viewer
	*/
	virtual i_docViewer* getDocViewer() = 0;


	/*!
		Show message in taskBar
	*/
	virtual void showMessageTaskBar(QString title, QString text) const = 0;


	/*!
		�������� dockWidget
	*/
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget) = 0;
	
};
Q_DECLARE_INTERFACE(i_wsGui, "launcher.i_wsGui/1.0")



class i_wsPluginCore
{
public:

	virtual ~i_wsPluginCore() {};


	/*!
		��������� �������
		���������� ���������� �����������
	*/
	virtual int loadPlugins(QObject* ws, QObject* splash = Q_NULLPTR) = 0;


	/*!
		���������� �������� ����������� ��������
	*/
	virtual QStringList getPluginNames() const = 0;


	/*!
		���������� ������ �� ��������
	*/
	virtual QObject* getPlugin(QString name) const = 0;


	/*!
		���������������� ������, ������������� � �������		 
	*/
	virtual bool registerObject(i_base* object, QString className) const = 0;


	/*!
		�������� ������ ������������������ ��������
	*/
	virtual QList<QString> getRegisterObjectNames() const = 0;


	/*!
		�������� ������������������ ������
	*/
	virtual i_base* getResterObject(QString name) const = 0;
};
Q_DECLARE_INTERFACE(i_wsPluginCore, "launcher.i_wsPluginCore/1.0")


class i_wsCore : public virtual i_base,
				 public virtual i_wsGui,
				 public virtual i_wsPluginCore,
				 public virtual i_wsSettingsCore				
{
	Q_INTERFACES(i_base i_wsGui i_wsPluginCore i_wsSettingsCore)
public:
	virtual ~i_wsCore() {};

	/*!
		���������� ����� ��� ������� � ��������
		������ �������� ������ � ����� i_wsProjectCore.h
		
		QObject* obj = getProjects();
		connect(obj, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt(i_wsProject*)));

		i_wsProjectCore* pr = qobject_cast<i_wsProjectCore*>(obj);
	*/
	virtual QObject* getProjects()  const = 0;


	/*!
		���������� ����� ��� ������� � ��������� ���������� � ��������� ���������
		������ ������� ������ � ����� i_wsConnectSocket.h

		QObject* obj = getConnectSocket();
		connect(obj, SIGNAL(signal()), SLOT(slt()));

		i_wsConnectSocket* pr = qobject_cast<i_wsConnectSocket*>(obj);
	*/
	virtual QObject* getConnectSocket() const = 0;


	/*!
		���������� ����� ��� �������������� � APIRosreestr
		������ ������� ������ � ����� i_wsAPIRosreestr.h

		QObject* obj = getAPIRosreestr();
		connect(obj, SIGNAL(sig()), SLOT(slt()));

		i_wsAPIRosreestr* pr = qobject_cast<i_wsAPIRosreestr*>(obj);
	*/
	virtual QObject* getAPIRosreestr() const = 0;
};
Q_DECLARE_INTERFACE(i_wsCore, "launcher.i_wsCore/1.0")