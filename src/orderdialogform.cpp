#include "orderdialogform.h"
#include "drivershiftdialogform.h"
//Constructor for add new order: isEdit=FALSE;
OrderDialogForm::OrderDialogForm(const QString &usname, QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	this->usname=usname;
	isEdit=FALSE;
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveOrder()));
	connect(parkingcomboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(LoadCost(const QString &)));
	connect(drivercomboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(LoadCar(const QString &)));
	connect(blacklisttoolButton, SIGNAL(clicked()), this, SLOT(RemoveToBlack()));

	setWindowTitle(tr("Add new order"));
	SetStartSize();
	SetEventFilter();
	SetValidator();
	LoadTypes();
	LoadDrivers();
	orderdateTimeEdit->setDateTime(QDateTime::currentDateTime());
	Status->setEnabled(false);
}

//Constructor for edit order: isEdit=TRUE;
OrderDialogForm::OrderDialogForm(const QString &usname, int pkey, QWidget *parent):QDialog(parent),pkey(pkey){
	setupUi(this);
	this->usname=usname;
	isEdit=TRUE;
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveOrder()));
	connect(parkingcomboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(LoadCost(const QString &)));
	connect(drivercomboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(LoadCar(const QString &)));
	connect(closepushButton, SIGNAL(clicked()), this, SLOT(CloseOrder()));
	connect(waitcheckBox, SIGNAL(clicked()), this, SLOT(SetWaitTime()));
	connect(gocheckBox, SIGNAL(clicked()), this, SLOT(SetGoTime()));
	connect(freecheckBox, SIGNAL(clicked()), this, SLOT(SetFreeTime()));
	//connect(spinBox, SIGNAL(editingFinished()), this, SLOT(SetRealCost()));
	setWindowTitle(tr("Edit order number: ") + QString::number(pkey));
	SetStartSize();
	SetEventFilter();
	LoadTypes();
	LoadDrivers();
	SetParametrsOfOrder();
	tabWidget->setCurrentIndex(1);
}

//Set startup size
void OrderDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;
}

//Install event filter
void OrderDialogForm::SetEventFilter(){
	clientlineEdit->installEventFilter(this);
	phonelineEdit->installEventFilter(this);
	orderdateTimeEdit->installEventFilter(this);
	addresslineEdit->installEventFilter(this);
	parkingcomboBox->installEventFilter(this);
	deferredcheckBox->installEventFilter(this);
	gotolineEdit->installEventFilter(this);
	waitdateTimeEdit->installEventFilter(this);
	godateTimeEdit->installEventFilter(this);
	freedateTimeEdit->installEventFilter(this);
	waitcheckBox->installEventFilter(this);
	gocheckBox->installEventFilter(this);
	freecheckBox->installEventFilter(this);
	spinBox->installEventFilter(this);
	drivercomboBox->installEventFilter(this);
	cachecheckBox->installEventFilter(this);
	savepushButton->installEventFilter(this);
	costlineEdit->installEventFilter(this);
}

