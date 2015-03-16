#ifndef COSTDIALOGFORM_H
#define COSTDIALOGFORM_H
#include "stable.h"
#include "ui_costdialog.h"
/**
*	Class for showing costs from database. 
*/

class CostDialogForm: public QDialog, private Ui::CostDialog
{
	Q_OBJECT
public:
	//!A constructor
	CostDialogForm(QWidget *parent=0);
protected:

private:
	//!set start size
	void SetStartSize();
	//!load costs from base
	void LoadCosts();
	//!model for costs
	QSqlQueryModel *model;
	//!selection of listView
	QItemSelectionModel *selection;
	//!index of current index
	QModelIndex costsindex;
	private slots:
		//!show dialog for add new cost
		void AddCost();
		//!show dialog for edit current cost
		void EditCost(const QModelIndex &);
		//!find cost
		void FindCost(QString);
		//!set index for current record
		void SetIndex(const QModelIndex &);
};






#endif //COSTDIALOGFORM_H
