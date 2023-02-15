#pragma once

#include "i_wsDescriptionSetting.h"

class QString;

class i_wsSettingsCore
{
public:
	virtual ~i_wsSettingsCore() {};

	/*!
		������� ��������
	*/
	virtual void createSetting(const QString& key, const QVariant& value, const bool visible = true) = 0;


	/*!
		������� ��������
	*/
	virtual void createSetting(i_wsDescriptionSetting* wsDS) = 0;


	/*!
		���������� ������ ����������
	*/
	virtual QList<i_wsDescriptionSetting*> getSettingsList() const = 0;


	/*!
		���������� �������� ���������
	*/
	virtual QVariant getSetting(const QString& name) const = 0;


	/*!
		�������� �������� ���������
	*/
	virtual void setSetting(const QString& name, const QVariant& value) = 0;


	/*!
		��������� ���� ��������
	*/
	virtual void addTranslator(const QString& filePath) = 0;
};
Q_DECLARE_INTERFACE(i_wsSettingsCore, "launcher.i_wsSettingsCore/1.0")
