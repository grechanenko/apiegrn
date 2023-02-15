#include <QDebug>
#include <QWidget>
#include <QTextCodec>

#include "QtnRibbonPage.h"
#include "QtnRibbonGroup.h"

#include "wgtPage.h"
#include "wgtSearchDialog.h"
#include "i_wsCore.h"
#include "i_wsAPIRosreestr.h"
#include "i_wsProjects.h"
#include "i_wsProject.h"

// ----------------------------------------------------------------
struct WGTPage::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;

	QAction* actionSearchSingle = Q_NULLPTR;
	QAction* actionSearchPackage = Q_NULLPTR;

};
// ----------------------------------------------------------------
WGTPage::WGTPage(i_wsCore* wsCore, QObject* parent)
	:QObject(parent)
{
	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}

	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->wsCore = wsCore;


	QWidget* tabWidget = Q_NULLPTR;
	tabWidget = m_data->wsCore->createTab(m_data->codec->toUnicode("Выписки"));
	if (tabWidget == Q_NULLPTR)
	{
		qDebug() << "Can not create TabWidget";
		return;
	}


	Qtitan::RibbonPage* tab = Q_NULLPTR;
	tab = qobject_cast<Qtitan::RibbonPage*>(tabWidget);
	if (tab == Q_NULLPTR)
	{
		qDebug() << "Convert QWidget* to RibbonPage* FAILED!";
		return;
	}

	// Блок "Поиска выписок"
	if (Qtitan::RibbonGroup* groupOrder = tab->addGroup(m_data->codec->toUnicode("Поиск")))
	{
		m_data->actionSearchSingle = groupOrder->addAction(QIcon(":/Resources/searchSingle.png"), m_data->codec->toUnicode("Одиночный"), Qt::ToolButtonTextUnderIcon);
		connect(m_data->actionSearchSingle, SIGNAL(triggered()), SLOT(slt_searchSingle()));
		m_data->actionSearchSingle->setShortcut(QKeySequence::Find);
		m_data->actionSearchSingle->setEnabled(false);

		m_data->actionSearchPackage = groupOrder->addAction(QIcon(":/Resources/searchPackage.png"), m_data->codec->toUnicode("Пакетный"), Qt::ToolButtonTextUnderIcon);
		connect(m_data->actionSearchPackage, SIGNAL(triggered()), SLOT(slt_searchPackage()));
		//m_data->actionSearchPackage->setShortcut(QKeySequence::Find);
		m_data->actionSearchPackage->setEnabled(false);
	}
}
// ----------------------------------------------------------------
WGTPage::~WGTPage()
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
void WGTPage::setEnabled(bool flag)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->actionSearchSingle != Q_NULLPTR)
			m_data->actionSearchSingle->setEnabled(flag);

		if (m_data->actionSearchPackage != Q_NULLPTR)
			m_data->actionSearchPackage->setEnabled(flag);
	}
	else
		qDebug() << "m_data is NULL";
}
// ----------------------------------------------------------------
void WGTPage::slt_searchSingle()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "WsCore is NULL";
		return;
	}

	WGTSearchDislog* dialog = Q_NULLPTR;
	dialog = new WGTSearchDislog();

	bool flag = false;
	QString query = QString();
	if (dialog->exec() == QDialog::Accepted)
	{
		flag = true;
		query = dialog->getText();
	}
	delete dialog;
	
	if (flag == false)
		return;
	
	QObject* objProjects = Q_NULLPTR;
	objProjects = m_data->wsCore->getProjects();
	if (objProjects == Q_NULLPTR)
	{
		qDebug() << "Object projects is NULL";
		m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Во время поиска объекта произошел сбой"));
		return;
	}

	i_wsProjects* projects = Q_NULLPTR;
	projects = qobject_cast<i_wsProjects*>(objProjects);
	if (projects == Q_NULLPTR)
	{
		qDebug() << "Can not convert Object* to i_wsProjects*";
		m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Во время поиска объекта произошел сбой"));
		return;
	}

	QObject* objAPI = Q_NULLPTR;
	objAPI = m_data->wsCore->getAPIRosreestr();
	if (objAPI == Q_NULLPTR)
	{
		qDebug() << "Object APIRosreestr is NULL";
		m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Во время поиска объекта произошел сбой"));
		return;
	}
	
	i_wsAPIRosreestr* api = Q_NULLPTR;
	api = qobject_cast<i_wsAPIRosreestr*>(objAPI);
	if (api == Q_NULLPTR)
	{
		qDebug() << "Can not convert Object* to i_wsAPIRosreestr*";
		m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Во время поиска объекта произошел сбой"));
		return;
	}

	i_wsProject* project = projects->getActive();

	if (api->findObject(query, project) == false)
	{
		qDebug() << "Can not find object";
		m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Поиск объекта"), m_data->codec->toUnicode("Во время поиска объекта произошел сбой"));
	}
}
// ----------------------------------------------------------------