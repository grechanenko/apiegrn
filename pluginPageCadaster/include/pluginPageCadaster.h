#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginpagecadaster_global.h"

class i_wsProject;
class PLUGINPAGECADASTER_EXPORT PluginPageCadaster : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginPageCadaster")
	DECLARE_SMARTPOINTER

public:
	PluginPageCadaster(QObject* parent = 0);
	~PluginPageCadaster();

	/*!
	   Инициализация плагина
	   i_wsCore = qobject_cast<i_wsCore>(wsCore)
	*/
	virtual bool initialize(QObject* wsCore);


	/*!
		Возвращает название плагина
		Используется для механизма зависимостей
	*/
	virtual QString getName() const;


	/*
		Зависимости от других плагинов
	*/
	virtual QStringList references() const;


	/*!
		Возвращает иконку плагина
	*/
	virtual QIcon getIcon() const;


	/*!
		Возвращает описание планига
	*/
	virtual QString about() const;

private:

	struct SDATA;
	SDATA* m_data;

private slots:

	/*!
		Сообщает о создании проекта
	*/
	void slt_createProject(i_wsProject*);  


	/*!
		Сообщает об открытии проекта
	*/
	void slt_openProject(i_wsProject*);


	/*!
		Сообщает о закрытии проекта
	*/
	void slt_closeProject(QString);
};
