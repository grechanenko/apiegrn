#pragma once

#include <QObject>
#include "i_wsPlugin.h"
#include "pluginfeedback_global.h"

class PLUGINFEEDBACK_EXPORT PluginFeedBack : public QObject, public virtual i_wsPlugin
{
	Q_OBJECT
	Q_INTERFACES(i_wsPlugin)
	Q_PLUGIN_METADATA(IID "PluginFeedBack")
	DECLARE_SMARTPOINTER

public:

	PluginFeedBack(QObject* parent = 0);
	~PluginFeedBack();

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
		���������� ����
	*/
	void slot_showDialog();

	/*!
		���������� ������ �������� ���������
	*/
	void mailStatus(QString);
};
