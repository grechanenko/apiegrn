#include <QProcess>
#include <QtWidgets/QApplication>
#include <QMainWindow>
#include <QSplashScreen>
#include <QPixmap>
#include <QThread>
#include <QTextStream>
#include <QFile>
#include <QDateTime>
#include <QDir>
#include <QDebug>

#include "wsSettingsCore.h"
#include "wsCore.h"

// Умный указатель на файл логирования
QScopedPointer<QFile>   infoLogFile;
QScopedPointer<QFile>   debugLogFile;
QScopedPointer<QFile>   warningLogFile;
QScopedPointer<QFile>   criticalLogFile;
int maxSizeLogFile = 4000000;

// Объявляение обработчика сообщений для логфайла
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

// Проверить файл, если больше разрешенного объема, создать новый, старый удалить
bool checkLogFile(QString path)
{
	QByteArray array;
	if (QFile::exists(path))
	{
		QFile file(path);
		if (file.open(QFile::ReadWrite | QFile::Text))
		{
			array = file.readAll();
			qDebug() << array;
		}
	}
	return false;
}


bool initLogFiles()
{
	QString path = QDir::homePath() + QString("/AppData/Roaming/APIService/log/");
	if (QDir(path).exists() == false)
		QDir().mkdir(path);

	// Устанавливаем файл логирования,
	infoLogFile.reset(new QFile(path + QString("info2.log")));
	debugLogFile.reset(new QFile(path + QString("debug2.log")));
	warningLogFile.reset(new QFile(path + QString("warning2.log")));
	criticalLogFile.reset(new QFile(path + QString("critical2.log")));
	
	// Открываем файл логирования
	infoLogFile.data()->open(QFile::Append | QFile::Text);
	debugLogFile.data()->open(QFile::Append | QFile::Text);
	warningLogFile.data()->open(QFile::Append | QFile::Text);
	criticalLogFile.data()->open(QFile::Append | QFile::Text);

	// Устанавливаем обработчик
	qInstallMessageHandler(messageHandler);

	return true;
}

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	qApp->setOrganizationName("OOO Apiservice");
	qApp->setApplicationName("APIRosreestr");

	// Create splashcreen
	QPixmap splashScreenPixmap(":/Resources/kassa.png");
	QSplashScreen splashScreen(splashScreenPixmap);
	splashScreen.show();
	QApplication::applicationDirPath();

	// Инициализация лог файлов
	initLogFiles();
	

	// Create class to working with settings
	i_wsSettingsCore* settings= Q_NULLPTR;
	settings = new WSSettingsCore();
	if (settings != Q_NULLPTR)
	{	
		settings->createSetting("logfile/maxsize", (int)4000000, true);
		maxSizeLogFile = settings->getSetting("logfile/maxsize").toInt();
		
		WSCore* wsCore = Q_NULLPTR;
		wsCore = WSCore::create(settings);
		if (wsCore != Q_NULLPTR)
		{
			// Load plugins
			int countPlugin = wsCore->loadPlugins(wsCore, &splashScreen);

			// Show main window
			QMainWindow* mainWindow = Q_NULLPTR;
			mainWindow = wsCore->getMainWindow();
			if (mainWindow != Q_NULLPTR)
				mainWindow->showMaximized();
		}		
	}	
	
	splashScreen.close();

	int returncode = a.exec();
	if (returncode == -100)
	{
		QProcess* proc = new QProcess();
		proc->start(QCoreApplication::applicationFilePath());
	}
	return returncode;
}

// Реализация обработчика
void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	// Берём название класса и метод
	QRegExp rx("([\\w-]+::[\\w-]+)");
	QString function = QString();
	if (rx.indexIn(context.function) != -1)
	{
		function = rx.cap(1);
		function = "(" + function + ") ";
	}
	
	switch (type)
	{
		case QtInfoMsg:     
		{
			QString path1 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/info1.log");
			QString path2 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/info2.log");
			QFileInfo fInfo(path2);
			if (fInfo.size() > maxSizeLogFile)
			{
				if (QFileInfo::exists(path1))				
					QFile::remove(path1);

				infoLogFile.data()->close();
				if (QFileInfo::exists(path2))
					QFile::rename(path2, path1);

				infoLogFile.reset(new QFile(path2));			
			}

			// Открываем поток записи в файл
			QTextStream out(infoLogFile.data());
			// Записываем дату записи
			out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: ");
			// Пишем сообщение
			out << msg << endl;
			// Очищаем буферизированные данные
			out.flush();    
			break;
		}
		case QtDebugMsg:    
		{	
			QString path1 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/debug1.log");
			QString path2 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/debug2.log");
			QFileInfo fInfo(path2);
			if (fInfo.size() > maxSizeLogFile)
			{
				if (QFileInfo::exists(path1))
					QFile::remove(path1);

				infoLogFile.data()->close();
				if (QFileInfo::exists(path2))
					QFile::rename(path2, path1);

				infoLogFile.reset(new QFile(path2));
			}

			qint64 sizeFile = debugLogFile.data()->size();
			// Открываем поток записи в файл
			QTextStream out(debugLogFile.data());
			// Записываем дату записи
			out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: ");
			// Пишем сообщение
			out << function << msg << endl;
			// Очищаем буферизированные данные
			out.flush();
			break;
		}
		case QtWarningMsg: 
		{
			QString path1 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/warning1.log");
			QString path2 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/warning2.log");
			QFileInfo fInfo(path2);
			if (fInfo.size() > maxSizeLogFile)
			{
				if (QFileInfo::exists(path1))
					QFile::remove(path1);

				infoLogFile.data()->close();
				if (QFileInfo::exists(path2))
					QFile::rename(path2, path1);

				infoLogFile.reset(new QFile(path2));
			}

			// Открываем поток записи в файл
			QTextStream out(warningLogFile.data());
			// Записываем дату записи
			out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: ");
			// Пишем сообщение
			out << function << msg << endl;
			// Очищаем буферизированные данные
			out.flush();
			break;
		}
		case QtCriticalMsg: 
		{
			QString path1 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/critical1.log");
			QString path2 = QDir::homePath() + QString("/AppData/Roaming/APIService/log/critical2.log");
			QFileInfo fInfo(path2);
			if (fInfo.size() > maxSizeLogFile)
			{
				if (QFileInfo::exists(path1))
					QFile::remove(path1);

				infoLogFile.data()->close();
				if (QFileInfo::exists(path2))
					QFile::rename(path2, path1);

				infoLogFile.reset(new QFile(path2));
			}

			// Открываем поток записи в файл
			QTextStream out(criticalLogFile.data());
			// Записываем дату записи
			out << QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz: ");
			// Пишем сообщение
			out << function << msg << endl;
			// Очищаем буферизированные данные
			out.flush();
			break;
		}		
	}	
}