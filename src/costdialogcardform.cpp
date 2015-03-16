#include "costdialogcardform.h"
//Constructor for add
CostDialogCardForm::CostDialogCardForm(bool isEdit, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(SaveCost()));
	this->isEdit=isEdit;
	LoadCartingInfo();
	SetEventFilter();
	SetValidator();

}

//Constructor for edit
CostDialogCardForm::CostDialogCardForm(const int &pk_costs, bool isEdit, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(SaveCost()));
	this->isEdit=isEdit;
	this->pk_costs=pk_costs;
	LoadCartingInfo();
	LoadCostInfo();
	SetEventFilter();
	SetValidator();
}

//Load cost info from base
void CostDialogCardForm::LoadCostInfo(){
	QSqlQuery query, costquery;
	query.prepare("SELECT carting_name FROM carting WHERE key=(SELECT carting_id FROM costs WHERE key=:key);");
	query.bindValue(":key", pk_costs);
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}


	query.first();
	cartingcomboBox->setCurrentIndex(cartingcomboBox->findText(query.value(0).toString()));

	costquery.prepare("SELECT cost_level FROM costs WHERE key=:key;");
	costquery.bindValue(":key", pk_costs);
	bool costok=costquery.exec();
	if (!costok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), costquery.lastError().databaseText());
		return;
	}
	costquery.first();
	costlineEdit->setText(costquery.value(0).toString());

}

//Save new cost.
void CostDialogCardForm::SaveCost(){
	if (isEdit)
	{
		//if isEdit key is true, load UpdateCost()
		UpdateCost();
	}else{
		//if isEdit key is true, load AddCost()
		AddCost();
	}

}

//Add new cost
void CostDialogCardForm::AddCost(){
	if (!cartingcomboBox->currentText().isEmpty() && !costlineEdit->text().isEmpty())
	{
		QSqlQuery query, cartingquery;

		bool cartingok=cartingquery.exec("SELECT key FROM carting WHERE carting_name LIKE '"+cartingcomboBox->currentText()+"';");
		if (!cartingok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), cartingquery.lastError().databaseText());
			return;
		}
		cartingquery.first();
		query.prepare("INSERT INTO costs(carting_id, cost_level) VALUES (:carting_id, :cost_level)");
		query.bindValue(":carting_id", cartingquery.value(0).toInt());
		query.bindValue(":cost_level", costlineEdit->text().toInt());
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}
		accept();

	}
}

//Update old cost
void CostDialogCardForm::UpdateCost(){

	QSqlQuery query, cartingquery;

	bool cartingok=cartingquery.exec("SELECT key FROM carting WHERE carting_name LIKE '"+cartingcomboBox->currentText()+"';");
	if (!cartingok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), cartingquery.lastError().databaseText());
		return;
	}
	cartingquery.first();

	query.prepare("UPDATE costs SET carting_id=:carting_id, cost_level=:cost_level WHERE key=:key");
	query.bindValue(":carting_id", cartingquery.value(0).toInt());
	query.bindValue(":cost_level", costlineEdit->text().toInt());
	query.bindValue(":key", pk_costs);
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	accept();
}

//Load carting info
void CostDialogCardForm::LoadCartingInfo(){
	cartingmodel=new QSqlQueryModel(this);
	cartingmodel=new QSqlQueryModel(this);
	cartingmodel->setQuery("SELECT carting_name FROM carting ORDER BY 1;");
	if (cartingmodel->lastError().isValid())
	{
		QMessageBox::warning(this, tr("Qtaxi"), cartingmodel->lastError().databaseText());
		return;
	} 
	cartingcomboBox->setModel(cartingmodel);
	cartingcomboBox->setCurrentIndex(-1);
}

//Set validator for input digit between '0'-'9999'
void CostDialogCardForm::SetValidator(){
	QRegExp rx("-?\\d{1,4}");
	QValidator *validator = new QRegExpValidator(rx, this);
	costlineEdit->setValidator(validator);
}

//Install event filter
void CostDialogCardForm::SetEventFilter(){
	cartingcomboBox->installEventFilter(this);
	costlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
}

//Event filter
bool  CostDialogCardForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==cartingcomboBox || trg==costlineEdit)
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

	if (trg==addpushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				SaveCost();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}


