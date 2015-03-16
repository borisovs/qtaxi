#include "driversform.h"
#include "drivercardform.h"

//Constructor
DriversForm::DriversForm(QWidget *parent):QDialog(parent){
	setupUi(this);
	connect(findlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindDrivers(QString)));
	connect(tableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(EditDriver(const QModelIndex &)));
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddDriver()));
	tableView->setAlternatingRowColors(true);
	tableView->verticalHeader()->setDefaultSectionSize(25);
	LoadDrivers();
	SetStartSize();
}

//Load drivers and shoing in QTableView
void DriversForm::LoadDrivers(){
	driversmodel=new QSqlQueryModel(this);
	driversmodel->setQuery("SELECT drivers.fullname, drivers.psn,  drivers.born_date, cities.city_name, streets.street_name, drivers.home, cars.car_name_num, drivers.nickname, drivers.phone_home, drivers.phone_mob FROM drivers, cities, streets, cars WHERE drivers.city=cities.key AND drivers.street=streets.key AND drivers.car=cars.key ORDER BY 1;");
	driversmodel->setHeaderData(0,Qt::Horizontal,tr("Full name"));
	driversmodel->setHeaderData(1,Qt::Horizontal,tr("Personal ¹"));
	driversmodel->setHeaderData(2,Qt::Horizontal,tr("Born"));
	driversmodel->setHeaderData(3,Qt::Horizontal,tr("City"));
	driversmodel->setHeaderData(4,Qt::Horizontal,tr("Street"));
	driversmodel->setHeaderData(5,Qt::Horizontal,tr("Home"));
	driversmodel->setHeaderData(6,Qt::Horizontal,tr("Car"));
	driversmodel->setHeaderData(7,Qt::Horizontal,tr("Nick"));
	driversmodel->setHeaderData(8,Qt::Horizontal,tr("Phone"));
	driversmodel->setHeaderData(9,Qt::Horizontal,tr("Mobile"));
	tableView->setModel(driversmodel);
	driverselection=tableView->selectionModel();
	connect(driverselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDriverIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
}

//Set start size
void DriversForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-100, height-100);
	delete dwidget;
	delete rect;

}



//Find drivers in database
void DriversForm::FindDrivers(QString finddriver){
	driversmodel=new QSqlQueryModel(this);
	driversmodel->setQuery("SELECT drivers.fullname, drivers.psn,  drivers.born_date, cities.city_name, streets.street_name, drivers.home, cars.car_name_num, drivers.nickname, drivers.phone_home, drivers.phone_mob FROM drivers, cities, streets, cars WHERE drivers.city=cities.key AND drivers.street=streets.key AND drivers.car=cars.key AND (fullname  LIKE '"+finddriver+"%' OR psn LIKE '" +finddriver+"%')  ORDER BY 1");
	driversmodel->setHeaderData(0,Qt::Horizontal,tr("Full name"));
	driversmodel->setHeaderData(1,Qt::Horizontal,tr("Personal ¹"));
	driversmodel->setHeaderData(2,Qt::Horizontal,tr("Born"));
	driversmodel->setHeaderData(3,Qt::Horizontal,tr("City"));
	driversmodel->setHeaderData(4,Qt::Horizontal,tr("Street"));
	driversmodel->setHeaderData(5,Qt::Horizontal,tr("Home"));
	driversmodel->setHeaderData(6,Qt::Horizontal,tr("Car"));
	driversmodel->setHeaderData(7,Qt::Horizontal,tr("Nick"));
	driversmodel->setHeaderData(8,Qt::Horizontal,tr("Phone"));
	driversmodel->setHeaderData(9,Qt::Horizontal,tr("Mobile"));
	tableView->setModel(driversmodel);
	driverselection=tableView->selectionModel();
	connect(driverselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDriverIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
}

//Method for set index where selection in tableView is changed
void DriversForm::SetDriverIndex(const QModelIndex &index){
	driverindex=index;
}


//Method calling DriverCardForm for edit record
void DriversForm::EditDriver(const QModelIndex &index){
	driverindex=index;
	DriverCardForm *dlg=new DriverCardForm(true, driversmodel->data(driversmodel->index(index.row(), 1), 0).toInt(), this);
	if (dlg->exec())
	{
		if (!findlineEdit->text().isEmpty())
		{
			findlineEdit->setText(NULL);
		} else{
			LoadDrivers();
		}

	}


	delete dlg;
}

//Method calling DriverCardForm for add new record
void DriversForm::AddDriver(){
	DriverCardForm *dlg=new DriverCardForm(false, this);
	if (dlg->exec())
	{
		LoadDrivers();
	}
	delete dlg;
}

