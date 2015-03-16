#include "streetsdialogform.h"

//Constructor
StreetsDialogForm::StreetsDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(listView, SIGNAL(activated(const QModelIndex &)), this, SLOT(LoadStreets(const QModelIndex &)));
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddStreet()));
	connect(findlineEdit, SIGNAL(textChanged(QString)), this, SLOT(FindStreet(QString))) ;
	listView->setAlternatingRowColors(true);
	tableView->setAlternatingRowColors(true);
	tableView->verticalHeader()->setDefaultSectionSize(25);
	LoadCities();
	SetStartSize();
	SetEventFilter();
}

//Method for loading streets and showing in QTableView
void StreetsDialogForm::LoadStreets(const QModelIndex &index){
	if(index.isValid())
	{
		citymodelindex=index;
		streetmodel=new QSqlQueryModel(this);
		streetmodel->setQuery("SELECT street_name FROM streets WHERE city_id=(SELECT key FROM cities WHERE city_name = '"+citymodel->data(index,0).toString()+"') ORDER BY 1;");
		streetmodel->setHeaderData(0, Qt::Horizontal, tr("Streets"));
		tableView->setModel(streetmodel);
		findlineEdit->clear();
		tableView->resizeColumnsToContents ();
		tableView->verticalHeader()->setDefaultSectionSize(25);
	}
}

//Method for loading cities and showing in QListView
void StreetsDialogForm::LoadCities(){
	citymodel=new QSqlQueryModel(this);
	citymodel->setQuery("SELECT city_name FROM cities ORDER BY 1;");
	listView->setModel(citymodel);
	cityselection=listView->selectionModel();
	connect(cityselection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(LoadStreets(const QModelIndex &)));
}

//Add new street
void StreetsDialogForm::AddStreet(){
	if (citymodelindex.isValid() && !addlineEdit->text().isEmpty())
	{
		QSqlQuery query, cityquery;
		//View what a key has city with name in combobox
		bool cityok=cityquery.exec("SELECT key FROM cities WHERE city_name = '" + citymodel->data(citymodelindex, 0).toString()+"';");
		if (!cityok)
		{
			QMessageBox::critical(this, tr("Qtaxi"), cityquery.lastError().databaseText());
			return;
		}
		cityquery.first();
		//Insert new street in base
		query.prepare("INSERT INTO streets(city_id, street_name) VALUES (:city_id, :streetname);");
		query.bindValue(":city_id", cityquery.value(0).toString());
		query.bindValue(":streetname", addlineEdit->text());
		bool ok=query.exec();
		if(!ok){
			QMessageBox::critical(this, tr("Qtaxi"), query.lastError().databaseText());
			return;
		}
		addlineEdit->clear();
		LoadStreets(citymodelindex);

	}
}

//Finding street
void StreetsDialogForm::FindStreet(QString){
	if(citymodelindex.isValid())
	{
		streetmodel=new QSqlQueryModel(this);
		streetmodel->setQuery("SELECT street_name FROM streets WHERE city_id=(SELECT key FROM cities WHERE city_name = '"+citymodel->data(citymodelindex,0).toString()+"') AND street_name LIKE '%"+findlineEdit->text()+"%'  ORDER BY 1;");
		streetmodel->setHeaderData(0, Qt::Horizontal, tr("Streets"));
		tableView->setModel(streetmodel);
		tableView->resizeColumnsToContents ();
	} else {
		QMessageBox::information(this, tr("Qtaxi"), tr("Select city, please..."));
	}
}

//Set startup size
void StreetsDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-300, height-200);
	delete dwidget;
	delete rect;

}

//Install event filter
void StreetsDialogForm::SetEventFilter(){
	findlineEdit->installEventFilter(this);
	listView->installEventFilter(this);
	addlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
}

//Event filter
bool StreetsDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==findlineEdit || trg==addlineEdit || trg==listView)
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
				AddStreet();
				return true;
			}
		}

	}

	return QDialog::eventFilter(trg, evt);
}

