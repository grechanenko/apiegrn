#include <QDebug>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>

#include "wgtChangeSetting.h"

struct WGTChangeSetting::SDATA
{
	QLineEdit* line = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTChangeSetting::WGTChangeSetting(QTreeWidgetItem* item, QWidget* parent)
	: QDialog(parent)
{
	if (item == Q_NULLPTR)
		return;

	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;

	this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->setWindowTitle(item->text(0));

	QVBoxLayout* layout = new QVBoxLayout;

	m_data->line = new QLineEdit(item->text(1));
	layout->addWidget(m_data->line);

	// ----- Buttons

	QPushButton* buttonOk = new QPushButton(tr("Ok"), this);
	connect(buttonOk, SIGNAL(released()), this, SLOT(accept()));

	QPushButton* buttonCancel = new QPushButton(tr("Cancel"), this);
	connect(buttonCancel, SIGNAL(released()), this, SLOT(reject()));

	QHBoxLayout* layoutButton = new QHBoxLayout;
	layoutButton->setAlignment(Qt::AlignRight);
	layoutButton->addWidget(buttonOk);
	layoutButton->addWidget(buttonCancel);

	layout->addLayout(layoutButton);

	this->setLayout(layout);
}
// ----------------------------------------------------------------
WGTChangeSetting::~WGTChangeSetting()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
QString WGTChangeSetting::getValue()
{
	if (m_data != Q_NULLPTR)
		if (m_data->line != Q_NULLPTR)
			return m_data->line->text();

	qDebug() << "WGTChangeSetting::getValue() m_data or line edit is NULL";
	return QString();
}
// ----------------------------------------------------------------