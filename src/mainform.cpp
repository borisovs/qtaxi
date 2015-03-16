#include "mainform.h"
#include "connform.h"
#include "driversform.h"
#include "citiesdialogform.h"
#include "streetsdialogform.h"
#include "carsdialogform.h"
#include "operatorsdialogform.h"
#include "costdialogform.h"
#include "cartingdialogform.h"
#include "parkingdialogform.h"
#include "drivershiftdialogform.h"
#include "orderdialogform.h"
#include "printpreview.h"
#include "profitdialogform.h"
#include "aboutdialogform.h"
#include "clientsdialogform.h"

//Constructor
MainForm::MainForm(){
	setupUi(this);
	setWindowState(Qt::WindowMaximized);
	connect(actionConnection, SIGNAL(triggered()), this, SLOT(ShowConnection()));
	connect(actionDrivers, SIGNAL(triggered()), this, SLOT(ShowDrivers()));
	connect(actionCities, SIGNAL(triggered()), this, SLOT(ShowCities()));
	connect(actionStreets, SIGNAL(triggered()), this, SLOT(ShowStreets()));
	connect(actionCars, SIGNAL(triggered()), this, SLOT(ShowCars()) );
	connect(actionOperators, SIGNAL(triggered()), this, SLOT(ShowOperators()));
	connect(actionCost, SIGNAL(triggered()), this, SLOT(ShowCosts()));
	connect(actionCarting, SIGNAL(triggered()), this, SLOT(ShowCartings()));
	connect(actionAccepted, SIGNAL(triggered()), this, SLOT(ShowAccepted()));
	connect(accepedfindlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindAccepted(const QString &)));
	connect(deferredfindlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindDeferred(const QString &)));
	connect(closedfindlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindClosed(const QString &)));
	connect(actionDeferred, SIGNAL(triggered()), this, SLOT(ShowDeferred()));
	connect(actionClosed, SIGNAL(triggered()), this, SLOT(ShowClosed()));
	connect(actionDriversonline, SIGNAL(triggered()), this, SLOT(ShowDriversOnLine()));
	connect(actionBlack, SIGNAL(triggered()), this, SLOT(ShowBlacklist()));
	connect(actionParking, SIGNAL(triggered()), this, SLOT(ShowParking()));
	connect(addacceptedpushButton, SIGNAL(clicked()), this, SLOT(AddOrder()));
	connect(adddeferredpushButton, SIGNAL(clicked()), this, SLOT(AddOrder()));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(TabChanged(int)));
	connect(todoltoolButton, SIGNAL(clicked()), this, SLOT(AddDriverToLine()));
	connect(fromdoltoolButton, SIGNAL(clicked()), this, SLOT(ShowCloseDriverOnLine()));
	connect(driversfindlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindDriverOnLine(const QString &)));
	connect(driverslistView, SIGNAL(activated(const QModelIndex &)), this, SLOT(AddDriverToLine(const QModelIndex &)));
	connect(driverstableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(CloseDriverOnLine(const QModelIndex &)));
	connect(blackfindlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindBlackList(const QString &)));
	connect(acceptedtableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(EditOrder(const QModelIndex &)));
	connect(deferredtableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(EditOrder(const QModelIndex &)));
	connect(printclosedpushButton, SIGNAL(clicked()), this, SLOT(PrintClosedList()));
	connect(closedtableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(PrintClosedReport(const QModelIndex &)));
	connect(printblackpushButton, SIGNAL(clicked()), this, SLOT(PrintBlackList()));
	connect(actionBest_operator, SIGNAL(triggered()), this, SLOT(PrintBestOperator()));
	connect(actionBest_driver, SIGNAL(triggered()), this, SLOT(PrintBestDriver()));
	connect(actionClients_list, SIGNAL(triggered()), this, SLOT(PrintClients()));
	connect(actionProfit_margin, SIGNAL(triggered()), this, SLOT(ShowProfitMargin()));
	connect(actionContext_help, SIGNAL(triggered()), this, SLOT(ShowHelp()));
	connect(actionAbout_Qtaxi, SIGNAL(triggered()), this, SLOT(AboutQtaxi()));
	connect(actionClients, SIGNAL(triggered()), this, SLOT(ShowClients()));
	ShowAccepted();
	SetParametrs();

}

//Show connection dialog. Modal mode.
void MainForm::ShowConnection(){
	ConnForm *dlg=new ConnForm(this);
	dlg->exec();
	delete dlg;
}

//Show drivers dialog. Modal mode.
void MainForm::ShowDrivers(){
	DriversForm *dlg=new DriversForm(this);
	dlg->exec();
	delete dlg;
}

