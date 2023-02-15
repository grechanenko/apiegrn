#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginmanagerproject_global.h"


class PLUGINMANAGERPROJECT_EXPORT PluginManagerProject : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginManagerProject")
	DECLARE_SMARTPOINTER

public:
	PluginManagerProject(QObject* parent = 0);
	~PluginManagerProject();

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
};
