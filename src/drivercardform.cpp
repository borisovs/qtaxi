#include "drivercardform.h"
#include "citiesdialogform.h"
#include "streetsdialogform.h"
#include "carsdialogform.h"
//Constructor for add new
DriverCardForm::DriverCardForm(bool isEdit, QWidget *parent):QDialog(parent){
	setupUi(this);
	this->isEdit=isEdit;
	this->setWindowTitle(tr("Add new driver:"));
	connect(addcitytoolButton, SIGNAL(clicked()), this, SLOT(AddCity()));
	connect(addstreettoolButton, SIGNAL(clicked()), this, SLOT(AddStreet()));
	connect(citycomboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(LoadStreets(QString)));
	connect(addautotoolButton, SIGNAL(clicked()), this, SLOT(AddCar()));
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveDriver()));
	connect(addphototoolButton, SIGNAL(clicked()), this, SLOT(LoadPhoto()));
	LoadCities();
	LoadCars();
	citycomboBox->setCurrentIndex(-1);
	autocomboBox->setCurrentIndex(-1);
	SetEventFilter();
	SetValidator();
}
//Constructor for edit old
DriverCardForm::DriverCardForm(bool isEdit, int psn, QWidget *parent):QDialog(parent){
	setupUi(this);
	this->isEdit=isEdit;
	this->psn=psn;
	this->setWindowTitle(tr("Edit driver:"));
	connect(addcitytoolButton, SIGNAL(clicked()), this, SLOT(AddCity()));
	connect(addstreettoolButton, SIGNAL(clicked()), this, SLOT(AddStreet()));
	connect(citycomboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(LoadStreets(QString)));
	connect(addautotoolButton, SIGNAL(clicked()), this, SLOT(AddCar()));
	connect(savepushButton, SIGNAL(clicked()), this, SLOT(SaveDriver()));
	connect(addphototoolButton, SIGNAL(clicked()), this, SLOT(LoadPhoto()));
	LoadCities();
	LoadCars();
	LoadDriverInfo();
	SetEventFilter();
	SetFormateDate();

}

//Set date format for correct working event filter
void DriverCardForm::SetFormateDate(){
	borndateEdit->setDisplayFormat("dd.MM.yyyy");
	passdateEdit->setDisplayFormat("dd.MM.yyyy");
}

//Load cities
void DriverCardForm::LoadCities(){
	citymodel=new QSqlQueryModel(this);
	citymodel->setQuery("SELECT city_name FROM cities ORDER BY 1;");
	citycomboBox->setModel(citymodel);

}

//Load streets
void DriverCardForm::LoadStreets(QString temp){
	currentcity=temp;
	if (streetcomboBox->currentIndex()!=-1){
		currentstreet=streetcomboBox->currentText();	
	} else{
		currentstreet="";
	}
	streetmodel=new QSqlQueryModel(this);
	streetmodel->setQuery("SELECT street_name FROM streets WHERE city_id=(SELECT key FROM cities WHERE city_name = '"+currentcity+"') ORDER BY 1;");
	streetcomboBox->setModel(streetmodel);
	if(currentstreet!=""){
		streetcomboBox->setCurrentIndex(streetcomboBox->findText(currentstreet));
	}else {
		streetcomboBox->setCurrentIndex(-1);
	}	
}

//Add new city
void DriverCardForm::AddCity(){
	currentcity=citycomboBox->currentText();
	CitiesDialogForm *dlg=new CitiesDialogForm(this);
	dlg->exec();
	delete dlg;
	LoadCities();
	citycomboBox->setCurrentIndex(citycomboBox->findText(currentcity));

}

//Add new street
void DriverCardForm::AddStreet(){
	QString currentstreet=streetcomboBox->currentText();
	StreetsDialogForm *dlg=new StreetsDialogForm(this);
	dlg->exec();
	delete dlg;
	LoadStreets(currentcity);
	streetcomboBox->setCurrentIndex(streetcomboBox->findText(currentstreet));

}