//Show cities dialog. Modal mode.
void MainForm::ShowCities(){
	CitiesDialogForm *dlg=new CitiesDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show streets dialog. Modal mode.
void MainForm::ShowStreets(){
	StreetsDialogForm *dlg=new StreetsDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show cars dialog. Modal mode.
void MainForm::ShowCars(){
	CarsDialogForm *dlg=new CarsDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show operators dialog. Modal mode.
void MainForm::ShowOperators(){
	OperatorsDialogForm *dlg=new OperatorsDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show costs dialog. Modal mode.
void MainForm::ShowCosts(){
	CostDialogForm *dlg=new CostDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show carting dialog. Modal mode.
void MainForm::ShowCartings(){
	CartingDialogForm *dlg=new CartingDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show parking dialog. Modal mode.
void MainForm::ShowParking(){
	ParkingDialogForm *dlg=new ParkingDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show new order dialog. Modal mode.
//If dialog accepted, reload all QueryModels for
//view changes
void MainForm::AddOrder(){
	OrderDialogForm *dlg=new OrderDialogForm(GetUserName(), this);
	if (dlg->exec()==QDialog::Accepted)
	{
		LoadAccepted();
		LoadDeferred();
		LoadClosed();
		LoadBlackList();
	}
	delete dlg;
}

//Show edit order dialog. Modal mode.
//If dialog accepted, reload all QueryModels for
//view changes
void MainForm::EditOrder(const QModelIndex &index){
	switch(tabWidget->currentIndex())
	{
	case 0:{
		OrderDialogForm *dlg=new OrderDialogForm(GetUserName(), acceptedmodel->data(acceptedmodel->index(index.row(), 0)).toInt() ,this);
		if (dlg->exec()==QDialog::Accepted)
		{
			LoadAccepted();
			LoadDeferred();
			LoadClosed();
		}
		delete dlg;
		break;
		   }
	case 1:{
		OrderDialogForm *dlg=new OrderDialogForm(GetUserName(), deferredmodel->data(deferredmodel->index(index.row(), 0)).toInt() ,this);
		if (dlg->exec()==QDialog::Accepted)
		{
			LoadAccepted();
			LoadDeferred();
			LoadClosed();
		}
		delete dlg;
		break;
		   }
	default:
		break;
	}
}

//Method call LoadAccepted(), and set active tabPage(0)
void MainForm::ShowAccepted(){
	LoadAccepted();
	if (tabWidget->currentIndex()!=0)
		tabWidget->setCurrentIndex(0);
}

//Load accepted orders and show in QTableView
void MainForm::LoadAccepted(){
	if (!accepedfindlineEdit->text().isEmpty())
	{
		FindAccepted(accepedfindlineEdit->text());
	}else{
		acceptedmodel=new QSqlQueryModel(this);
		acceptedmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, carting.carting_name, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isaccepted=TRUE ORDER BY time_order;");
		if (acceptedmodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), acceptedmodel->lastError().databaseText());
			return;
		}
		acceptedmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
		acceptedmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
		acceptedmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
		acceptedmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
		acceptedmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
		acceptedmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
		acceptedmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
		acceptedmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
		acceptedmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
		acceptedmodel->setHeaderData(9, Qt::Horizontal, tr("Carting"));
		acceptedmodel->setHeaderData(10, Qt::Horizontal, tr("Nick"));
		acceptedtableView->setModel(acceptedmodel);
		acceptedselection=acceptedtableView->selectionModel();
		connect(acceptedselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetAcceptedIndex(const QModelIndex &)));
		acceptedtableView->resizeColumnsToContents();
		acceptedtableView->resizeRowsToContents();
	}
}

//Find accepted orders
void MainForm::FindAccepted(const QString &text){
	acceptedmodel=new QSqlQueryModel(this);
	acceptedmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, carting.carting_name, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isaccepted=TRUE AND (orders.phone LIKE '%"+text+"%' OR orders.address LIKE '%"+text+"%') ORDER BY time_order;");
	if (acceptedmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), acceptedmodel->lastError().databaseText());
		return;
	}
	acceptedmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
	acceptedmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
	acceptedmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
	acceptedmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
	acceptedmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
	acceptedmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
	acceptedmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
	acceptedmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
	acceptedmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
	acceptedmodel->setHeaderData(9, Qt::Horizontal, tr("Carting"));
	acceptedmodel->setHeaderData(10, Qt::Horizontal, tr("Nick"));
	acceptedtableView->setModel(acceptedmodel);
	acceptedselection=acceptedtableView->selectionModel();
	connect(acceptedselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetAcceptedIndex(const QModelIndex &)));
	acceptedtableView->resizeColumnsToContents();
	acceptedtableView->resizeRowsToContents();
}

//Method call LoadDeferred(), and set active tabPage(1)
void MainForm::ShowDeferred(){
	LoadDeferred();
	if (tabWidget->currentIndex()!=1)
		tabWidget->setCurrentIndex(1);
}

//Load deferred orders and show in QTableView
void MainForm::LoadDeferred(){
	if (!deferredfindlineEdit->text().isEmpty())
	{
		FindDeferred(deferredfindlineEdit->text());
	}else{
		//deferredmodel=new QSqlQueryModel(this);
		deferredmodel=new OrderRedStringModel(this);
		//deferredmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, carting.carting_name  FROM orders, carting, operators WHERE orders.isdeferred=TRUE AND orders.typecarting=carting.key AND operators.key=orders.operator_open ORDER BY time_order;");
		deferredmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, carting.carting_name, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isdeferred=TRUE ORDER BY time_order;");
		if (deferredmodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), deferredmodel->lastError().databaseText());
			return;
		}
		deferredmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
		deferredmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
		deferredmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
		deferredmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
		deferredmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
		deferredmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
		deferredmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
		deferredmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
		deferredmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
		deferredmodel->setHeaderData(9, Qt::Horizontal, tr("Carting"));
		deferredmodel->setHeaderData(10, Qt::Horizontal, tr("Nick"));
		deferredtableView->setModel(deferredmodel);
		deferredselection=deferredtableView->selectionModel();
		connect(deferredselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDeferredIndex(const QModelIndex &)));
		deferredtableView->resizeColumnsToContents();
		deferredtableView->resizeRowsToContents();
	}
}

