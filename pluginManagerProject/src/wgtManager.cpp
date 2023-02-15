#include <QDebug>
#include <QTextCodec>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVector>

#include "wgtManager.h"
#include "i_wsProjects.h"
#include "i_wsProject.h"

// ----------------------------------------------
struct WGTManager::SDATA
{
	QTextCodec* codec = Q_NULLPTR;	
	QObject* projects = Q_NULLPTR;
	QTreeWidget* tWidget = Q_NULLPTR;	
};
// ----------------------------------------------
WGTManager::WGTManager(QObject* projects, QWidget* parent)
	:QDockWidget(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->tWidget = new QTreeWidget(this);
	m_data->tWidget->setHeaderHidden(true);

	m_data->projects = projects;
	if (m_data->projects != Q_NULLPTR)
	{
		connect(m_data->projects, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt_showProject(i_wsProject*)));
		connect(m_data->projects, SIGNAL(sig_openProject(i_wsProject*)), SLOT(slt_showProject(i_wsProject*)));
		//connect(m_data->projects, SIGNALS(), SLOTS());
	}	

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
	
	this->setWindowTitle(m_data->codec->toUnicode("Управление проектами"));
	this->setMinimumWidth(200);
	this->setWindowFlags(Qt::SubWindow | Qt::CustomizeWindowHint | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint);

	this->setWidget(m_data->tWidget);
}
// ----------------------------------------------
WGTManager::~WGTManager()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->projects = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------
void WGTManager::slt_showProject(i_wsProject* project)
{
	if (project == Q_NULLPTR)
	{
		qDebug() << "Can not add project. Project is NULL";
		return;
	}
	
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}
	
	if (m_data->tWidget == Q_NULLPTR)
	{
		qDebug() << "Tree widget is NULL";
		return;
	}

	QString name = QString();
	name = project->name();
	if (name.isEmpty() == true)
		name = QString("Default");


	QTreeWidgetItem* orderItems = new QTreeWidgetItem(m_data->tWidget);
	orderItems->setText(0, name);
	orderItems->setIcon(0, QIcon(":/Resources/project.png"));
}
// ----------------------------------------------