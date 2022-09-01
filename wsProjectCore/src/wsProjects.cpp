#include <QDebug>
#include <QTextCodec>

#include "wsProjects.h"
#include "i_wsCore.h"
#include "i_wsAPIRosreestr.h"

#include "wsProject.h"
#include "projectFile.h"

IMPLEMENT_SMARTPOINTER(WSProjects)
// ----------------------------------------------------------------
struct WSProjects::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
	QVector<i_wsProject*> projects;
	unsigned int indexActivePrjoect = 0;
};
// ----------------------------------------------------------------
WSProjects::WSProjects(i_wsCore* wsCore, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->wsCore = wsCore;
}
// ----------------------------------------------------------------
WSProjects::~WSProjects()
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
bool WSProjects::createProject(QString path)
{
	if (path.isEmpty() == true)
	{
		qDebug() << "File path is NULL";
		return false;
	}	

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "WsCore is NULL";
		return false;
	}

	QObject* objAPI = Q_NULLPTR;
	objAPI = m_data->wsCore->getAPIRosreestr();
	if (objAPI == Q_NULLPTR)
	{
		qDebug() << "Object APIRosreestr is NULL";
		return false;
	}

	i_wsAPIRosreestr* api = Q_NULLPTR;
	api = qobject_cast<i_wsAPIRosreestr*>(objAPI);
	if (api == Q_NULLPTR)
	{
		qDebug() << "Can not convert QObject* to i_wsAPIRosreestr*";
		return false;
	}


	bool flag = WSProject::createProjectFile(path, api);
	if (flag == false)
	{
		qDebug() << "Can not creat project file";
		return false;
	}

	i_wsProject* project = Q_NULLPTR;
	project = WSProject::create(path, objAPI);
	
	if (m_data != Q_NULLPTR)
	{
		m_data->projects.push_back(project);
		emit sig_createProject(project);
		return true;
	}
	else
		qDebug() << "m_data is NULL";
	return false;
}
// ----------------------------------------------------------------
bool WSProjects::openProject(QString filePath)
{
	if (filePath.isEmpty() == true)
	{
		qDebug() << "File path is empty";
		return false;
	}

	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}
		
	bool flag = WSProject::isValidProjectFile(filePath);
	if (flag == false)
	{
		qDebug() << "File project is failed";
		emit sig_message(m_data->codec->toUnicode("Отрытие проекта"), m_data->codec->toUnicode("Не могу открыть файл. Неправильная структура"));
		return false;
	}

	QObject* objAPI = Q_NULLPTR;
	objAPI = m_data->wsCore->getAPIRosreestr();
	if (objAPI == Q_NULLPTR)
	{
		qDebug() << "Object APIRosreestr is NULL";
		return false;
	}	

	i_wsProject* p = WSProject::create(filePath, objAPI);
	m_data->projects.push_back(p);

	emit sig_openProject(p);

	return true;
}
// ----------------------------------------------------------------
i_wsProject* WSProjects::getProject(int index) const
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->projects.size() > index)
		{
			i_wsProject* p = m_data->projects[index];
			return p;
		}
		else
			qDebug() << "Out of range";
	}
	else
		qDebug() << "m_data is NULL";
	
	return Q_NULLPTR;
}
// ----------------------------------------------------------------
void WSProjects::closeProject(int index)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->projects.size() > index)
		{
			QString filePath = m_data->projects[index]->filePath();

			m_data->projects[index]->release();
			m_data->projects[index] = Q_NULLPTR;	
			m_data->projects.remove(index);

			emit sig_closeProject(filePath);
		}
		else
			qDebug() << "Out of range";
	}
	else
		qDebug() << "m_data is NULL";	
}
// ----------------------------------------------------------------
QVector<i_wsProject*> WSProjects::projects()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QVector<i_wsProject*>();
	}

	return m_data->projects;
}
// ----------------------------------------------------------------
i_wsProject* WSProjects::getActive()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return Q_NULLPTR;
	}

	QVector<i_wsProject*> projects = m_data->projects;
	if (projects.size() > m_data->indexActivePrjoect)
	{
		return projects.at(m_data->indexActivePrjoect);
	}
	else
		qDebug() << "Out of range";

	return Q_NULLPTR;
}
// ----------------------------------------------------------------
bool WSProjects::setActive(unsigned int index)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return false;
	}

	QVector<i_wsProject*> projects = this->projects();
	if (projects.size() < index)
	{
		m_data->indexActivePrjoect = index;
		return true;
	}
	else
		qDebug() << "Out of range";
	
	return false;
}
// ----------------------------------------------------------------
WSProjects* WSProjects::create(i_wsCore* wsCore)
{
	return(new WSProjects(wsCore));
}
// ----------------------------------------------------------------
