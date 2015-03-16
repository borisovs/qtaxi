#include "costdialogform.h"
#include "costdialogcardform.h"
//Constructor
CostDialogForm::CostDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddCost()));
	connect(tableView, SIGNAL(activated(const QModelIndex &)), this ,SLOT(EditCost(const QModelIndex &)));
	connect(findlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindCost(QString)));
	tableView->verticalHeader()->setDefaultSectionSize(25);
	tableView->setAlternatingRowColors(true);
	LoadCosts();
	SetStartSize();

}

//Add new cost
void CostDialogForm::AddCost(){
	CostDialogCardForm *dlg=new CostDialogCardForm(false, this);

	if ( dlg->exec())
	{
		LoadCosts();
	}
	delete dlg;

}

//Set start size
void CostDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;

}

//Load cost and show in QListView
void CostDialogForm::LoadCosts(){
	model=new QSqlQueryModel(this);
	model->setQuery("SELECT carting.carting_name, costs.cost_level FROM carting, costs WHERE carting.key=costs.carting_id ORDER BY 1;");
	model->setHeaderData(0, Qt::Horizontal, tr("Carting"));
	model->setHeaderData(1, Qt::Horizontal, tr("Cost"));
	if (model->lastError().isValid())
	{
		QMessageBox::warning(this, tr("Qtaxi"), model->lastError().databaseText());
		return;
	} 
	tableView->setModel(model);
	selection=tableView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents ();
}

//Find cost
void CostDialogForm::FindCost(QString str){
	model=new QSqlQueryModel(this);
	model->setQuery("SELECT carting.carting_name, costs.cost_level FROM carting, costs WHERE carting.key=costs.carting_id AND carting.carting_name LIKE '%"+str+"%' ORDER BY 1;");
	model->setHeaderData(0, Qt::Horizontal, tr("Carting"));
	model->setHeaderData(1, Qt::Horizontal, tr("Cost"));
	if (model->lastError().isValid())
	{
		QMessageBox::warning(this, tr("Qtaxi"), model->lastError().databaseText());
		return;
	} 
	tableView->setModel(model);
	selection=tableView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents ();
}

//Method for set index where selection in listView is changed
void CostDialogForm::SetIndex(const QModelIndex &index){
	costsindex=index;
}

//Edit cost. Show CostDialogCardForm()
void CostDialogForm::EditCost(const QModelIndex &index){
	if (index.isValid()){
		QSqlQuery query;
		bool ok=query.exec("SELECT key FROM costs WHERE carting_id=(SELECT key FROM carting WHERE carting_name LIKE '"+model->data(model->index(index.row(), 0), Qt::DisplayRole).toString() +"');");
		if (!ok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}
		query.first();

		CostDialogCardForm *dlg=new CostDialogCardForm(query.value(0).toInt(), true, this);

		if ( dlg->exec())
		{
			LoadCosts();
		}
		delete dlg;
	}

}




