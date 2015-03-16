#ifndef AUTHENTICDIALOGFORM_H
#define AUTHENTICDIALOGFORM_H
#include "stable.h"
#include "ui_authenticdialog.h"
/**
*	Class for login to program. 
*/
class AuthenticDialogForm: public QDialog, private Ui::AuthenticDialog
{
	Q_OBJECT
public:
	AuthenticDialogForm(QWidget *parent=0);
	//!A constructor
protected:
	//!Install event filter for tab order by enter key
	bool eventFilter(QObject *, QEvent *);
signals:
	//!Signal emitted when user name && password are correct 
	void changeusername(const QString &);
protected:
	private slots:
	//!Verify user name and password	
		void Request();
private:
	//!Install event filter
	void SetEventFilter();
	//!Load operators
	void LoadUSers();
	//!Model for operators
	QSqlQueryModel *model;
};

#endif //AUTHENTICDIALOGFORM_H
