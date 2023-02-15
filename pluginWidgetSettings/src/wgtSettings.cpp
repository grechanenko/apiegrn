#include <QDebug>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QStringList>
#include <QMessageBox>
#include <QTextCodec>

#include "i_wsCore.h"
#include "i_wsDescriptionSetting.h"
#include "wgtSettings.h"
#include "wgtChangeSetting.h"


struct WGTSettings::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	i_wsCore* wsCore = Q_NULLPTR;
	QTreeWidget* treeSetting = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTSettings::WGTSettings(i_wsCore* wsCore, QWidget* parent)
	: QDialog(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->setWindowTitle(m_data->codec->toUnicode("Настройки"));	

	if (wsCore == Q_NULLPTR)
		return;
	m_data->wsCore = wsCore;

	QVBoxLayout* layout = new QVBoxLayout;

	// ----- Central widgets

	m_data->treeSetting = new QTreeWidget();
	connect(m_data->treeSetting, SIGNAL(itemClicked(QTreeWidgetItem*, int)), SLOT(slot_itemActivated(QTreeWidgetItem*, int)));
	QStringList headerList;
	headerList << m_data->codec->toUnicode("Параметр") << m_data->codec->toUnicode("Значение");
	m_data->treeSetting->setHeaderLabels(headerList);
	layout->addWidget(m_data->treeSetting);

	this->readSettings();

	// ----- Buttons

	QPushButton* buttonOk = new QPushButton(m_data->codec->toUnicode("Сохранить"), this);
	connect(buttonOk, SIGNAL(released()), this, SLOT(accept()));

	//QPushButton* buttonApply = new QPushButton(tr("Apply"), this);
	//connect(buttonApply, SIGNAL(released()), this, SLOT(slot_apply()));

	QPushButton* buttonCancel = new QPushButton(m_data->codec->toUnicode("Отменить"), this);
	connect(buttonCancel, SIGNAL(released()), this, SLOT(reject()));

	QHBoxLayout* layoutButton = new QHBoxLayout;
	layoutButton->setAlignment(Qt::AlignRight);
	layoutButton->addWidget(buttonOk);
	//layoutButton->addWidget(buttonApply);
	layoutButton->addWidget(buttonCancel);

	// ----- set layout

	layout->addLayout(layoutButton);

	this->setLayout(layout);
}
// ----------------------------------------------------------------
WGTSettings::~WGTSettings()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void WGTSettings::readSettings()
{
	if (m_data == Q_NULLPTR)
		return;

	if (m_data->wsCore == Q_NULLPTR)
		return;

	if (m_data->treeSetting == Q_NULLPTR)
		return;

	QList<i_wsDescriptionSetting*> listDS = m_data->wsCore->getSettingsList();

	if (listDS.size() < 1)
		return;

	for each (i_wsDescriptionSetting* ds in listDS)
	{
		QString key = ds->getKey();
		if (key.isEmpty() == true)
			continue;

		if (ds->isVisible() == false)
			continue;

		this->addSetting(key, ds->getValue());		
	}
}
// ----------------------------------------------------------------
QStringList WGTSettings::parseKey(QString key)
{
	if (key.isEmpty() == true)
		return QStringList();

	QStringList list;
	QString str = QString("");
	for (int i = 0; i < key.size(); i++)
	{
		if (key[i] != '/')
			str += key[i];
		else
		{
			list.push_back(str);
			str = QString("");
		}

		if (i == key.size() - 1)
		{
			list.push_back(str);
			str = QString("");
		}
	}
	return list;
}
// ----------------------------------------------------------------
void WGTSettings::addSetting(QString key, QVariant value)
{
	if (m_data == Q_NULLPTR)
		return;

	if (m_data->treeSetting == Q_NULLPTR)
		return;

	if (key.isEmpty() == true)
		return;

	if (value.isValid() == false)
		return;

	QStringList settingList = this->parseKey(key);
	if (settingList.size() < 1)
		return;

	QTreeWidgetItem* item = Q_NULLPTR;
	for each (QString name in settingList)
	{
		if (name.isEmpty() == true)
			continue;

		item = this->getItem(name, item);
	}
	item->setText(1, value.toString());
	item->setData(1, Qt::UserRole, (QVariant)key);
}
// ----------------------------------------------------------------
QTreeWidgetItem* WGTSettings::getItem(QString name, QTreeWidgetItem* parent)
{
	if (parent == Q_NULLPTR)
	{
		QList<QTreeWidgetItem*> items = m_data->treeSetting->findItems(name, Qt::MatchFlag::MatchExactly);

		if (items.size() < 1)
		{
			QTreeWidgetItem* item = new QTreeWidgetItem(m_data->treeSetting);
			item->setText(0, name);
			return item;
		}
		else
		{
			return items[0];
		}
	}

	int childCount = parent->childCount();
	if (childCount < 1)
	{
		QTreeWidgetItem* item = new QTreeWidgetItem(parent);
		item->setText(0, name);
		return item;
	}
	else
	{
		for (int i = 0; i < childCount; i++)
		{
			QTreeWidgetItem* childItem = parent->child(i);
			if (childItem->text(0) == name)
				return childItem;
		}
	}

	QTreeWidgetItem* item = new QTreeWidgetItem(parent);
	item->setText(0, name);
	return item;
}
// ----------------------------------------------------------------
void WGTSettings::slot_apply()
{

}
// ----------------------------------------------------------------
void WGTSettings::slot_itemActivated(QTreeWidgetItem *item, int column)
{
	if (item == Q_NULLPTR)
		return;

	if (column != 1)
		return;

	if (item->text(1).isEmpty() == true)
		return;

	WGTChangeSetting* wgt = Q_NULLPTR;
	wgt = new WGTChangeSetting(item);
	if (wgt != Q_NULLPTR)
	{
		if (wgt->exec() == QDialog::Accepted)
		{
			QString value = wgt->getValue();

			// Меняем значение параметра в настройках
			QString keyItem = item->data(1, Qt::UserRole).toString();
			m_data->wsCore->setSetting(keyItem, (QVariant)value);

			// Меняем значение параметра в виджете			
			item->setText(1, value);
		}
		delete wgt;
		wgt = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------