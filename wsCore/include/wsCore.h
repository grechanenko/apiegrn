#pragma once

#include <QObject>

#include "i_wsCore.h"
#include "wscore_global.h"

class QMainWindow;
class QMenu;
class QDockWidget;
class i_docViewer;
class i_wsSettingsCore;
class i_wsDescriptionSetting;
class i_wsProjectCore;


class WSCORE_EXPORT WSCore : public QObject, public virtual i_wsCore
{
	Q_OBJECT
	Q_INTERFACES(i_wsCore)
	DECLARE_SMARTPOINTER

public:

	WSCore(i_wsSettingsCore* settings);
	~WSCore();

	/*!
		Create action from right bar window
	*/
	virtual QAction* addAction(const QIcon& icon, const QString& caption, Qt::ToolButtonStyle style, QMenu* menu = Q_NULLPTR);


	/*!
		Create menu from right bar window
	*/
	virtual QMenu* addMenu(const QString& caption);


	/*!
		�������� �������
	*/
	virtual QWidget* createTab(QString name);


	/*!
		������� ������ ���������� ���� "File"
	*/
	virtual QAction* addSystemButton(QIcon iconLogo, QString name);


	/*!
		Create quick access bar
	*/
	virtual QWidget* createQuickAccessBar();


	/*!
		Get status bar
	*/
	virtual QStatusBar *statusBar() const;


	/*!
		Set status bar
	*/
	virtual void setStatusBar(QStatusBar *statusbar);

	
	/*!
		Get document viewer
	*/
	virtual i_docViewer* getDocViewer();

	
	/*!
		�������� dockWidget
	*/
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
	   

	/*!
		��������� �������
		���������� ���������� �����������
	*/
	virtual int loadPlugins(QObject* ws, QObject* splash = Q_NULLPTR);


	/*!
		���������� �������� ����������� ��������
	*/
	virtual QStringList getPluginNames() const;


	/*!
		���������� ������ �� ��������
	*/
	virtual QObject* getPlugin(QString name) const;


	/*!
		���������������� ������, ������������� � �������		 
	*/
	virtual bool registerObject(i_base* object, QString className) const;


	/*!
		�������� ������ ������������������ ��������
	*/
	virtual QList<QString> getRegisterObjectNames() const;


	/*!
		�������� ������������������ ������
	*/
	virtual i_base* getResterObject(QString name) const;


	/*!
		���������� ������� ����
	*/
	QMainWindow* getMainWindow() const;


	/*!
		������� ��������
	*/
	virtual void createSetting(const QString& key, const QVariant& value, const bool visible = true);


	/*!
		������� ��������
	*/
	virtual void createSetting(i_wsDescriptionSetting* wsDS);


	/*!
		���������� ������ ����������
	*/
	virtual QList<i_wsDescriptionSetting*> getSettingsList() const;


	/*!
		���������� �������� ���������
	*/
	virtual QVariant getSetting(const QString& name) const;


	/*!
		�������� �������� ���������
	*/
	virtual void setSetting(const QString& name, const QVariant& value);


	/*!
		��������� ���� ��������
	*/
	virtual void addTranslator(const QString& filePath);


// -------------------------------- PROJECT

	/*!
		���������� ����� ��� ������� � ��������
		������ �������� ������ � ����� i_wsProjectCore.h
		
		QObject* obj = getProjects();
		connect(obj, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt(i_wsProject*)));

		i_wsProjectCore* pr = qobject_cast<i_wsProjectCore*>(obj);	
	*/
	virtual QObject* getProjects()  const;


	/*!
		���������� ����� ��� ������� � ��������� ���������� � ��������� ���������
		������ ������� ������ � ����� i_wsConnectSocket.h

		QObject* obj = getProjects();
		connect(obj, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt(i_wsProject*)));

		i_wsProjectCore* pr = qobject_cast<i_wsProjectCore*>(obj);
	*/
	virtual QObject* getConnectSocket() const;


	/*!
		���������� ����� ��� �������������� � APIRosreestr
		������ ������� ������ � ����� i_wsAPIRosreestr.h

		QObject* obj = getAPIRosreestr();
		connect(obj, SIGNAL(sig()), SLOT(slt()));

		i_wsAPIRosreestr* pr = qobject_cast<i_wsAPIRosreestr*>(obj);
	*/
	virtual QObject* getAPIRosreestr() const;


	/*!
		������� ����
	*/
	static WSCore* create(i_wsSettingsCore* settings);
	

private:
	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Show message in taskBar
	*/
	virtual void showMessageTaskBar(QString title, QString text) const;
};
