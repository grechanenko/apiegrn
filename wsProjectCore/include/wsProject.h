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
		�������� ������(�)

		i_wsJsonParser* parser = qoubject_cast<i_wsJsonParser*>(data);
	*/
	virtual bool addObject(QObject* data);


	/*!
		���������� ����������
	*/
	virtual QVariant getProperty(QString) const;


	/*!
		���������� ����������
	*/
	virtual void setProperty(QString name, QVariant value);


	/*!
		���������� ������������� ������������ ���������� ������
	*/
	virtual int getIDUser() const;


	/*!
		true - ���� ������ ������ ��������� �������������
	*/
	virtual bool isUser(QString login);


	/*!
		���������� ������� ����� �������
	*/
	virtual QDir workingDirectory() const;


	/*!
		���������� ������ ���� � ����� �������
	*/
	virtual QString filePath() const;


	/*!
		���������� �������� �������
	*/
	virtual QString name() const;


	/*!
		��������� ��������� � ����
	*/
	virtual bool save();


	/*!
		������� ���� �������
	*/
	static bool createProjectFile(QString filePath, i_wsAPIRosreestr* APIRosreestr);


	/*!
		��������� ���� ������� �� ����������
	*/
	static bool isValidProjectFile(QString filePath);


	/*!
		������� ������ ������
	*/
	static WSProject* create(QString filePath, QObject* APIRosreestr);


private:

	struct SDATA;
	SDATA* m_data;

signals:

	/*!
		�������� �������������� ���������
	*/
	void message(QString title, QString text);
};