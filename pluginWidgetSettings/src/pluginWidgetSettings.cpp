#include <QAction>
#include <QIcon>
#include <QTextCodec>

#include "i_wsCore.h"
#include "pluginWidgetSettings.h"
#include "wgtSettings.h"

IMPLEMENT_SMARTPOINTER(PluginWidgetSettings)
// ------------------------------------------------------------
struct PluginWidgetSettings::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
};
// ------------------------------------------------------------
PluginWidgetSettings::PluginWidgetSettings(QObject* parent)
	:QObject(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ------------------------------------------------------------
PluginWidgetSettings::~PluginWidgetSettings()
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
bool PluginWidgetSettings::initialize(QObject* wsCore)
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
		action = m_data->wsCore->addAction(QIcon(":/Resources/gear.png"), m_data->codec->toUnicode("Настройки"), Qt::ToolButtonIconOnly);
		if (action != Q_NULLPTR)
		{
			action->setObjectName("actionSettings");
			connect(action, SIGNAL(triggered()), SLOT(slot_showSettings()));
		}

	}

	return true;
}
// ------------------------------------------------------------
QString PluginWidgetSettings::getName() const
{
	return QString("pluginWidgetSettings");
}
// ------------------------------------------------------------
QStringList PluginWidgetSettings::references() const
{
	return QStringList();
}
// ------------------------------------------------------------
QIcon PluginWidgetSettings::getIcon() const
{
	return QIcon();
}
// ------------------------------------------------------------
QString PluginWidgetSettings::about() const
{
	return QString(tr("Plugin widget settings"));
}
// ----------------------------------------------------------------
void PluginWidgetSettings::slot_showSettings()
{
	if (m_data != Q_NULLPTR)
	{
		if (m_data->wsCore != Q_NULLPTR)
		{			
			WGTSettings* wgtSetting = Q_NULLPTR;
			wgtSetting = new WGTSettings(m_data->wsCore);
			if (wgtSetting != Q_NULLPTR)
			{
				if (wgtSetting->exec() == QDialog::Accepted)
				{

				}
				delete wgtSetting;
				wgtSetting = Q_NULLPTR;
			}			
		}
	}
}
// ------------------------------------------------------------
