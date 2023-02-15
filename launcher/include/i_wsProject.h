#pragma once

#include "i_base.h"

class QDir;
class i_wsProject : public i_base
{
public:
	virtual ~i_wsProject() {}


	/*!
		Добавить объект(ы)
	*/
	virtual bool addObject(QObject* data) = 0;


	/*!
		Возвращает метаданные
	*/
	virtual QVariant getProperty(QString) const = 0;


	/*!
		Установить метаданные
	*/
	virtual void setProperty(QString name, QVariant value) = 0;


	/*!
		Возвращает идентификатор пользователя создавшего проект
	*/
	virtual int getIDUser() const = 0;


	/*!
		true - если проект создан указанным пользователем
	*/
	virtual bool isUser(QString login) = 0;


	/*!
		Возвращает рабочую папку проекта
	*/
	virtual QDir workingDirectory() const = 0;


	/*!
		Возвращает полный путь к файлу проекта
	*/
	virtual QString filePath() const = 0;


	/*!
		Возвращает название проекта
	*/
	virtual QString name() const = 0;


	/*!
		Сохранить изменения в файл
	*/
	virtual bool save() = 0;

};
Q_DECLARE_INTERFACE(i_wsProject, "launcher.i_wsProject/1.0")