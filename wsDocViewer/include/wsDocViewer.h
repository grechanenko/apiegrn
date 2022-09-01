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
		Добавить список документов
	*/
	virtual void addListDocument(QStringList docs, QString tabName);


	/*!
		Добавить pdf документ
	*/
	virtual void addPDFDocument(QString path) const;


	/*!
		Добавить HTML документ из файла
	*/
	virtual void addHTMLDocument(QString path) const;


	/*!
		Добавить HTML документ по URL
	*/
	virtual void addHTMLDocument(QUrl url) const;


	/*!
		Закрыть все документы
	*/
	virtual void clearViewer();

private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Закрыть вкладку
	*/
	void closeTab(int);
};
