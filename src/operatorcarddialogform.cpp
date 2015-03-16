#include "operatorcarddialogform.h"

//Constructor for add operator
OperatorCardDialogForm::OperatorCardDialogForm(bool isEdit, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(SaveOperator()));
	connect(phototoolButton, SIGNAL(clicked()), this, SLOT(LoadPhoto()));
	curpasslineEdit->setEnabled(isEdit);
	this->isEdit=isEdit;
	psnlineEdit->setReadOnly(isEdit);
	SetStartSize();
	SetEventFilter();
	setWindowTitle(tr("Add new operator"));
	SetValidator();
	SetFormateDate();
}

//Constructor for edit operator
OperatorCardDialogForm::OperatorCardDialogForm(bool isEdit, const int &psn, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(SaveOperator()));
	connect(phototoolButton, SIGNAL(clicked()), this, SLOT(LoadPhoto()));
	curpasslineEdit->setEnabled(isEdit);
	this->isEdit=isEdit;
	this->psn=psn;
	psnlineEdit->setReadOnly(isEdit);
	SetStartSize();
	SetEventFilter();
	LoadOperatorInfo();
	setWindowTitle(tr("Edit operator"));
	SetFormateDate();
}

//Set date format for correct working event filter
void OperatorCardDialogForm::SetFormateDate(){
	borndateEdit->setDisplayFormat("dd.MM.yyyy");
}

//Set startup size
void OperatorCardDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-200, height-400);
	delete dwidget;
	delete rect;
}

//Save operator. If isEdit=TRUE switch to UpdateOperator(); else AddOperator();
void OperatorCardDialogForm::SaveOperator(){
	if(!namelineEdit->text().isEmpty() && !psnlineEdit->text().isEmpty() && !adresslineEdit->text().isEmpty()){
		if (isEdit)
		{
			UpdateOperator();
		} else {
			AddOperator();
		}
	}else{
		QMessageBox::information(this, tr("Qtaxi"), tr("Name, adress and personal number is empty!"));
	}
}

//Add new operator
void OperatorCardDialogForm::AddOperator(){
	QSqlQuery query;
	if (!photolabel->pixmap()==NULL)
	{

		const QPixmap  *pixmap=photolabel->pixmap();
		QBuffer buffer(&bytes);
		buffer.open(QIODevice::WriteOnly);
		pixmap->save(&buffer, "PNG");


	}else bytes=NULL;
	query.prepare("INSERT INTO operators(operator_name, psn, born_date, operator_phone, operator_phone_mobile, operator_adress, operator_pass, photo) VALUES(:op_name, :psn, :born_date, :op_phone, :op_mobile, :op_adress, :op_pass , :photo);");
	query.bindValue(":op_name", namelineEdit->text());
	query.bindValue(":psn", psnlineEdit->text());
	query.bindValue(":born_date", borndateEdit->date());
	query.bindValue(":op_phone", phonelineEdit->text());
	query.bindValue(":op_mobile", mobilelineEdit->text());
	query.bindValue(":op_adress", adresslineEdit->text());
	query.bindValue(":op_pass", newpasslineEdit->text());
	query.bindValue(":photo", bytes);
	if (!CheckPass()){
		QMessageBox::information(this, tr("Qtaxi"), tr("Password not correctly!"));
		return;
	}
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	accept();
}

bool OperatorCardDialogForm::CheckPass(){
	if(!newpasslineEdit->text().isEmpty() && !controlpasslineEdit->text().isEmpty()){

		if (newpasslineEdit->text()==controlpasslineEdit->text()){
			return true;
		} else return false;
	}else return false;
}


//Load and show photo
void OperatorCardDialogForm::LoadPhoto(){

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/", tr("Images (*.png *.xpm *.jpg)"));
	if (!fileName.isNull())
	{
		QPixmap pixmap;
		pixmap.load(fileName);
		photolabel->setPixmap(pixmap);
	}
}

//Install event filter
void OperatorCardDialogForm::SetEventFilter(){
	namelineEdit->installEventFilter(this);
	psnlineEdit->installEventFilter(this);
	borndateEdit->installEventFilter(this);
	phonelineEdit->installEventFilter(this);
	mobilelineEdit->installEventFilter(this);
	adresslineEdit->installEventFilter(this);
	newpasslineEdit->installEventFilter(this);
	curpasslineEdit->installEventFilter(this);
	controlpasslineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);

}

