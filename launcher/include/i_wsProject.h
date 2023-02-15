#pragma once

#include "i_base.h"

class QDir;
class i_wsProject : public i_base
{
public:
	virtual ~i_wsProject() {}


	/*!
		�������� ������(�)
	*/
	virtual bool addObject(QObject* data) = 0;


	/*!
		���������� ����������
	*/
	virtual QVariant getProperty(QString) const = 0;


	/*!
		���������� ����������
	*/
	virtual void setProperty(QString name, QVariant value) = 0;


	/*!
		���������� ������������� ������������ ���������� ������
	*/
	virtual int getIDUser() const = 0;


	/*!
		true - ���� ������ ������ ��������� �������������
	*/
	virtual bool isUser(QString login) = 0;


	/*!
		���������� ������� ����� �������
	*/
	virtual QDir workingDirectory() const = 0;


	/*!
		���������� ������ ���� � ����� �������
	*/
	virtual QString filePath() const = 0;


	/*!
		���������� �������� �������
	*/
	virtual QString name() const = 0;


	/*!
		��������� ��������� � ����
	*/
	virtual bool save() = 0;

};
Q_DECLARE_INTERFACE(i_wsProject, "launcher.i_wsProject/1.0")