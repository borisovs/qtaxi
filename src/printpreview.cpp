#include "printpreview.h"


const QString rsrcPath = ":/32";

static inline int inchesToPixels(float inches, QPaintDevice *device)
{
	return qRound(inches * device->logicalDpiY());
}

static inline qreal mmToInches(double mm)
{
	return mm*0.039370147;
}


/**
*	Class for showing reports viewer. 
*	Copyright Trolltech ASA
*	QTDIR/demos/textedit
*/
class PreviewView : public QAbstractScrollArea
{
	Q_OBJECT
public:
	PreviewView(QTextDocument *document);

	inline void updateLayout() { resizeEvent(0); viewport()->update(); }

	public slots:
		void zoomIn();
		void zoomOut();

protected:
	virtual void paintEvent(QPaintEvent *e);
	virtual void resizeEvent(QResizeEvent *);
	virtual void mousePressEvent(QMouseEvent *e);
	virtual void mouseMoveEvent(QMouseEvent *e);
	virtual void mouseReleaseEvent(QMouseEvent *e);

private:
	void paintPage(QPainter *painter, int page);
	QTextDocument *doc;
	qreal scale;
	int interPageSpacing;
	QPoint mousePressPos;
	QPoint scrollBarValuesOnMousePress;
};

PreviewView::PreviewView(QTextDocument *document)
{
	
	verticalScrollBar()->setSingleStep(20);
	horizontalScrollBar()->setSingleStep(20);

	viewport()->setBackgroundRole(QPalette::Dark);

	doc = document;
	scale = 1.0;
	interPageSpacing = 30;
}

void PreviewView::zoomIn()
{
	scale += 0.2;
	resizeEvent(0);
	viewport()->update();
}

void PreviewView::zoomOut()
{
	scale -= 0.2;
	resizeEvent(0);
	viewport()->update();
}

void PreviewView::paintEvent(QPaintEvent *)
{
	QPainter p(viewport());

	p.translate(-horizontalScrollBar()->value(), -verticalScrollBar()->value());
	p.translate(interPageSpacing, interPageSpacing);

	const int pages = doc->pageCount();
	for (int i = 0; i < pages; ++i) {
		p.save();
		p.scale(scale, scale);

		paintPage(&p, i);

		p.restore();
		p.translate(0, interPageSpacing + doc->pageSize().height() * scale);
	}
}

void PreviewView::paintPage(QPainter *painter, int page)
{
	const QSizeF pgSize = doc->pageSize();

	QColor col(Qt::black);

	painter->setPen(col);
	painter->setBrush(Qt::white);
	painter->drawRect(QRectF(QPointF(0, 0), pgSize));
	painter->setBrush(Qt::NoBrush);

	col = col.light();
	painter->drawLine(QLineF(pgSize.width(), 1, pgSize.width(), pgSize.height() - 1));

	col = col.light();
	painter->drawLine(QLineF(pgSize.width(), 2, pgSize.width(), pgSize.height() - 2));

	QRectF docRect(QPointF(0, page * pgSize.height()), pgSize);
	QAbstractTextDocumentLayout::PaintContext ctx;
	ctx.clip = docRect;

	// don't use the system palette text as default text color, on HP/UX
	// for example that's white, and white text on white paper doesn't
	// look that nice
	ctx.palette.setColor(QPalette::Text, Qt::black);

	painter->translate(0, - page * pgSize.height());
	painter->setClipRect(docRect);
	doc->documentLayout()->draw(painter, ctx);
}

void PreviewView::resizeEvent(QResizeEvent *)
{
	const QSize viewportSize = viewport()->size();

	QSize docSize;
	docSize.setWidth(qRound(doc->pageSize().width() * scale + 2 * interPageSpacing));
	const int pageCount = doc->pageCount();
	docSize.setHeight(qRound(pageCount * doc->pageSize().height() * scale + (pageCount + 1) * interPageSpacing));

	horizontalScrollBar()->setRange(0, docSize.width() - viewportSize.width());
	horizontalScrollBar()->setPageStep(viewportSize.width());

	verticalScrollBar()->setRange(0, docSize.height() - viewportSize.height());
	verticalScrollBar()->setPageStep(viewportSize.height());
}

