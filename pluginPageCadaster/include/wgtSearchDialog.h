#pragma once

#include <QDialog>

class WGTSearchDislog : public QDialog
{
	Q_OBJECT
public:

	WGTSearchDislog(QWidget* prent = 0);
	~WGTSearchDislog();

	/*!
		���������� ������
	*/
	QString getText();

private:

	struct SDATA;
	SDATA* m_data;
};