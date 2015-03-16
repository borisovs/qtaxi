#include "authenticdialogform.h"


//Constructor
AuthenticDialogForm::AuthenticDialogForm(QWidget *parent/* =0 */)
:QDialog(parent)
{
	setupUi(this);
	connect(loginpushButton, SIGNAL(clicked()), this, SLOT(Request()));
	LoadUSers();
	SetEventFilter();
}

//Method for loaded operators name and show thay in combobox
void AuthenticDialogForm::LoadUSers(){

	model=new QSqlQueryModel(this);
	model->setQuery("SELECT operator_name FROM operators");
	namecomboBox->setModel(model);
	namecomboBox->setCurrentIndex(-1);
}

//Method for query pass and user names
void AuthenticDialogForm::Request(){
	if (!namecomboBox->currentText().isEmpty())
	{

		QSqlQuery query;
		query.prepare("SELECT operator_pass FROM operators WHERE operator_name=:username");
		query.bindValue(":username", namecomboBox->currentText());
		bool res=query.exec();
		if (!res) QMessageBox::warning(this, tr("Qtaxi"),query.lastError().text());
		query.first();
		if (query.value(0).toString()== passlineEdit->text())
		{
			emit changeusername(namecomboBox->currentText());
			accept();
		} else {
			QMessageBox::warning(this, tr("Qtaxi"), tr("Wrong password"));
			passlineEdit->clear();
		}
	}else {
		QMessageBox::warning(this, tr("Qtaxi"), tr("Select user name"));
	}
}


//Method for install event filter
void AuthenticDialogForm::SetEventFilter(){
	namecomboBox->installEventFilter(this);
	passlineEdit->installEventFilter(this);
	loginpushButton->installEventFilter(this);
}

//Event filter
bool AuthenticDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==namecomboBox || trg==passlineEdit)
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

	if (trg==loginpushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				Request();
				return true;
			}
		}
	}

	return QDialog::eventFilter(trg, evt);
}
