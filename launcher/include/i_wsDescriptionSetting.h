#pragma once


#include <QVariant>
#include "i_base.h"

class i_wsDescriptionSetting : public virtual i_base
{
public:
	virtual ~i_wsDescriptionSetting() {}


	/*!
		�������� ���������
	*/
	virtual QString getDescription() const = 0;


	/*!
		��� ������
	*/
	virtual QVariant::Type getType() = 0;


	/*!
		���������� ����
	*/
	virtual QString getKey() const = 0;


	/*!
		�������� �� ���������
	*/
	virtual QVariant getDefaultValue() = 0;


	/*!
		������� ��������
	*/
	virtual QVariant getValue() = 0;


	/*!
		���������� ����� ��������
	*/
	virtual void setValue(const QVariant&) = 0;


	/*!
		���������� ��������� ��� ������������
	*/
	virtual void setVisible(const bool flag) = 0;


	/*!
		true - ���� ������ �� ������������
	*/
	virtual bool isVisible() const = 0;


	/*!
		������ ��� ��������������
	*/
	virtual QWidget* getWidget(QWidget* parent) = 0;
};

Q_DECLARE_INTERFACE(i_wsDescriptionSetting, "launcher.i_wsDescriptionSetting/1.0")