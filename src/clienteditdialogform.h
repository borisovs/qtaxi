#ifndef CLIENTEDITDIALOGFORM_H
#define CLIENTEDITDIALOGFORM_H
#include "stable.h"
#include "ui_clienteditdialog.h"

/**
*	Class for add/edit clients in database. 
*/
class ClientEditDialogForm: public QDialog, private Ui::ClientEditDialog
{
	Q_OBJECT
public:
	//!Constructor of dialog form for add new client
	ClientEditDialogForm(bool isEdit, QWidget *parent=0);
	//!Constructor of dialog form for edit new client
	ClientEditDialogForm(int key, bool isEdit, QWidget *parent=0);
protected:
	//!Reimplementing event filter
	bool eventFilter(QObject *, QEvent *);
	private slots:
		//!Method for switch insert or update method
		void SaveClient();
		//!Insert new client in base
		void InsertClient();
		//!Update client in base
		void UpdateClient();
private:
	//!Unique number client field of record
	int key;
	//!Editable flag
	bool isEdit;
	//!Method for load client list
	void LoadClientInfo();
	//!Method to install event filter for same components
	void SetEventFilter();
	//!Set size of dialog form
	void SetStartSize();
	//!set validator for only digit input between '0'-'9999'
	void SetValidator();
};


#endif //CLIENTEDITDIALOGFORM_H
