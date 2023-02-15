#pragma once

#include <QObject>
#include "i_wsProject.h"
#include "wsprojectcore_global.h"

class QDir;
class i_wsAPIRosreestr;
class WSPROJECTCORE_EXPORT WSProject : public QObject, public virtual i_wsProject
{
	Q_OBJECT
	Q_INTERFACES(i_wsProject)
	DECLARE_SMARTPOINTER

public:
	WSProject(QString filePath, QObject* parent = 0);
	~WSProject();
	

	/*!
		Добавить объект(ы)

		i_wsJsonParser* parser = qoubject_cast<i_wsJsonParser*>(data);
	*/
	virtual bool addObject(QObject* data);


	/*!
		Возвращает метаданные
	*/
	virtual QVariant getProperty(QString) const;


	/*!
		Установить метаданные
	*/
	virtual void setProperty(QString name, QVariant value);


	/*!
		Возвращает идентификатор пользователя создавшего проект
	*/
	virtual int getIDUser() const;


	/*!
		true - если проект создан указанным пользователем
	*/
	virtual bool isUser(QString login);


	/*!
		Возвращает рабочую папку проекта
	*/
	virtual QDir workingDirectory() const;


	/*!
		Возвращает полный путь к файлу проекта
	*/
	virtual QString filePath() const;


	/*!
		Возвращает название проекта
	*/
	virtual QString name() const;


	/*!
		Сохранить изменения в файл
	*/
	virtual bool save();


	/*!
		Создать файл проекта
	*/
	static bool createProjectFile(QString filePath, i_wsAPIRosreestr* APIRosreestr);


	/*!
		Проверить файл проекта на валидность
	*/
	static bool isValidProjectFile(QString filePath);


	/*!
		Создать объект класса
	*/
	static WSProject* create(QString filePath, QObject* APIRosreestr);


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		Высылает информационное сообщение
	*/
	void message(QString title, QString text);
};