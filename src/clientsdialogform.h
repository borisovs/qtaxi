#ifndef CLIENTSDIALOGGFORM_H
#define CLIENTSDIALOGGFORM_H
#include "stable.h"
#include "ui_clientsdialog.h"

/**
*	Class for showing clients in database. 
*/
class ClientsDialogForm: public QDialog, private Ui::ClientsDialog
{
	Q_OBJECT
public:
	ClientsDialogForm(QWidget *parent=0);
protected:
	private slots:
		//!find clients
		void FindClients(QString);
		//!set index for current client
		void SetClientIndex(const QModelIndex &);
		//!show edit client dialog
		void EditClient(const QModelIndex &);
		//!show add client dialog
		void AddClient();
private:
	//!load clients from base
	void LoadClients();
	//!set start size
	void SetStartSize();
	//!selection of tableView
	QItemSelectionModel *clientselection;
	//!model for clients
	QSqlQueryModel *clientsmodel;
	//!index of current client
	QModelIndex clientindex;

};

#endif //CLIENTSDIALOGGFORM_H
