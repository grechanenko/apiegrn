#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginpersonalarea_global.h"

class PLUGINPERSONALAREA_EXPORT PluginPersonalArea : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginPersonalArea")
	DECLARE_SMARTPOINTER

public:
	PluginPersonalArea();
	~PluginPersonalArea();

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
		�������� ���������� ������� ���� (����� ����������)
	*/
	void slt_conneted();


	/*!
		�������� ���������� ������ ���� (����� ���������)
	*/
	void slt_disconneted();
};
