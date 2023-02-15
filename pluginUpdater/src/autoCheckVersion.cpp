#include <QThread>
#include <QNetworkRequest>
#include <QNetworkReply>

#include "autoCheckVersion.h"
#include "checkVersion.h"

// -------------------------------------------------
struct AutoCheckVersion::SDATA
{	
	QNetworkAccessManager* manager = Q_NULLPTR;
	int interval = 15000;
};
// -------------------------------------------------
AutoCheckVersion::AutoCheckVersion(int interval, QObject* parent)
	: QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->interval = interval;
	
	m_data->manager = new QNetworkAccessManager(this);
	connect(m_data->manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));
}
// -------------------------------------------------
AutoCheckVersion::~AutoCheckVersion()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->manager != Q_NULLPTR)
		{
			delete m_data->manager;
			m_data->manager = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// -------------------------------------------------
void AutoCheckVersion::checkVersion()
{
	CheckVersion* chVersion = new CheckVersion();
	QThread* thread = new QThread;
	chVersion->moveToThread(thread);

	// Получить результат
	connect(chVersion, SIGNAL(update()), this, SLOT(slot_update()), Qt::DirectConnection);
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
// -------------------------------------------------
void AutoCheckVersion::start()
{
	if (m_data != Q_NULLPTR)
	{
		while (true)
		{
			this->checkVersion();

			QThread::sleep(m_data->interval);
		}
	}
}
// -------------------------------------------------
void AutoCheckVersion::slot_update()
{
	emit update();
}
// -------------------------------------------------