#ifndef STREETSDIALOGFORM_H
#define STREETSDIALOGFORM_H
#include "stable.h"
#include "ui_streetsdialog.h"
/**
*	Class for showing and adding streets . 
*/

class StreetsDialogForm: public QDialog, private Ui::StreetsDialog
{
	Q_OBJECT
public:
	//!A constructor
	StreetsDialogForm(QWidget *parent=0);

protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
	private slots:
		//!load streets where (const QModelIndex & = index of curent city)
		void LoadStreets(const QModelIndex &);
		//!add new street
		void AddStreet();
		//!find new street
		void FindStreet(QString);
private:
	//!install event filter
	void SetEventFilter();
	//!set star tsize
	void SetStartSize();
	//!models for cities and streets
	QSqlQueryModel *citymodel, *streetmodel;
	//1selection of city listView
	QItemSelectionModel *cityselection;
	//1load cities
	void LoadCities();
	//!index of current city
	QModelIndex citymodelindex;


};

#endif
