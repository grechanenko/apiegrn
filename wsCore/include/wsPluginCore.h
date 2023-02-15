#pragma once

#include <QObject>
#include "i_wsCore.h"
#include "wscore_global.h"

class WSCORE_EXPORT WSPluginCore : public QObject, public virtual i_wsPluginCore, public virtual i_base
{
	Q_OBJECT
	Q_INTERFACES(i_wsPluginCore i_base)
	DECLARE_SMARTPOINTER

public:
	WSPluginCore();
	~WSPluginCore();

	/*!
		Загрузить плагины
		Возвращает количество загруженных
	*/
	virtual int loadPlugins(QObject* ws, QObject* splash = Q_NULLPTR);


	/*!
		Возвращает названи¤ загруженных плагинов
	*/
	virtual QStringList getPluginNames() const;


	/*!
		Возвращает плагин по названию
	*/
	virtual QObject* getPlugin(QString name) const;
	

	/*!
		true - если плагин загружен
	*/
	bool checkLoadReference(QObject* plugin);


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

private:

	struct SDATA;
	SDATA* m_data;
};