//Add  new car
void DriverCardForm::AddCar(){
	if (autocomboBox->currentIndex()!=-1){
		currentcar=autocomboBox->currentText();
	} else{
		currentcar="";
	}

	CarsDialogForm *dlg=new CarsDialogForm(this);
	dlg->exec();
	delete dlg;
	LoadCars();
	if(currentcar!=""){
		autocomboBox->setCurrentIndex(autocomboBox->findText(currentcar));
	}else {
		autocomboBox->setCurrentIndex(-1);
	}	

}

//Load cars
void DriverCardForm::LoadCars(){
	carmodel=new QSqlQueryModel(this);
	carmodel->setQuery("SELECT car_name_num FROM cars ORDER BY 1;");
	autocomboBox->setModel(carmodel);

}


//Set startup size
void DriverCardForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-150, height-150);
	delete dwidget;
	delete rect;
}

//Load driver info and set it in fields
void DriverCardForm::LoadDriverInfo(){
	QSqlQuery query;
	query.prepare("SELECT drivers.fullname, drivers.psn, drivers.born_date, cities.city_name, streets.street_name, drivers.home, cars.car_name_num, drivers.nickname, drivers.phone_home, drivers.phone_mob, drivers.passport_ser, drivers.passport_num, drivers.passport_date, drivers.passport_get, drivers.photo FROM drivers, cities, streets, cars WHERE drivers.city=cities.key AND drivers.street=streets.key AND drivers.car=cars.key AND drivers.psn=:psn ORDER BY 1;");
	query.bindValue(":psn", psn);
	bool ok=query.exec();
	if(!ok){
		QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		return;
	}
	pnumlineEdit->setReadOnly(true);
	query.first();
	fullnamelineEdit->setText(query.value(0).toString());
	pnumlineEdit->setText(query.value(1).toString());
	borndateEdit->setDate(query.value(2).toDate());
	nicklineEdit->setText(query.value(7).toString());
	homelineEdit->setText(query.value(5).toString());
	phonelineEdit->setText(query.value(8).toString());
	moblineEdit->setText(query.value(9).toString());
	passerlineEdit->setText(query.value(10).toString());
	passnumlineEdit->setText(query.value(11).toString());
	passdateEdit->setDate(query.value(12).toDate());
	getoutlineEdit->setText(query.value(13).toString());

	citycomboBox->setCurrentIndex(citycomboBox->findText(query.value(3).toString()));
	LoadStreets(query.value(3).toString());
	streetcomboBox->setCurrentIndex(streetcomboBox->findText(query.value(4).toString()));
	autocomboBox->setCurrentIndex(autocomboBox->findText(query.value(6).toString()));
	if(!query.value(14).toByteArray().isNull()){
		QPixmap pixmap;
		QByteArray loadimage;
		loadimage=query.value(14).toByteArray();
		pixmap.loadFromData(loadimage);
		photolabel->setPixmap(pixmap);
	}

}

//Save driver
void DriverCardForm::SaveDriver(){
	if(isEdit){
		//If isEdit key is TRUE calling UpdateDriver
		UpdateDriver();
	}else{ 
		//If isEdit is FALSE calling AddDriver();
		AddDriver();
	}
}

