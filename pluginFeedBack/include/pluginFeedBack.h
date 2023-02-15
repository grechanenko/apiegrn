#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginfeedback_global.h"

class PLUGINFEEDBACK_EXPORT PluginFeedBack : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginFeedBack")
	DECLARE_SMARTPOINTER

public:

	PluginFeedBack(QObject* parent = 0);
	~PluginFeedBack();

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
		Отобразить окно
	*/
	void slot_showDialog();

	/*!
		Отобразить статус отправки сообщения
	*/
	void mailStatus(QString);
};
