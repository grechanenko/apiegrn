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
		Создать проект
	*/
	virtual bool createProject(QString path);


	/*!
		Открыть проект
	*/
	virtual bool openProject(QString filePaht);


	/*!
		Возвращает проект по индексу
	*/
	virtual i_wsProject* getProject(int) const;
		

	/*!
		Закрыть проект по индексу
	*/
	virtual void closeProject(int);
	
	
	/*!
		Получить список проектов
	*/
	virtual QVector<i_wsProject*> projects();


	/*!
		Получить активный проект
	*/
	virtual i_wsProject* getActive();


	/*!
		Установить активный проект по индексу
	*/
	virtual bool setActive(unsigned int);


	/*!
		Создает себя
	*/
	static WSProjects* create(i_wsCore*);

private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Сообщает о создании проекта
	*/
	void sig_createProject(i_wsProject*);

	/*!
		Сообщает об открытии проекта
	*/
	void sig_openProject(i_wsProject*);

	/*!
		Сообщает о закрытии проекта
	*/
	void sig_closeProject(QString);


	/*!
		Информационное сообщение
	*/
	void sig_message(QString title, QString text);
};
