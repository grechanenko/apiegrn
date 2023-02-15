#pragma once

#include <QDialog>

class WGTFeedback : public QDialog
{
	Q_OBJECT
public:
	WGTFeedback(QDialog* parent = 0);
	~WGTFeedback();

	/*!
		Возвращает текст отзыва
	*/
	QString getText();

	/*!
		Возвращает тему письма
	*/
	QString getTheme();

private:

	struct SDATA;
	SDATA* m_data;
};
