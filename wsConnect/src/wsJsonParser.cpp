#include <QDebug>
#include <QTextCodec>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "wsJsonParser.h"

IMPLEMENT_SMARTPOINTER(WSJsonParser)
// ----------------------------------------------------------------
struct WSJsonParser::SDATA
{
	QJsonObject root = QJsonObject();
	QTextCodec* codec = Q_NULLPTR;
	DOC_TYPE docType = DOC_TYPE::ERR;
	QString lastParseError = QString();
	QString cadNumber = QString();

	float priceXZP = 0.0;
	float priceSOPP = 0.0;
	float priceKPT = 0.0;
};
// ----------------------------------------------------------------
WSJsonParser::WSJsonParser(QJsonDocument doc, DOC_TYPE docType, QObject* parent)
{
	m_data = Q_NULLPTR;
	m_data = new SDATA;	

	if (doc.isEmpty() != true)
		if (doc.object().empty() != true)
			m_data->root = doc.object();

	m_data->docType = docType;

	m_data->codec = QTextCodec::codecForName("Windows-1251");
	QTextCodec::setCodecForLocale(m_data->codec);
}
// ----------------------------------------------------------------
WSJsonParser::~WSJsonParser()
{
	if (m_data != Q_NULLPTR)
	{
		delete m_data;
		m_data = Q_NULLPTR;
	}
}
// ----------------------------------------------------------------
DOC_TYPE WSJsonParser::docType() const
{
	if (m_data != Q_NULLPTR)
		return m_data->docType;
	return DOC_TYPE::ERR;
}
// ----------------------------------------------------------------
S_DOCUMENTS WSJsonParser::documents() const
{
	S_DOCUMENTS res;

	if (m_data == Q_NULLPTR)
		return res;
	if (m_data->docType != DOC_TYPE::FULLINFO)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return res;
	}
	if (m_data->root.isEmpty() == true)
		return res;

	QJsonValue objectsValue = m_data->root.value("documents");
	if (objectsValue.isNull() == false)
	{
		QJsonObject objects = objectsValue.toObject();

		QJsonValue vxzp = objects.value("XZP");
		if (vxzp.isNull() == false)
		{
			QJsonObject xzp = vxzp.toObject();
			res.xzp.label = m_data->codec->toUnicode(xzp.value("label").toString().toLocal8Bit());
			res.xzp.available = xzp.value("available").toBool();
			res.xzp.price = xzp.value("price").toDouble();
		}

		QJsonValue vsoop = objects.value("SOPP");
		if (vsoop.isNull() == false)
		{
			QJsonObject soop = vsoop.toObject();
			res.soop.label = m_data->codec->toUnicode(soop.value("label").toString().toLocal8Bit());
			res.soop.available = soop.value("available").toBool();
			res.soop.price = soop.value("price").toDouble();
		}

		QJsonValue vkpt = objects.value("KPT");
		if (vkpt.isNull() == false)
		{
			QJsonObject kpt = vkpt.toObject();
			res.kpt.label = m_data->codec->toUnicode(kpt.value("label").toString().toLocal8Bit());
			res.kpt.available = kpt.value("available").toBool();
			res.kpt.price = kpt.value("price").toDouble();
		}
	}
	return res;
}
// ----------------------------------------------------------------
QVector<S_DOCUMENTS_ID> WSJsonParser::documentsID() const
{
	QVector<S_DOCUMENTS_ID> res;

	if (m_data == Q_NULLPTR)
		return res;

	if (m_data->docType != DOC_TYPE::DOC_ORDER)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return res;
	}

	if (m_data->root.isEmpty() == true)
		return res;

	QJsonValue docVal = m_data->root.value("documents_id");
	if (docVal.isNull() == false)
	{
		QJsonObject docObj = docVal.toObject();

		{
			QJsonValue val = docObj.value("XZP");
			if (val.isNull() == false)
			{
				S_DOCUMENTS_ID docID;
				docID.type = QString("XZP");
				docID.id = val.toInt();
				res.push_back(docID);
			}
		}

		{
			QJsonValue val = docObj.value("SOPP");
			if (val.isNull() == false)
			{
				S_DOCUMENTS_ID docID;
				docID.type = QString("SOPP");
				docID.id = val.toInt();
				res.push_back(docID);
			}
		}

		{
			QJsonValue val = docObj.value("KPT");
			if (val.isNull() == false)
			{
				S_DOCUMENTS_ID docID;
				docID.type = QString("KPT");
				docID.id = val.toInt();
				res.push_back(docID);
			}
		}
	}

	return res;
}
// ----------------------------------------------------------------
QString WSJsonParser::encoded_object() const
{
	if (m_data == Q_NULLPTR)
		return QString();
	if (m_data->docType != DOC_TYPE::FULLINFO)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return QString();
	}
	if (m_data->root.isEmpty() == true)
		return QString();

	// Áåðåì ýëåìåíò region
	return m_data->root.value("encoded_object").toString();
}
// ----------------------------------------------------------------
S_ERROR WSJsonParser::error() const
{
	S_ERROR res;
	res.code = 0;
	res.message = QString();

	if (m_data == Q_NULLPTR)
		return res;

	if (m_data->root.isEmpty() == true)
		return res;

	// Áåðåì ýëåìåíò ñ îøèáêîé
	QJsonValue error = m_data->root.value("error");
	if (error.isNull() == false)
	{
		// Êîíâåðòèðóåì â îáúåêò
		QJsonObject era = error.toObject();
		if (era.count() > 0)
		{
			res.code = era.value("code").toInt();
			res.message = era.value("mess").toString();
		}
	}
	return res;
}
// ----------------------------------------------------------------
int WSJsonParser::found() const
{ 
	if (m_data == Q_NULLPTR)
		return 0;
	if (m_data->root.isEmpty() == true)
		return 0;
	if (m_data->docType != DOC_TYPE::SEARCH)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return 0;
	}

	return m_data->root.value("found").toInt();
}
// ----------------------------------------------------------------
S_EGRN WSJsonParser::getEGRN() const
{
	S_EGRN res;

	if (m_data == Q_NULLPTR)
		return res;

	if (m_data->docType != DOC_TYPE::FULLINFO)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return res;
	}

	if (m_data->root.isEmpty() == true)
		return res;

	QJsonValue objectsValue = m_data->root.value("EGRN");
	if (objectsValue.isNull() == false)
	{
		QJsonObject objectsEGRN = objectsValue.toObject();

		QJsonValue objEgrnValue = objectsEGRN.value("object");
		if (objEgrnValue.isNull() == false)
		{
			QJsonObject egrnObj = objEgrnValue.toObject();
			res.obj.cadnomer = m_data->codec->toUnicode(egrnObj.value("CADNOMER").toString().toLocal8Bit());
			res.obj.address = m_data->codec->toUnicode(egrnObj.value("ADDRESS").toString().toLocal8Bit());
			res.obj.type = m_data->codec->toUnicode(egrnObj.value("TYPE").toString().toLocal8Bit());
			res.obj.area = m_data->codec->toUnicode(egrnObj.value("AREA").toString().toLocal8Bit());
			res.obj.category = m_data->codec->toUnicode(egrnObj.value("CATEGORY").toString().toLocal8Bit());
		}

		QJsonValue detEgrnValue = objectsEGRN.value("details");
		if (detEgrnValue.isNull() == false)
		{
			QJsonObject egrnDet = detEgrnValue.toObject();
			res.details.type_obj = egrnDet.value(m_data->codec->toUnicode("Тип объекта")).toString();
			res.details.cadnum = egrnDet.value(m_data->codec->toUnicode("Кадастровый номер")).toString();
			res.details.status_obj = egrnDet.value(m_data->codec->toUnicode("Статус объекта")).toString();
			res.details.datePosNaCadastr = egrnDet.value(m_data->codec->toUnicode("Дата постановки на кадастровый учет")).toString();
			res.details.catZem = egrnDet.value(m_data->codec->toUnicode("Категория земель")).toString();
			res.details.razreshIspolz = egrnDet.value(m_data->codec->toUnicode("Разрешенное использование")).toString();
			res.details.area = egrnDet.value(m_data->codec->toUnicode("Площадь")).toDouble();
			res.details.edIzmer = egrnDet.value(m_data->codec->toUnicode("Единица измерения (код)")).toString();
			res.details.cadPrice = egrnDet.value(m_data->codec->toUnicode("Кадастровая стоимость")).toDouble();
			res.details.dateOpredPrice = egrnDet.value(m_data->codec->toUnicode("Дата определения стоимости")).toString();
			res.details.dateVnesPrice = egrnDet.value(m_data->codec->toUnicode("Дата внесения стоимости")).toString();
			res.details.dateUtverPrice = egrnDet.value(m_data->codec->toUnicode("Дата утверждения стоимости")).toString();
			res.details.adres = egrnDet.value(m_data->codec->toUnicode("Адрес (местоположение)")).toString();
			res.details.dateObnovInfo = egrnDet.value(m_data->codec->toUnicode("Дата обновления информации")).toString();
			res.details.formaSobstv = egrnDet.value(m_data->codec->toUnicode("Форма собственности")).toString();
			res.details.colPravoobl = egrnDet.value(m_data->codec->toUnicode("Количество правообладателей")).toInt();
			res.details.cadInjener = egrnDet.value(m_data->codec->toUnicode("Кадастровый инженер")).toString();
		}
	}
	return res;
}
// ----------------------------------------------------------------
bool WSJsonParser::isGrouped() const
{
	if (m_data == Q_NULLPTR)
		return false;

	if (m_data->docType != DOC_TYPE::SEARCH)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return false;
	}

	if (m_data->root.isEmpty() == true)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Отсутствует корневой элемент JSON");
		return false;
	}

	return m_data->root.value("grouped").toBool();
}
// ----------------------------------------------------------------
QString WSJsonParser::lastParseError() const
{
	if (m_data != Q_NULLPTR)
		return m_data->lastParseError;
	return QString();
}
// ----------------------------------------------------------------
MODE_TYPE WSJsonParser::mode() const
{
	if (m_data == Q_NULLPTR)
		return lite;

	if (m_data->docType != DOC_TYPE::SEARCH)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return lite;
	}

	if (m_data->root.isEmpty() == true)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Отсутствует корневой элемент JSON");
		return lite;
	}

	QString value = m_data->root.value("mode").toString().toLower();

	if (value.isEmpty() == false)
	{
		if (value == "lite")
			return lite;
		if (value == "normal")
			return normal;
	}
	return lite;
}
// ----------------------------------------------------------------
QVector<S_OBJECT> WSJsonParser::objects() const
{
	QVector<S_OBJECT> resObjects;

	if (m_data == Q_NULLPTR)
		return resObjects;
	if (m_data->root.isEmpty() == true)
		return resObjects;
	if (m_data->docType != DOC_TYPE::SEARCH)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return resObjects;
	}

	QJsonValue objects = m_data->root.value("objects");
	if (objects.isNull() == false)
	{
		if (objects.isArray() == true)
		{
			//Çàáèðàåì ìàññèâ èç äàííîãî ñâîéñòâà
			QJsonArray era = objects.toArray();
			for (int i = 0; i < era.count(); i++)
			{
				S_OBJECT object;
				QJsonObject subtree = era.at(i).toObject();
				object.cadnomer = m_data->codec->toUnicode(subtree.value("CADNOMER").toString().toLocal8Bit());
				object.address = m_data->codec->toUnicode(subtree.value("ADDRESS").toString().toLocal8Bit());
				object.type = m_data->codec->toUnicode(subtree.value("TYPE").toString().toLocal8Bit());
				object.area = m_data->codec->toUnicode(subtree.value("AREA").toString().toLocal8Bit());
				object.category = m_data->codec->toUnicode(subtree.value("CATEGORY").toString().toLocal8Bit());

				resObjects.push_back(object);
			}
		}
	}
	return resObjects;
}
// ----------------------------------------------------------------
bool WSJsonParser::paid() const
{
	if (m_data == Q_NULLPTR)
		return false;

	if (m_data->docType != DOC_TYPE::DOC_ORDER)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return false;
	}

	if (m_data->root.isEmpty() == true)
		return false;

	return m_data->root.value("paid").toBool();
}
// ----------------------------------------------------------------
QString WSJsonParser::region() const
{
	if (m_data == Q_NULLPTR)
		return QString();

	if (m_data->docType != DOC_TYPE::FULLINFO)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return QString();
	}

	if (m_data->root.isEmpty() == true)
		return QString();

	// Áåðåì ýëåìåíò region
	return m_data->codec->toUnicode(m_data->root.value("region").toString().toLocal8Bit());
}
// ----------------------------------------------------------------
int WSJsonParser::transactionID() const
{
	if (m_data == Q_NULLPTR)
		return 0;

	if (m_data->docType != DOC_TYPE::DOC_ORDER)
	{
		m_data->lastParseError = m_data->codec->toUnicode("Запрашиваемый метод не соответствует типу документа");
		return 0;
	}

	if (m_data->root.isEmpty() == true)
		return 0;

	return m_data->root.value("transaction_id").toInt();
}
// ----------------------------------------------------------------
void WSJsonParser::setCadNumber(QString cadNumber)
{
	if (m_data == Q_NULLPTR)
		return;

	m_data->cadNumber = cadNumber;
}
// ----------------------------------------------------------------
QString WSJsonParser::getCadNumber() const
{
	if (m_data == Q_NULLPTR)
		return QString();
	return m_data->cadNumber;
}
// ----------------------------------------------------------------
void WSJsonParser::setPriceXZP(float price)
{
	if (m_data != Q_NULLPTR)
		m_data->priceXZP = price;
}
// ----------------------------------------------------------------
float WSJsonParser::getPriceXZP() const
{
	if (m_data != Q_NULLPTR)
		return m_data->priceXZP;
	return 0.0;
}
// ----------------------------------------------------------------
void WSJsonParser::setPriceSOPP(float price)
{
	if (m_data != Q_NULLPTR)
		m_data->priceSOPP = price;
}
// ----------------------------------------------------------------
float WSJsonParser::getPriceSOPP() const
{
	if (m_data != Q_NULLPTR)
		return m_data->priceSOPP;
	return 0.0;
}
// ----------------------------------------------------------------
void WSJsonParser::setPriceKPT(float price)
{
	if (m_data != Q_NULLPTR)
		m_data->priceKPT = price;
}
// ----------------------------------------------------------------
float WSJsonParser::getPriceKPT() const
{
	if (m_data != Q_NULLPTR)
		return m_data->priceKPT;
	return 0.0;
}
// ----------------------------------------------------------------
WSJsonParser* WSJsonParser::create(QJsonDocument doc, DOC_TYPE docType)
{
	return (new WSJsonParser(doc, docType));
}
// ----------------------------------------------------------------