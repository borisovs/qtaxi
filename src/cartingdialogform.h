#ifndef CARTINGDIALOGFORM_H
#define CARTINGDIALOGFORM_H
#include "stable.h"
#include "ui_cartingdialog.h"
/**
*	Class for showing and edit parkings in database. 
*/
class CartingDialogForm: public QDialog, private Ui::CartingDialog
{ 
	Q_OBJECT
public:
	//!A constructor
	CartingDialogForm(QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!Set start size
	void SetStartSize();
	//!load cars
	void LoadCartings();
	//!set event filter
	void SetEventFilter();
	//! model for cartings
	QSqlQueryModel *model;
	//!selection of listView
	QItemSelectionModel *selection;
	//!index of current record
	QModelIndex cartingindex;
	private slots:
		//! add new carting
		void AddCarting();
		//!set index of current record
		void SetIndex(const QModelIndex &);
		//!find carting
		void FindCarting(QString);

};

#endif //CARTINGDIALOGFORM_H
