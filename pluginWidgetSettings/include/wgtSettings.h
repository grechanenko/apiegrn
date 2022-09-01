#pragma once

#include <QDialog>

class i_wsCore;
class QTreeWidgetItem;
class WGTSettings : public QDialog
{
	Q_OBJECT

public:
	WGTSettings(i_wsCore* wsCore, QWidget* parent = 0);
	~WGTSettings();

private:

	/*!
		��������� ������ �������
	*/
	void readSettings();

	/*!
		������ ����
	*/
	QStringList parseKey(QString key);

	/*!
		�������� ���������
	*/
	void addSetting(QString key, QVariant value);

	/*!
		���������� ������������ item �� ��������, ���� ��� ���, �� �������
	*/
	QTreeWidgetItem* getItem(QString name, QTreeWidgetItem* parent);

private:

	struct SDATA;
	SDATA* m_data;

private slots:

	/*!
		���������
	*/
	void slot_apply();

	/*!
		���������� ��� ��������� �������� ���������
	*/
	void slot_itemActivated(QTreeWidgetItem *item, int column);
};