#ifndef PRINTPREVIEW_H
#define PRINTPREVIEW_H
#include "stable.h"
class PreviewView;
class QTextDocument;

/**
*	Class for showing reports viewer. 
*	Copyright Trolltech ASA
*	QTDIR/demos/textedit
*	Not documented. We trust Trolltech!
*/

class PrintPreview : public QMainWindow
{
	Q_OBJECT
public:
	PrintPreview(const QTextDocument *document, QWidget *parent);
	virtual ~PrintPreview();

	private slots:
		void print();
		void pageSetup();

private:
	void setup();

	QTextDocument *doc;
	PreviewView *view;
	QPrinter printer;
};

#endif // PRINTPREVIEW_H
