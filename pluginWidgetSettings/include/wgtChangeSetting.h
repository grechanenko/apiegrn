#pragma once

#include <QDialog>

class QTreeWidgetItem;
class WGTChangeSetting : public QDialog
{
	Q_OBJECT

public:

	WGTChangeSetting(QTreeWidgetItem* item, QWidget* parent = 0);
	~WGTChangeSetting();

	/*!
		���������� �������� �� lineEdit
		��� ���������� ��������� ��� ������� Ok
	*/
	QString getValue();

private:

	struct SDATA;
	SDATA* m_data;

};