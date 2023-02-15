#pragma once

#include <QObject>

class i_wsCore;
class WGTPage : public QObject
{
	Q_OBJECT
public:
	WGTPage(i_wsCore* wsCore, QObject* parent = 0);
	~WGTPage();
	
private:

	struct SDATA;
	SDATA* m_data;

public slots:

	/*!
		Изменить активность панели
	*/
	void setEnabled(bool);

private slots:

	/*!
		Запустить одиночный поиск объектов
	*/
	void slt_searchSingle();
};