#include "profitdialogform.h"
#include "printpreview.h"

//Constructor
ProfitDialogForm::ProfitDialogForm(QWidget *parent/* =0 */):QDialog(parent){
	setupUi(this);
	connect(printpushButton, SIGNAL(clicked()), this, SLOT(PrintReport()));
	SetEventFilter();
	dateEdit->setDate(QDate::currentDate());
	enddateEdit->setDate(QDate::currentDate());
	SetFormatDate();
}

//Set date format for correct working event filter
void ProfitDialogForm::SetFormatDate(){
	dateEdit->setDisplayFormat("dd.MM.yyyy");
	enddateEdit->setDisplayFormat("dd.MM.yyyy");
}


//Print report
void ProfitDialogForm::PrintReport(){
	editor=new QTextEdit(this);

	QSqlQuery profit;
	profit.prepare("SELECT COUNT(cost_real), SUM(cost_real)  FROM orders WHERE isclosed=TRUE AND time_order_close >:starttoday AND time_order_close < :endtoday;");
	profit.bindValue(":starttoday", dateEdit->date().toString(Qt::SystemLocaleDate)+" 00:00:00");
	profit.bindValue(":endtoday", enddateEdit->date().toString(Qt::SystemLocaleDate)+" 23:59:59");
	profit.exec();
	QTextCursor cursor(editor->textCursor());
	cursor.movePosition(QTextCursor::Start);
	QTextCharFormat boldFormat;
	boldFormat.setFontWeight(QFont::Bold);
	QTextCharFormat normalFormat;
	normalFormat.setFontWeight(QFont::Normal);
	QTextFrame *topFrame=cursor.currentFrame();
	QTextFrameFormat topFrameFormat=topFrame->frameFormat();


	topFrameFormat.setPosition(QTextFrameFormat::FloatRight);
	topFrame->setFrameFormat(topFrameFormat);
	//cursor.insertFrame(topFrameFormat);

	QTextTableFormat orderTableFormat;
	orderTableFormat.setHeaderRowCount(1);
	orderTableFormat.setAlignment(Qt::AlignHCenter);	

	cursor.insertText(tr("Profit margin between dates: ") +dateEdit->date().toString(Qt::SystemLocaleDate)+" - "+ enddateEdit->date().toString(Qt::SystemLocaleDate), boldFormat);
	cursor.insertBlock();
	cursor.insertBlock();	

	QTextTable *orderTable = cursor.insertTable(1, 2, orderTableFormat);
	cursor=orderTable->cellAt(0, 0).firstCursorPosition();
	cursor.insertText(tr("Count orders"), boldFormat);
	cursor=orderTable->cellAt(0, 1).firstCursorPosition();
	cursor.insertText(tr("Profit"), boldFormat);

	while (profit.next())
	{
		int row=orderTable->rows();
		orderTable->insertRows(row, 1);

		cursor=orderTable->cellAt(row, 0).firstCursorPosition();
		cursor.insertText(profit.value(0).toString());

		cursor=orderTable->cellAt(row, 1).firstCursorPosition();
		cursor.insertText(profit.value(1).toString());
	}

	PrintPreview *preview = new PrintPreview(editor->document(), this);
	preview->setWindowModality(Qt::WindowModal);
	preview->setAttribute(Qt::WA_DeleteOnClose);
	preview->setWindowState(Qt::WindowMaximized);
	preview->show();
}

//Install event filter
void ProfitDialogForm::SetEventFilter(){
	dateEdit->installEventFilter(this);
	enddateEdit->installEventFilter(this);
	printpushButton->installEventFilter(this);
}

//Event filter
bool ProfitDialogForm::eventFilter(QObject *trg, QEvent *evt){

	if (trg==printpushButton)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				printpushButton->click();
				return true;
			}
		}

	}

	if (trg==dateEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( dateEdit->currentSection()==QDateEdit::YearSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(dateEdit->currentSection())
					{
					case QDateEdit::DaySection:
						{
							dateEdit->setSelectedSection(QDateEdit::MonthSection);
							break;
						}
					case QDateEdit::MonthSection:{
						dateEdit->setSelectedSection(QDateEdit::YearSection)	;
						break;
												 }
					default:
						break;
					}


				}
			}
		}
	}

	if (trg==enddateEdit)
	{
		if (evt->type()==QEvent::KeyRelease)
		{

			QKeyEvent *keyEvent = static_cast<QKeyEvent *>(evt);
			if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter ) {
				if ( enddateEdit->currentSection()==QDateEdit::YearSection)
				{

					focusNextChild();
					return true;

				} else {
					switch(enddateEdit->currentSection())
					{
					case QDateEdit::DaySection:
						{
							enddateEdit->setSelectedSection(QDateEdit::MonthSection);
							break;
						}
					case QDateEdit::MonthSection:{
						enddateEdit->setSelectedSection(QDateEdit::YearSection)	;
						break;
												 }
					default:
						break;
					}


				}
			}
		}
	}

	return QDialog::eventFilter(trg, evt);
}
