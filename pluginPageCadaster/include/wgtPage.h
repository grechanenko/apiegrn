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
		�������� ���������� ������
	*/
	void setEnabled(bool);

private slots:

	/*!
		��������� ��������� ����� ��������
	*/
	void slt_searchSingle();
};