#include <QMessageBox>
#include <QDebug>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QThread>
#include <QProgressBar>
#include <QTextCodec>

#include "i_wsAPIRosreestr.h"
#include "i_wsCore.h"
#include "wgtLogin.h"

// ----------------------------------------------------------------
struct WGTLogin::SDATA
{
	QTextCodec* codec = Q_NULLPTR;
	QLineEdit* lineLogin = Q_NULLPTR;
	QLineEdit* linePass = Q_NULLPTR;
	QCheckBox* checkRemember = Q_NULLPTR;
	QProgressBar* progressBar = Q_NULLPTR;
	QPushButton* buttonOk = Q_NULLPTR;

	i_wsCore* wsCore = Q_NULLPTR;
};
// ----------------------------------------------------------------
WGTLogin::WGTLogin(i_wsCore* wsCore, QDialog* parent)
	:QDialog(parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);

	this->setWindowTitle(m_data->codec->toUnicode("Вход"));
	this->setWindowFlags(Qt::WindowSystemMenuHint);

	if (wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}
	m_data->wsCore = wsCore;

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
	m_data->checkRemember->setChecked(m_data->wsCore->getSetting("auth/remember").toBool());

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
WGTLogin::~WGTLogin()
{
	if (m_data != Q_NULLPTR)
	{
		m_data->codec = Q_NULLPTR;
		m_data->wsCore = Q_NULLPTR;

		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
void WGTLogin::slt_okButton()
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (m_data->wsCore == Q_NULLPTR)
	{
		qDebug() << "wsCore is NULL";
		return;
	}

	if ((m_data->lineLogin == Q_NULLPTR) && (m_data->linePass == Q_NULLPTR))
	{
		qDebug() << "Line edit is NULL";
		return;
	}

	QObject* objAPI = Q_NULLPTR;
	objAPI = m_data->wsCore->getAPIRosreestr();
	if (objAPI == Q_NULLPTR)
	{
		qDebug() << "Object APIRosreestr is NULL";
		return;
	}
	connect(objAPI, SIGNAL(connected(bool)), SLOT(slt_connected(bool)));

	i_wsAPIRosreestr* api = Q_NULLPTR;
	api = qobject_cast<i_wsAPIRosreestr*>(objAPI);
	if (api == Q_NULLPTR)
	{
		qDebug() << "Can not convert Object* to i_wsAPIRosreestr*";
		return;
	}

	this->slt_showProgressBar();
	api->connecting(m_data->lineLogin->text(), m_data->linePass->text());
}
// ----------------------------------------------------------------
void WGTLogin::slt_enableOkButton(const QString &)
{
	if ((m_data != Q_NULLPTR) && (m_data->lineLogin != Q_NULLPTR) && (m_data->linePass != Q_NULLPTR) && (m_data->buttonOk != Q_NULLPTR))
	{
		m_data->buttonOk->setEnabled(false);
		if ((m_data->lineLogin->text().isEmpty() == false) && (m_data->linePass->text().isEmpty() == false))
			m_data->buttonOk->setEnabled(true);
	}
}
// ----------------------------------------------------------------
void WGTLogin::slt_showProgressBar()
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
void WGTLogin::slt_hideProgressBar()
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
void WGTLogin::slt_connected(bool flag)
{
	if (m_data == Q_NULLPTR)
	{
		qDebug() << "m_data is NULL";
		return;
	}

	if (flag == true)
	{		
		if (m_data->checkRemember != Q_NULLPTR)
		{
			// Если стоит галочка запомнить
			if (m_data->checkRemember->isChecked() == true)
			{
				m_data->wsCore->setSetting("auth/remember", (bool)true);
				if (m_data->lineLogin != Q_NULLPTR)
					m_data->wsCore->setSetting("auth/login", (QVariant)m_data->lineLogin->text());
				if (m_data->linePass != Q_NULLPTR)
					m_data->wsCore->setSetting("auth/pass", (QVariant)m_data->linePass->text());
			}
		}
		
		this->accept();
	}
	else
	{	
		if (m_data->codec != Q_NULLPTR)
			QMessageBox::warning(this, m_data->codec->toUnicode("Вход"), m_data->codec->toUnicode("Неизвестный логин или пароль"));
		else
		{
			qDebug() << "Codec is NULL";
			QMessageBox::warning(this, "Login", "Login or password failed");
		}

		this->slt_hideProgressBar();
	}
}
// ----------------------------------------------------------------