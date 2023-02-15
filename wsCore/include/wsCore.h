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
		Добавить вкладку
	*/
	virtual QWidget* createTab(QString name);


	/*!
		Создать раздел системного меню "File"
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
		Добавить dockWidget
	*/
	virtual void addDockWidget(Qt::DockWidgetArea area, QDockWidget *dockwidget);
	   

	/*!
		Загрузить плагины
		Возвращает количество загруженных
	*/
	virtual int loadPlugins(QObject* ws, QObject* splash = Q_NULLPTR);


	/*!
		Возвращает названия загруженных плагинов
	*/
	virtual QStringList getPluginNames() const;


	/*!
		Возвращает плагин по названию
	*/
	virtual QObject* getPlugin(QString name) const;


	/*!
		Зарегистрировать объект, реализованный в плагине		 
	*/
	virtual bool registerObject(i_base* object, QString className) const;


	/*!
		Получить список зарегистрированных объектов
	*/
	virtual QList<QString> getRegisterObjectNames() const;


	/*!
		Получить зарегистрированный объект
	*/
	virtual i_base* getResterObject(QString name) const;


	/*!
		Возвращает главное окно
	*/
	QMainWindow* getMainWindow() const;


	/*!
		Создать параметр
	*/
	virtual void createSetting(const QString& key, const QVariant& value, const bool visible = true);


	/*!
		Создать параметр
	*/
	virtual void createSetting(i_wsDescriptionSetting* wsDS);


	/*!
		Возвращает список параметров
	*/
	virtual QList<i_wsDescriptionSetting*> getSettingsList() const;


	/*!
		Возвращает значение параметра
	*/
	virtual QVariant getSetting(const QString& name) const;


	/*!
		Изменить значение параметра
	*/
	virtual void setSetting(const QString& name, const QVariant& value);


	/*!
		Загрузить файл перевода
	*/
	virtual void addTranslator(const QString& filePath);


// -------------------------------- PROJECT

	/*!
		Возвращает класс для доступа к проектам
		список сигналов указан в файле i_wsProjectCore.h
		
		QObject* obj = getProjects();
		connect(obj, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt(i_wsProject*)));

		i_wsProjectCore* pr = qobject_cast<i_wsProjectCore*>(obj);	
	*/
	virtual QObject* getProjects()  const;


	/*!
		Возвращает класс для доступа к сокетному соединению с серевером сообщений
		список сигнало указан в файле i_wsConnectSocket.h

		QObject* obj = getProjects();
		connect(obj, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt(i_wsProject*)));

		i_wsProjectCore* pr = qobject_cast<i_wsProjectCore*>(obj);
	*/
	virtual QObject* getConnectSocket() const;


	/*!
		Возвращает класс для взаимодействия с APIRosreestr
		список сигнало указан в файле i_wsAPIRosreestr.h

		QObject* obj = getAPIRosreestr();
		connect(obj, SIGNAL(sig()), SLOT(slt()));

		i_wsAPIRosreestr* pr = qobject_cast<i_wsAPIRosreestr*>(obj);
	*/
	virtual QObject* getAPIRosreestr() const;


	/*!
		Создает себя
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