//Find deferred orders
void MainForm::FindDeferred(const QString &text){
	deferredmodel=new OrderRedStringModel(this);
	deferredmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, carting.carting_name, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isdeferred=TRUE AND (orders.phone LIKE '%"+text+"%' OR orders.address LIKE '%"+text+"%')  ORDER BY time_order;");
	if (deferredmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), deferredmodel->lastError().databaseText());
		return;
	}
	deferredmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
	deferredmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
	deferredmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
	deferredmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
	deferredmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
	deferredmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
	deferredmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
	deferredmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
	deferredmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
	deferredmodel->setHeaderData(9, Qt::Horizontal, tr("Carting"));
	deferredmodel->setHeaderData(10, Qt::Horizontal, tr("Nick"));
	deferredtableView->setModel(deferredmodel);
	deferredselection=deferredtableView->selectionModel();
	connect(deferredselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDeferredIndex(const QModelIndex &)));
	deferredtableView->resizeColumnsToContents();
	deferredtableView->resizeRowsToContents();
}

//Method call Loadlosed(), and set active tabPage(2)
void MainForm::ShowClosed(){
	LoadClosed();
	if (tabWidget->currentIndex()!=2)
		tabWidget->setCurrentIndex(2);

}


//Load closed orders and show in QTableView
void MainForm::LoadClosed(){
	if (!closedfindlineEdit->text().isEmpty())
	{
		FindClosed(closedfindlineEdit->text());
	}else{
		closedmodel=new QSqlQueryModel(this);
		closedmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, orders.time_driver_free, orders.time_order_close, carting.carting_name, orders.cost_real, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isclosed=TRUE ORDER BY time_order;");
		if (closedmodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), closedmodel->lastError().databaseText());
			return;
		}
		closedmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
		closedmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
		closedmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
		closedmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
		closedmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
		closedmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
		closedmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
		closedmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
		closedmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
		closedmodel->setHeaderData(9, Qt::Horizontal, tr("Driver free"));
		closedmodel->setHeaderData(10, Qt::Horizontal, tr("Order closed"));
		closedmodel->setHeaderData(11, Qt::Horizontal, tr("Carting"));
		closedmodel->setHeaderData(12, Qt::Horizontal, tr("Real cost"));
		closedmodel->setHeaderData(13, Qt::Horizontal, tr("Nick"));
		closedtableView->setModel(closedmodel);
		closedselection=closedtableView->selectionModel();
		closedtableView->resizeColumnsToContents();
		closedtableView->resizeRowsToContents();
		connect(closedselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetClosedIndex(const QModelIndex &)));
	}
}

//Find closed orders
void MainForm::FindClosed(const QString &text){
	closedmodel=new QSqlQueryModel(this);
	closedmodel->setQuery("SELECT orders.key, orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name, orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, orders.time_driver_free, orders.time_order_close, carting.carting_name, orders.cost_real, drivers.nickname FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key  WHERE orders.isclosed=TRUE AND (orders.phone LIKE '%"+text+"%' OR orders.address LIKE '%"+text+"%')  ORDER BY time_order;");
	if (closedmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), closedmodel->lastError().databaseText());
		return;
	}
	closedmodel->setHeaderData(0, Qt::Horizontal, tr("Num"));
	closedmodel->setHeaderData(1, Qt::Horizontal, tr("Order time"));
	closedmodel->setHeaderData(2, Qt::Horizontal, tr("Call to"));
	closedmodel->setHeaderData(3, Qt::Horizontal, tr("Address"));
	closedmodel->setHeaderData(4, Qt::Horizontal, tr("Go to"));
	closedmodel->setHeaderData(5, Qt::Horizontal, tr("Order operator"));
	closedmodel->setHeaderData(6, Qt::Horizontal, tr("Driver go to"));
	closedmodel->setHeaderData(7, Qt::Horizontal, tr("Driver wait"));
	closedmodel->setHeaderData(8, Qt::Horizontal, tr("Driver out"));
	closedmodel->setHeaderData(9, Qt::Horizontal, tr("Driver free"));
	closedmodel->setHeaderData(10, Qt::Horizontal, tr("Order closed"));
	closedmodel->setHeaderData(11, Qt::Horizontal, tr("Carting"));
	closedmodel->setHeaderData(12, Qt::Horizontal, tr("Real cost"));
	closedmodel->setHeaderData(13, Qt::Horizontal, tr("Nick"));
	closedtableView->setModel(closedmodel);
	closedselection=closedtableView->selectionModel();
	closedtableView->resizeColumnsToContents();
	closedtableView->resizeRowsToContents();
	connect(closedselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetClosedIndex(const QModelIndex &)));
}



