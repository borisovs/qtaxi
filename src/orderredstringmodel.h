#ifndef ORDERREDSTRINGMODEL_H
#define ORDERREDSTRINGMODEL_H
#include "stable.h"
/**
*	Class for showing overtime deferred orders in red color. 
*/

class OrderRedStringModel : public QSqlQueryModel
{
	Q_OBJECT
public:
	//!A constructor
	OrderRedStringModel(QObject *parent=0):QSqlQueryModel(parent){};
	//!managed view of data if overtime Qt::BackgroundRole=red color Qt::ForegroundRole=white color
	virtual QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const{
		QDateTime today=QDateTime::currentDateTime();
		if (role==Qt::BackgroundRole ){
			QDateTime modeldatetime=record(index.row()).value(1).toDateTime();
			if (today>modeldatetime) return QVariant(QColor(Qt::red));
			if (today<=modeldatetime) return QVariant(QColor(Qt::yellow));
		}
		if (role==Qt::ForegroundRole ){
			QDateTime modeldatetime=record(index.row()).value(1).toDateTime();
			if (today>modeldatetime) return QVariant(QColor(Qt::white));
			if (today<=modeldatetime) return QVariant(QColor(Qt::blue));
		}
	
			return QSqlQueryModel::data(index, role);
	}


protected:
private:
};

#endif 
