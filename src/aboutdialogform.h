#ifndef ABOUTDIALOGFORM_H
#define ABOUTDIALOGFORM_H
#include "stable.h"
#include "ui_aboutdialog.h"
/**
*	This class show about program information	
*/
class AboutDialogForm: public QDialog, private Ui::AboutDialog
{
	Q_OBJECT
public:
	//!A constructor
	AboutDialogForm(QWidget *parent=0);

protected:

};


#endif //ABOUTDIALOGFORM_H
