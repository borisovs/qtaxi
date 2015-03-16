#include "carsdialogform.h"
//Constructor
CarsDialogForm::CarsDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddCar()));
	connect(findlineEdit, SIGNAL(textChanged(QString)), this, SLOT(FindCar(QString)));
	listView->setAlternatingRowColors(true);
	LoadCars();
	SetStartSize();
	SetEventFilter();
}

//Method for add car
void CarsDialogForm::AddCar(){
	if (!addlineEdit->text().isEmpty())
	{

		QSqlQuery query;
		query.prepare("INSERT INTO cars(car_name_num) VALUES (:car_name_num);");
		query.bindValue(":car_name_num", addlineEdit->text());
		bool ok=query.exec();
		if(!ok){
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		}
		addlineEdit->clear();
		LoadCars();
	}

}

//Method for load cars and show they in QListView
void CarsDialogForm::LoadCars(){
	carmodel=new QSqlQueryModel(this);
	carmodel->setQuery("SELECT car_name_num FROM cars ORDER BY 1;");
	listView->setModel(carmodel);
	selection=listView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
}

//Method for set index where selection in listView is changed
void CarsDialogForm::SetIndex(const QModelIndex &index){
	carindex=index;
}

//Method for find cars
void CarsDialogForm::FindCar(QString carname){
	carmodel=new QSqlQueryModel(this);
	carmodel->setQuery("SELECT car_name_num FROM cars WHERE car_name_num LIKE '%"+carname+"%' ORDER BY 1;");
	listView->setModel(carmodel);
}

//Set startup size
void CarsDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;
}

//Install evet filter
void CarsDialogForm::SetEventFilter(){
	findlineEdit->installEventFilter(this);
	addlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
}
//Event filter
bool CarsDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==findlineEdit || trg==addlineEdit)
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
				AddCar();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}