//Update driver
void DriverCardForm::UpdateDriver(){
	if(!fullnamelineEdit->text().isEmpty() && !pnumlineEdit->text().isEmpty() && !nicklineEdit->text().isEmpty() && !autocomboBox->currentText().isEmpty() && !citycomboBox->currentText().isEmpty() && !streetcomboBox->currentText().isEmpty() && !homelineEdit->text().isEmpty()){


		QSqlQuery query, cityquery, streetquery, carquery;

		bool carok=carquery.exec("SELECT key FROM cars WHERE car_name_num LIKE '%"+autocomboBox->currentText()+"%';");
		if (!carok)
		{
			QMessageBox::information(this, tr("Qtaxi"), carquery.lastError().databaseText());
			return;
		}
		carquery.first();

		bool cityok=cityquery.exec("SELECT key FROM cities WHERE city_name LIKE '%"+citycomboBox->currentText()+"%';");
		if (!cityok)
		{
			QMessageBox::information(this, tr("Qtaxi"), cityquery.lastError().databaseText());
			return;
		}
		cityquery.first();

		bool streetok=streetquery.exec("SELECT key FROM streets WHERE street_name LIKE '%"+streetcomboBox->currentText()+"%';");
		if (!streetok)
		{
			QMessageBox::information(this, tr("Qtaxi"), streetquery.lastError().databaseText());
			return;
		}
		streetquery.first();


		if (!photolabel->pixmap()==NULL)
		{

			const QPixmap  *pixmap=photolabel->pixmap();
			QBuffer buffer(&bytes);
			buffer.open(QIODevice::WriteOnly);
			pixmap->save(&buffer, "PNG");


		}else bytes=NULL;



		query.prepare("UPDATE drivers SET fullname=:fullname, born_date=:born_date, car=:car, nickname=:nickname, city=:city, street=:street, home=:home, phone_home=:phone_home, phone_mob=:phone_mob, passport_ser=:passport_ser, passport_num=:passport_num, passport_date=:passport_date, passport_get=:passport_get, photo=:photo WHERE psn=:psn;");
		query.bindValue(":fullname", fullnamelineEdit->text());
		query.bindValue(":psn", pnumlineEdit->text());
		query.bindValue(":born_date", borndateEdit->date());
		query.bindValue(":car",carquery.value(0).toInt());
		query.bindValue(":nickname",nicklineEdit->text());
		query.bindValue(":city",cityquery.value(0).toInt());
		query.bindValue(":street", streetquery.value(0).toInt());
		query.bindValue(":home",homelineEdit->text());
		query.bindValue(":phone_home", phonelineEdit->text());
		query.bindValue(":phone_mob", moblineEdit->text());
		query.bindValue(":passport_ser", passerlineEdit->text());
		query.bindValue(":passport_num", passnumlineEdit->text());
		query.bindValue(":passport_date",passdateEdit->date());
		query.bindValue(":passport_get",getoutlineEdit->text());
		query.bindValue(":photo", bytes);
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}
		//close();
	}else {
		QMessageBox::information(this, tr("Qtaxi"), tr("Required fields are empty"));
		return;
	}
	accept();
}

