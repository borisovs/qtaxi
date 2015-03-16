#include "drivershiftdialogform.h"
//Constructor
DriverShiftDialogForm::DriverShiftDialogForm(const QString &nick, bool isEdit, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(okpushButton, SIGNAL(clicked()), this, SLOT(UpdateStatus()));
	connect(closepushButton, SIGNAL(clicked()), this, SLOT(CloseShift()));
	this->isEdit=isEdit;
	this->nick=nick;
	SetStartSize();
	LoadParking();
	SetParam();
	SetEventFilter();
	driverlineEdit->setText(this->nick);

}

//Load parking from database
void DriverShiftDialogForm::LoadParking(){
	parkingmodel=new QSqlQueryModel(this);
	parkingmodel->setQuery("SELECT parking_name FROM parkings ORDER BY 1;");
	if(parkingmodel->lastError().isValid()) {
		QMessageBox::warning(this, tr("Qtaxi"), parkingmodel->lastError().databaseText());
		return;
	}
	parkingscomboBox->setModel(parkingmodel);

}

//Set start size
void DriverShiftDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-800);
	delete dwidget;
	delete rect;
}

//Set parameters for current record
void DriverShiftDialogForm::SetParam(){
	if (!isEdit)
	{
		parkingscomboBox->setCurrentIndex(-1);
		closepushButton->setEnabled(isEdit);

	}else{
		QSqlQuery parkingquery;
		bool parkingok=parkingquery.exec("SELECT parking FROM drivers WHERE nickname LIKE '"+nick+"';");
		if (!parkingok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), parkingquery.lastError().databaseText());
			return;

		}
		parkingquery.first();
		parkingscomboBox->setCurrentIndex(parkingscomboBox->findText(parkingquery.value(0).toString()));
		oldparking=parkingscomboBox->currentText();


	}
}

//UPdate table and show parameters on edit record
void DriverShiftDialogForm::UpdateStatus(){
	if (parkingscomboBox->currentIndex()!=-1)
	{

		if (isEdit)
		{
			QSqlQuery query, downparkingquery;
			query.prepare("UPDATE drivers SET parking=:parking, parking_num=(SELECT COUNT(*) FROM drivers WHERE parking LIKE :parking)+1 WHERE online=TRUE AND nickname=:nickname;");
			query.bindValue(":parking", parkingscomboBox->currentText());
			query.bindValue(":nickname", nick);
			bool ok=query.exec();
			if (!ok)
			{
				QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
				return;

			}

			downparkingquery.prepare("UPDATE drivers SET parking_num=parking_num - 1 WHERE parking=:parking AND parking_num > 1;");
			downparkingquery.bindValue(":parking", oldparking);
			//downparkingquery.bindValue(":nickname", nick);
			bool downok=downparkingquery.exec();
			if (!downok)
			{
				QMessageBox::warning(this, tr("Qtaxi"), downparkingquery.lastError().databaseText());
				return;

			}

			accept();

		}else{
			QSqlQuery query, nickquery, driverquery;
			nickquery.prepare("SELECT key FROM drivers WHERE nickname=:nickname");
			nickquery.bindValue(":nickname", nick);
			bool nickok=nickquery.exec();
			if (!nickok)
			{
				QMessageBox::warning(this, tr("Qtaxi"), nickquery.lastError().databaseText());
				return;

			}
			nickquery.first();

			query.prepare("INSERT INTO driversshift(driverid, startdate, starttime) VALUES(:nickname, :startdate, :starttime);");
			query.bindValue(":nickname", nickquery.value(0).toInt());
			query.bindValue(":startdate", QDate::currentDate());
			query.bindValue(":starttime", QTime::currentTime());
			bool ok=query.exec();
			if (!ok)
			{
				QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
				return;

			}
			driverquery.prepare("UPDATE drivers SET online=TRUE, parking=:parking, parking_num=(SELECT COUNT(*) FROM drivers WHERE parking LIKE :parking)+1 WHERE nickname=:nickname;");
			driverquery.bindValue(":parking", parkingscomboBox->currentText());
			driverquery.bindValue(":nickname", nick);
			bool driverok=driverquery.exec();
			if (!driverok)
			{
				QMessageBox::warning(this, tr("Qtaxi"), driverquery.lastError().databaseText());
				return;

			}

			accept();
		}
	}else{
		QMessageBox::warning(this, tr("Qtaxi"), tr("Change parking"));
	}
}


//Close driver shift.
void DriverShiftDialogForm::CloseShift(){
	QString message=tr("Are you realy want to close job shift?");
	int r=QMessageBox::warning(this, tr("Qtaxi"), message, QMessageBox::Yes | QMessageBox::No);

	if (r==QMessageBox::Yes)
	{

		QSqlQuery query, driverupdatequery, driveronlineupdatecountquery;
		query.prepare("UPDATE driversshift SET enddate=:enddate, endtime=:endtime, closed=TRUE WHERE closed=FALSE AND driverid=(SELECT key FROM drivers WHERE nickname=:nickname);");
		query.bindValue(":enddate", QDate::currentDate());
		query.bindValue(":endtime", QTime::currentTime());
		query.bindValue(":nickname", nick);
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
			return;

		}

		driveronlineupdatecountquery.prepare("UPDATE drivers SET parking_num=(parking_num - 1) WHERE parking=:parking  AND parking_num > 1;");
		driveronlineupdatecountquery.bindValue(":parking", parkingscomboBox->currentText());
		bool driveronlineok=driveronlineupdatecountquery.exec();
		if (!driveronlineok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), driveronlineupdatecountquery.lastError().databaseText());
			return;

		}

		driverupdatequery.prepare("UPDATE drivers SET online=FALSE, parking=NULL, parking_num=NULL WHERE online=TRUE AND nickname=:nickname;");
		driverupdatequery.bindValue(":nickname", nick);
		bool driverupdateok=driverupdatequery.exec();
		if (!driverupdateok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), driverupdatequery.lastError().databaseText());
			return;

		}



		accept();
	}
}

//Install event filter
void DriverShiftDialogForm::SetEventFilter(){
	driverlineEdit->installEventFilter(this);
	parkingscomboBox->installEventFilter(this);
	okpushButton->installEventFilter(this);
	closepushButton->installEventFilter(this);
}

//Event filter
bool DriverShiftDialogForm::eventFilter(QObject *trg, QEvent *evt){

	if (trg==driverlineEdit || trg==parkingscomboBox)
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

	if (trg==okpushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				UpdateStatus();
				return true;
			}
		}

	}
	if (trg==closepushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				CloseShift();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}
