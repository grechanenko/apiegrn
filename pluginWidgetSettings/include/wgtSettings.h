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
		Заполнить дерево свойств
	*/
	void readSettings();

	/*!
		Парсит ключ
	*/
	QStringList parseKey(QString key);

	/*!
		Добавить настройку
	*/
	void addSetting(QString key, QVariant value);

	/*!
		Возвращает существующий item по названию, если его нет, то создает
	*/
	QTreeWidgetItem* getItem(QString name, QTreeWidgetItem* parent);

private:

	struct SDATA;
	SDATA* m_data;

private slots:

	/*!
		Применить
	*/
	void slot_apply();

	/*!
		Вызывается при изменении активной настройки
	*/
	void slot_itemActivated(QTreeWidgetItem *item, int column);
};