#pragma once

#include "i_wsDescriptionSetting.h"

class QString;

class i_wsSettingsCore
{
public:
	virtual ~i_wsSettingsCore() {};

	/*!
		Создать параметр
	*/
	virtual void createSetting(const QString& key, const QVariant& value, const bool visible = true) = 0;


	/*!
		Создать параметр
	*/
	virtual void createSetting(i_wsDescriptionSetting* wsDS) = 0;


	/*!
		Возвращает список параметров
	*/
	virtual QList<i_wsDescriptionSetting*> getSettingsList() const = 0;


	/*!
		Возвращает значение параметра
	*/
	virtual QVariant getSetting(const QString& name) const = 0;


	/*!
		Изменить значение параметра
	*/
	virtual void setSetting(const QString& name, const QVariant& value) = 0;


	/*!
		Загрузить файл перевода
	*/
	virtual void addTranslator(const QString& filePath) = 0;
};
Q_DECLARE_INTERFACE(i_wsSettingsCore, "launcher.i_wsSettingsCore/1.0")
