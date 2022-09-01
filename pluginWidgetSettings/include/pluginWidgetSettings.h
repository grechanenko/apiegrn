#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginwidgetsettings_global.h"

class PLUGINWIDGETSETTINGS_EXPORT PluginWidgetSettings : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginWidgetSettings")
	DECLARE_SMARTPOINTER

public:
	PluginWidgetSettings(QObject* parent = 0);
	~PluginWidgetSettings();

	/*!
	   ������������� �������
	   i_wsCore = qobject_cast<i_wsCore>(wsCore)
	*/
	virtual bool initialize(QObject* wsCore);


	/*!
		���������� �������� �������
		������������ ��� ��������� ������������
	*/
	virtual QString getName() const;


	/*
		����������� �� ������ ��������
	*/
	virtual QStringList references() const;


	/*!
		���������� ������ �������
	*/
	virtual QIcon getIcon() const;


	/*!
		���������� �������� �������
	*/
	virtual QString about() const;

private:

	struct SDATA;
	SDATA* m_data;

private slots:

	/*!
		���������� ���� ��������
	*/
	void slot_showSettings();
};
