#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QComboBox>
#include <QTextCodec>

#include "wgtFeedback.h"

struct WGTFeedback::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QTextEdit* line = Q_NULLPTR;
	QComboBox* comboBox = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTFeedback::WGTFeedback(QDialog* parent)
	: QDialog(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	this->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	this->setWindowModality(Qt::ApplicationModal);
	this->setWindowTitle(m_data->codec->toUnicode("Отзыв"));
	this->setWindowIcon(QIcon(":/Resources/icon32.png"));
	
	QVBoxLayout* layout = Q_NULLPTR;
	layout = new QVBoxLayout;

	// Тема письма
	layout->addWidget(new QLabel(m_data->codec->toUnicode("Выберите тему:")));
	
	QStringList listThemes;
	listThemes.append(m_data->codec->toUnicode("Сообщение"));
	listThemes.append(m_data->codec->toUnicode("Предлагаю переделать"));
	listThemes.append(m_data->codec->toUnicode("Хочу"));
	listThemes.append(m_data->codec->toUnicode("Найдена ошибка"));
	m_data->comboBox = new QComboBox();
	m_data->comboBox->addItems(listThemes);
	layout->addWidget(m_data->comboBox);

	// Текст письма
	layout->addWidget(new QLabel(m_data->codec->toUnicode("Отзыв или предложение:")));
	m_data->line = new QTextEdit();
	layout->addWidget(m_data->line);

	QPushButton* buttonOk = Q_NULLPTR;
	buttonOk = new QPushButton(m_data->codec->toUnicode("Отправить"));
	connect(buttonOk, SIGNAL(released()), this, SLOT(accept()));

	QPushButton* buttonCancel = Q_NULLPTR; 
	buttonCancel = new QPushButton(m_data->codec->toUnicode("Отмена"), this);
	connect(buttonCancel, SIGNAL(released()), this, SLOT(reject()));

	QHBoxLayout* layoutButton = new QHBoxLayout;
	layoutButton->setAlignment(Qt::AlignRight);
	layoutButton->addWidget(buttonOk);	
	layoutButton->addWidget(buttonCancel);
	layout->addLayout(layoutButton);

	this->setLayout(layout);
}
// ----------------------------------------------------------------
WGTFeedback::~WGTFeedback()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
QString WGTFeedback::getText()
{
	if (m_data != Q_NULLPTR)
		if (m_data->line != Q_NULLPTR)
			return m_data->line->toPlainText();
	return QString();
}
// ----------------------------------------------------------------
QString WGTFeedback::getTheme()
{
	if (m_data != Q_NULLPTR)
		if (m_data->comboBox != Q_NULLPTR)
			return m_data->comboBox->currentText();
	return QString(m_data->codec->toUnicode("Сообщение"));
}
// ----------------------------------------------------------------