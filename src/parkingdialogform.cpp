#include "parkingdialogform.h"

//Constructor
ParkingDialogForm::ParkingDialogForm(QWidget *parent/* =0 */):QDialog(parent){

	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddParking()));
	connect(findlineEdit, SIGNAL(textChanged(QString)), this, SLOT(FindParking(QString)));
	listView->setAlternatingRowColors(true);
	LoadParking();
	SetStartSize();
	SetEventFilter();
}

//Method for add new parking
void ParkingDialogForm::AddParking(){
	if (!addlineEdit->text().isEmpty())
	{

		QSqlQuery query;
		query.prepare("INSERT INTO parkings(parking_name) VALUES (:parking_name);");
		query.bindValue(":parking_name", addlineEdit->text());
		bool ok=query.exec();
		if(!ok){
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		}
		addlineEdit->clear();
		LoadParking();
	}

}

//Method for loading parkings and showing in QListView
void ParkingDialogForm::LoadParking(){
	parkingmodel=new QSqlQueryModel(this);
	parkingmodel->setQuery("SELECT parking_name FROM parkings ORDER BY 1;");
	parkingmodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	if(parkingmodel->lastError().isValid()) {
		QMessageBox::warning(this, tr("Qtaxi"), parkingmodel->lastError().databaseText());
		return;
	}
	listView->setModel(parkingmodel);
	selection=listView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
}

//Method for set index where selection in listView is changed
void ParkingDialogForm::SetIndex(const QModelIndex &index){
	parkingindex=index;
}

//Find parking
void ParkingDialogForm::FindParking(QString parkingname){
	parkingmodel=new QSqlQueryModel(this);
	parkingmodel->setQuery("SELECT parking_name FROM parkings WHERE parking_name LIKE '%"+parkingname+"%' ORDER BY 1;");
	parkingmodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	listView->setModel(parkingmodel);
}

//Set startup size
void ParkingDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;
}

//Install event filter
void ParkingDialogForm::SetEventFilter(){
	findlineEdit->installEventFilter(this);
	addlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
}
//Event filter
bool ParkingDialogForm::eventFilter(QObject *trg, QEvent *evt){
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
				AddParking();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}