void PreviewView::mousePressEvent(QMouseEvent *e)
{
	mousePressPos = e->pos();
	scrollBarValuesOnMousePress.rx() = horizontalScrollBar()->value();
	scrollBarValuesOnMousePress.ry() = verticalScrollBar()->value();
	e->accept();
}

void PreviewView::mouseMoveEvent(QMouseEvent *e)
{
	if (mousePressPos.isNull()) {
		e->ignore();
		return;
	}

	horizontalScrollBar()->setValue(scrollBarValuesOnMousePress.x() - e->pos().x() + mousePressPos.x());
	verticalScrollBar()->setValue(scrollBarValuesOnMousePress.y() - e->pos().y() + mousePressPos.y());
	horizontalScrollBar()->update();
	verticalScrollBar()->update();
	e->accept();
}

void PreviewView::mouseReleaseEvent(QMouseEvent *e)
{
	mousePressPos = QPoint();
	e->accept();
}

PrintPreview::PrintPreview(const QTextDocument *document, QWidget *parent)
: QMainWindow(parent), printer(QPrinter::HighResolution)
{
	setWindowTitle(tr("Qtaxi - report view"));

	printer.setFullPage(true);
	doc = document->clone();

	view = new PreviewView(doc);
	setCentralWidget(view);
	resize(800, 600);

	doc->setUseDesignMetrics(true);
	doc->documentLayout()->setPaintDevice(view->viewport());

	// add a nice 2 cm margin
	const qreal margin = inchesToPixels(mmToInches(20), this);
	QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
	fmt.setMargin(margin);
	doc->rootFrame()->setFrameFormat(fmt);

	setup();

	QFont f(doc->defaultFont());
	f.setPointSize(10);
	doc->setDefaultFont(f);

	QToolBar *tb = addToolBar(tr("Print"));
	tb->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	QAction *a;
	a = new QAction(QIcon(rsrcPath + "/print.png"), tr("&Print"), this);
	a->setShortcut(Qt::CTRL + Qt::Key_P);
	connect(a, SIGNAL(triggered()), this, SLOT(print()));
	tb->addAction(a);

	a = new QAction(QIcon(rsrcPath + "/print_preview.png"), tr("&Setup"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(pageSetup()));
	tb->addAction(a);

	tb->addSeparator();

	a = new QAction(QIcon(rsrcPath + "/zoomin.png"), tr("&In"), this);
	connect(a, SIGNAL(triggered()), view, SLOT(zoomIn()));
	tb->addAction(a);

	a = new QAction(QIcon(rsrcPath + "/zoomout.png"), tr("&Out"), this);
	connect(a, SIGNAL(triggered()), view, SLOT(zoomOut()));
	tb->addAction(a);

	tb->addSeparator();

	a = new QAction(QIcon(rsrcPath + "/undo.png"), tr("&Close"), this);
	connect(a, SIGNAL(triggered()), this, SLOT(close()));
	tb->addAction(a);

	statusBar()->setSizeGripEnabled(true);
}

void PrintPreview::setup()
{
	QSizeF page = printer.pageRect().size();
	page.setWidth(page.width() * view->logicalDpiX() / printer.logicalDpiX());
	page.setHeight(page.height() * view->logicalDpiY() / printer.logicalDpiY());

	// add a nice 2 cm margin
	const qreal margin = inchesToPixels(mmToInches(20), this);
	QTextFrameFormat fmt = doc->rootFrame()->frameFormat();
	fmt.setMargin(margin);
	doc->rootFrame()->setFrameFormat(fmt);

	doc->setPageSize(page);
}

PrintPreview::~PrintPreview()
{
	delete doc;
}

void PrintPreview::print()
{
	QPrintDialog *dlg = new QPrintDialog(&printer, this);
	if (dlg->exec() == QDialog::Accepted) {
		doc->print(&printer);
	}
	delete dlg;
}

void PrintPreview::pageSetup()
{
	QPageSetupDialog dlg(&printer, this);
	if (dlg.exec() == QDialog::Accepted) {
		setup();
		view->updateLayout();
	}
}

#include "printpreview.moc"
