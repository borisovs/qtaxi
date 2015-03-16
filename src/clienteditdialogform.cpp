#include "clienteditdialogform.h"

//Constructor of dialog form for add new client
ClientEditDialogForm::ClientEditDialogForm(bool isEdit, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveClient()));
	this->isEdit=isEdit;
	setWindowTitle(tr("Add new client"));
	SetStartSize();
	SetEventFilter();
	SetValidator();

}

//Constructor of dialog form for edit current client
ClientEditDialogForm::ClientEditDialogForm(int key, bool isEdit, QWidget *parent/* = 0*/):QDialog(parent){
	setupUi(this);
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveClient()));
	this->isEdit=isEdit;
	this->key=key;
	setWindowTitle(tr("Edit client"));
	SetStartSize();
	SetEventFilter();
	LoadClientInfo();
	SetValidator();
	numlineEdit->setReadOnly(true);
}

//Set size of dialog form
void ClientEditDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-700, height-550);
	delete dwidget;
	delete rect;

}

//Method to install event filter for same components
void ClientEditDialogForm::SetEventFilter(){
	numlineEdit->installEventFilter(this);
	namelineEdit->installEventFilter(this);
	phonelineEdit->installEventFilter(this);
	spinBox->installEventFilter(this);
	savepushButton->installEventFilter(this);

}

void ClientEditDialogForm::SetValidator(){
	QRegExp rx("-?\\d{1,4}");
	QValidator *validator = new QRegExpValidator(rx, this);
	numlineEdit->setValidator(validator);
}


//Method for load client list
void ClientEditDialogForm::LoadClientInfo(){
	QSqlQuery query;
	query.prepare("SELECT client_num, client_name, client_phone, client_discount FROM clients WHERE client_num=:num;");
	query.bindValue(":num", key);
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	query.first();
	numlineEdit->setText(query.value(0).toString());
	namelineEdit->setText(query.value(1).toString());
	phonelineEdit->setText(query.value(2).toString());
	spinBox->setValue(query.value(3).toInt());
}

//Method for switch insert or update method
void ClientEditDialogForm::SaveClient(){
	if (!numlineEdit->text().isEmpty()  && !phonelineEdit->text().isEmpty() && !namelineEdit->text().isEmpty() && spinBox->value()>0)
	{
		if (isEdit)
		{
			//If isEdit key is TRUE calling UpdateClient
			UpdateClient();
		}else{
			//If isEdit is FALSE calling InsertClient();
			InsertClient();
		}
	}else{
		QMessageBox::information(this, tr("Qtaxi"), tr("Required fields are empty"));
		return;

	}
}

//Insert new client in base
void ClientEditDialogForm::InsertClient(){
	QSqlQuery query;
	query.prepare("INSERT INTO clients(client_num, client_name, client_phone, client_discount, client_date) VALUES(:cnum, :cname, :cphone, :cdiscount, :cdate);");
	query.bindValue(":cnum", numlineEdit->text());
	query.bindValue(":cname", namelineEdit->text());
	query.bindValue(":cphone", phonelineEdit->text());
	query.bindValue(":cdiscount", spinBox->value());
	query.bindValue(":cdate", QDate::currentDate());
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}


	accept();
}

//Update client in base
void ClientEditDialogForm::UpdateClient(){
	QSqlQuery query;
	query.prepare("UPDATE clients SET client_name=:cname, client_phone=:cphone, client_discount=:cdiscount WHERE client_num=:cnum");
	query.bindValue(":cname", namelineEdit->text());
	query.bindValue(":cphone", phonelineEdit->text());
	query.bindValue(":cdiscount", spinBox->value());
	query.bindValue(":cnum", key);
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}

	accept();
}

//Reimplementing event filter
bool ClientEditDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==numlineEdit  || trg==phonelineEdit || trg==namelineEdit || trg==spinBox)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				focusNextChild();
				return true;
			}
		}

	}
	if (trg==savepushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				SaveClient();
				return true;
			}
		}

	}
	return QDialog::eventFilter(trg, evt);
}