//Load drivers who is online and show in QTableView
void MainForm::LoadDriversOnLine(){
	if (!driversfindlineEdit->text().isEmpty())
	{
		FindDriverOnLine(driversfindlineEdit->text());
	} else{
		driversfrombasemodel=new QSqlQueryModel(this);
		driversfrombasemodel->setQuery("SELECT nickname FROM drivers WHERE online=false;");
		if (driversfrombasemodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), driversfrombasemodel->lastError().databaseText());
			return;
		}
		driverslistView->setModel(driversfrombasemodel);
		driversfrombaseselection=driverslistView->selectionModel();
		connect(driversfrombaseselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDriverFromBaseIndex(const QModelIndex &)));

		driversonlinemodel=new QSqlQueryModel(this);
		driversonlinemodel->setQuery("SELECT nickname, fullname, car_name_num, parking, parking_num FROM drivers, cars WHERE drivers.car=cars.key AND online=TRUE AND jobstatus=FALSE ORDER BY nickname, parking_num;");
		if (driversonlinemodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), driversonlinemodel->lastError().databaseText());
			return;
		}
		driversonlinemodel->setHeaderData(0, Qt::Horizontal, tr("Nick"));
		driversonlinemodel->setHeaderData(1, Qt::Horizontal, tr("Name"));
		driversonlinemodel->setHeaderData(2, Qt::Horizontal, tr("Car"));
		driversonlinemodel->setHeaderData(3, Qt::Horizontal, tr("Parking"));
		driversonlinemodel->setHeaderData(4, Qt::Horizontal, tr("Sequence"));
		driverstableView->setModel(driversonlinemodel);
		driverstableView->resizeColumnsToContents ();
		driverstableView->resizeRowsToContents ();
		driversonlineselection=driverstableView->selectionModel();
		connect(driversonlineselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDriverOnlineBaseIndex(const QModelIndex &)));

		parkingmodel=new QSqlQueryModel(this);
		parkingmodel->setQuery("SELECT parking, COUNT(parking)  FROM drivers  WHERE online=TRUE AND jobstatus=FALSE GROUP BY parking;");
		if (parkingmodel->lastError().isValid())
		{
			QMessageBox::warning(this, tr("Qtaxi"), parkingmodel->lastError().databaseText());
			return;
		}
		parkingmodel->setHeaderData(0, Qt::Horizontal, tr("Parking"));
		parkingmodel->setHeaderData(1, Qt::Horizontal, tr("Count"));
		parkingtreeView->setModel(parkingmodel);

	}

}



//Method call LoadDriversOnLine(), and set active tabPage(3)
void MainForm::ShowDriversOnLine(){
	LoadDriversOnLine();
	if (tabWidget->currentIndex()!=3)
		tabWidget->setCurrentIndex(3);
}

//Find drivers on line
void MainForm::FindDriverOnLine(const QString &text){
	driversonlinemodel=new QSqlQueryModel(this);
	driversonlinemodel->setQuery("SELECT nickname, fullname, car_name_num, parking, parking_num FROM drivers, cars WHERE drivers.car=cars.key AND online=TRUE AND (nickname LIKE '%"+text+"%') ORDER BY nickname, parking_num;");
	if (driversonlinemodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), driversonlinemodel->lastError().databaseText());
		return;
	}
	driversonlinemodel->setHeaderData(0, Qt::Horizontal, tr("Nick"));
	driversonlinemodel->setHeaderData(1, Qt::Horizontal, tr("Name"));
	driversonlinemodel->setHeaderData(2, Qt::Horizontal, tr("Car"));
	driversonlinemodel->setHeaderData(3, Qt::Horizontal, tr("Parking"));
	driversonlinemodel->setHeaderData(4, Qt::Horizontal, tr("Sequence"));
	driverstableView->setModel(driversonlinemodel);
	driverstableView->resizeColumnsToContents ();
	driversonlineselection=driverstableView->selectionModel();
	connect(driversonlineselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetDriverOnlineBaseIndex(const QModelIndex &)));
}



//Load blacklist and show in QTableView
void MainForm::LoadBlackList(){
	if (!blackfindlineEdit->text().isEmpty())
	{
		FindBlackList(blackfindlineEdit->text());
	} else{
		blacklistmodel=new QSqlQueryModel(this);
		blacklistmodel->setQuery("SELECT phone, start_date, description FROM blacklist;");
		if (blacklistmodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), blacklistmodel->lastError().databaseText());
			return;
		}
		blacklistmodel->setHeaderData(0, Qt::Horizontal, tr("Phone"));
		blacklistmodel->setHeaderData(1, Qt::Horizontal, tr("Date"));
		blacklistmodel->setHeaderData(2, Qt::Horizontal, tr("Description"));
		blacktableView->setModel(blacklistmodel);
		blacktableView->resizeColumnsToContents();
		blacktableView->resizeRowsToContents();
		blacklistselection=blacktableView->selectionModel();
		connect(blacklistselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetBlackListIndex(const QModelIndex &)));
	}


}


//Method call LoadBlackList(), and set active tabPage(4)
void MainForm::ShowBlacklist(){
	LoadBlackList();
	if (tabWidget->currentIndex()!=4)
		tabWidget->setCurrentIndex(4);
}