//Event filter
bool OperatorCardDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==namelineEdit  || trg==psnlineEdit || trg==phonelineEdit || trg==mobilelineEdit || trg==adresslineEdit || trg==newpasslineEdit || trg==curpasslineEdit || trg==controlpasslineEdit)
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

	if (trg==borndateEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( borndateEdit->currentSection()==QDateEdit::YearSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(borndateEdit->currentSection())
					{
					case QDateEdit::DaySection:
						{
							borndateEdit->setSelectedSection(QDateEdit::MonthSection);
							break;
						}
					case QDateEdit::MonthSection:{
						borndateEdit->setSelectedSection(QDateEdit::YearSection)	;
						break;
												 }
					default:
						break;
					}


				}
			}
		}
	}

	if (trg==addpushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				SaveOperator();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}

//Loaded operator info. Calling only in edit mode
void OperatorCardDialogForm::LoadOperatorInfo(){
	QSqlQuery query;
	query.prepare("SELECT operator_name, born_date, operator_phone, operator_phone_mobile, operator_adress, photo FROM operators WHERE psn=:psn ORDER BY 1;");
	query.bindValue(":psn", psn);
	bool ok=query.exec();
	if(!ok){
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	query.first();
	namelineEdit->setText(query.value(0).toString());
	borndateEdit->setDate(query.value(1).toDate());
	phonelineEdit->setText(query.value(2).toString());
	mobilelineEdit->setText(query.value(3).toString());
	adresslineEdit->setText(query.value(4).toString());
	psnlineEdit->setText(QString::number(psn));
	if(!query.value(5).toByteArray().isNull()){
		QPixmap pixmap;
		QByteArray loadimage;
		loadimage=query.value(5).toByteArray();
		pixmap.loadFromData(loadimage);
		photolabel->setPixmap(pixmap);
	}
}

//Update current record
void OperatorCardDialogForm::UpdateOperator(){
	if (!CheckPass())
	{
		QMessageBox::information(this, tr("Qtaxi"), tr("New password is empty!"));
		return;

	}

	QSqlQuery passquery, query;
	passquery.prepare("SELECT operator_pass FROM operators WHERE psn LIKE :psn;");
	passquery.bindValue(":psn", psn);
	bool passok=passquery.exec();
	if (!passok)
	{
		QMessageBox::information(this, tr("Qtaxi"), passquery.lastError().databaseText());
		return;
	}
	passquery.first();
	if(passquery.value(0).toString()==curpasslineEdit->text()){

		if (!photolabel->pixmap()==NULL)
		{

			const QPixmap  *pixmap=photolabel->pixmap();
			QBuffer buffer(&bytes);
			buffer.open(QIODevice::WriteOnly);
			pixmap->save(&buffer, "PNG");


		}else bytes=NULL;
		query.prepare("UPDATE operators SET operator_name=:op_name, born_date=:born_date, operator_phone=:op_phone, operator_phone_mobile=:op_phone_mobile, operator_adress=:op_adress, operator_pass=:op_pass, photo=:photo WHERE psn=:psn;");
		query.bindValue(":op_name", namelineEdit->text());
		query.bindValue(":born_date", borndateEdit->date());
		query.bindValue(":op_phone", phonelineEdit->text());
		query.bindValue(":op_phone_mobile", mobilelineEdit->text());
		query.bindValue(":op_adress", adresslineEdit->text());
		query.bindValue(":op_pass", newpasslineEdit->text());
		query.bindValue(":photo", bytes);
		query.bindValue(":psn", psn);
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}


	} else{
		QMessageBox::information(this, tr("Qtaxi"), tr("Current password is wrong!"));
		return;
	}


	accept();
}

//Set validator for input digit only between '0'-'9999'
void OperatorCardDialogForm::SetValidator(){
	QRegExp rx("-?\\d{1,4}");
	QValidator *validator = new QRegExpValidator(rx, this);
	psnlineEdit->setValidator(validator);
}