//Method to switch save or edit order 
void OrderDialogForm::SaveOrder(){
	if (phonelineEdit->text().isEmpty() || addresslineEdit->text().isEmpty() || gotolineEdit->text().isEmpty() || parkingcomboBox->currentText().isEmpty())
	{
		QMessageBox::information(this, tr("Qtaxi"), tr("Required fields are empty!"));
		return;
	}else{
		if (isEdit)
		{
			//If isEdit=TRUE call UpdateOrder();
			UpdateOrder();

		}else{
			//If isEdit=FALSE
			QSqlQuery query;
			query.prepare("INSERT INTO orders(phone, address, address_finished, time_order, operator_open, typecarting, isaccepted, isdeferred, client) VALUES(:ph, :adr, :adrf, :to, :opo, :tpc, :ia, :id, :client);");
			query.bindValue(":ph", phonelineEdit->text());
			query.bindValue(":adr", addresslineEdit->text());
			query.bindValue(":adrf", gotolineEdit->text());
			query.bindValue(":to", orderdateTimeEdit->dateTime());

			//need query for select driver from base

			QSqlQuery opoquery;
			bool opook=opoquery.exec("SELECT key FROM operators WHERE operator_name LIKE '"+usname+"';");
			if (!opook)
			{
				QMessageBox::information(this, tr("Qtaxi"), opoquery.lastError().databaseText());
				return;
			}
			opoquery.first();
			query.bindValue(":opo", opoquery.value(0).toInt());


			QSqlQuery tpcquery;
			bool tpcok=tpcquery.exec("SELECT key FROM carting WHERE carting_name LIKE '"+parkingcomboBox->currentText()+"';");
			if (!tpcok)
			{
				QMessageBox::information(this, tr("Qtaxi"), tpcquery.lastError().databaseText());
				return;
			}
			tpcquery.first();
			query.bindValue(":tpc", tpcquery.value(0).toInt());
			bool checked=deferredcheckBox->isChecked();
			query.bindValue(":ia", !checked);
			query.bindValue(":id", checked);


			if (!clientlineEdit->text().isEmpty())
			{

				QSqlQuery clientquery;
				clientquery.prepare("SELECT key FROM clients WHERE client_num=:clnum;");
				clientquery.bindValue(":clnum", clientlineEdit->text());
				bool clientcok=clientquery.exec();
				if (!clientcok)
				{
					QMessageBox::information(this, tr("Qtaxi"), clientquery.lastError().databaseText());
					return;
				}
				clientquery.first();
				query.bindValue(":client", clientquery.value(0).toInt());
			} else 	
			{
				query.bindValue(":client", -1);
			}

			bool ok=query.exec();
			if (!ok)
			{
				QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
				return;
			}


			accept();

		}
	}
}

//Method for update old order
void OrderDialogForm::UpdateOrder(){
	if (!drivercomboBox->currentText().isEmpty())
	{

		QSqlQuery query, driverquery, waitquery, goquery, freequery, lockdrvquery;

		bool drvok=driverquery.exec("SELECT key FROM drivers WHERE nickname LIKE '"+drivercomboBox->currentText()+"';");
		if (!drvok)
		{
			QMessageBox::information(this, tr("Qtaxi"), driverquery.lastError().databaseText());
			return;
		}
		driverquery.first();

		query.prepare("UPDATE orders SET driver=:drv, isaccepted=TRUE, isdeferred=FALSE WHERE key=:pkey;");
		query.bindValue(":pkey", pkey);
		query.bindValue(":drv", driverquery.value(0).toInt());
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}

		if (waitcheckBox->isChecked())
		{
			waitquery.prepare("UPDATE orders SET time_driver_wait=:tw WHERE key=:pkey;");
			waitquery.bindValue(":tw", waitdateTimeEdit->dateTime());
			waitquery.bindValue(":pkey", pkey);
			waitquery.exec();
		} 

		if (gocheckBox->isChecked())
		{
			goquery.prepare("UPDATE orders SET  time_driver_go=:tg WHERE key=:pkey;");
			goquery.bindValue(":tg", godateTimeEdit->dateTime());
			goquery.bindValue(":pkey", pkey);
			goquery.exec();

		} 

		if (freecheckBox->isChecked())
		{
			freequery.prepare("UPDATE orders SET time_driver_free=:tf WHERE key=:pkey;");
			freequery.bindValue(":tf", freedateTimeEdit->dateTime());
			freequery.bindValue(":pkey", pkey);
			freequery.exec();
		}

		if (drivercomboBox->isEnabled())
		{
			lockdrvquery.prepare("UPDATE orders SET time_driver_lock=:tdl WHERE key=:pkey; " );
			lockdrvquery.bindValue(":tdl", QDateTime::currentDateTime());
			lockdrvquery.bindValue(":pkey", pkey);
			lockdrvquery.exec();

			lockdrvquery.prepare("UPDATE drivers SET jobstatus=TRUE WHERE nickname=:fn;");
			lockdrvquery.bindValue(":fn", drivercomboBox->currentText());
			lockdrvquery.exec();

			QSqlQuery findparking;
			findparking.prepare("SELECT parking FROM drivers WHERE nickname=:fn;");
			findparking.bindValue(":fn", drivercomboBox->currentText());
			findparking.exec();
			findparking.first();

			lockdrvquery.prepare("UPDATE drivers SET parking_num=NULL, parking=NULL WHERE nickname=:fn;");
			lockdrvquery.bindValue(":fn", drivercomboBox->currentText());
			lockdrvquery.exec();

			lockdrvquery.prepare("UPDATE drivers SET parking_num=parking_num - 1 WHERE parking=:parking AND parking_num > 1;");
			lockdrvquery.bindValue(":parking", findparking.value(0).toString());
			lockdrvquery.exec();


		}
		int yesToclose=QMessageBox::information(this, tr("Qtaxi"), tr("Close dialog?"), QMessageBox::Yes | QMessageBox::No);
		if (yesToclose==QMessageBox::Yes)
		{
			accept();
		}

	} else{
		QMessageBox::information(this, tr("Qtaxi"), tr("Select a driver!"));
	}
}


