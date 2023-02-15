#include <QFileDialog>
#include <QApplication>
#include <QDesktopWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QProgressBar>
#include <QLineEdit>
#include <QRadioButton>
#include <QScrollBar>
#include <QFontComboBox>
#include <QAction>
#include <QMenu>
#include <QLabel>
#include <QSpinBox>
#include <QMessageBox>
#include <QButtonGroup>
#include <QPushButton>
#include <QSettings>
#include <QEvent>
#include <QPainter>
#include <QPaintEvent>
#include <QWidget>
#include <QDebug>

#include "QtnOfficeDefines.h"
#include "QtnRibbonQuickAccessBar.h"
#include "QtnOfficePopupWindow.h"

#include "mainwindow.h"
#include "wsDocViewer.h"

// ----------------------------------------------------------------
struct MainWindow::SDATA
{
	Qtitan::RibbonPage* pageContext;
	WSDocViewer* docViewer = Q_NULLPTR;
};
// ----------------------------------------------------------------
MainWindow::MainWindow(QWidget* parent)
	: RibbonWindow(parent)	
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	
	this->setMinimumHeight(700);
	this->setMinimumWidth(700);
		
	qApp->setStyle(new RibbonStyle);			
	
	ribbonBar()->setTitleBarVisible(false);
	ribbonBar()->setFrameThemeEnabled();	
		
	m_data->docViewer = new WSDocViewer();
	if (m_data->docViewer != Q_NULLPTR)
		this->setCentralWidget(m_data->docViewer);
		
	//this->statusBar();
}
// ----------------------------------------------------------------
MainWindow::~MainWindow()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}

	this->deleteLater();
}
// ----------------------------------------------------------------
QAction* MainWindow::addAction(const QIcon& icon, const QString& caption, Qt::ToolButtonStyle style, QMenu* menu)
{
	Qtitan::RibbonBar* bar = Q_NULLPTR;
	bar = ribbonBar();
	if (bar != Q_NULLPTR)
	{
		QAction* action = Q_NULLPTR;
		action = bar->addAction(icon, caption, style);
		return action;
	}
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QMenu* MainWindow::addMenu(const QString& caption)
{
	Qtitan::RibbonBar* bar = Q_NULLPTR;
	bar = ribbonBar();
	if (bar != Q_NULLPTR)
	{
		QMenu* menu = Q_NULLPTR;
		menu = ribbonBar()->addMenu(caption);
		if (menu != Q_NULLPTR)
			return menu;
	}
	qDebug() << "Can not create menu. Ribbon bar is NULL";
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QWidget* MainWindow::createTab(QString name)
{
	Qtitan::RibbonPage* pageButtons = ribbonBar()->addPage(name);
	return qobject_cast<QWidget*>(pageButtons);
}
// ----------------------------------------------------------------
QAction* MainWindow::addSystemButton(QIcon iconLogo, QString name)
{
	if (!iconLogo.isNull())
		if (!name.isEmpty())
		{
			QAction* action = Q_NULLPTR;
			action = ribbonBar()->addSystemButton(iconLogo, name);

			QToolButton* button = Q_NULLPTR;
			if (button = ribbonBar()->getSystemButton())
				button->setToolButtonStyle(Qt::ToolButtonTextOnly);

			return action;
		}
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
QWidget* MainWindow::createQuickAccessBar()
{
	Qtitan::RibbonBar* bar = Q_NULLPTR;
	bar = ribbonBar();
	if (bar != Q_NULLPTR)
	{
		Qtitan::RibbonQuickAccessBar* quickAccessBar = Q_NULLPTR;
		quickAccessBar = bar->quickAccessBar();
		if (quickAccessBar != Q_NULLPTR)
		{
			QWidget* widget = Q_NULLPTR;
			widget = qobject_cast<QWidget*>(quickAccessBar);

			if (widget != Q_NULLPTR)
			{
				bar->showQuickAccess(true);
				return widget;
			}
		}
	}
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
i_docViewer* MainWindow::getDocViewer()
{
	if (m_data != Q_NULLPTR)
		if (m_data->docViewer != Q_NULLPTR)
		{
			i_docViewer* viewer = Q_NULLPTR;
			viewer = qobject_cast<i_docViewer*>(m_data->docViewer);
			if (viewer != Q_NULLPTR)
				return viewer;
		}
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
void MainWindow::showMessageTaskBar(QString title, QString text) const
{
	if (text.isEmpty() == true)
	{
		qDebug() << "ShowText is NULL";
		return;
	}	

	if (title.isEmpty() == true)
		title = QString("APIService");

	QWidget* parent = Q_NULLPTR;
	parent = this->parentWidget();

	OfficePopupWindow* popup = new OfficePopupWindow(parent);	

	//connect(popup, SIGNAL(releaseMouseOnWidget()), SLOT(slot_releaseMouseOnMessage()));

	popup->setTitleText("<h4>&nbsp; APIService</h4> <h4>&nbsp; " + title + "</h4><p> </p>");
	popup->setBodyText("<p>&nbsp; " + text + "&nbsp; <p>");
	popup->setTitleCloseButtonVisible();
	if (Qtitan::OfficeStyle* ribbonStyle = qobject_cast<Qtitan::OfficeStyle*>(popup->style()))
		ribbonStyle->setPopupDecoration(OfficeStyle::PopupOfficeFlatDecoration);
	   	
	popup->setDragDropEnabled(false); 
	popup->setTransparency((qreal)200.0 / 255.0);
	popup->setDisplayTime(5000);
	popup->setAnimation(PopupAnimationSlide);
	popup->setAnimationSpeed(1);
	popup->setAttribute(Qt::WA_DeleteOnClose);
	popup->showPopup();
}