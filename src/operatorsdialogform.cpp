#include "operatorsdialogform.h"
#include "operatorcarddialogform.h"

//Constructor
OperatorsDialogForm::OperatorsDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(findlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT (FindOperator(QString)));
	connect(tableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(EditOperator(const QModelIndex &)));
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddOperator()));
	tableView->verticalHeader()->setDefaultSectionSize(25);
	tableView->setAlternatingRowColors(true);
	LoadOperators();
	SetStartSize();
}

//Set startup size
void OperatorsDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-100, height-100);
	delete dwidget;
	delete rect;
}

//FindOperator in database
void OperatorsDialogForm::FindOperator(QString str){
	operatormodel=new QSqlQueryModel(this);
	operatormodel->setQuery("SELECT operator_name, psn,  born_date, operator_adress, operator_phone, operator_phone_mobile FROM operators WHERE (operator_name LIKE '%"+str+"%' OR psn LIKE '%"+str+"%') ORDER BY 1;");
	operatormodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	operatormodel->setHeaderData(1, Qt::Horizontal, tr("Personal:"));
	operatormodel->setHeaderData(2, Qt::Horizontal, tr("Born"));
	operatormodel->setHeaderData(3, Qt::Horizontal, tr("Adress"));
	operatormodel->setHeaderData(4, Qt::Horizontal, tr("Phone"));
	operatormodel->setHeaderData(5, Qt::Horizontal, tr("Mobile"));
	if (operatormodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), operatormodel->lastError().databaseText());
		return;
	}
	tableView->setModel(operatormodel);
	operatorselection=tableView->selectionModel();
	connect(operatorselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents ();
}

//Call OperatorCardDialogForm(false, this); for add new operator
void OperatorsDialogForm::AddOperator(){
	OperatorCardDialogForm *dlg=new OperatorCardDialogForm(false, this);
	if (dlg->exec())
	{
		LoadOperators();
	}
	delete dlg;

}

//Method for set index where selection in tableView is changed
void OperatorsDialogForm::SetIndex(const QModelIndex &index){
	operatorindex=index;
}

//Load operators
void OperatorsDialogForm::LoadOperators(){
	operatormodel=new QSqlQueryModel(this);
	operatormodel->setQuery("SELECT operator_name, psn, born_date, operator_adress, operator_phone, operator_phone_mobile FROM operators ORDER BY 1;");
	operatormodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	operatormodel->setHeaderData(1, Qt::Horizontal, tr("Personal:"));
	operatormodel->setHeaderData(2, Qt::Horizontal, tr("Born"));
	operatormodel->setHeaderData(3, Qt::Horizontal, tr("Adress"));
	operatormodel->setHeaderData(4, Qt::Horizontal, tr("Phone"));
	operatormodel->setHeaderData(5, Qt::Horizontal, tr("Mobile"));
	if (operatormodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), operatormodel->lastError().databaseText());
	}
	tableView->setModel(operatormodel);
	operatorselection=tableView->selectionModel();
	connect(operatorselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents ();

}

//Call OperatorCardDialogForm(true, this); for edit current operator
void OperatorsDialogForm::EditOperator(const QModelIndex &index){
	if (index.isValid())
	{

		operatorindex=index;
		OperatorCardDialogForm *dlg=new OperatorCardDialogForm(true, operatormodel->data(operatormodel->index(operatorindex.row(),1),Qt::DisplayRole).toInt() ,  this);
		if (dlg->exec())
		{
			LoadOperators();
		}
		delete dlg;
	}
}

