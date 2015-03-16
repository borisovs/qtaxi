#ifndef OPERATORCARDDIALOGFORM_H
#define OPERATORCARDDIALOGFORM_H
#include "stable.h"
#include "ui_operatorcarddialog.h"
/**
*	Class for add and edit current operator record in database. 
*/
class OperatorCardDialogForm: public QDialog, private Ui::OperatorCardDialog
{
	Q_OBJECT
public:
	//!A constructor for add new operator	
	OperatorCardDialogForm(bool, QWidget *parent=0);
	//!A constructor for edit current operator
	OperatorCardDialogForm(bool, const int&, QWidget *parent=0);
protected:
	//!A event filter
	bool eventFilter(QObject *, QEvent *);
	private slots:
		//!add new operator
		void AddOperator();
		//!update current operator
		void UpdateOperator();
		//!switch between AddOperator() or UpdateOperator() (isEdit flag)
		void SaveOperator();
		//!load photo 
		void LoadPhoto();
		//!before save, verify password
		bool CheckPass();
private:
	//!set format date for correct event filter worked
	void SetFormateDate();
	//!set validator for only digit input between '0'-'9999'
	void SetValidator();
	//!install event filter
	void SetEventFilter();
	//!load current operator info
	void LoadOperatorInfo();
	//!byte array for loading photo
	QByteArray bytes;
	//!personal number of current operator (edit mode)
	int psn;
	//!edit flag
	bool isEdit;
	//!set start size
	void SetStartSize();
};
#endif   //PERATORCARDDIALOGFORM_H

