#ifndef  CARSDIALOGFORM_H
#define	 CARSDIALOGFORM_H
#include "stable.h"
#include "ui_carsdialog.h"
/**
*	Class for showing and edit cars in database. 
*/

class CarsDialogForm: public QDialog, private Ui::CarsDialog
{
	Q_OBJECT
public:
	//!A constructor
	CarsDialogForm(QWidget *parent=0);

protected:
	//!Install event filter for tab order by enter key
	bool eventFilter(QObject *, QEvent *);
	private slots:
		//!Add car
		void AddCar();
		//!Set index on change selection
		void SetIndex(const QModelIndex &);
		//!Find car
		void FindCar(QString);
private:
	//!Install event filter
	void SetEventFilter();
	//!Set start size
	void SetStartSize();
	//!Load cars
	void LoadCars();
	//!Model for cars
	QSqlQueryModel *carmodel;
	//!Selection the listview of cars
	QItemSelectionModel *selection;
	//!selected index
	QModelIndex carindex;
};

#endif //CARSDIALOGFORM_H
