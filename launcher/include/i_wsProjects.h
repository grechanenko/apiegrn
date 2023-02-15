#pragma once

/*!

signals:
	
	void sig_createProject(i_wsProject*); // Сообщает о создании проекта
	void sig_openProject(i_wsProject*);	  // Сообщает об открытии проекта	
	void sig_closeProject(QString);		  // Сообщает о закрытии проекта

*/


#include "i_base.h"

class i_wsProject;
class i_wsProjects : public i_base
{
public:
	virtual ~i_wsProjects() {}


	/*!
		Возвращает проект по индексу
	*/
	virtual i_wsProject* getProject(int) const = 0;
	   

	/*!
		Закрыть проект по индексу
	*/
	virtual void closeProject(int) = 0;

	   
	/*!
		Открыть проект
	*/
	virtual bool openProject(QString) = 0;


	/*!
		Создать проект
	*/
	virtual bool createProject(QString) = 0;


	/*!
		Получить список проектов
	*/
	virtual QVector<i_wsProject*> projects() = 0;


	/*!
		Получить активный проект
	*/
	virtual i_wsProject* getActive() = 0;


	/*!
		Установить активный проект
	*/
	virtual bool setActive(unsigned int) = 0;

};
Q_DECLARE_INTERFACE(i_wsProjects, "launcher.i_wsProjects/1.0")