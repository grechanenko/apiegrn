#pragma once

#include <QObject>

class i_wsAPIRosreestr;
class JsonParser;
class ProjectFile : public QObject
{
	Q_OBJECT
public:

	ProjectFile(QString filePath, QObject* parent = 0);
	~ProjectFile();


	/*!
		Добавить данные об обекте в файл
	*/
	bool addObject(JsonParser* parser);


	/*!
		Создать файл проект
	*/
	static bool create(QString filePath, i_wsAPIRosreestr* APIRosreestr);


	/*!
		Проверить файл проекта на валидность
	*/
	static bool isValid(QString filePath);


	/*!
		Проверить файл проекта на принадлежность к текущему пользователю
	*/
	static bool isValidUser(QString filePath);

private:

	struct SDATA;
	SDATA* m_data;
};