//Add driver
void DriverCardForm::AddDriver(){
	if(!fullnamelineEdit->text().isEmpty() && !pnumlineEdit->text().isEmpty() && !nicklineEdit->text().isEmpty() && !autocomboBox->currentText().isEmpty() && !citycomboBox->currentText().isEmpty() && !streetcomboBox->currentText().isEmpty() && !homelineEdit->text().isEmpty()){


		QSqlQuery query, cityquery, streetquery, carquery;

		bool carok=carquery.exec("SELECT key FROM cars WHERE car_name_num LIKE '%"+autocomboBox->currentText()+"%';");
		if (!carok)
		{
			QMessageBox::information(this, tr("Qtaxi"), carquery.lastError().databaseText());
			return;
		}
		carquery.first();

		bool cityok=cityquery.exec("SELECT key FROM cities WHERE city_name LIKE '%"+citycomboBox->currentText()+"%';");
		if (!cityok)
		{
			QMessageBox::information(this, tr("Qtaxi"), cityquery.lastError().databaseText());
			return;
		}
		cityquery.first();

		bool streetok=streetquery.exec("SELECT key FROM streets WHERE street_name LIKE '%"+streetcomboBox->currentText()+"%';");
		if (!streetok)
		{
			QMessageBox::information(this, tr("Qtaxi"), streetquery.lastError().databaseText());
			return;
		}
		streetquery.first();

		if (!photolabel->pixmap()==NULL)
		{

			const QPixmap  *pixmap=photolabel->pixmap();
			QBuffer buffer(&bytes);
			buffer.open(QIODevice::WriteOnly);
			pixmap->save(&buffer, "PNG");


		}else bytes=NULL;

		query.prepare("INSERT INTO drivers(fullname, psn, born_date, car,nickname, city, street, home, phone_home, phone_mob, passport_ser, passport_num, passport_date, passport_get, photo) VALUES(:fullname,:psn, :born_date, :car, :nickname, :city, :street, :home, :phone_home, :phone_mob, :passport_ser, :passport_num, :passport_date, :passport_get, :photo)");
		query.bindValue(":fullname", fullnamelineEdit->text());
		query.bindValue(":psn", pnumlineEdit->text());
		query.bindValue(":born_date", borndateEdit->date());
		query.bindValue(":car",carquery.value(0).toInt());
		query.bindValue(":nickname",nicklineEdit->text());
		query.bindValue(":city",cityquery.value(0).toInt());
		query.bindValue(":street", streetquery.value(0).toInt());
		query.bindValue(":home",homelineEdit->text());
		query.bindValue(":phone_home", phonelineEdit->text());
		query.bindValue(":phone_mob", moblineEdit->text());
		query.bindValue(":passport_ser", passerlineEdit->text());
		query.bindValue(":passport_num", passnumlineEdit->text());
		query.bindValue(":passport_date",passdateEdit->date());
		query.bindValue(":passport_get",getoutlineEdit->text());
		query.bindValue(":photo", bytes);
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}
	}else {
		QMessageBox::information(this, tr("Qtaxi"), tr("Required fields are empty"));
		return;
	}
	accept();
}

//Load photo from base
void DriverCardForm::LoadPhoto(){

	QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
		"/", tr("Images (*.png *.xpm *.jpg)"));
	if (!fileName.isNull())
	{
		QPixmap pixmap;
		pixmap.load(fileName);
		photolabel->setPixmap(pixmap);
	}
}

//Set validator for input only digit between '0'-'9999'
void DriverCardForm::SetValidator(){
	QRegExp rx("-?\\d{1,4}");
	QValidator *validator = new QRegExpValidator(rx, this);
	pnumlineEdit->setValidator(validator);
}

//Install event filter
void DriverCardForm::SetEventFilter(){
	fullnamelineEdit->installEventFilter(this);
	pnumlineEdit->installEventFilter(this);
	borndateEdit->installEventFilter(this); 
	autocomboBox->installEventFilter(this);
	nicklineEdit->installEventFilter(this);
	citycomboBox->installEventFilter(this);
	streetcomboBox->installEventFilter(this);
	homelineEdit->installEventFilter(this);
	phonelineEdit->installEventFilter(this);
	moblineEdit->installEventFilter(this);
	passerlineEdit->installEventFilter(this);
	passnumlineEdit->installEventFilter(this);
	passdateEdit->installEventFilter(this); 
	getoutlineEdit->installEventFilter(this);
	savepushButton->installEventFilter(this);
}

//Event filter
bool DriverCardForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==fullnamelineEdit || trg==pnumlineEdit || trg==autocomboBox || trg==nicklineEdit || trg==citycomboBox || trg==streetcomboBox || trg==homelineEdit || trg==phonelineEdit || trg==moblineEdit || trg==passerlineEdit || trg==passnumlineEdit || trg==getoutlineEdit)
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
				SaveDriver();
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

	if (trg==passdateEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( passdateEdit->currentSection()==QDateEdit::YearSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(passdateEdit->currentSection())
					{
					case QDateEdit::DaySection:
						{
							passdateEdit->setSelectedSection(QDateEdit::MonthSection);
							break;
						}
					case QDateEdit::MonthSection:{
						passdateEdit->setSelectedSection(QDateEdit::YearSection)	;
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



