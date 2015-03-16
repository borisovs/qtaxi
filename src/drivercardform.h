#ifndef DRIVERCARDFORM_H
#define DRIVERCARDFORM_H
#include "stable.h"
#include "ui_drivercarddialog.h"
/**
*	Class for edit driver from database. 
*/

class DriverCardForm: public QDialog, private Ui::DriverCardDialog
{
	Q_OBJECT
public:
	//!A constructor for add new driver
	DriverCardForm( bool isEdit, QWidget *parent);
	//!A constructor for edit driver
	DriverCardForm( bool isEdit, int psn, QWidget *parent);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!set format date for correct work eventFilter
	void SetFormateDate();
	//!set evet filter
	void SetEventFilter();
	//!set validator for only digit input between '0'-'9999'
	void SetValidator();
	//! models for cities, streets, cars
	QSqlQueryModel *citymodel, *streetmodel, *carmodel;
	//!temp variables
	QString currentcity, currentstreet, currentcar;
	//set start size
	void SetStartSize();
	//!edit flag
	bool isEdit;
	//!personal number of current driver
	int psn;
	//!byte array for load image from hdd
	QByteArray bytes;
	private slots:
		//!load cities
		void LoadCities();
		//!load streets with (QString city) parameters
		void LoadStreets(QString);
		//!call add city dialog
		void AddCity();
		//!call add street dialog
		void AddStreet();
		//!call add car dialog
		void AddCar();
		//!load cars
		void LoadCars();
		//!load drivers
		void AddDriver();
		//!update current driver
		void UpdateDriver();
		//!save new driver
		void SaveDriver();
		//!load foto from HDD
		void LoadPhoto();

		//!load current driver info from base
		void LoadDriverInfo();

};

#endif
