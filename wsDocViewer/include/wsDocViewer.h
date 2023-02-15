#pragma once

#include <QWidget>
#include "i_docViewer.h"
#include "wsdocviewer_global.h"

class WSDOCVIEWER_EXPORT WSDocViewer : public QWidget, public virtual i_docViewer
{
	Q_OBJECT
	Q_INTERFACES(i_docViewer)
	DECLARE_SMARTPOINTER

public:
	WSDocViewer(QWidget* parent = 0);
	~WSDocViewer();
	

	/*!
		�������� ������ ����������
	*/
	virtual void addListDocument(QStringList docs, QString tabName);


	/*!
		�������� pdf ��������
	*/
	virtual void addPDFDocument(QString path) const;


	/*!
		�������� HTML �������� �� �����
	*/
	virtual void addHTMLDocument(QString path) const;


	/*!
		�������� HTML �������� �� URL
	*/
	virtual void addHTMLDocument(QUrl url) const;


	/*!
		������� ��� ���������
	*/
	virtual void clearViewer();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		������� �������
	*/
	void closeTab(int);
};
