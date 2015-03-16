#include "cartingdialogform.h"
//Constructor
CartingDialogForm::CartingDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(addpushButton, SIGNAL(clicked()), this, SLOT(AddCarting()));
	connect(findlineEdit, SIGNAL(textChanged(QString)), this, SLOT(FindCarting(QString)));
	listView->setAlternatingRowColors(true);
	LoadCartings();
	SetStartSize();
	SetEventFilter();
}

//Set startup size
void CartingDialogForm::SetStartSize(){
	QDesktopWidget *dwidget=new QDesktopWidget();
	QRect *rect=new QRect(dwidget->screenGeometry(0));
	int width=rect->width();
	int height=rect->height();
	this->setMinimumSize(width-600, height-300);
	delete dwidget;
	delete rect;
}

//Method for loading cartings and showing in QListView
void CartingDialogForm::LoadCartings(){
	model=new QSqlQueryModel(this);
	model->setQuery("SELECT carting_name FROM carting ORDER BY 1;");
	if (model->lastError().isValid())
	{
		QMessageBox::warning(this, tr("Qtaxi"), model->lastError().databaseText());
		return;
	} 
	listView->setModel(model);
	selection=listView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
}

//Method for add new carting
void CartingDialogForm::AddCarting(){
	if (!addlineEdit->text().isEmpty())
	{

		QSqlQuery query;
		query.prepare("INSERT INTO carting(carting_name) VALUES(:carting_name);");
		query.bindValue(":carting_name", addlineEdit->text());
		bool ok=query.exec();
		if (!ok)
		{
			QMessageBox::warning(this, tr("Qtaxi"), query.lastError().databaseText());
		}
		addlineEdit->clear();
		LoadCartings();
	}
}

//Method for find carting
void CartingDialogForm::FindCarting(QString str){
	model=new QSqlQueryModel(this);
	model->setQuery("SELECT carting_name FROM carting WHERE carting_name LIKE '%"+str+"%'ORDER BY 1;");
	if (model->lastError().isValid())
	{
		QMessageBox::warning(this, tr("Qtaxi"), model->lastError().databaseText());
		return;
	} 
	listView->setModel(model);
	selection=listView->selectionModel();
	connect(selection, SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)), this, SLOT(SetIndex(const QModelIndex &)));
}


//Method for set index where selection in listView is changed
void CartingDialogForm::SetIndex(const QModelIndex &index){
	cartingindex=index;
}

//Install event filter
void CartingDialogForm::SetEventFilter(){
	listView->installEventFilter(this);
	addlineEdit->installEventFilter(this);
	addpushButton->installEventFilter(this);
	findlineEdit->installEventFilter(this);
}

//Event filter
bool CartingDialogForm::eventFilter(QObject *trg, QEvent *evt){
	if (trg==listView || trg==addlineEdit || trg==findlineEdit)
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
				AddCarting();
				return true;
			}
		}

	}




	return QDialog::eventFilter(trg, evt);
}
