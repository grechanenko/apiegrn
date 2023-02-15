#include <QFileInfo>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWebEngineView>
#include <QUrl>

#include "xpdfWidget.h"

#include "wsDocViewer.h"

IMPLEMENT_SMARTPOINTER(WSDocViewer)

struct WSDocViewer::SDATA 
{
	QTabWidget* tabWidget = Q_NULLPTR;
};

// ----------------------------------------------------------
WSDocViewer::WSDocViewer(QWidget* parent)
	:QWidget(parent),
	m_data(Q_NULLPTR)
{	
	m_data = new SDATA;

	m_data->tabWidget = new QTabWidget(this);
	connect(m_data->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
	m_data->tabWidget->setTabsClosable(true);

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_data->tabWidget);
	this->setLayout(layout);

	//this->addPDFDocument("C:/URI/pdf.pdf");	
	this->addHTMLDocument(QUrl("https://apirosreestr.ru/mapswidget/"));
	//this->addHTMLDocument(QUrl("http://192.168.44.55:8080/mapswidget/"));
}
// ----------------------------------------------------------
WSDocViewer::~WSDocViewer()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------
void WSDocViewer::addListDocument(QStringList docs, QString tabName) 
{
	this->clearViewer();

	if (m_data != Q_NULLPTR)
		if (m_data->tabWidget != Q_NULLPTR)
		{
			QTableWidget* table = new QTableWidget(docs.count(), 2, this);
			QStringList header;
			header << tr("File") << tr("Open");
			table->setHorizontalHeaderLabels(header);

			for (int i = 0; i < docs.count(); i++)
			{
				QTableWidgetItem* itemPath = new QTableWidgetItem(docs[i]);
				QSize size = itemPath->sizeHint();
				size.setWidth(200);

				itemPath->setSizeHint(size);
				table->setItem(i, 0, itemPath);

				QWidget* pWidget = new QWidget();
				QPushButton* btn_edit = new QPushButton();
				connect(btn_edit, SIGNAL(released()), SLOT(open()));
				btn_edit->setText(tr("Open"));
				btn_edit->setProperty(0, (QVariant)docs[i]);

				QHBoxLayout* pLayout = new QHBoxLayout(pWidget);
				pLayout->addWidget(btn_edit);
				pLayout->setAlignment(Qt::AlignCenter);
				pLayout->setContentsMargins(0, 0, 0, 0);
				pWidget->setLayout(pLayout);

				table->setCellWidget(i, 1, pWidget);
			}
			table->resizeColumnsToContents();

			m_data->tabWidget->addTab(table, tabName);
		}
}
// ----------------------------------------------------------
void WSDocViewer::addPDFDocument(QString path) const
{
	if (m_data != Q_NULLPTR)
		if (m_data->tabWidget != Q_NULLPTR)
		{		
			QFileInfo fInfo(path);		
			XpdfWidget* pdfWidget = Q_NULLPTR;
			pdfWidget = new XpdfWidget();
			if (pdfWidget != Q_NULLPTR)
			{
				pdfWidget->loadFile(path);
				m_data->tabWidget->addTab(pdfWidget, fInfo.baseName());
			}
		}
}
// ----------------------------------------------------------
void WSDocViewer::addHTMLDocument(QString path) const
{
	if (m_data != Q_NULLPTR)
		if (m_data->tabWidget != Q_NULLPTR)
		{
			QFileInfo fInfo(path);
			QWebEngineView* view = Q_NULLPTR;
			view = new QWebEngineView();
			if (view != Q_NULLPTR)
			{
				view->load(QUrl(path));
				m_data->tabWidget->addTab(view, fInfo.baseName());
			}
		}
}
// ----------------------------------------------------------
void WSDocViewer::addHTMLDocument(QUrl url) const
{
	if (m_data != Q_NULLPTR)
		if (m_data->tabWidget != Q_NULLPTR)
		{		
			if (url.isEmpty() == false)
			{
				QWebEngineView* view = Q_NULLPTR;
				view = new QWebEngineView();
				if (view != Q_NULLPTR)
				{
					view->load(url);
					m_data->tabWidget->addTab(view, url.host());
				}
			}
		}
}
// ----------------------------------------------------------
void WSDocViewer::clearViewer()
{
	if (m_data != Q_NULLPTR)
		if (m_data->tabWidget != Q_NULLPTR)
		{
			int count = m_data->tabWidget->count();
			if (count < 1)
				return;

			for (int i = 0; i < count; i++)
			{
				QWidget* widget = Q_NULLPTR;
				widget = m_data->tabWidget->widget(i);
				if (widget != Q_NULLPTR)
				{
					delete widget;
					widget = Q_NULLPTR;
				}
			}
		}	
}
// ----------------------------------------------------------
void WSDocViewer::closeTab(int index)
{
	QWidget* widget = Q_NULLPTR;
	widget = m_data->tabWidget->widget(index);
	if (widget != Q_NULLPTR)
	{
		delete widget;
		widget = Q_NULLPTR;
	}
}
// ----------------------------------------------------------
