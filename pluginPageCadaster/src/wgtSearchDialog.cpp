#include <QDebug>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QTextCodec>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "wgtSearchDialog.h"

// ----------------------------------------------------------------
struct WGTSearchDislog::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QLineEdit* line = Q_NULLPTR;		
};
// ----------------------------------------------------------------
WGTSearchDislog::WGTSearchDislog(QWidget* parent)
	: QDialog(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;
	if (m_data == Q_NULLPTR)
		return;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	QVBoxLayout* layout = new QVBoxLayout;

	layout->addWidget(new QLabel(m_data->codec->toUnicode("Укажите адрес или кадастровый номер:")));

	// Line input data
	m_data->line = new QLineEdit();	
	m_data->line->setText(m_data->codec->toUnicode("46:29:101001:10"));	
	m_data->line->setMinimumWidth(400);

	QPushButton* buttonSend = new QPushButton(m_data->codec->toUnicode("Найти"));
	connect(buttonSend, SIGNAL(released()), SLOT(accept()));

	QPushButton* buttonCancel = new QPushButton(m_data->codec->toUnicode("Отмена"));
	connect(buttonSend, SIGNAL(released()), SLOT(reject));

	QHBoxLayout* inputLayout = new QHBoxLayout;
	inputLayout->addWidget(m_data->line);
	inputLayout->addWidget(buttonSend);
	layout->addLayout(inputLayout);	

	this->setLayout(layout);
}
// ----------------------------------------------------------------
WGTSearchDislog::~WGTSearchDislog()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
QString WGTSearchDislog::getText()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return QString();
	}

	if (m_data->line == Q_NULLPTR)
	{
		qDebug() << "Line edit is NULL";
		return QString();
	}
	return m_data->line->text();
}
// ----------------------------------------------------------------