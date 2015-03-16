#ifndef PROFITDIALOGFORM_H
#define PROFITDIALOGFORM_H
#include "stable.h"
#include "ui_profitdialog.h"
/**
*	Class for showing/edit parameters for profit margin reports. 
*/

class ProfitDialogForm:public QDialog, private Ui::ProfitDialog
{
	Q_OBJECT
public:
	//!A constructor
	ProfitDialogForm(QWidget *parent=0);
protected:
	//!Event Filter
	bool eventFilter(QObject *, QEvent *);
private:
	//!set formate date for correct event filter working
	void SetFormatDate();
	//!install event filter
	void SetEventFilter();
	//!class for insert report strings
	QTextEdit *editor;
	private slots:
		//!print preview report
		void PrintReport();
};
#endif
