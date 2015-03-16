#ifndef CITIESDIALOGFORM_H
#define CITIESDIALOGFORM_H
#include "stable.h"
#include "ui_citiesdialog.h"

/**
*	Class for showing and edit cities in database. 
*/

class CitiesDialogForm:public QDialog, private Ui::CitiesDialog
{
	Q_OBJECT
public:
	//!A constructor
	CitiesDialogForm(QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!install event filter
	void SetEventFilter();
	//!load cities
	void LoadCities();
	//!set start size
	void SetStartSize();
	//!model for cities
	QSqlQueryModel *citymodel;
	//!selection of listView
	QItemSelectionModel *selection;
	//!index of current record
	QModelIndex cityindex;
	private slots:
		//!add new city
		void AddCity();
		//!set index of current record
		void SetIndex(const QModelIndex &);
		//!find city
		void FindCity(QString);

};


#endif
