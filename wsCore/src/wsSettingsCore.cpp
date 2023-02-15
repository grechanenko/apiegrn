#include <QDebug>
#include <QMutex>
#include <QSettings>
#include <QTranslator>
#include <QCoreApplication>
#include <QDir>
#include <QList>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QLabel>

#include "wsSettingsCore.h"
#include "i_wsDescriptionSetting.h"

IMPLEMENT_SMARTPOINTER(WSDescriptionSetting)
IMPLEMENT_SMARTPOINTER(WSSettingsCore)
struct WSDescriptionSetting::SDATA
{
	QString key = QString();
	QVariant value = QVariant();
	QVariant defaultValue = QVariant();
	QVariant::Type typeData = QVariant::Type::Invalid;
	bool visible = true;
	QWidget* widget = Q_NULLPTR;
};
// ----------------------------------------------------------------
WSDescriptionSetting::WSDescriptionSetting(const QString& key, const QVariant& value, const bool visible)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;

	if (key.isEmpty() == true)
		return;
	m_data->key = key;

	if (value.isValid() == false)
		return;
	m_data->defaultValue = value;
	m_data->value = value;

	QSettings settings;
	if (settings.contains(key) == true)
		m_data->value = settings.value(key);

	m_data->visible = visible;
	m_data->typeData = value.type();
}
// ----------------------------------------------------------------
WSDescriptionSetting::~WSDescriptionSetting()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->key = QString();
		m_data->value.clear();

		if (m_data->widget != Q_NULLPTR)
		{
			delete m_data->widget;
			m_data->widget = Q_NULLPTR;
		}

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
QString WSDescriptionSetting::getDescription() const
{
	return QString(tr(""));
}
// ----------------------------------------------------------------
QVariant::Type WSDescriptionSetting::getType()
{
	if (m_data != Q_NULLPTR)
		return m_data->typeData;
	return QVariant::Type::Invalid;
}
// ----------------------------------------------------------------
QString WSDescriptionSetting::getKey() const
{
	if (m_data != Q_NULLPTR)
		return m_data->key;
	return QString();
}
// ----------------------------------------------------------------
QVariant WSDescriptionSetting::getDefaultValue()
{
	if (m_data != Q_NULLPTR)
		return m_data->defaultValue;
	return QVariant();
}
// ----------------------------------------------------------------
QVariant WSDescriptionSetting::getValue()
{
	if (m_data != Q_NULLPTR)
	{
		QVariant v = m_data->value;
		return (QVariant)v;
	}
	return QVariant();
}
// ----------------------------------------------------------------
void WSDescriptionSetting::setValue(const QVariant& value)
{
	if (m_data != Q_NULLPTR)
		if (m_data->key.isEmpty() != true)
			if (value.isValid() == true)
			{
				QSettings settings;
				settings.setValue(m_data->key, value);
				m_data->value = value;
			}
}
// ----------------------------------------------------------------
void WSDescriptionSetting::setVisible(const bool flag)
{
	if (m_data != Q_NULLPTR)
		m_data->visible = flag;
}
// ----------------------------------------------------------------
bool WSDescriptionSetting::isVisible() const
{
	if (m_data != Q_NULLPTR)
		return m_data->visible;
	return true;
}
// ----------------------------------------------------------------
QWidget* WSDescriptionSetting::getWidget(QWidget* parent)
{
	if (m_data == Q_NULLPTR)
		return Q_NULLPTR;

	QVBoxLayout* layout = new QVBoxLayout;
	layout->setAlignment(Qt::AlignTop);

	layout->addWidget(new QLabel(tr("Description:")));

	QLabel* label = new QLabel();
	label->setText(this->getDescription());
	layout->addWidget(label);

	QLineEdit* lEdit = new QLineEdit();
	lEdit->setText(m_data->value.toString());
	layout->addWidget(lEdit);

	QWidget* widget = new QWidget(parent);
	widget->setLayout(layout);

	return widget;
}
// ----------------------------------------------------------------
struct WSSettingsCore::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QMutex mutex;						//  Äëÿ áåçîïàñíîãî ìíîãîïîòî÷íîãî äîñòóïà ê íàñòðîéêàì
	QSettings* m_settings = Q_NULLPTR;  // Äëÿ ôîðìèðîâàíèÿ íàñòðîåê	
	QList<QTranslator*> m_translators;	// Ñïèñîê çàãðóæåííûõ ôàéëîâ ïåðåâîäà		
	QList<WSPtr<i_wsDescriptionSetting>> listDS;
};
// ----------------------------------------------------------------
WSSettingsCore::WSSettingsCore()
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;
}
// ----------------------------------------------------------------
WSSettingsCore::~WSSettingsCore()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void WSSettingsCore::createSetting(const QString& key, const QVariant& value, const bool visible)
{
	if (m_data == Q_NULLPTR)
		return;

	if (key.isEmpty() == true)
		return;

	if (value.isValid() == false)
		return;

	m_data->mutex.lock();

	WSPtr<i_wsDescriptionSetting> wsDS = Q_NULLPTR;
	wsDS = new WSDescriptionSetting(key, value, visible);
	if (wsDS != Q_NULLPTR)
		m_data->listDS.push_back(wsDS);
	m_data->mutex.unlock();
}
// ----------------------------------------------------------------
void WSSettingsCore::createSetting(i_wsDescriptionSetting* wsDS)
{
	if (wsDS != Q_NULLPTR)
		if (m_data != Q_NULLPTR)
		{
			m_data->mutex.lock();
			m_data->listDS.push_back(wsDS);
			m_data->mutex.unlock();
		}
}
// ----------------------------------------------------------------
QList<i_wsDescriptionSetting*> WSSettingsCore::getSettingsList() const
{
	QList<i_wsDescriptionSetting*> list;
	if (m_data != Q_NULLPTR)
	{
		if (m_data->listDS.size() > 0)
		{
			for each (WSPtr<i_wsDescriptionSetting> wsDS in m_data->listDS)
			{
				list.push_back(wsDS.data());
			}
		}
	}
	return list;
}
// ----------------------------------------------------------------
QVariant WSSettingsCore::getSetting(const QString& key) const
{
	if (m_data == Q_NULLPTR)
		return QVariant();

	if (key.isEmpty() == true)
		return QVariant();

	QVariant value;
	m_data->mutex.lock();
	if (m_data->listDS.size() > 0)
	{
		for each (WSPtr<i_wsDescriptionSetting> wsDS in m_data->listDS)
		{
			if (wsDS->getKey() == key)
				value = wsDS->getValue();
		}
	}
	m_data->mutex.unlock();
	return value;
}
// ----------------------------------------------------------------
void WSSettingsCore::setSetting(const QString& key, const QVariant& value)
{
	if (m_data == Q_NULLPTR)
		return;

	if (key.isEmpty() == true)
		return;

	if (value.isValid() == false)
		return;

	m_data->mutex.lock();
	if (m_data->listDS.size() > 0)
	{
		for each (WSPtr<i_wsDescriptionSetting> wsDS in m_data->listDS)
		{
			if (wsDS->getKey() == key)
				wsDS->setValue(value);
		}
	}
	m_data->mutex.unlock();
}
// ----------------------------------------------------------------
void WSSettingsCore::addTranslator(const QString& file)
{
	if (m_data == Q_NULLPTR)
		return;

	if (file.isEmpty() == true)
		return;

	QDir dir(qApp->applicationDirPath());
	if (!dir.cd("language"))
	{
		qDebug() << "WSSettingsCore::addTranslator(). Language directory does not exist";
		return;
	}

	QTranslator* translator = new QTranslator();
	if (translator->load(file, dir.absolutePath()) == false)
		qDebug() << "Can not load translator file " << file;

	qApp->installTranslator(translator);
}
// ----------------------------------------------------------------