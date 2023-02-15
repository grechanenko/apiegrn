#pragma once

#include "i_base.h"

enum DOC_TYPE { SEARCH, FULLINFO, DOC_ORDER, ERR };

struct S_ERROR
{
	int code = 0;
	QString message;
};

// ==== DOC_TYPE SEARCH =====

enum MODE_TYPE { lite, normal };

struct S_OBJECT
{
	QString cadnomer;
	QString address;
	QString type;
	QString area;
	QString category;
};

// ==== DOC_TYPE FULLINFO =====

struct S_DETAILS
{
	QString type_obj;			// Тип объекта
	QString cadnum;				// Кадастровый номер
	QString status_obj;			// Статус объекта
	QString datePosNaCadastr;	// Дата постановки на кадастровый учет
	QString catZem;				// Категория земель
	QString razreshIspolz;		// "Разрешенное использование" : "Для индивидуальной жилой застройки"
	float area;					// "Площадь" : "1427"
	QString edIzmer;			// "Единица измерения (код)" : "Квадратный метр"
	double cadPrice;			// "Кадастровая стоимость" : "1167214,65"
	QString dateOpredPrice;		// "Дата определения стоимости" : "19.02.2016"
	QString dateVnesPrice;		// "Дата внесения стоимости" : "10.12.2016"
	QString dateUtverPrice;		// "Дата утверждения стоимости" : "10.12.2016"
	QString adres;				// "Адрес (местоположение)" : "îáë. Êóðñêàÿ, ã. Êóðñê, óë. Èëüè÷à, äîì 48"
	QString dateObnovInfo;		// "Дата обновления информации" : "10.12.2016"
	QString formaSobstv;		// "Форма собственности" : "×àñòíàÿ"
	int colPravoobl;			// "Количество правообладателей" : "2"
	QString cadInjener;			// "Кадастровый инженер" : "Краснобородько Кирилл Александрович (аттестат 46-14-207)"
};

struct S_EGRN
{
	S_OBJECT obj;
	S_DETAILS details;
};

struct S_DOCUMENT
{
	QString label;	// Название документа
	bool available;	// Доступность для заказа
	float price;	// Стоимость в рублях
};

struct S_DOCUMENTS
{
	S_DOCUMENT xzp;
	S_DOCUMENT soop;
	S_DOCUMENT kpt;
};

// ==== DOC_TYPE DOC_ORDER =====

struct S_DOCUMENTS_ID
{
	QString type;
	int id;
};


class i_wsJsonParser : public i_base
{

public:

	virtual ~i_wsJsonParser() {}

	
	/*!
		Возвращает тип документа
	*/
	virtual DOC_TYPE docType() const = 0;

	/*!
		Информация о документах
		DOC_TYPE = FULLINFO
	*/
	virtual S_DOCUMENTS documents() const = 0;

	/*!
		Возвращает id документов
		DOC_TYPE = DOC_ORDER
	*/
	virtual QVector<S_DOCUMENTS_ID> documentsID() const = 0;

	/*!
		Ключ
	*/
	virtual QString encoded_object() const = 0;

	/*!
		Возвращает ошибку
	*/
	virtual S_ERROR error() const = 0;

	/*!
		Количество найденных объектов
		DOC_TYPE = SEARCH
	*/
	virtual int found() const = 0;

	/*!
		Информация об объекте
		DOC_TYPE = FULLINFO
	*/
	virtual S_EGRN getEGRN() const = 0;

	/*!
		true - если найденные объекты сгруппированы
		DOC_TYPE = SEARCH
	*/
	virtual bool isGrouped() const = 0;

	/*!
		Возвращает последнюю ошибку возникшую при чтении JSON
	*/
	virtual QString lastParseError() const = 0;

	/*!
		Тип набора данных
		DOC_TYPE = SEARCH
	*/
	virtual MODE_TYPE mode() const = 0;

	/*!
		Объекты
		DOC_TYPE = SEARCH
	*/
	virtual QVector<S_OBJECT> objects() const = 0;

	/*!
		Возвращает paid
		DOC_TYPE = DOC_ORDER
	*/
	virtual bool paid() const = 0;

	/*!
		Возвращает название региона
		DOC_TYPE = FULLINFO
	*/
	virtual QString region() const = 0;

	/*!
		Возвращает transaction_id
		DOC_TYPE = DOC_ORDER
	*/
	virtual int transactionID() const = 0;

	/*!
		Установить кадастровый номер
	*/
	virtual void setCadNumber(QString) = 0;

	/*!
		Получить кадастровый номер
	*/
	virtual QString getCadNumber() const = 0;

	virtual void setPriceXZP(float) = 0;
	virtual float getPriceXZP() const = 0;

	virtual void setPriceSOPP(float) = 0;
	virtual float getPriceSOPP() const = 0;

	virtual void setPriceKPT(float) = 0;
	virtual float getPriceKPT() const = 0;
};
Q_DECLARE_INTERFACE(i_wsJsonParser, "launcher.i_wsJsonParser/1.0")