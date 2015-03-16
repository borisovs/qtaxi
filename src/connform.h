#ifndef CONNFORM_H
#define CONNFORM_H
#include "stable.h"
#include "ui_conndialog.h"
/**
*	Class for setup connection to the database. 
*/

class ConnForm: public QDialog, private Ui::ConnDialog
{
	Q_OBJECT
public:
	//!A constructor
	ConnForm(QWidget *parent=0);
	public slots:
		//!Write new settings
		void WriteSettings();
protected:
private:
	//!read settings from registry on windows or config file on *nix.
	void ReadSettings();
};

#endif //CONNFORM_H

