#pragma once

#include <QObject>
#include "i_wsCore.h"
#include "wscore_global.h"

class WSCORE_EXPORT WSPluginCore : public QObject, public virtual i_wsPluginCore, public virtual i_base
{
	Q_OBJECT
	Q_INTERFACES(i_wsPluginCore i_base)
	DECLARE_SMARTPOINTER

public:
	WSPluginCore();
	~WSPluginCore();

	/*!
		��������� �������
		���������� ���������� �����������
	*/
	virtual int loadPlugins(QObject* ws, QObject* splash = Q_NULLPTR);


	/*!
		���������� ������� ����������� ��������
	*/
	virtual QStringList getPluginNames() const;


	/*!
		���������� ������ �� ��������
	*/
	virtual QObject* getPlugin(QString name) const;
	

	/*!
		true - ���� ������ ��������
	*/
	bool checkLoadReference(QObject* plugin);


	/*!
		���������������� ������, ������������� � �������
	*/
	virtual bool registerObject(i_base* object, QString className) const;


	/*!
		�������� ������ ������������������ ��������
	*/
	virtual QList<QString> getRegisterObjectNames() const;


	/*!
		�������� ������������������ ������
	*/
	virtual i_base* getResterObject(QString name) const;

private:

	struct SDATA;
	SDATA* m_data;
};