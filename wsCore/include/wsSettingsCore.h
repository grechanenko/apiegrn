#pragma once

#include <QObject>
#include <QString>
#include <QVariant>

#include "i_wsSettingsCore.h"
#include "i_wsDescriptionSetting.h"
#include "wscore_global.h"

class WSCORE_EXPORT WSDescriptionSetting : public QObject, public virtual i_wsDescriptionSetting
{
	Q_OBJECT
	Q_INTERFACES(i_wsDescriptionSetting)
	DECLARE_SMARTPOINTER

public:
	WSDescriptionSetting(const QString& key, const QVariant& value, const bool visible = true);
	~WSDescriptionSetting();


	/*!
		Return description
	*/
	virtual QString getDescription() const;


	/*!
		Get data type 
	*/
	virtual QVariant::Type getType();


	/*!
		Get key of data
	*/
	virtual QString getKey() const;


	/*!
		Get default value
	*/
	virtual QVariant getDefaultValue();


	/*!
		Get value
	*/
	virtual QVariant getValue();


	/*!
		Set value
	*/
	virtual void setValue(const QVariant&);


	/*!
		Set visibility for user
	*/
	virtual void setVisible(const bool flag);


	/*!
		true - if visible for user
	*/
	virtual bool isVisible() const;


	/*!
		Return widget setting
	*/
	virtual QWidget* getWidget(QWidget* parent);

private:

	struct SDATA;
	SDATA* m_data;
};


class WSCORE_EXPORT WSSettingsCore : public QObject, public virtual i_wsSettingsCore, public virtual i_base
{
	Q_OBJECT
	Q_INTERFACES(i_wsSettingsCore i_base)
	DECLARE_SMARTPOINTER

public:
	WSSettingsCore();
	~WSSettingsCore();

	/*!
		Create setting
	*/
	virtual void createSetting(const QString& key, const QVariant& value, const bool visible = true);


	/*!
		Create setting
	*/
	virtual void createSetting(i_wsDescriptionSetting* wsDS);


	/*!
		Return settings list
	*/
	virtual QList<i_wsDescriptionSetting*> getSettingsList() const;


	/*!
		Return setting
	*/
	virtual QVariant getSetting(const QString& key) const;


	/*!
		Set setting
	*/
	virtual void setSetting(const QString& key, const QVariant& value);


	/*!
		Add translator
	*/
	virtual void addTranslator(const QString& file);

private:

	struct SDATA;
	SDATA* m_data;
};