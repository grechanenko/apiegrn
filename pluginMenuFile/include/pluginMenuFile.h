#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginmenufile_global.h"

class PLUGINMENUFILE_EXPORT PluginMenuFile : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginMenuFile")
	DECLARE_SMARTPOINTER

public:
	PluginMenuFile();
	~PluginMenuFile();

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
		Изменить активность кнопкок меню (Режим подключено к серверу)
	*/
	void slt_conneted(bool);


	/*!
		Изменить активность кнопок меню (Режим отключено к серверу)
	*/
	void slt_disconneted();
};