//Find phone in blacklist
void MainForm::FindBlackList(const QString &text){
	blacklistmodel=new QSqlQueryModel(this);
	blacklistmodel->setQuery("SELECT phone, start_date, description FROM blacklist WHERE phone LIKE '%"+text+"%';");
	if (blacklistmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), blacklistmodel->lastError().databaseText());
		return;
	}
	blacklistmodel->setHeaderData(0, Qt::Horizontal, tr("Phone"));
	blacklistmodel->setHeaderData(1, Qt::Horizontal, tr("Date"));
	blacklistmodel->setHeaderData(2, Qt::Horizontal, tr("Description"));
	blacktableView->setModel(blacklistmodel);
	blacktableView->resizeColumnsToContents();
	blacktableView->resizeRowsToContents();
	blacklistselection=blacktableView->selectionModel();
	connect(blacklistselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetBlackListIndex(const QModelIndex &)));
}


//Set operator name in status bar. 
void MainForm::SetUserNameLabel(const QString &username){
	usname=username;
	QLabel *usernamelabel=new QLabel(tr("User:  ")+usname+" ", statusBar());
	statusBar()->setSizeGripEnabled(false);
	this->statusBar()->addWidget(usernamelabel, 1 );
	SetTime();
}

//Override method, then program ask user about want to quit
void MainForm::closeEvent(QCloseEvent *evt){
	QString message=tr("Are you realy want to quit?");
	int r=QMessageBox::warning(this, tr("Qtaxi"), message, QMessageBox::Yes | QMessageBox::No);
	if (r==QMessageBox::Yes){
		evt->accept();
	} else{
		evt->ignore();
	}
}


//Set current time in status bar. Runing timer for times update.
void MainForm::SetTime(){
	timelabel=new QLabel(QDateTime::currentDateTime().toString(Qt::SystemLocaleDate) ,statusBar());
	this->statusBar()->addWidget(timelabel);
	QTimer *timer=new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT (UpdateTime()));
	timer->start(1000);

}
//Update time label on status bar when timer is over.
void MainForm::UpdateTime(){
	timelabel->setText(QDateTime::currentDateTime().toString(Qt::SystemLocaleDate));
}

//After tab changed, calling method for update information about orders, drivers, or blacklist
void MainForm::TabChanged(int taborder){
	switch(taborder)
	{
	case 0:{
		ShowAccepted();
		   }
		   break;
	case 1:{
		ShowDeferred();
		   }
		   break;
	case 2:{
		ShowClosed();
		   }
		   break;
	case 3: {
		ShowDriversOnLine();
			}
			break;
	case  4:{
		ShowBlacklist();
			}
			break;
	default:
		break;
	}
}


//Set visual parameters
void MainForm::SetParametrs(){
	acceptedtableView->setAlternatingRowColors(true);
	deferredtableView->setAlternatingRowColors(true);
	closedtableView->setAlternatingRowColors(true);
	driverstableView->setAlternatingRowColors(true);
	driverslistView->setAlternatingRowColors(true);
	blacktableView->setAlternatingRowColors(true);
	parkingtreeView->setAlternatingRowColors(true);
}

//Move driver to line.
void MainForm::AddDriverToLine(){
	if (driversfrombaseindex.isValid())
	{

		DriverShiftDialogForm *dlg=new DriverShiftDialogForm((driversfrombasemodel->data(driversfrombaseindex, 0).toString()), false, this);
		//DriverShiftDialogForm *dlg=new DriverShiftDialogForm((driversfrombasemodel->data(driversfrombasemodel->index(driversfrombaseindex.row(),0), 0).toString()), false, this);

		if(dlg->exec()){
			TabChanged(tabWidget->currentIndex());
		}
		delete dlg;
	}
}

//Move driver to line, by double click.
void MainForm::AddDriverToLine(const QModelIndex &index){
	driversfrombaseindex=index;
	if (driversfrombaseindex.isValid())
	{

		DriverShiftDialogForm *dlg=new DriverShiftDialogForm((driversfrombasemodel->data(driversfrombaseindex, 0).toString()), false, this);


		if(dlg->exec()){
			TabChanged(tabWidget->currentIndex());
		}
		delete dlg;
	}
}


//Method for set index where selection in tableView is changed
void MainForm::SetDriverFromBaseIndex(const QModelIndex &index){
	driversfrombaseindex=index;
}
//Method for set index where selection in tableView is changed
void MainForm::SetBlackListIndex(const QModelIndex &index){
	blacklistindex=index;
}
//Method for close driver shift and move out from line by double clicked
void MainForm::CloseDriverOnLine(const QModelIndex &index){
	if (index.isValid())
	{

		DriverShiftDialogForm *dlg=new DriverShiftDialogForm(driversonlinemodel->data(driversonlinemodel->index(index.row(),0), 0).toString(), true, this);

		if(dlg->exec()){
			TabChanged(tabWidget->currentIndex());
		}
		delete dlg;

	}
}


//Method for set index where selection in tableView is changed
void MainForm::SetDriverOnlineBaseIndex(const QModelIndex &index){
	driversonlineindex=index;
}


//Method for close driver shift and move out from line
void MainForm::ShowCloseDriverOnLine(){
	CloseDriverOnLine(driversonlineindex);
}


//Method for set index where selection in tableView is changed
void MainForm::SetAcceptedIndex(const QModelIndex &index){
	acceptedindex=index;
}

