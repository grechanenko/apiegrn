#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginpersonalarea_global.h"

class PLUGINPERSONALAREA_EXPORT PluginPersonalArea : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginPersonalArea")
	DECLARE_SMARTPOINTER

public:
	PluginPersonalArea();
	~PluginPersonalArea();

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
		Изменить активность кнопкок меню (Режим подключено)
	*/
	void slt_conneted();


	/*!
		Изменить активность кнопок меню (Режим отключено)
	*/
	void slt_disconneted();
};
