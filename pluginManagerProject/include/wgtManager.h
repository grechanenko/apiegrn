#pragma once

#include <QDockWidget>

class i_wsProjectCore;
class i_wsProject;
class WGTManager : public QDockWidget
{
	Q_OBJECT
public:
	WGTManager(QObject* projects, QWidget* parent = 0);
	~WGTManager();

private:

	struct SDATA;
	SDATA* m_data;

private slots:

	/*!
		Отобразить проект
	*/
	void slt_showProject(i_wsProject*);	
	
};
