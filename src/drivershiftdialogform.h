#ifndef DRIVERSHIFTDIALOGFORM_H
#define DRIVERSHIFTDIALOGFORM_H
#include "stable.h"
#include "ui_drivershiftdialog.h"
/**
*	Class for showing and edit  drivers shift in database. 
*/

class DriverShiftDialogForm: public QDialog, private Ui::DriverShiftDialog
{
	Q_OBJECT
public:
	//!A constructor
	DriverShiftDialogForm(const QString &, bool isEdit, QWidget *parent=0);

protected:
	//Event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!model for parkings
	QSqlQueryModel *parkingmodel;
	//!oldparking parameters
	QString oldparking;
	//!set parameters of current index
	void SetParam();
	//!set start size
	void SetStartSize();
	//!install event filter
	void SetEventFilter();
	//!edit flag 
	bool isEdit;
	//!nick name of current driver
	QString nick;
	private slots:
		//!load parkings
		void LoadParking();
		//!update driver parking location
		void UpdateStatus();
		//!close driver shift
		void CloseShift();

};


#endif //DRIVERSHIFTDIALOGFORM_H
