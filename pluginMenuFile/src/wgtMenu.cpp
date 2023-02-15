#include <QDebug>
#include <QAction>
#include <QMenu>
#include <QTextCodec>
#include <QFileDialog>

#include "QtnRibbonSystemPopupBar.h"

#include "i_wsProjects.h"
#include "i_wsCore.h"
#include "i_wsAPIRosreestr.h"
#include "wgtMenu.h"

// ----------------------------------------------------------------
struct WGTMenu::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
	QAction* actionFile = Q_NULLPTR;
	QAction* actionCreate = Q_NULLPTR;
	QAction* actionOpen = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTMenu::WGTMenu(i_wsCore* wsCore, QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}
	m_data->wsCore = wsCore;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	bool flagConnect = false;
	QObject* objAPIRosreestr = Q_NULLPTR;
	objAPIRosreestr = m_data->wsCore->getAPIRosreestr();
	if (objAPIRosreestr != Q_NULLPTR)
	{
		i_wsAPIRosreestr* wsAPI = Q_NULLPTR;
		wsAPI = qobject_cast<i_wsAPIRosreestr*>(objAPIRosreestr);
		if (wsAPI != Q_NULLPTR)
		{
			flagConnect = wsAPI->isConnecting();
		}
		else
			qDebug() << "wsAPI is NULL";

	}
	else
		qDebug() << "Object APIRosreest is NULL";

	QIcon icon;
	icon.addPixmap(QPixmap(":/Resources/icon32.png"));
	icon.addPixmap(QPixmap(":/Resources/icon32.png"));

	m_data->actionFile = m_data->wsCore->addSystemButton(icon, m_data->codec->toUnicode("&Файл"));
	if (m_data->actionFile == Q_NULLPTR)
	{
		qDebug() << "Can not crate action file. Action file is NULL";
		return;
	}
	
	m_data->actionFile->setToolTip(m_data->codec->toUnicode("Открыть/закрыть проект"));

	Qtitan::RibbonSystemPopupBar* popupBar = Q_NULLPTR;
	popupBar = qobject_cast<Qtitan::RibbonSystemPopupBar*>(m_data->actionFile->menu());

	if (popupBar == Q_NULLPTR)
	{
		qDebug() << "Popub bar is NULL";
		return;
	}

	m_data->actionCreate = popupBar->addAction(QIcon(":/Resources/new.png"), m_data->codec->toUnicode("&Создать..."));
	connect(m_data->actionCreate, SIGNAL(triggered()), this, SLOT(slt_create()));
	m_data->actionCreate->setShortcut(tr("Ctrl+N"));
	m_data->actionCreate->setStatusTip(m_data->codec->toUnicode("Создать новый проект"));
	m_data->actionCreate->setToolTip(m_data->codec->toUnicode("Создать"));
	m_data->actionCreate->setEnabled(false);

	m_data->actionOpen = popupBar->addAction(QIcon(":/Resources/open.png"), m_data->codec->toUnicode("&Открыть..."));
	connect(m_data->actionOpen, SIGNAL(triggered()), this, SLOT(slt_open()));
	m_data->actionOpen->setShortcut(tr("Ctrl+O"));
	m_data->actionOpen->setToolTip(m_data->codec->toUnicode("Открыть"));
	m_data->actionOpen->setStatusTip(m_data->codec->toUnicode("Открыть существующий проект"));
	m_data->actionOpen->setEnabled(true);


	/*QAction* saveFile = popupBar->addAction(QIcon(":/Resources/save.png"), tr("&Save"));
	saveFile->setShortcut(tr("Ctrl+S"));
	saveFile->setToolTip(tr("Save"));
	saveFile->setStatusTip(tr("Save the active document"));
	connect(saveFile, SIGNAL(triggered()), this, SLOT(save()));

	QAction* saveAsFile = popupBar->addAction(QIcon(":/Resources/save.png"),  tr("Save &As..."));
	saveAsFile->setToolTip(tr("Save As"));
	saveAsFile->setStatusTip(tr("Save the active document with a new name"));
	connect(saveAsFile, SIGNAL(triggered()), this, SLOT(save()));

	QAction* actClose = popupBar->addAction(QIcon(":/Resources/close.png"), tr("&Close"));
	actClose->setShortcut(tr("Ctrl+C"));
	actClose->setStatusTip(tr("Exit"));
	connect(actClose, SIGNAL(triggered()), this, SLOT(close()));						*/
	
	



	/*QMenu* menu = m_data->wsCore->addMenu(m_data->codec->toUnicode("Личный кабинет"));

	m_data->actionConnect = menu->addAction(m_data->codec->toUnicode("Вход"));
	connect(m_data->actionConnect, SIGNAL(triggered()), this, SLOT(slt_connect()));
	m_data->actionConnect->setEnabled(false);

	m_data->actionDisconnect = menu->addAction(m_data->codec->toUnicode("Выход"));
	connect(m_data->actionDisconnect, SIGNAL(triggered()), this, SLOT(slt_disconnect()));
	m_data->actionDisconnect->setEnabled(false);

	*/

	this->setEnabledForConnection(flagConnect);
}
// ----------------------------------------------------------------
WGTMenu::~WGTMenu()
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
void WGTMenu::setEnabledForConnection(bool flag)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->actionCreate != Q_NULLPTR)
	{
		m_data->actionCreate->setEnabled(flag);
	}
	else
		qDebug() << "Action Create is NULL";	
}
// ----------------------------------------------------------------
void WGTMenu::slt_create()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}

	QString filePath = QFileDialog::getSaveFileName(Q_NULLPTR, m_data->codec->toUnicode("Создать файл проекта"), QDir::homePath(), "*.xml");
	if (filePath.isEmpty() == true)
	{
		qDebug() << "Saved filePath is NULL";
		return;
	}

	QObject* obj = Q_NULLPTR;
	obj = m_data->wsCore->getProjects();

	i_wsProjects* prs = Q_NULLPTR;
	prs = qobject_cast<i_wsProjects*>(obj);

	if (prs == Q_NULLPTR)
	{
		qDebug() << "Class projects is NULL";
		return;
	}

	bool res = prs->createProject(filePath);
	if (res == false)
		qDebug() << "Can not create file project";
}
// ----------------------------------------------------------------
void WGTMenu::slt_open()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}

	QObject* obj = Q_NULLPTR;
	obj = m_data->wsCore->getProjects();
	if (obj == Q_NULLPTR)
	{
		qDebug() << "Projects is NULL";
		return;
	}

	i_wsProjects* prs = Q_NULLPTR;
	prs = qobject_cast<i_wsProjects*>(obj);
	if (prs == Q_NULLPTR)
	{
		qDebug() << "Class projects is NULL";
		return;
	}

	QString filePath = QFileDialog::getOpenFileName(Q_NULLPTR, m_data->codec->toUnicode("Открыть файл проекта"), QDir::homePath(), "*.xml");
	if (filePath.isEmpty() == true)
	{
		qDebug() << "Saved filePath is NULL";
		return;
	}

	bool res = prs->openProject(filePath);
	if (res == false)
		qDebug() << "Can not open file project";
}
// ----------------------------------------------------------------