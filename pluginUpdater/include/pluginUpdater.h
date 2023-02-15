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
		��������� ������� �������� ������� ����������
	*/
	void slot_checkUpdate();


	/*!
		�������� � ������� ����������
	*/
	void slot_availableUpdate();


	/*!
		�������� � ������������� �������� 
	*/
	void slot_noUpdate(QString);	
};
