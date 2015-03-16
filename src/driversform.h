#ifndef DRIVERSFORM_H
#define DRIVERSFORM_H
#include "stable.h"
#include "ui_driversdialog.h"

/**
*	Class for showing drivers from database. 
*/

class DriversForm: public QDialog, private Ui::DriversDialog
{
	Q_OBJECT
public:
	//!A constructor
	DriversForm(QWidget *parent=0);
protected:

	private slots:
		//!find drivers
		void FindDrivers(QString);
		//!set index for current driver
		void SetDriverIndex(const QModelIndex &);
		//!show edit driver dialog
		void EditDriver(const QModelIndex &);
		//!show add driver dialog
		void AddDriver();


private:
	//!load drivers from base
	void LoadDrivers();
	//!set start size
	void SetStartSize();
	//!selection of tableView
	QItemSelectionModel *driverselection;
	//!model for drivers
	QSqlQueryModel *driversmodel;
	//!index of current driver
	QModelIndex driverindex;

};

#endif
