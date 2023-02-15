#pragma once

//enum DOC_TYPE { SEARCH, FULLINFO, DOC_ORDER, ERR };
//
//struct S_ERROR
//{
//	int code = 0;
//	QString message = QString();
//};
//
//// ==== DOC_TYPE SEARCH =====
//
//enum MODE_TYPE { lite, normal };
//
//struct S_OBJECT
//{
//	QString cadnomer = QString();
//	QString address = QString();
//	QString type = QString();
//	QString area = QString();
//	QString category = QString();
//};
//
//// ==== DOC_TYPE FULLINFO =====
//
//struct S_DETAILS
//{
//	QString type_obj = QString();			// Тип объекта
//	QString cadnum = QString();				// Кадастровый номер
//	QString status_obj = QString();			// Статус объекта
//	QString datePosNaCadastr = QString();	// Дата постановки на кадастровый учет
//	QString catZem = QString();				// Категория земель
//	QString razreshIspolz = QString();		// "Разрешенное использование" : "Для индивидуальной жилой застройки"
//	float area = 0.0;						// "Площадь" : "1427"
//	QString edIzmer = QString();			// "Единица измерения (код)" : "Квадратный метр"
//	double cadPrice = 0.0;					// "Кадастровая стоимость" : "1167214,65"
//	QString dateOpredPrice = QString();		// "Дата определения стоимости" : "19.02.2016"
//	QString dateVnesPrice = QString();		// "Дата внесения стоимости" : "10.12.2016"
//	QString dateUtverPrice = QString();		// "Дата утверждения стоимости" : "10.12.2016"
//	QString adres = QString();				// "Адрес (местоположение)" : "îáë. Êóðñêàÿ, ã. Êóðñê, óë. Èëüè÷à, äîì 48"
//	QString dateObnovInfo = QString();		// "Дата обновления информации" : "10.12.2016"
//	QString formaSobstv = QString();		// "Форма собственности" : "×àñòíàÿ"
//	int colPravoobl = 0;					// "Количество правообладателей" : "2"
//	QString cadInjener = QString();			// "Кадастровый инженер" : "Краснобородько Кирилл Александрович (аттестат 46-14-207)"
//};
//
//struct S_EGRN
//{
//	S_OBJECT obj;
//	S_DETAILS details;
//};
//
//struct S_DOCUMENT
//{
//	QString label = QString();	// Название документа
//	bool available = false;		// Доступность для заказа
//	float price = 0.0;			// Стоимость в рублях
//};
//
//struct S_DOCUMENTS
//{
//	S_DOCUMENT xzp;
//	S_DOCUMENT soop;
//	S_DOCUMENT kpt;
//};
//
//// ==== DOC_TYPE DOC_ORDER =====
//
//struct S_DOCUMENTS_ID
//{
//	QString type = QString();
//	int id = 0;
//};

#include <QObject>

#include "i_wsJsonParser.h"

class QJsonDocument;
class WSJsonParser : public QObject, public virtual i_wsJsonParser
{
	Q_OBJECT
	Q_INTERFACES(i_wsJsonParser)
	DECLARE_SMARTPOINTER

public:
	WSJsonParser(QJsonDocument doc, DOC_TYPE docType, QObject* parent = 0);
	~WSJsonParser();

	/*!
		Возвращает тип документа
	*/
	virtual DOC_TYPE docType() const;

	/*!
		Информация о документах
		DOC_TYPE = FULLINFO
	*/
	virtual S_DOCUMENTS documents() const;

	/*!
		Возвращает id документов
		DOC_TYPE = DOC_ORDER
	*/
	virtual QVector<S_DOCUMENTS_ID> documentsID() const;

	/*!
		Ключ
	*/
	virtual QString encoded_object() const;

	/*!
		Возвращает ошибку
	*/
	virtual S_ERROR error() const;

	/*!
		Количество найденных объектов
		DOC_TYPE = SEARCH
	*/
	virtual int found() const;

	/*!
		Информация об объекте
		DOC_TYPE = FULLINFO
	*/
	virtual S_EGRN getEGRN() const;

	/*!
		true - если найденные объекты сгруппированы
		DOC_TYPE = SEARCH
	*/
	virtual bool isGrouped() const;

	/*!
		Возвращает последнюю ошибку возникшую при чтении JSON
	*/
	virtual QString lastParseError() const;

	/*!
		Тип набора данных
		DOC_TYPE = SEARCH
	*/
	virtual MODE_TYPE mode() const;

	/*!
		Объекты
		DOC_TYPE = SEARCH
	*/
	virtual QVector<S_OBJECT> objects() const;

	/*!
		Возвращает paid
		DOC_TYPE = DOC_ORDER
	*/
	virtual bool paid() const;

	/*!
		Возвращает название региона
		DOC_TYPE = FULLINFO
	*/
	virtual QString region() const;

	/*!
		Возвращает transaction_id
		DOC_TYPE = DOC_ORDER
	*/
	virtual int transactionID() const;

	/*!
		Установить кадастровый номер
	*/
	virtual void setCadNumber(QString);

	/*!
		Получить кадастровый номер
	*/
	virtual QString getCadNumber() const;

	virtual void setPriceXZP(float);
	virtual float getPriceXZP() const;

	virtual void setPriceSOPP(float);
	virtual float getPriceSOPP() const;

	virtual void setPriceKPT(float);
	virtual float getPriceKPT() const;


	/*!
		Создать
	*/
	static WSJsonParser* create(QJsonDocument doc, DOC_TYPE docType);

private:

	struct SDATA;
	SDATA* m_data;
};