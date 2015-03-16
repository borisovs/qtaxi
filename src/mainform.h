#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include "stable.h"
#include "orderredstringmodel.h"
#include "ui_mainwindow.h"
/**
*	Main worked class.  
*	
*/

class MainForm: public QMainWindow, private Ui::MainWindow
{
	Q_OBJECT
public:
	//!A constructor
	MainForm();
	//!Method returned operator name
	QString GetUserName();
	public slots:
		//!Put operator name in status bar
		void SetUserNameLabel(const QString &);
protected:
	//!Method ask operator about quit for correct logout 
	void closeEvent(QCloseEvent *);
private:
	//!variable for insert report strings
	QTextEdit *editor;
	//!operator name
	QString usname;
	//!timelabel for show current time
	QLabel *timelabel;
	//!models for orders, blacklist and drivers
	QSqlQueryModel *acceptedmodel, *closedmodel, *driversfrombasemodel, *driversonlinemodel, *blacklistmodel, *parkingmodel;
	OrderRedStringModel *deferredmodel;
	//!selections for orders, blacklist and drivers
	QItemSelectionModel *acceptedselection, *deferredselection, *closedselection, *driversfrombaseselection, *driversonlineselection, *blacklistselection;
	//!index for orders, blacklist and drivers
	QModelIndex acceptedindex, deferredindex, closedindex, driversfrombaseindex, driversonlineindex, blacklistindex;
	//!set tables visual parameters
	void SetParametrs();
	private slots:
		//!Show cars dialog. Modal mode.
		void ShowCars();
		//!Show connection dialog. Modal mode.
		void ShowConnection();
		//!Show drivers dialog. Modal mode.
		void ShowDrivers();
		//!Show cities dialog. Modal mode.
		void ShowCities();
		//!Show streets dialog. Modal mode.
		void ShowStreets();
		//!Show operators dialog. Modal mode.
		void ShowOperators();
		//!Show costs dialog. Modal mode.
		void ShowCosts();
		//!Show carting dialog. Modal mode.
		void ShowCartings();
		//!Show parking dialog. Modal mode.
		void ShowParking();
		//!Method call LoadAccepted(), and set active tabPage(0)
		void ShowAccepted();
		//!Method call LoadDeferred(), and set active tabPage(1)
		void ShowDeferred();
		//!Method call Loadlosed(), and set active tabPage(2)
		void ShowClosed();
		//!Method call LoadDriversOnLine(), and set active tabPage(3)
		void ShowDriversOnLine();
		//!Method call LoadBlackList(), and set active tabPage(4)
		void ShowBlacklist();
		//!Load accepted orders and show in QTableView
		void LoadAccepted();
		//!Find accepted orders
		void FindAccepted(const QString &);
		//!Method for set index where selection in tableView is changed
		void SetAcceptedIndex(const QModelIndex &);
		//!Load deferred orders and show in QTableView
		void LoadDeferred();
		//!Find deferred orders
		void FindDeferred(const QString &);
		//!Method for set index where selection in deferred tableView is changed
		void SetDeferredIndex(const QModelIndex &);
		//!Load closed orders and show in QTableView
		void LoadClosed();
		//!Find closed orders
		void FindClosed(const QString &);
		//!Method for set index where selection in closed tableView is changed
		void SetClosedIndex(const QModelIndex &);
		//!Load drivers who is online and show in QTableView
		void LoadDriversOnLine();
		//!Find drivers who is online and show in QTableView
		void FindDriverOnLine(const QString &);
		//!Method for set index where selection in drivers on line tableView is changed
		void SetDriverOnlineBaseIndex(const QModelIndex &);
		//!Method for set index where selection in listView is changed
		void SetDriverFromBaseIndex(const QModelIndex &);
		//!Load blacklist and show in QTableView
		void LoadBlackList();
		//!Find phones in blacklist
		void FindBlackList(const QString &);
		//!Method for set index where selection in blacklist tableView is changed
		void SetBlackListIndex(const QModelIndex &);
		/*Show new order dialog. Modal mode.
		*If dialog accepted, reload all QueryModels for view changes
		*/
		void AddOrder();
		/*Show edit order dialog. Modal mode.
		*If dialog accepted, reload all QueryModels for view changes
		*/
		void EditOrder(const QModelIndex &);

		//!Set current time in status bar. Runing timer for times update.
		void SetTime();
		//!Update time label on status bar when timer is over.
		void UpdateTime();
		//!After tab changed, calling method for update information about orders, drivers, or blacklist
		void TabChanged(int);
		//!Move driver to line.
		void AddDriverToLine();
		//!Move driver to line, by double click.
		void AddDriverToLine(const QModelIndex &);
		//!Method for close driver shift and move out from line by double clicked
		void CloseDriverOnLine(const QModelIndex &);
		//!Method for close driver shift and move out from line
		void ShowCloseDriverOnLine();
		//!Print current closed report information by double click
		void PrintClosedReport(const QModelIndex &);
		//!Print all closed reports information
		void PrintClosedList();
		//!Print all black list information
		void PrintBlackList();
		//!Print a best operator information
		void PrintBestOperator();
		//!Print a best driver information
		void PrintBestDriver();
		//!Print clients list.
		void PrintClients();
		//!Print profit marging information. Show parameters dialog.
		void ShowProfitMargin();
		//!Show help information
		void ShowHelp();
		//!Show about program information
		void AboutQtaxi();
		//!Show clients dialog. Modal mode.
		void ShowClients();




};

#endif
