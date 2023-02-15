#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginmenufile_global.h"

class PLUGINMENUFILE_EXPORT PluginMenuFile : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginMenuFile")
	DECLARE_SMARTPOINTER

public:
	PluginMenuFile();
	~PluginMenuFile();

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
		�������� ���������� ������� ���� (����� ���������� � �������)
	*/
	void slt_conneted(bool);


	/*!
		�������� ���������� ������ ���� (����� ��������� � �������)
	*/
	void slt_disconneted();
};
