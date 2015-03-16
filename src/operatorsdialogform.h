#ifndef OPERATORSDIALOGFORM_H
#define OPERATORSDIALOGFORM_H
#include "stable.h"
#include "ui_operatorsdialog.h"
/**
*	Class for showing operators in database. 
*/

class OperatorsDialogForm: public QDialog, private Ui::OperatorsDialog
{
	Q_OBJECT
public:
	//!A constructor
	OperatorsDialogForm(QWidget *parent=0);
protected:
private:
	//!set start size
	void SetStartSize();
	//!selection of listView
	QItemSelectionModel *operatorselection;
	//!model for operators
	QSqlQueryModel *operatormodel;
	//!index of current operator
	QModelIndex operatorindex;
	private slots:
		//!show add operator form
		void AddOperator();
		//!show edit operator form
		void EditOperator(const QModelIndex &);
		//!set index for current operator
		void SetIndex(const QModelIndex &);
		//!find operators
		void FindOperator(QString);
		//!load operators from base
		void LoadOperators();
};

#endif

