#pragma once

#include "i_base.h"

class QObject;
class QString;
class QIcon;
class QObject;
class QStringList;

class i_wsPlugin : public i_base
{
public:
    virtual ~i_wsPlugin(){}


    /*!
        Инициализация плагина
        i_wsCore = qobject_cast<i_wsCore>(wsCore)
    */
    virtual bool initialize( QObject* wsCore) = 0;


	/*!
		Возвращает название плагина
		Используется для механизма зависимостей
	*/
	virtual QString getName() const = 0;


    /*
        Зависимости от других плагинов
    */
    virtual QStringList references() const = 0;


	/*!
		Возвращает иконку плагина
	*/
	virtual QIcon getIcon() const = 0;


	/*!
		Возвращает описание планига
	*/
	virtual QString about() const = 0;
};

Q_DECLARE_INTERFACE(i_wsPlugin, "launcher.i_plugin/1.0")