//Method for set index where selection in tableView is changed
void MainForm::SetDeferredIndex(const QModelIndex &index){
	deferredindex=index;
}

//Method for set index where selection in tableView is changed
void MainForm::SetClosedIndex(const QModelIndex &index){
	closedindex=index;
}

//Return operator name
QString MainForm::GetUserName(){
	return usname;
}

//Print current closed report information by double click
void MainForm::PrintClosedReport(const QModelIndex &index){
	if (index.isValid())
	{
		closedindex=index;
		int number=closedmodel->data(closedmodel->index(closedindex.row(),0), Qt::DisplayRole).toInt();
		editor=new QTextEdit(this);	
		QSqlQuery query;
		query.prepare("SELECT  orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name,  orders.time_driver_lock, orders.time_driver_wait, orders.time_driver_go, orders.time_driver_free, orders.time_order_close,  carting.carting_name, orders.cost_real, drivers.nickname, orders.key FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key WHERE orders.isclosed=TRUE AND orders.key=:pkey ORDER BY 1;");
		query.bindValue(":pkey", number);
		bool ok=query.exec();
		if (!ok)
		{
			return;
		}
		query.first();

		QTextCursor cursor(editor->textCursor());
		cursor.movePosition(QTextCursor::Start);
		QTextCharFormat boldFormat;
		boldFormat.setFontWeight(QFont::Bold);
		QTextCharFormat normalFormat;
		normalFormat.setFontWeight(QFont::Normal);
		QTextFrame *topFrame=cursor.currentFrame();
		QTextFrameFormat topFrameFormat=topFrame->frameFormat();


		topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
		topFrame->setFrameFormat(topFrameFormat);
		//cursor.insertFrame(topFrameFormat);

		QTextTableFormat orderTableFormat;
		orderTableFormat.setHeaderRowCount(1);
		orderTableFormat.setAlignment(Qt::AlignHCenter);	

		cursor.insertText(tr("Closed order number: ")+QString::number(closedmodel->data(closedmodel->index(closedindex.row(),0), Qt::DisplayRole).toDouble())+'\n', boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		QDateTime ordertime=QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
		//cursor.insertText(tr("Date and time of order:  ")+ ordertime.toString(Qt::SystemLocaleDate));
		cursor.insertText(tr("Date and time of order:  "), normalFormat); 

		cursor.insertText(ordertime.toString(Qt::SystemLocaleDate), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();
		//QTextTable *dateordertable=cursor.insertTable(1,1, orderTableFormat);
		//cursor=dateordertable->cellAt(0,0).firstCursorPosition();
		//cursor.insertText(ordertime.toString(Qt::SystemLocaleDate));
		//cursor.setPosition(topFrame->lastPosition());

		cursor.insertText(tr("Order phone:    "), normalFormat);
		cursor.insertText(query.value(1).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		cursor.insertText(tr("Order address:  "), normalFormat);
		cursor.insertText( query.value(2).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		cursor.insertText(tr("To address:       "), normalFormat);
		cursor.insertText( query.value(3).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		cursor.insertText(tr("Operator:          "), normalFormat);
		cursor.insertText( query.value(4).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		cursor.insertText(tr("Driver:             "), normalFormat);
		cursor.insertText( query.value(12).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();


		QTextTable *drvmoviestable=cursor.insertTable(1,4, orderTableFormat);
		cursor=drvmoviestable->cellAt(0,0).firstCursorPosition();
		cursor.insertText(tr("Driver lock"),boldFormat);
		cursor=drvmoviestable->cellAt(0,1).firstCursorPosition();
		cursor.insertText(tr("Driver wait"), boldFormat);
		cursor=drvmoviestable->cellAt(0,2).firstCursorPosition();
		cursor.insertText(tr("Driver go"), boldFormat);
		cursor=drvmoviestable->cellAt(0,3).firstCursorPosition();
		cursor.insertText(tr("Driver free"), boldFormat);
		//cursor.setPosition(topFrame->lastPosition());

		int row=drvmoviestable->rows();
		drvmoviestable->insertRows(row, 1);

		cursor=drvmoviestable->cellAt(1,0).firstCursorPosition();
		QDateTime locktime=QDateTime::fromString(query.value(5).toString(), Qt::ISODate);
		cursor.insertText(locktime.toString(Qt::SystemLocaleDate));
		cursor=drvmoviestable->cellAt(1,1).firstCursorPosition();
		QDateTime waittime=QDateTime::fromString(query.value(6).toString(), Qt::ISODate);
		cursor.insertText(waittime.toString(Qt::SystemLocaleDate));
		cursor=drvmoviestable->cellAt(1,2).firstCursorPosition();
		QDateTime gotime=QDateTime::fromString(query.value(7).toString(), Qt::ISODate);
		cursor.insertText(gotime.toString(Qt::SystemLocaleDate));
		cursor=drvmoviestable->cellAt(1,3).firstCursorPosition();
		QDateTime freetime=QDateTime::fromString(query.value(8).toString(), Qt::ISODate);
		cursor.insertText(freetime.toString(Qt::SystemLocaleDate));
		cursor.setPosition(topFrame->lastPosition());
		cursor.insertBlock();
		cursor.insertBlock();

		cursor.insertText(tr("Carting:             "), normalFormat);
		cursor.insertText( query.value(10).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();


		cursor.insertText(tr("Real cost:           "), normalFormat);
		cursor.insertText( query.value(11).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		QDateTime orderclosetime=QDateTime::fromString(query.value(9).toString(), Qt::ISODate);
		cursor.insertText(tr("Date and time of close:  "), normalFormat); 
		cursor.insertText(orderclosetime.toString(Qt::SystemLocaleDate), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		QSqlQuery opclose;
		opclose.prepare("SELECT operator_name FROM operators WHERE operators.key=(SELECT operator_close FROM orders WHERE key=:pkey);");
		opclose.bindValue(":pkey", number);
		bool opok=opclose.exec();
		if (!opok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), opclose.lastError().databaseText());
		}
		opclose.first();

		cursor.insertText(tr("Who close:           "), normalFormat);
		cursor.insertText(opclose.value(0).toString(), boldFormat);
		cursor.insertBlock();
		cursor.insertBlock();

		PrintPreview *preview = new PrintPreview(editor->document(), this);
		preview->setWindowModality(Qt::WindowModal);
		preview->setAttribute(Qt::WA_DeleteOnClose);
		preview->setWindowState(Qt::WindowMaximized);
		preview->show();
	}

}


//Print all closed reports information
void MainForm::PrintClosedList(){
	editor=new QTextEdit(this);	
	QSqlQuery query;
	bool ok=query.exec("SELECT  orders.time_order, orders.phone, orders.address, orders.address_finished, operators.operator_name,  orders.time_order_close, carting.carting_name, orders.cost_real, drivers.nickname, orders.key FROM orders LEFT JOIN carting ON orders.typecarting=carting.key LEFT JOIN operators ON operators.key=orders.operator_open LEFT JOIN drivers on orders.driver=drivers.key WHERE orders.isclosed=TRUE ORDER BY 1;");
	if (!ok)
	{
		return;
	}
	//  Building report
	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();

	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);


	cursor.insertText(tr("Closed orders: ")+'\n');
	cursor.insertBlock();

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);
	QTextTable *orderTable = cursor.insertTable(1, 10, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Time order"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Phone"), boldFormat);
	cursor=orderTable->cellAt(0, 2).firstCursorPosition();
	cursor.insertText(tr("Address"), boldFormat);
	cursor=orderTable->cellAt(0, 3).firstCursorPosition();
	cursor.insertText(tr("To address"), boldFormat);
	cursor=orderTable->cellAt(0, 4).firstCursorPosition();
	cursor.insertText(tr("Operator"), boldFormat);
	cursor=orderTable->cellAt(0, 5).firstCursorPosition();
	cursor.insertText(tr("Order close"), boldFormat);
	cursor=orderTable->cellAt(0, 6).firstCursorPosition();
	cursor.insertText(tr("Carting"), boldFormat);
	cursor=orderTable->cellAt(0, 7).firstCursorPosition();
	cursor.insertText(tr("Real cost"), boldFormat);

	cursor=orderTable->cellAt(0, 8).firstCursorPosition();
	cursor.insertText(tr("Driver"), boldFormat);

	cursor=orderTable->cellAt(0, 9).firstCursorPosition();
	cursor.insertText(tr("Num"), boldFormat);

	int limitcount=0;

	while (query.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		QDateTime orderdatetime=QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
		cursor.insertText(orderdatetime.toString(Qt::SystemLocaleDate));

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(query.value(1).toString());

		cursor=orderTable->cellAt(row, 2).firstCursorPosition();
		cursor.insertText(query.value(2).toString());

		cursor=orderTable->cellAt(row, 3).firstCursorPosition();
		cursor.insertText(query.value(3).toString());

		cursor=orderTable->cellAt(row, 4).firstCursorPosition();
		cursor.insertText(query.value(4).toString());

		cursor=orderTable->cellAt(row, 5).firstCursorPosition();
		QDateTime closeorderdate=QDateTime::fromString(query.value(5).toString(), Qt::ISODate);
		cursor.insertText(closeorderdate.toString(Qt::SystemLocaleDate));

		cursor=orderTable->cellAt(row, 6).firstCursorPosition();
		cursor.insertText(query.value(6).toString());

		cursor=orderTable->cellAt(row, 7).firstCursorPosition();
		cursor.insertText(query.value(7).toString());

		cursor=orderTable->cellAt(row, 8).firstCursorPosition();
		cursor.insertText(query.value(8).toString());

		cursor=orderTable->cellAt(row, 9).firstCursorPosition();
		cursor.insertText(query.value(9).toString());


		limitcount++;
	}


	cursor.setPosition(topFrame->lastPosition());
	cursor.insertBlock();
	cursor.insertText(tr("Records count: ")+ QString::number(limitcount));

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();


	//
}



//Print all black list information
void MainForm::PrintBlackList(){
	editor=new QTextEdit(this);	
	QSqlQuery query;
	bool ok=query.exec("SELECT  phone, start_date, description  FROM blacklist ORDER BY 1;");
	if (!ok)
	{
		return;
	}
	//  Building report
	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();

	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);


	cursor.insertText(tr("Blacklist: ")+'\n');
	cursor.insertBlock();

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);
	QTextTable *orderTable = cursor.insertTable(1, 3, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Phone"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("When"), boldFormat);
	cursor=orderTable->cellAt(0, 2).firstCursorPosition();
	cursor.insertText(tr("Decsription"), boldFormat);


	int limitcount=0;

	while (query.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		QDateTime orderdatetime=QDateTime::fromString(query.value(0).toString(), Qt::ISODate);
		cursor.insertText(query.value(0).toString());

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		QDateTime blackdatetime=QDateTime::fromString(query.value(1).toString(), Qt::ISODate);
		cursor.insertText(blackdatetime.toString(Qt::SystemLocaleDate));

		cursor=orderTable->cellAt(row, 2).firstCursorPosition();
		cursor.insertText(query.value(2).toString());

		limitcount++;
	}


	cursor.setPosition(topFrame->lastPosition());
	cursor.insertBlock();
	cursor.insertText(tr("Records count: ")+ QString::number(limitcount));

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();
}


//Print a best operator information
void MainForm::PrintBestOperator(){
	editor=new QTextEdit(this);

	QSqlQuery opbest;
	opbest.exec("SELECT operators.operator_name, COUNT(orders.operator_open)  FROM orders, operators WHERE orders.operator_open=operators.key AND orders.isclosed=TRUE GROUP BY operator_name;");

	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextCharFormat normalFormat;
	normalFormat.setFontWeight(QFont::Normal);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();


	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);	

	cursor.insertText(tr("Best operator! (all dates)"), boldFormat);
	cursor.insertBlock();
	cursor.insertBlock();	

	QTextTable *orderTable = cursor.insertTable(1, 2, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Operator name"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Accepted orders"), boldFormat);

	while (opbest.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		cursor.insertText(opbest.value(0).toString());

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(opbest.value(1).toString());
	}

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();

}


//Print a best driver information
void MainForm::PrintBestDriver(){
	editor=new QTextEdit(this);

	QSqlQuery opbest;
	opbest.exec("SELECT drivers.fullname, COUNT(orders.driver)  FROM orders, drivers WHERE orders.driver=drivers.key AND orders.isclosed=TRUE GROUP BY drivers.fullname;");

	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextCharFormat normalFormat;
	normalFormat.setFontWeight(QFont::Normal);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();


	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);	

	cursor.insertText(tr("Best driver! (all dates)"), boldFormat);
	cursor.insertBlock();
	cursor.insertBlock();	

	QTextTable *orderTable = cursor.insertTable(1, 2, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Driver name"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Accepted orders"), boldFormat);

	while (opbest.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		cursor.insertText(opbest.value(0).toString());

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(opbest.value(1).toString());
	}

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();
}

