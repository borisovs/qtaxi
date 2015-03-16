#ifndef COSTDIALOGCARDFORM_H
#define COSTDIALOGCARDFORM_H
#include "stable.h"
#include "ui_costdialogcard.h"
/**
*	Class for add and edit cost of carting  
*/

class CostDialogCardForm: public QDialog, private Ui::CostDialogCard
{
	Q_OBJECT
public:
	//!A constructor for add new cost
	CostDialogCardForm(bool, QWidget *parent=0);
	//!A constructor for edit cost
	CostDialogCardForm(const int &, bool,QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!edit flag
	bool isEdit;
	//!primary key of current record
	int pk_costs;
	//!loading info about current cost
	void LoadCostInfo();
	//!loading info about current card
	void LoadCartingInfo();
	//!install event filter
	void SetEventFilter();
	//!set validator for only digit input between '0'-'9999'
	void SetValidator();
	//!model for carting
	QSqlQueryModel *cartingmodel;
	private slots:
		//!save cost
		void SaveCost();
		//!if isEdit flag is false add new cost
		void AddCost();
		//!if isEdit flag is true updta current cost
		void UpdateCost();


};


#endif //COSTDIALOGCARDFORM_H
