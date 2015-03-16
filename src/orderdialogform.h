#ifndef ORDERDIALOGFORM_H
#define ORDERDIALOGFORM_H
#include "stable.h"
#include "ui_orderdialog.h"
/**
*	Class for add and edit orders. 
*/

class OrderDialogForm: public QDialog, private Ui::OrderDialog
{
	Q_OBJECT
public:
	//!A constructor for add new order	
	OrderDialogForm(const QString &, QWidget *parent=0);
	//!A constructor for edit order
	OrderDialogForm(const QString &, int pkey, QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!operator name
	QString usname;
	//!edit flag
	bool isEdit;
	//!primary key of current order
	int pkey;
	//!models for types, drivers
	QSqlQueryModel *typesmodel, *driversmodel;
	//!set start size
	void SetStartSize();
	//!install event filter
	void SetEventFilter();

	private slots:
		//!save new order
		void SaveOrder();
		//!update current order
		void UpdateOrder();
		//!close current order
		void CloseOrder();
		//!load types of carting
		void LoadTypes();
		//!set widget parameters by current order field
		void SetParametrsOfOrder();
		//!load cost of current carting
		void LoadCost(const QString &);
		//!load drivers
		void LoadDrivers();
		//!load cars
		void LoadCar(const QString &);
		//!move current phone to blacklist 
		void RemoveToBlack();
		//!update table orders. set wait time
		void SetWaitTime();
		//!update table orders. set go time
		void SetGoTime();
		//!update table orders. set free time
		void SetFreeTime();
		//!Load client information
		void LoadClientInfo();
		//!Set validator for correct input client number
		void SetValidator();
		//!Set real cost for client
		void SetRealCost();
};
#endif  //ORDERDIALOGFORM_H
