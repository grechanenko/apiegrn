#pragma once

/*!

signals:
	
	void sig_createProject(i_wsProject*); // �������� � �������� �������
	void sig_openProject(i_wsProject*);	  // �������� �� �������� �������	
	void sig_closeProject(QString);		  // �������� � �������� �������

*/


#include "i_base.h"

class i_wsProject;
class i_wsProjects : public i_base
{
public:
	virtual ~i_wsProjects() {}


	/*!
		���������� ������ �� �������
	*/
	virtual i_wsProject* getProject(int) const = 0;
	   

	/*!
		������� ������ �� �������
	*/
	virtual void closeProject(int) = 0;

	   
	/*!
		������� ������
	*/
	virtual bool openProject(QString) = 0;


	/*!
		������� ������
	*/
	virtual bool createProject(QString) = 0;


	/*!
		�������� ������ ��������
	*/
	virtual QVector<i_wsProject*> projects() = 0;


	/*!
		�������� �������� ������
	*/
	virtual i_wsProject* getActive() = 0;


	/*!
		���������� �������� ������
	*/
	virtual bool setActive(unsigned int) = 0;

};
Q_DECLARE_INTERFACE(i_wsProjects, "launcher.i_wsProjects/1.0")