void MainForm::PrintClients(){
	editor=new QTextEdit(this);

	QSqlQuery clientsquery;
	clientsquery.exec("SELECT client_name, client_num, client_phone, client_discount, client_date FROM clients ORDER BY 1;");

	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextCharFormat normalFormat;
	normalFormat.setFontWeight(QFont::Normal);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();


	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);	

	cursor.insertText(tr("Clients list"), boldFormat);
	cursor.insertBlock();
	cursor.insertBlock();	

	QTextTable *orderTable = cursor.insertTable(1, 5, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Fullname"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Number"), boldFormat);
	cursor=orderTable->cellAt(0, 2).firstCursorPosition();
	cursor.insertText(tr("Phone"), boldFormat);
	cursor=orderTable->cellAt(0, 3).firstCursorPosition();
	cursor.insertText(tr("Discount"), boldFormat);
	cursor=orderTable->cellAt(0, 4).firstCursorPosition();
	cursor.insertText(tr("Date"), boldFormat);

	while (clientsquery.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		cursor.insertText(clientsquery.value(0).toString());

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(clientsquery.value(1).toString());

		cursor=orderTable->cellAt(row, 2).firstCursorPosition();
		cursor.insertText(clientsquery.value(2).toString());

		cursor=orderTable->cellAt(row, 3).firstCursorPosition();
		cursor.insertText(clientsquery.value(3).toString() + "%");
		
		QDate clientdate=QDate::fromString(clientsquery.value(4).toString(), Qt::ISODate);
		cursor=orderTable->cellAt(row, 4).firstCursorPosition();
		cursor.insertText(clientdate.toString(Qt::SystemLocaleDate));
	}

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();

}

//Print profit marging information. Show parameters dialog.
void MainForm::ShowProfitMargin(){
	ProfitDialogForm *dlg=new ProfitDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show help information
void MainForm::ShowHelp(){
	QProcess *proc=new QProcess(this);
#ifdef Q_OS_WIN
	QString program="C:/Program Files/Internet Explorer/IEXPLORE.EXE";
#endif
#ifdef Q_OS_LINUX
	QString program="firefox";
#endif
	QStringList param;
	param<<qApp->applicationDirPath()+"/help/index.htm";
	proc->start(program, param);
}

//Show about program information
void MainForm::AboutQtaxi(){
	AboutDialogForm *dlg=new AboutDialogForm(this);
	dlg->exec();
	delete dlg;
}

//Show clients information
void MainForm::ShowClients(){
	ClientsDialogForm *dlg=new ClientsDialogForm(this);
	dlg->exec();
	delete dlg;
}
