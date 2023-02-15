#include <QApplication>
#include <QProcess>
#include <QDebug>
#include <QThread>
#include <QAction>
#include <QIcon>
#include <QTextCodec>
#include <QMessageBox>
#include <QUrl>
#include <QProgressBar>

#include "pluginUpdater.h"
#include "i_wsCore.h"
#include "checkVersion.h"
#include "autoCheckVersion.h"

IMPLEMENT_SMARTPOINTER(PluginUpdater)
// ------------------------------------------------------------
struct PluginUpdater::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;	
	QAction* actionUpdate = Q_NULLPTR;
	bool flagUpdate = false; // true - если есть обновление
};
// ------------------------------------------------------------
PluginUpdater::PluginUpdater()
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;	

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ------------------------------------------------------------
PluginUpdater::~PluginUpdater()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->wsCore = Q_NULLPTR;		

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ------------------------------------------------------------
bool PluginUpdater::initialize(QObject* wsCore)
{
	
	if (m_data == Q_NULLPTR)
		return false;

	if (wsCore == Q_NULLPTR)
		return false;

	m_data->wsCore = qobject_cast<i_wsCore*>(wsCore);
	if (m_data->wsCore == Q_NULLPTR)
		return false;


	if (m_data->wsCore != Q_NULLPTR)
	{
		m_data->wsCore->createSetting("updater/updatePeriod", (int)3600, true);
		m_data->wsCore->createSetting("updater/autoupdate", true, true);

		m_data->actionUpdate = m_data->wsCore->addAction(QIcon(":/Resources/checkUpdate.png"), m_data->codec->toUnicode("Обновить"), Qt::ToolButtonIconOnly);
		if (m_data->actionUpdate != Q_NULLPTR)
		{
			m_data->actionUpdate->setObjectName("actionUpdate");
			connect(m_data->actionUpdate, SIGNAL(triggered()), SLOT(slot_checkUpdate()));
		}


		// Start autocheck
		if (m_data->wsCore->getSetting("updater/autoupdate").toBool() == true)
		{
			int period = m_data->wsCore->getSetting("updater/updatePeriod").toInt();
			AutoCheckVersion* chVersion = new AutoCheckVersion(period);
			QThread* thread = new QThread;
			chVersion->moveToThread(thread);

			// Получить результат
			connect(chVersion, SIGNAL(update()), this, SLOT(slot_availableUpdate()));
			// Запустить процесс
			connect(thread, SIGNAL(started()), chVersion, SLOT(start()));
			// Запустить поток
			thread->start();
		}
	}

	return true;
}
// ------------------------------------------------------------
QString PluginUpdater::getName() const
{
	return QString("pluginUpdater");
}
// ------------------------------------------------------------
QStringList PluginUpdater::references() const
{
	return QStringList();
}
// ------------------------------------------------------------
QIcon PluginUpdater::getIcon() const
{
	return QIcon();
}
// ------------------------------------------------------------
QString PluginUpdater::about() const
{
	if (m_data != Q_NULLPTR)
		if (m_data->codec != Q_NULLPTR)
			return QString(m_data->codec->toUnicode("Плагин выполняет проверку наличия обновления программы на сервере"));
	return QString(tr("Plugin performs check for program update on server"));
}
// ----------------------------------------------------------------
void PluginUpdater::slot_checkUpdate()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->actionUpdate == Q_NULLPTR)
	{
		qDebug() << "Action update is NULL";
		return;
	}
	
	// Если есть обновление - обновить
	if (m_data->flagUpdate == true)
	{
		int res = QMessageBox::question(Q_NULLPTR, m_data->codec->toUnicode("Обновление"), m_data->codec->toUnicode("Программа будет закрыта. Продолжить?"));
		if (res == QMessageBox::Yes)
		{
			m_data->actionUpdate->setEnabled(false);

			QString pathUpdater = qApp->applicationDirPath() + QString("/uninstall.exe");
			QStringList param;
			param.push_back(QString("--updater"));

			QProcess test;
			test.startDetached(pathUpdater, param);

			QCoreApplication::exit(-100);

		}
	}
	else // Иначе проверить наличие обновления
	{
		m_data->actionUpdate->setEnabled(false);

		CheckVersion* chVersion = new CheckVersion();
		QThread* thread = new QThread;
		chVersion->moveToThread(thread);

		// Получить результат
		connect(chVersion, SIGNAL(update()), this, SLOT(slot_availableUpdate()));
		connect(chVersion, SIGNAL(notupdate(QString)), this, SLOT(slot_noUpdate(QString)));
		// Запустить процесс
		connect(thread, SIGNAL(started()), chVersion, SLOT(start()));
		// Остановить поток
		connect(chVersion, SIGNAL(finished()), thread, SLOT(quit()));
		// Удалить обработчик
		connect(chVersion, SIGNAL(finished()), chVersion, SLOT(deleteLater()));
		// Удалить поток
		connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
		// Запустить поток
		thread->start();
	}
}
// ----------------------------------------------------------------
void PluginUpdater::slot_availableUpdate()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->wsCore != Q_NULLPTR)
			m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Модуль обновлений"), m_data->codec->toUnicode("Доступно обновление"));

		if (m_data->actionUpdate != Q_NULLPTR)
		{
			m_data->actionUpdate->setIcon(QIcon(":/Resources/update.png"));
			m_data->actionUpdate->setEnabled(true);
			m_data->flagUpdate = true;
		}
	}	
}
// ----------------------------------------------------------------
void PluginUpdater::slot_noUpdate(QString message)
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->actionUpdate != Q_NULLPTR)
		{
			m_data->actionUpdate->setIcon(QIcon(":/Resources/checkUpdate.png"));
			m_data->actionUpdate->setEnabled(true);
		}

		if (m_data->wsCore != Q_NULLPTR)
			m_data->wsCore->showMessageTaskBar(m_data->codec->toUnicode("Модуль обновлений"), message);
	}	
}
// ----------------------------------------------------------------