#include "clientsdialogform.h"
#include "clienteditdialogform.h"


//Constructor for ClientsDialogForm
ClientsDialogForm::ClientsDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddClient()));
	connect(tableView, SIGNAL(activated(const QModelIndex &)), this, SLOT(EditClient(const QModelIndex &)));
	connect(findlineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(FindClients(QString)));
	tableView->setAlternatingRowColors(true);
	tableView->verticalHeader()->setDefaultSectionSize(25);
	LoadClients();
	SetStartSize();
}

//Load all clients from base, and showing in QtableView
void ClientsDialogForm::LoadClients(){
	if (findlineEdit->text().isEmpty())
	{
		clientsmodel=new QSqlQueryModel(this);
		clientsmodel->setQuery("SELECT client_name, client_num, client_phone, client_discount, client_date FROM clients ORDER BY 1;");
		clientsmodel->setHeaderData(0,Qt::Horizontal,tr("Full name"));
		clientsmodel->setHeaderData(1,Qt::Horizontal,tr("Personal ¹"));
		clientsmodel->setHeaderData(2,Qt::Horizontal,tr("Phone"));
		clientsmodel->setHeaderData(3,Qt::Horizontal,tr("Discount"));
		clientsmodel->setHeaderData(4,Qt::Horizontal,tr("Date"));
		if (clientsmodel->lastError().isValid())
		{
			QMessageBox::information(this, tr("Qtaxi"), clientsmodel->lastError().databaseText());
			return;
		}
		tableView->setModel(clientsmodel);
		clientselection=tableView->selectionModel();
		connect(clientselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetClientIndex(const QModelIndex &)));
		tableView->resizeColumnsToContents();
		tableView->resizeRowsToContents();
	}else{
		FindClients(findlineEdit->text());
	}
}

//Set size of dialog form
void ClientsDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-100, height-100);
	delete dwidget;
	delete rect;

}

//Find clients in database
void ClientsDialogForm::FindClients(QString findclient){
	clientsmodel=new QSqlQueryModel(this);
	clientsmodel->setQuery("SELECT client_name, client_num, client_phone, client_discount, client_date FROM clients WHERE (client_name LIKE '%"+findclient+"%' OR client_num LIKE '%"+findclient+"%')ORDER BY 1;");
	clientsmodel->setHeaderData(0,Qt::Horizontal,tr("Full name"));
	clientsmodel->setHeaderData(1,Qt::Horizontal,tr("Personal ¹"));
	clientsmodel->setHeaderData(2,Qt::Horizontal,tr("Phone"));
	clientsmodel->setHeaderData(3,Qt::Horizontal,tr("Discount"));
	clientsmodel->setHeaderData(4,Qt::Horizontal,tr("Date"));
	if (clientsmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), clientsmodel->lastError().databaseText());
		return;
	}
	tableView->setModel(clientsmodel);
	clientselection=tableView->selectionModel();
	connect(clientselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetClientIndex(const QModelIndex &)));
	tableView->resizeColumnsToContents();
	tableView->resizeRowsToContents();
}

//Method for set index where selection in tableView is changed
void ClientsDialogForm::SetClientIndex(const QModelIndex &index){
	clientindex=index;
}


//Method calling DriverCardForm for edit record
void ClientsDialogForm::EditClient(const QModelIndex &index){
	if (index.isValid())
	{
	clientindex=index;
	ClientEditDialogForm *dlg=new ClientEditDialogForm(clientsmodel->data(clientsmodel->index(clientindex.row(), 1), 0).toInt(), true, this);
	if (dlg->exec())
	{
		LoadClients();

	}


	delete dlg;
}
}

//Method calling DriverCardForm for add new record
void ClientsDialogForm::AddClient(){
	ClientEditDialogForm *dlg=new ClientEditDialogForm(false, this);
	if (dlg->exec())
	{
		LoadClients();
	}
	delete dlg;
}
