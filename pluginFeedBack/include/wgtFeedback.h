#pragma once

#include <QDialog>

class WGTFeedback : public QDialog
{
	Q_OBJECT
public:
	WGTFeedback(QDialog* parent = 0);
	~WGTFeedback();

	/*!
		���������� ����� ������
	*/
	QString getText();

	/*!
		���������� ���� ������
	*/
	QString getTheme();

private:

	struct SDATA;
	SDATA* m_data;
};
