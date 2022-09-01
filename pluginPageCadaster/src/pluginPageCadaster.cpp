#include <QDebug>
#include <QTextCodec>
#include <QIcon>

#include "pluginPageCadaster.h"
#include "i_wsCore.h"
#include "i_wsProjects.h"
#include "i_wsProject.h"

#include "wgtPage.h"

IMPLEMENT_SMARTPOINTER(PluginPageCadaster)
// ----------------------------------------------------------------
struct PluginPageCadaster::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;	
	WGTPage* wgtPage = Q_NULLPTR;
	i_wsProjects* projects = Q_NULLPTR;
};
// ----------------------------------------------------------------
PluginPageCadaster::PluginPageCadaster(QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ----------------------------------------------------------------
PluginPageCadaster::~PluginPageCadaster()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->wsCore = Q_NULLPTR;		

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
bool PluginPageCadaster::initialize(QObject* wsCore)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	if (m_data->codec == Q_NULLPTR)
	{
		qDebug() << "Codec is NULL";
		return false;
	}
		
	m_data->wsCore = qobject_cast<i_wsCore*>(wsCore);
	if (m_data->wsCore != Q_NULLPTR)
	{
		// Создаем вкладку
		m_data->wgtPage = new WGTPage(m_data->wsCore);
		if (m_data->wgtPage != Q_NULLPTR)
		{
			// Берем проекты
			QObject* objProjects = Q_NULLPTR;
			objProjects = m_data->wsCore->getProjects();
			// Следим за составом открытых проектов
			connect(objProjects, SIGNAL(sig_createProject(i_wsProject*)), SLOT(slt_createProject(i_wsProject*))); // Сообщает о создании проекта
			connect(objProjects, SIGNAL(sig_openProject(i_wsProject*)), SLOT(slt_openProject(i_wsProject*)));	  // Сообщает об открытии проекта	
			connect(objProjects, SIGNAL(sig_closeProject(QString)), SLOT(slt_closeProject(QString)));			  // Сообщает о закрытии проекта

			
			m_data->projects = qobject_cast<i_wsProjects*>(objProjects);

			if (m_data->projects != Q_NULLPTR)
			{
				QVector<i_wsProject*> listProject = m_data->projects->projects();
				if (listProject.size() > 0)
					m_data->wgtPage->setEnabled(true);
			}

		}
		else
			qDebug() << "Can not create Widget paget";
	}
	else
	{
		qDebug() << "Convert QObject* to i_wsCore* FAILED!";
		return false;
	}
	

	qDebug() << "Initialise failed";
	return false;
}
// ----------------------------------------------------------------
QString PluginPageCadaster::getName() const
{	
	return QString("PluginPageCadaster");
}
// ----------------------------------------------------------------
QStringList PluginPageCadaster::references() const
{
	return QStringList();
}
// ----------------------------------------------------------------
QIcon PluginPageCadaster::getIcon() const
{
	return QIcon();
}
// ----------------------------------------------------------------
QString PluginPageCadaster::about() const
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->codec != Q_NULLPTR)
		{
			return m_data->codec->toUnicode("Данный модуль предоставляет функционал для работы с выписками");
		}
		else
			qDebug() << "codec is NULL";
	}
	else
		qDebug() << "m_data is NULL";

	return QString(tr("Plugin page cadaster"));
}
// ----------------------------------------------------------------
void PluginPageCadaster::slt_createProject(i_wsProject* project)
{
	if (project == Q_NULLPTR)
	{
		qDebug() << "Project is NULL";
		return;
	}

	if (m_data != Q_NULLPTR)
	{
		if (m_data->wgtPage != Q_NULLPTR)
		{
			m_data->wgtPage->setEnabled(true);
		}
		else
			qDebug() << "wgtPage is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------
void PluginPageCadaster::slt_openProject(i_wsProject* project)
{
	if (project == Q_NULLPTR)
	{
		qDebug() << "Project is NULL";
		return;
	}

	if (m_data != Q_NULLPTR)
	{
		if (m_data->wgtPage != Q_NULLPTR)
		{
			m_data->wgtPage->setEnabled(true);
		}
		else
			qDebug() << "wgtPage is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------
void PluginPageCadaster::slt_closeProject(QString)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->projects != Q_NULLPTR)
		{
			QVector<i_wsProject*> listProject = m_data->projects->projects();

			if (m_data->wgtPage != Q_NULLPTR)
			{
				if (listProject.size() > 0)
					m_data->wgtPage->setEnabled(true);
				else
					m_data->wgtPage->setEnabled(false);
			}
			else
				qDebug() << "wgtPage is NULL";
		}
		else
			qDebug() << "Projects is NULL";
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------