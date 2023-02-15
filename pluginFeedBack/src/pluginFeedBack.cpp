#include <QDir>
#include <QAction>
#include <QIcon>
#include <QMessageBox>
#include <QTextCodec>

#include "i_wsCore.h"
#include "pluginFeedBack.h"
#include "wgtFeedback.h"
#include "smtp.h"

IMPLEMENT_SMARTPOINTER(PluginFeedBack)
// ------------------------------------------------------------
struct PluginFeedBack::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
	Smtp* smtp = Q_NULLPTR;
};
// ------------------------------------------------------------
PluginFeedBack::PluginFeedBack(QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	m_data->smtp = new Smtp("apirosreestrdesctop@mail.ru", "22zxcv22", "smtp.mail.ru", 465);
	connect(m_data->smtp, SIGNAL(status(QString)), this, SLOT(mailStatus(QString)));
}
// ------------------------------------------------------------
PluginFeedBack::~PluginFeedBack()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		m_data->wsCore = Q_NULLPTR;

		if (m_data->smtp != Q_NULLPTR)
		{
			delete m_data->smtp;
			m_data->smtp = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ------------------------------------------------------------
bool PluginFeedBack::initialize(QObject* wsCore)
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
		QAction* action = Q_NULLPTR;
		action = m_data->wsCore->addAction(QIcon(":/Resources/feedback.png"), m_data->codec->toUnicode("Отправить отзыв"), Qt::ToolButtonIconOnly);
		if (action != Q_NULLPTR)
		{
			action->setObjectName("actionFeedBack");
			connect(action, SIGNAL(triggered()), SLOT(slot_showDialog()));
		}		
	}

	return true;
}
// ------------------------------------------------------------
QString PluginFeedBack::getName() const
{
	return QString("pluginFeedBack");
}
// ------------------------------------------------------------
QStringList PluginFeedBack::references() const
{
	return QStringList();
}
// ------------------------------------------------------------
QIcon PluginFeedBack::getIcon() const
{
	return QIcon();
}
// ------------------------------------------------------------
QString PluginFeedBack::about() const
{
	return QString(tr("Plugin widget feedback"));
}
// ----------------------------------------------------------------
void PluginFeedBack::slot_showDialog()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->wsCore != Q_NULLPTR)
		{
			WGTFeedback* wgtFeedback = Q_NULLPTR;
			wgtFeedback = new WGTFeedback();
			if (wgtFeedback != Q_NULLPTR)
			{
				if (wgtFeedback->exec() == QDialog::Accepted)
				{
					QString text = wgtFeedback->getText();
					QString theme = wgtFeedback->getTheme();

					{
						text += "\r\n" + QString("-------------------");
						text += "\r\n" + m_data->codec->toUnicode("Отправил: ") + m_data->wsCore->getSetting("auth/login").toString();
					}

					if (text.isEmpty() == true)
						return;
										

					QString path = QDir::homePath() + QString("/AppData/Roaming/APIService/log/");					
					QStringList files;					
					{
						QDir dir(path);
						QStringList listFiles = dir.entryList(QDir::Files);
						if (listFiles.size() > 0)
							for each (QString file in listFiles)
							{
								QFileInfo fInfo(file);
								if (fInfo.suffix() == "log")
									files.append(path + file);
							}
					}

					if (m_data->smtp != Q_NULLPTR)
					{
						if (files.size() < 0)
							m_data->smtp->sendMail("apirosreestrdesctop@mail.ru", "apirosreestrdesctop@mail.ru", theme, text);
						else
							m_data->smtp->sendMail("apirosreestrdesctop@mail.ru", "apirosreestrdesctop@mail.ru", theme, text, files);
					}
				}
				delete wgtFeedback;
				wgtFeedback = Q_NULLPTR;				
			}
		}
	}
}
// ------------------------------------------------------------
void PluginFeedBack::mailStatus(QString message)
{
	qInfo() << m_data->codec->toUnicode("Отправка сообщения") << message;
	if (m_data == Q_NULLPTR)
	{
		qCritical() << "m_data is null";
		return;
	}
	
	if (message.isEmpty() == false)
		QMessageBox::information(Q_NULLPTR, m_data->codec->toUnicode("Отзывы"), message);
}
// ------------------------------------------------------------
