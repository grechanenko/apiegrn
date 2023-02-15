#pragma once

#include "i_base.h"

class QWidget;
class QStringList;

class i_docViewer : public i_base
{
public:
	virtual ~i_docViewer() {}

	
	/*!
		�������� ������ ����������
	*/
	virtual void addListDocument(QStringList docs, QString tabName) = 0;


	/*!
		�������� pdf ��������
	*/
	virtual void addPDFDocument(QString path) const = 0;


	/*!
		�������� HTML �������� �� �����
	*/
	virtual void addHTMLDocument(QString path) const = 0;


	/*!
		�������� HTML �������� �� URL
	*/
	virtual void addHTMLDocument(QUrl url) const = 0;


	/*!
		������� ��� ���������
	*/
	virtual void clearViewer() = 0;
};

Q_DECLARE_INTERFACE(i_docViewer, "launcher.i_docViewer/1.0")