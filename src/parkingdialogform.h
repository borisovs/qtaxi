#ifndef PARKINGDIALOGFORM_H
#define PARKINGDIALOGFORM_H
#include "stable.h"
#include "ui_parkingdialog.h"
/**
*	Class for showing and adding parkings in database. 
*/

class ParkingDialogForm:public QDialog, private Ui::ParkingDialog
{
	Q_OBJECT
public:
	//!A constructor
	ParkingDialogForm(QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!install event filter
	void SetEventFilter();
	//!load parkings
	void LoadParking();
	//!set start size
	void SetStartSize();
	//!model for parkings
	QSqlQueryModel *parkingmodel;
	//!selection of listView
	QItemSelectionModel *selection;
	//!index of current parking
	QModelIndex parkingindex;
	private slots:
		//!add new parking
		void AddParking();
		//!set index for current parking
		void SetIndex(const QModelIndex &);
		//!find parking
		void FindParking(QString);
};
#endif
