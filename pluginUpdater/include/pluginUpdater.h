#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginupdater_global.h"

class PLUGINUPDATER_EXPORT PluginUpdater : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginUpdater")
	DECLARE_SMARTPOINTER

public:
	PluginUpdater();
	~PluginUpdater();

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
		Запускает процесс проверки наличия обновления
	*/
	void slot_checkUpdate();


	/*!
		Сообщить о наличии обновления
	*/
	void slot_availableUpdate();


	/*!
		Сообщить о невозможности обновить 
	*/
	void slot_noUpdate(QString);	
};
