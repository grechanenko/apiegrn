#include <QMessageBox>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QThread>
#include <QProgressBar>
#include <QTextCodec>

#include "i_wsSettingsCore.h"
#include "wsAuthDialog.h"
#include "wsAuthServer.h"

struct WSAuthDialog::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QLineEdit* lineLogin = Q_NULLPTR;
	QLineEdit* linePass = Q_NULLPTR;
	QCheckBox* checkRemember = Q_NULLPTR;	
	QProgressBar* progressBar = Q_NULLPTR;
	QPushButton* buttonOk = Q_NULLPTR;

	i_wsSettingsCore* settings = Q_NULLPTR;
};
// ----------------------------------------------------------------
WSAuthDialog::WSAuthDialog(i_wsSettingsCore* settings, QDialog* parent)
	:QDialog(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	this->setWindowTitle(m_data->codec->toUnicode("Вход"));
	this->setWindowFlags(Qt::WindowSystemMenuHint);

	if (settings != Q_NULLPTR)
	{
		m_data->settings = settings;				
	}

	QVBoxLayout* layout = new QVBoxLayout;

	layout->addWidget(new QLabel(m_data->codec->toUnicode("Логин:")));
	m_data->lineLogin = new QLineEdit(QString(""));
	connect(m_data->lineLogin, SIGNAL(textChanged(const QString &)), SLOT(slt_enableOkButton(const QString &)));
	m_data->lineLogin->setToolTip(m_data->codec->toUnicode("Укажите логин"));
	layout->addWidget(m_data->lineLogin);


	layout->addWidget(new QLabel(m_data->codec->toUnicode("Пароль:")));
	m_data->linePass = new QLineEdit(QString(""));
	connect(m_data->linePass, SIGNAL(textChanged(const QString &)), SLOT(slt_enableOkButton(const QString &)));
	m_data->linePass->setToolTip(m_data->codec->toUnicode("Укажите пароль"));
	m_data->linePass->setEchoMode(QLineEdit::Password);
	layout->addWidget(m_data->linePass);


	QHBoxLayout* layoutCheck = new QHBoxLayout;
	m_data->checkRemember = new QCheckBox();	
	m_data->checkRemember->setCheckable(true);	
	m_data->checkRemember->setChecked(m_data->settings->getSetting("auth/remember").toBool());

	layoutCheck->addWidget(m_data->checkRemember);
	layoutCheck->addWidget(new QLabel(m_data->codec->toUnicode("Запомнить")));
	layoutCheck->setAlignment(Qt::AlignLeft);

	layout->addLayout(layoutCheck);

	QHBoxLayout* layoutButton = new QHBoxLayout;

	m_data->buttonOk = new QPushButton(m_data->codec->toUnicode("Войти"));
	connect(m_data->buttonOk, SIGNAL(released()), this, SLOT(slt_okButton()));
	m_data->buttonOk->setEnabled(false);

	QPushButton* buttonCancel = new QPushButton(m_data->codec->toUnicode("Отмена"));
	connect(buttonCancel, SIGNAL(released()), this, SLOT(reject()));

	layoutButton->addWidget(m_data->buttonOk);
	layoutButton->addWidget(buttonCancel);
	layoutButton->setAlignment(Qt::AlignRight);

	layout->addLayout(layoutButton);

	QHBoxLayout *layoutProgress = new QHBoxLayout();
	m_data->progressBar = new QProgressBar();
	m_data->progressBar->setRange(0, 0);
	m_data->progressBar->setVisible(false);
	layoutProgress->addWidget(new QLabel(m_data->codec->toUnicode("Подключение к серверу...")));
	layoutProgress->setContentsMargins(10, 0, 0, 0);
	m_data->progressBar->setLayout(layoutProgress);

	layout->addWidget(m_data->progressBar);

	this->setLayout(layout);
}
// ----------------------------------------------------------------
WSAuthDialog::~WSAuthDialog()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
bool WSAuthDialog::isAuth(i_wsSettingsCore* settings)
{
	if (settings == Q_NULLPTR)
		return false;

	bool flagRemember = settings->getSetting("auth/remember").toBool();
	if (flagRemember == true)
	{
		QString cookie1 = settings->getSetting(QString("auth/sessionid")).toString();
		QString cookie2 = settings->getSetting(QString("auth/userid")).toString();
		QString cookie3 = settings->getSetting(QString("auth/autologin")).toString();

		if ((!cookie1.isEmpty()) && (!cookie2.isEmpty()) && (!cookie3.isEmpty()))
			return true;
	}
	return false;	
}
// ----------------------------------------------------------------
void WSAuthDialog::slt_okButton()
{
	if (m_data != Q_NULLPTR)
		if ((m_data->lineLogin != Q_NULLPTR) && (m_data->linePass != Q_NULLPTR))
		{
			WSAuthServer* authServer = Q_NULLPTR;
			authServer = new WSAuthServer(m_data->lineLogin->text(), m_data->linePass->text(), m_data->settings);
			QThread* thread = new QThread;
			authServer->moveToThread(thread);

			// Получить результат
			connect(authServer, SIGNAL(s_connectid(bool)), SLOT(slt_connectid(bool)));

			// Запустить процесс
			connect(thread, SIGNAL(started()), authServer, SLOT(start()));
			connect(thread, SIGNAL(started()), this, SLOT(slt_showProgressBar()));

			// Остановить поток
			connect(authServer, SIGNAL(finished()), thread, SLOT(quit()));
			connect(authServer, SIGNAL(finished()), this, SLOT(slt_hideProgressBar()));

			// Удалить класс
			connect(authServer, SIGNAL(finished()), authServer, SLOT(deleteLater()));

			// Удалить поток 
			connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));

			// Запустить поток
			thread->start();
		}
}
// ----------------------------------------------------------------
void WSAuthDialog::slt_enableOkButton(const QString &)
{
	if ((m_data != Q_NULLPTR) && (m_data->lineLogin != Q_NULLPTR) && (m_data->linePass != Q_NULLPTR) && (m_data->buttonOk != Q_NULLPTR))
	{
		m_data->buttonOk->setEnabled(false);
		if ((m_data->lineLogin->text().isEmpty() == false) && (m_data->linePass->text().isEmpty() == false))
			m_data->buttonOk->setEnabled(true);
	}
}
// ----------------------------------------------------------------
void WSAuthDialog::slt_showProgressBar()
{
	if (m_data != Q_NULLPTR)
		if (m_data->progressBar != Q_NULLPTR)
		{
			m_data->progressBar->setVisible(true);
			m_data->progressBar->setAlignment(Qt::AlignCenter);


			if (m_data->buttonOk != Q_NULLPTR)
				m_data->buttonOk->setEnabled(false);
		}
}
// ----------------------------------------------------------------
void WSAuthDialog::slt_hideProgressBar()
{
	if (m_data != Q_NULLPTR)
		if (m_data->progressBar != Q_NULLPTR)
		{
			m_data->progressBar->setVisible(false);
			if (m_data->buttonOk != Q_NULLPTR)
				m_data->buttonOk->setEnabled(true);
		}
}
// ----------------------------------------------------------------
void WSAuthDialog::slt_connectid(bool flag)
{
	if (flag)
	{
		if (m_data != Q_NULLPTR)
			if (m_data->settings != Q_NULLPTR)
				if (m_data->checkRemember != Q_NULLPTR)
				{
					bool fl = m_data->checkRemember->isChecked();
					m_data->settings->setSetting("auth/remember", (bool)fl);
					if (m_data->lineLogin != Q_NULLPTR)
						m_data->settings->setSetting("auth/login", (QVariant)m_data->lineLogin->text());
					if (m_data->linePass != Q_NULLPTR)
						m_data->settings->setSetting("auth/pass", (QVariant)m_data->linePass->text());
				}
		this->accept();
	}
	else
	{
		if (m_data != Q_NULLPTR)
			if (m_data->codec != Q_NULLPTR)
				QMessageBox::warning(this, m_data->codec->toUnicode("Вход"), m_data->codec->toUnicode("Неизвестный логин или пароль"));
	}
}
// ----------------------------------------------------------------
