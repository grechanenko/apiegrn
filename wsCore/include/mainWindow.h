#pragma once

#include <QTimer>
#include "ribbonwindow.h"

#include "i_docViewer.h"

/* MainWindow */
class MainWindow : public RibbonWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget* parent = Q_NULLPTR);
	virtual ~MainWindow();

	
	/*!
		Create action from right bar window
	*/
	QAction* addAction(const QIcon& icon, const QString& caption, Qt::ToolButtonStyle style, QMenu* menu = Q_NULLPTR);


	/*!
		Create menu from right bar window
	*/
	QMenu* addMenu(const QString& caption);


	/*!
		Create tab in ribbon
	*/
	QWidget* createTab(QString name);


	/*!
		Create system menu "File"
	*/
	QAction* addSystemButton(QIcon iconLogo, QString name);


	/*!
		Create quic access bar
	*/
	QWidget* createQuickAccessBar();
	

	/*!
		Get document viewer
	*/
	i_docViewer* getDocViewer();

	/*!
		Show message in task bar
	*/
	void showMessageTaskBar(QString title, QString text) const;

private:

	struct SDATA;
	SDATA* m_data;	
};