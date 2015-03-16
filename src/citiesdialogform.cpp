#include "citiesdialogform.h"
//Constructor
CitiesDialogForm::CitiesDialogForm(QWidget *parent/* =0 */):QDialog(parent){

	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddCity()));
	connect(findlineEdit, SIGNAL(textChanged(QString)), this, SLOT(FindCity(QString)));
	listView->setAlternatingRowColors(true);
	LoadCities();
	SetStartSize();
	SetEventFilter();
}
//Method for add new city
void CitiesDialogForm::AddCity(){
	if (!addlineEdit->text().isEmpty())
	{

		QSqlQuery query;
		query.prepare("INSERT INTO cities(city_name) VALUES (:city_name);");
		query.bindValue(":city_name", addlineEdit->text());
		bool ok=query.exec();
		if(!ok){
			QMessageBox::information(this, tr("Qtaxi"), query.lastError().databaseText());
		}
		addlineEdit->clear();
		LoadCities();
	}

}

//Method for loading cities and showing in QListView
void CitiesDialogForm::LoadCities(){
	citymodel=new QSqlQueryModel(this);
	citymodel->setQuery("SELECT city_name FROM cities ORDER BY 1;");
	citymodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	listView->setModel(citymodel);
	selection=listView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
}

//Method for set index where selection in listView is changed
void CitiesDialogForm::SetIndex(const QModelIndex &index){
	cityindex=index;
}

//Method for find city
void CitiesDialogForm::FindCity(QString cityname){
	citymodel=new QSqlQueryModel(this);
	citymodel->setQuery("SELECT city_name FROM cities WHERE city_name LIKE '%"+cityname+"%' ORDER BY 1;");
	citymodel->setHeaderData(0, Qt::Horizontal, tr("Name"));
	listView->setModel(citymodel);
}
//Set startup size
void CitiesDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;
}

//Install event filter
void CitiesDialogForm::SetEventFilter(){
	findlineEdit->installEventFilter(this);
	addlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
}

//Event filter
bool CitiesDialogForm::eventFilter(QObject *trg, QEvent *evt){
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
				AddCity();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}

