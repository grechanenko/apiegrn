#pragma once

#include <QObject>
#include "i_wsProjects.h"

#include "wsprojectcore_global.h"

class i_wsCore;

class WSPROJECTCORE_EXPORT WSProjects : public QObject, public virtual i_wsProjects
{
	Q_OBJECT
	Q_INTERFACES(i_wsProjects)
	DECLARE_SMARTPOINTER

public:
	WSProjects(i_wsCore* wsCore, QObject* parent = 0);
	~WSProjects();


	/*!
		������� ������
	*/
	virtual bool createProject(QString path);


	/*!
		������� ������
	*/
	virtual bool openProject(QString filePaht);


	/*!
		���������� ������ �� �������
	*/
	virtual i_wsProject* getProject(int) const;
		

	/*!
		������� ������ �� �������
	*/
	virtual void closeProject(int);
	
	
	/*!
		�������� ������ ��������
	*/
	virtual QVector<i_wsProject*> projects();


	/*!
		�������� �������� ������
	*/
	virtual i_wsProject* getActive();


	/*!
		���������� �������� ������ �� �������
	*/
	virtual bool setActive(unsigned int);


	/*!
		������� ����
	*/
	static WSProjects* create(i_wsCore*);

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		�������� � �������� �������
	*/
	void sig_createProject(i_wsProject*);

	/*!
		�������� �� �������� �������
	*/
	void sig_openProject(i_wsProject*);

	/*!
		�������� � �������� �������
	*/
	void sig_closeProject(QString);


	/*!
		�������������� ���������
	*/
	void sig_message(QString title, QString text);
};