//Load types of carting
void OrderDialogForm::LoadTypes(){
	typesmodel=new QSqlQueryModel(this);
	typesmodel->setQuery("SELECT carting_name FROM carting");
	if (typesmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), typesmodel->lastError().databaseText());
		return;
	}
	parkingcomboBox->setModel(typesmodel);
	parkingcomboBox->setCurrentIndex(-1);
}


//Load cost of current carting
void OrderDialogForm::LoadCost(const QString &text){
	QSqlQuery query;
	//query.prepare()
	bool ok=query.exec("SELECT cost_level FROM costs WHERE carting_id=(SELECT key FROM carting WHERE carting_name LIKE '"+text+"');");
	if (!ok)
	{
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	query.first();
	costlineEdit->setText(query.value(0).toString());
}

//Load drivers with online=TRUE option.
void OrderDialogForm::LoadDrivers(){
	driversmodel=new QSqlQueryModel(this);
	driversmodel->setQuery("SELECT nickname FROM drivers WHERE online=TRUE");
	if (driversmodel->lastError().isValid())
	{
		QMessageBox::information(this, tr("Qtaxi"), driversmodel->lastError().databaseText());
		return;
	}
	drivercomboBox->setModel(driversmodel);
	drivercomboBox->setCurrentIndex(-1);
}

//Load car for current driver
void OrderDialogForm::LoadCar(const QString &text){
	QSqlQuery query;
	bool ok=query.exec("SELECT car_name_num FROM cars WHERE key=(SELECT car FROM drivers WHERE nickname LIKE '"+text+"');");
	if (!ok)
	{
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	query.first();
	carlineEdit->setText(query.value(0).toString());
}

//Set parametrs of order. Calling only in edit mode.
void OrderDialogForm::SetParametrsOfOrder(){
	QSqlQuery phoneAddressOrderTimequery, typesquery, driverquery, timesquery, clientquery;

	phoneAddressOrderTimequery.prepare("SELECT phone, address, time_order, address_finished, isdeferred FROM orders WHERE key=:pkey;");
	phoneAddressOrderTimequery.bindValue(":pkey", pkey);
	bool phoneok=phoneAddressOrderTimequery.exec();
	if (!phoneok)
	{
		QMessageBox::information(this, tr("Qtaxi"), phoneAddressOrderTimequery.lastError().databaseText());
		return;
	}
	phoneAddressOrderTimequery.first();
	phonelineEdit->setText(phoneAddressOrderTimequery.value(0).toString());
	addresslineEdit->setText(phoneAddressOrderTimequery.value(1).toString());
	orderdateTimeEdit->setDateTime(phoneAddressOrderTimequery.value(2).toDateTime());
	gotolineEdit->setText(phoneAddressOrderTimequery.value(3).toString());
	deferredcheckBox->setChecked(phoneAddressOrderTimequery.value(4).toBool());

	typesquery.prepare("SELECT carting_name FROM carting WHERE key=(SELECT typecarting FROM orders WHERE key=:pkey);");
	typesquery.bindValue(":pkey", pkey);
	bool typesok=typesquery.exec();
	if (!typesok)
	{
		QMessageBox::information(this, tr("Qtaxi"), typesquery.lastError().databaseText());
		return;
	}
	typesquery.first();
	parkingcomboBox->setCurrentIndex(parkingcomboBox->findText(typesquery.value(0).toString()));

	driverquery.prepare("SELECT nickname FROM drivers WHERE key=(SELECT driver FROM orders WHERE key=:pkey);");
	driverquery.bindValue(":pkey", pkey);
	bool driversok=driverquery.exec();
	if (!driversok)
	{
		QMessageBox::information(this, tr("Qtaxi"), driverquery.lastError().databaseText());
		return;
	}
	driverquery.first();
	drivercomboBox->setCurrentIndex(drivercomboBox->findText(driverquery.value(0).toString()));
	if (drivercomboBox->currentIndex()!=-1) drivercomboBox->setEnabled(FALSE);

	timesquery.prepare("SELECT  time_driver_wait, time_driver_go, time_driver_free FROM orders WHERE key=:pkey");
	timesquery.bindValue(":pkey", pkey);
	bool timesqueryok=timesquery.exec();
	if (!timesqueryok)
	{
		QMessageBox::information(this, tr("Qtaxi"), timesquery.lastError().databaseText());
		return;
	}
	timesquery.first();
	if (!timesquery.value(0).isNull())
	{
		disconnect(waitcheckBox, SIGNAL(clicked(bool)), waitdateTimeEdit, SLOT(setEnabled(bool)));
		waitcheckBox->setCheckState(Qt::Checked);
		waitdateTimeEdit->setEnabled(FALSE);
		waitdateTimeEdit->setDateTime(timesquery.value(0).toDateTime());
	}

	if (!timesquery.value(1).isNull())
	{
		disconnect(gocheckBox, SIGNAL(clicked(bool)), godateTimeEdit, SLOT(setEnabled(bool)));
		gocheckBox->setCheckState(Qt::Checked);
		godateTimeEdit->setEnabled(FALSE);
		godateTimeEdit->setDateTime(timesquery.value(1).toDateTime());

	}
	if (!timesquery.value(2).isNull())
	{
		disconnect(freecheckBox, SIGNAL(clicked(bool)), freedateTimeEdit, SLOT(setEnabled(bool)));
		freecheckBox->setCheckState(Qt::Checked);
		freedateTimeEdit->setEnabled(FALSE);
		freedateTimeEdit->setDateTime(timesquery.value(2).toDateTime());
	}

	clientquery.prepare("SELECT client_num FROM clients WHERE key=(SELECT client FROM orders WHERE key=:pkey);");
	clientquery.bindValue(":pkey", pkey);
	bool clientqueryok=clientquery.exec();
	if (!clientqueryok)
	{
		QMessageBox::information(this, tr("Qtaxi"), clientquery.lastError().databaseText());
		return;
	}
	clientquery.first();
	if (!clientquery.value(0).isNull())
	{
		clientlineEdit->setText(clientquery.value(0).toString());
	}
}

//Close current order and update driver status.
void OrderDialogForm::CloseOrder(){
	if (spinBox->value()!=0 && drivercomboBox->currentIndex()!=-1 && waitcheckBox->isChecked() && gocheckBox->isChecked() && freecheckBox->isChecked())
	{
		QSqlQuery query,lockdrvquery, opcquery;

		bool opcok=opcquery.exec("SELECT key FROM operators WHERE operator_name LIKE '"+usname+"';");
		if (!opcok)
		{
			QMessageBox::information(this, tr("Qtaxi"), opcquery.lastError().databaseText());
			return;
		}
		opcquery.first();

		query.prepare("UPDATE orders SET operator_close=:opclose, cost_real=:cr, iscache=:iscache, isaccepted=FALSE, isdeferred=FALSE, isclosed=TRUE, time_order_close=:toc WHERE key=:pkey;");
		query.bindValue(":opclose", opcquery.value(0).toInt());
		query.bindValue(":cr", spinBox->value());
		query.bindValue(":toc", QDateTime::currentDateTime());
		query.bindValue(":iscache", cachecheckBox->isChecked());
		query.bindValue(":pkey", pkey);
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}

		lockdrvquery.prepare("UPDATE drivers SET jobstatus=FALSE WHERE nickname=:fn;");
		lockdrvquery.bindValue(":fn", drivercomboBox->currentText());
		bool lockdrvok=lockdrvquery.exec();
		if (!lockdrvok)
		{
			QMessageBox::information(this, tr("Qtaxi"), lockdrvquery.lastError().databaseText());
			return;
		}
		QMessageBox::information(this, tr("Qtaxi"), tr("closing order complete!"));
		DriverShiftDialogForm *dlg=new DriverShiftDialogForm(drivercomboBox->currentText(), true, this);
		if (dlg->exec())
		{
			accept();
		}

	}else {
		QMessageBox::information(this, tr("Qtaxi"), tr("Type real cost order, select a driver, type time's of driver steps!"));
		return;
	}
}

//If current phone number is a bad, move it into the blacklist
void OrderDialogForm::RemoveToBlack(){
	if (!phonelineEdit->text().isEmpty())
	{
		bool okstr;
		QString text = QInputDialog::getText(this, tr("Qtaxi"),tr("Input description"), QLineEdit::Normal, "Spam",	 &okstr);
		if (okstr && !text.isEmpty()){
			QSqlQuery query;
			query.prepare("INSERT INTO blacklist(phone, start_date, description) VALUES(:phone, :date, :desc);");
			query.bindValue(":phone", phonelineEdit->text());
			query.bindValue(":date", QDateTime::currentDateTime());
			query.bindValue(":desc", text);
			bool ok=query.exec();
			if (!ok)
			{
				QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
				return;
			}
			phonelineEdit->clear();
			clientlineEdit->clear();
		}
	}
}

//Set time, when driver is start wait
void OrderDialogForm::SetWaitTime(){
	waitdateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

//Set time, when driver is go with client
void OrderDialogForm::SetGoTime(){
	godateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

//Set time, when driver is free
void OrderDialogForm::SetFreeTime(){
	freedateTimeEdit->setDateTime(QDateTime::currentDateTime());
}

//Load client information
void OrderDialogForm::LoadClientInfo(){
	if (!clientlineEdit->text().isEmpty())
	{

		QSqlQuery clientquery;
		clientquery.prepare("SELECT client_phone FROM clients WHERE client_num=:clnum;");
		clientquery.bindValue(":clnum", clientlineEdit->text());
		bool clientqueryok=clientquery.exec();
		if (!clientqueryok)
		{
			QMessageBox::information(this, tr("Qtaxi"), clientquery.lastError().databaseText());
			return;
		}
		clientquery.first();
		if (!clientquery.value(0).isNull())
		{
			phonelineEdit->setText(clientquery.value(0).toString());
		}

	}
}

//Set validator for correct input client number
void OrderDialogForm::SetValidator(){
	QRegExp rx("-?\\d{1,4}");
	QValidator *validator = new QRegExpValidator(rx, this);
	clientlineEdit->setValidator(validator);
}

//Set real cost for client
void OrderDialogForm::SetRealCost(){
	QSqlQuery query, discountquery;
	query.prepare("SELECT client FROM orders WHERE key=:pkey;");
	query.bindValue(":pkey", pkey);
	bool ok=query.exec();
	if (!ok)
	{
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	query.first();
	if (!query.value(0).isNull())
	{
		discountquery.prepare("SELECT client_discount FROM clients WHERE key=:clnum;");
		discountquery.bindValue(":clnum", query.value(0).toInt());
		bool disok=discountquery.exec();
		if (!disok)
		{
			QMessageBox::information(this, tr("Qtaxi"), discountquery.lastError().databaseText());
			return;
		}
		discountquery.first();
		if (!discountquery.value(0).isNull())
		{

			int discount=discountquery.value(0).toInt();
			int realcost=spinBox->value() - spinBox->value()*discount/100;
			spinBox->setValue(realcost);
		}
	}
}

//Event filter
bool OrderDialogForm::eventFilter(QObject *trg, QEvent *evt){

	if (trg==addresslineEdit || trg==gotolineEdit || trg==parkingcomboBox || trg==deferredcheckBox || trg==gotolineEdit || trg==waitcheckBox || trg==gocheckBox || trg==freecheckBox || trg==drivercomboBox || trg==cachecheckBox || trg==costlineEdit)
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
	if (trg==spinBox)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				SetRealCost();
				focusNextChild();
				return true;
			}
		}

	}

	if (trg==clientlineEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				LoadClientInfo();
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
				SaveOrder();
				return true;
			}
		}

	}


	if (trg==phonelineEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				QSqlQuery query;
				query.prepare("SELECT phone FROM blacklist WHERE phone LIKE :phone;");
				query.bindValue(":phone", phonelineEdit->text());
				query.exec();
				query.first();
				if (!query.value(0).isNull())
				{
					QMessageBox::information(this, tr("Qtaxi"),  tr("Phone blocked, see blacklist!!!"));
					phonelineEdit->clear();
					clientlineEdit->clear();
					focusPreviousChild();
				}else{

					focusNextChild();
					return true;
				}


			}
		}
	}

	if (trg==orderdateTimeEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( orderdateTimeEdit->currentSection()==QDateTimeEdit::MinuteSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(orderdateTimeEdit->currentSection())
					{
					case QDateTimeEdit::DaySection:
						{
							orderdateTimeEdit->setSelectedSection(QDateTimeEdit::MonthSection);
							break;
						}
					case QDateTimeEdit::MonthSection:{
						orderdateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection)	;
						break;
													 }
					case QDateTimeEdit::YearSection:{
						orderdateTimeEdit->setSelectedSection(QDateTimeEdit::HourSection)	;
						break;
													}
					case QDateTimeEdit::HourSection:{
						orderdateTimeEdit->setSelectedSection(QDateTimeEdit::MinuteSection)	;
						break;
													}
					default:
						break;
					}
				}
			}
		}
	}

	if (trg==waitdateTimeEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( waitdateTimeEdit->currentSection()==QDateTimeEdit::MinuteSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(waitdateTimeEdit->currentSection())
					{
					case QDateTimeEdit::DaySection:
						{
							waitdateTimeEdit->setSelectedSection(QDateTimeEdit::MonthSection);
							break;
						}
					case QDateTimeEdit::MonthSection:{
						waitdateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection)	;
						break;
													 }
					case QDateTimeEdit::YearSection:{
						waitdateTimeEdit->setSelectedSection(QDateTimeEdit::HourSection)	;
						break;
													}
					case QDateTimeEdit::HourSection:{
						waitdateTimeEdit->setSelectedSection(QDateTimeEdit::MinuteSection)	;
						break;
													}
					default:
						break;
					}
				}
			}
		}
	}


	if (trg==godateTimeEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( godateTimeEdit->currentSection()==QDateTimeEdit::MinuteSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(godateTimeEdit->currentSection())
					{
					case QDateTimeEdit::DaySection:
						{
							godateTimeEdit->setSelectedSection(QDateTimeEdit::MonthSection);
							break;
						}
					case QDateTimeEdit::MonthSection:{
						godateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection)	;
						break;
													 }
					case QDateTimeEdit::YearSection:{
						godateTimeEdit->setSelectedSection(QDateTimeEdit::HourSection)	;
						break;
													}
					case QDateTimeEdit::HourSection:{
						godateTimeEdit->setSelectedSection(QDateTimeEdit::MinuteSection)	;
						break;
													}
					default:
						break;
					}
				}
			}
		}
	}

	if (trg==freedateTimeEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( freedateTimeEdit->currentSection()==QDateTimeEdit::MinuteSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(freedateTimeEdit->currentSection())
					{
					case QDateTimeEdit::DaySection:
						{
							freedateTimeEdit->setSelectedSection(QDateTimeEdit::MonthSection);
							break;
						}
					case QDateTimeEdit::MonthSection:{
						freedateTimeEdit->setSelectedSection(QDateTimeEdit::YearSection)	;
						break;
													 }
					case QDateTimeEdit::YearSection:{
						freedateTimeEdit->setSelectedSection(QDateTimeEdit::HourSection)	;
						break;
													}
					case QDateTimeEdit::HourSection:{
						freedateTimeEdit->setSelectedSection(QDateTimeEdit::MinuteSection)	;
						break;
													}
					default:
						break;
					}
				}
			}
		}
	}

	return QDialog::eventFilter(trg, evt);
}

