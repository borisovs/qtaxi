#include <QMessageBox>
#include <QSettings>
#include "connform.h"
//Constructor
ConnForm::ConnForm(QWidget *parent/* =0 */)
:QDialog(parent){
	setupUi(this);
	setWindowTitle(tr("Connection properties"));
	connect(okpushButton,SIGNAL(clicked()), this, SLOT(WriteSettings()));
	ReadSettings();
}
//Read settings
void ConnForm::ReadSettings(){
	QSettings settings("qtaxi", "Connection");
	QString host=settings.value("host", "localhost").toString();
	int port=settings.value("port", "5432").toInt();
	QString base=settings.value("base", "postgres").toString();
	QString user=settings.value("user", "postgres").toString();
	QString pass=settings.value("pass", "").toString();
	hostlineEdit->setText(host);
	portspinBox->setValue(port);
	baselineEdit->setText(base);
	userlineEdit->setText(user);
	passlineEdit->setText(pass);


}
//Write new settings
void ConnForm::WriteSettings(){
	QSettings settings("qtaxi", "Connection");
	settings.setValue("host", hostlineEdit->text());
	settings.setValue("port", portspinBox->value());
	settings.setValue("base", baselineEdit->text());
	settings.setValue("user", userlineEdit->text());
	settings.setValue("pass", passlineEdit->text());
	QMessageBox::information(this ,tr("Qtaxi application"), tr("To reload settings restart application..."));
	accept();	
}



