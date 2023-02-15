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
		�������� ������ �� ������ � ����
	*/
	bool addObject(JsonParser* parser);


	/*!
		������� ���� ������
	*/
	static bool create(QString filePath, i_wsAPIRosreestr* APIRosreestr);


	/*!
		��������� ���� ������� �� ����������
	*/
	static bool isValid(QString filePath);


	/*!
		��������� ���� ������� �� �������������� � �������� ������������
	*/
	static bool isValidUser(QString filePath);

private:

	struct SDATA;
	SDATA* m_data;
};