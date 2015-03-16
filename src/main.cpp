#include "stable.h"
#include "mainform.h"
#include "authenticdialogform.h"
#include "connform.h"
/**
*	Main function. 
*/

//! Call connect to database function
bool Connection();



int main(int argc, char* argv[]){
	QApplication app(argc, argv);
	Q_INIT_RESOURCE(resources);
	QTranslator qtTranslator;
	qtTranslator.load("qt_" + QLocale::system().name(), app.applicationDirPath());
	app.installTranslator(&qtTranslator);

	QTranslator myappTranslator;
	myappTranslator.load("qtaxi_" + QLocale::system().name(), app.applicationDirPath());
	app.installTranslator(&myappTranslator);

//Create splash screen
	QSplashScreen *splash=new QSplashScreen;
	splash->setPixmap(QPixmap(":/splash/splash.png"));
	splash->show();
	Qt::Alignment bottonLeft= Qt::AlignLeft | Qt::AlignBottom;
	
	QString message=QObject::tr("Version: 0.0.1")+'\n'+QObject::tr("License: GPL")+'\n'+QObject::tr("Authors: Sergey Borisov")+'\n'+QObject::tr("All rights reserved. 2007 year.");
	splash->showMessage(message+QChar(0x00A9),
		bottonLeft, Qt::yellow);

	app.setQuitOnLastWindowClosed(true);
	if (!Connection()){
		return 1;

	}
	//!If connect return true, show authentificate user dialog
	AuthenticDialogForm dlg;
	MainForm mf;
	//on change user name and accepted, set operator name in status bar
	QObject::connect(&dlg, SIGNAL(changeusername(const QString &)), &mf, SLOT(SetUserNameLabel(const QString &)));
	if (!dlg.exec()==QDialog::Accepted){
		delete splash;
		return 1; 
	}else {
		
		mf.show();
		splash->finish(&mf);
		delete splash;
		return app.exec();
	}
}

bool Connection(){
	QSettings settings("qtaxi", "Connection");
	QString host=settings.value("host").toString();
	int port=settings.value("port").toInt();
	QString base=settings.value("base").toString();
	QString user=settings.value("user").toString();
	QString pass=settings.value("pass").toString();
	QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL");
	db.setHostName(host);
	db.setPort(port);
	db.setDatabaseName(base);
	db.setUserName(user);
	db.setPassword(pass);
	if (!db.open()){
		QMessageBox::warning(0, QObject::tr("Qtaxi application"), db.lastError().databaseText());
		ConnForm dlg;
		dlg.exec();
		return 0;
	}
	return 1;
}

