#pragma once


#include <QVariant>
#include "i_base.h"

class i_wsDescriptionSetting : public virtual i_base
{
public:
	virtual ~i_wsDescriptionSetting() {}


	/*!
		Описание настройки
	*/
	virtual QString getDescription() const = 0;


	/*!
		Тип данных
	*/
	virtual QVariant::Type getType() = 0;


	/*!
		Возвращает ключ
	*/
	virtual QString getKey() const = 0;


	/*!
		Значение по умолчанию
	*/
	virtual QVariant getDefaultValue() = 0;


	/*!
		Текущее значение
	*/
	virtual QVariant getValue() = 0;


	/*!
		Установить новое значение
	*/
	virtual void setValue(const QVariant&) = 0;


	/*!
		Установить видимость для пользователя
	*/
	virtual void setVisible(const bool flag) = 0;


	/*!
		true - если скрыто от пользователя
	*/
	virtual bool isVisible() const = 0;


	/*!
		Виджет для редактирования
	*/
	virtual QWidget* getWidget(QWidget* parent) = 0;
};

Q_DECLARE_INTERFACE(i_wsDescriptionSetting, "launcher.i_wsDescriptionSetting/1.0")