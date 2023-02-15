#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginpagecadaster_global.h"

class i_wsProject;
class PLUGINPAGECADASTER_EXPORT PluginPageCadaster : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginPageCadaster")
	DECLARE_SMARTPOINTER

public:
	PluginPageCadaster(QObject* parent = 0);
	~PluginPageCadaster();

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
		�������� � �������� �������
	*/
	void slt_createProject(i_wsProject*);  


	/*!
		�������� �� �������� �������
	*/
	void slt_openProject(i_wsProject*);


	/*!
		�������� � �������� �������
	*/
	void slt_closeProject(QString);
};
