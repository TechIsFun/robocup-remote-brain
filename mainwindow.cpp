/**
	* <p>Title: MainWindow.cpp</p>
	*	<p>Description: Implementation of the graphical interface of the Remote Brain application.</p>
	* @author Andrea Maglie (Matr.456188)
	* @version 1.0
	*/


//include
#include <QtGui>
#include <QPoint>
#include <QPainter>
#include "mainwindow.h"

#define FIELDW 570   //length of kid filed image
#define FIELDH 419  //height of kid filed image
#define NUMBEROFROBOTS 2
#define RECVBUFFERSIZE 512

//#define FIELDWT 740  //length of teen filed image
//#define FIELDHT 540  //length of teen filed image

QString str;
QPushButton *openPortButton;
QPushButton *closePortButton;
QPushButton *setPosButton;
QPushButton *startButton;
QPushButton *stopButton;
QPushButton *goButton;
QComboBox *combo;
QComboBox *comboPort;
QComboBox *comboFieldSize;
QTextEdit *info1;
QTextEdit *info2;
QTextEdit *cmdLine;
QLabel *imageLabel;    
bool robotchanged;
int pos_x[NUMBEROFROBOTS];
int pos_y[NUMBEROFROBOTS];   
int r[NUMBEROFROBOTS];
int g[NUMBEROFROBOTS];
int b[NUMBEROFROBOTS];
int robot; // 1=Robot1, 2=Robot2, 3=Both
char recvbuffer[RECVBUFFERSIZE];
char* fieldport = "y";
char* message;
HANDLE portHandle;

char* portacom;

using namespace std;

/**
 * Main window constructor.
 */
MainWindow::MainWindow()
{

	serialp = new SerialPort(this);

	QWidget *w = new QWidget;
  setCentralWidget(w);

  QWidget *topFiller = new QWidget;
  topFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	        	
	// Open Port Button
  openPortButton = new QPushButton(tr("Open ComPort"), this);
  connect(openPortButton, SIGNAL(clicked()), this, SLOT(openComPort()));
  openPortButton->setEnabled(true);
  openPortButton->setStatusTip(tr("Open the selected COM port"));
  
  // Close Port Button
  closePortButton = new QPushButton(tr("Close ComPort"), this);
  connect(closePortButton, SIGNAL(clicked()), this, SLOT(closeComPort()));
  closePortButton->setEnabled(false);
  closePortButton->setStatusTip(tr("Close the selected COM port"));
	 
  // Set Position Button
	setPosButton = new QPushButton(tr("Set Position"), this);
  //setPosButton->setGeometry(20, 30, 90, 30);
  //setPosButton->setFont(QFont("Arial", 10, QFont::Bold));
  connect(setPosButton, SIGNAL(clicked()), this, SLOT(setPos()));
  setPosButton->setStatusTip(tr("Set the Robot's Position"));
	setPosButton->setEnabled(false);        
	
  // Start Button
  startButton = new QPushButton(tr("Start"), this);
  //startButton->setGeometry(130, 30, 90, 30);
  //startButton->setFont(QFont("Times", 10, QFont::Bold));
  connect(startButton, SIGNAL(clicked()), this, SLOT(start()));
  startButton->setStatusTip(tr("Start the selcted Robot"));
  startButton->setEnabled(false);
        
  // Stop Button
  stopButton = new QPushButton(tr("Stop"), this);
  //stopButton->setGeometry(240, 30, 90, 30);
	//stopButton->setFont(QFont("Times", 10, QFont::Bold));
	connect(stopButton, SIGNAL(clicked()), this, SLOT(stop()));
	stopButton->setStatusTip(tr("Stop the selected Robot"));
	stopButton->setEnabled(false);

	// Goal Button
	goButton = new QPushButton(tr("Goal"), this);
  //goButton->setGeometry(350, 30, 90, 30);
  //goButton->setFont(QFont("Times", 10, QFont::Bold));
  connect(goButton, SIGNAL(clicked()), this, SLOT(goal()));
  goButton->setStatusTip(tr("Attack against the selected goal"));
	goButton->setEnabled(false);

	refresh = new QPushButton(tr("REFRESH"), this);
    //setPosButton->setGeometry(20, 30, 90, 30);
    //setPosButton->setFont(QFont("Arial", 10, QFont::Bold));
    connect(refresh, SIGNAL(clicked()), this, SLOT(aggiorna()));
    refresh->setStatusTip(tr("Set the Robot's Position"));
    
    connect(this,SIGNAL(daAggiornare()),this, SLOT(aggiorna()));


  QPushButton *sendCmdButton = new QPushButton(tr("Send Command"), this);
  //startButton->setGeometry(130, 30, 90, 30);
  //startButton->setFont(QFont("Times", 10, QFont::Bold));
  connect(sendCmdButton, SIGNAL(clicked()), this, SLOT(sendCmd()));
  sendCmdButton->setStatusTip(tr("Send the command"));

	// Robot selection combo
  combo = new QComboBox();
  combo->addItem(tr("Robot1"));
  combo->addItem(tr("Robot2"));
  combo->addItem(tr("BOTH"));
  //combo->setGeometry(450, 30, 90, 30);
  connect(combo, SIGNAL(activated ( int  )), this, SLOT(robotSel()));
//  connect(combo, SIGNAL(highlighted ( const QString & text  )), this, SLOT(robotSel()));
  //  connect(combo, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(robotSel()));
  //  void QComboBox::highlighted ( const QString & text )  [signal]
  //selRobot = new QLabel("Select Robot:");
  robot = 0;
  robotchanged = false;
  
  // Port selection combo
  comboPort = new QComboBox();
  comboPort->addItem(tr("COM1"));
  comboPort->addItem(tr("COM2"));
  comboPort->addItem(tr("COM3"));
  comboPort->addItem(tr("COM4"));
  comboPort->addItem(tr("COM5"));
  comboPort->addItem(tr("COM6"));
  comboPort->addItem(tr("COM7"));
  comboPort->addItem(tr("COM8"));
  comboPort->addItem(tr("COM9"));
  
//  connect(comboPort, SIGNAL(activated ( int index ) ), this, SLOT(portSel()));
  connect(comboPort, SIGNAL(activated ( int  )), this, SLOT(portSel()));
//  connect(comboPort, SIGNAL(highlighted ( const QString & text  )), this, SLOT(portSel()));
//  connect(comboPort, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(portSel()));
  //selCom = new QLabel("Select COM Port:");
  portacom = "COM4";
        
  
  // Combo for Field Size selection
  //comboFieldSize = new QComboBox();
  //comboFieldSize->addItem(tr("Teen Size"));
  //comboFieldSize->addItem(tr("Kid Size"));
  //kidsize = false;
  //connect(comboFieldSize, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(fieldSel()));
        
  // bottom
  QWidget *bottomFiller = new QWidget;
  bottomFiller->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  // Grid layout
	QGridLayout *grid = new QGridLayout();

	// Layout boxes
  QVBoxLayout *vbox = new QVBoxLayout;
  //QVBoxLayout *vbox2 = new QVBoxLayout;
  QHBoxLayout *hbox = new QHBoxLayout;
  QHBoxLayout *hbox2 = new QHBoxLayout;
  QHBoxLayout *hbox3 = new QHBoxLayout;
  hbox->setMargin(5);
  hbox->addWidget(openPortButton);
  hbox->addWidget(closePortButton);
  hbox->addWidget(setPosButton);
  hbox->addWidget(startButton);
  hbox->addWidget(stopButton);
  hbox->addWidget(goButton);
  hbox->addWidget(refresh);
  //hbox->addWidget(selRobot);
  hbox2->addWidget(combo);
  //hbox->addWidget(selCom);
  hbox2->addWidget(comboPort);
  //hbox2->addWidget(comboFieldSize);
  //hbox2->setMargin(1);
  //hbox->addWidget(sendTestButton);
  
  // Info widget
  QLabel *labelInfo1 = new QLabel("PC Info");
  QLabel *labelInfo2 = new QLabel("Robot Info");  
  info1 = new QTextEdit("");
  info2 = new QTextEdit("");
  cmdLine = new QTextEdit("");
  // info are not editable
  info1->setReadOnly(true);
  info1->ensureCursorVisible();
  info1->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );
  info2->setReadOnly(true);
  info2->ensureCursorVisible();
  info2->setVerticalScrollBarPolicy ( Qt::ScrollBarAlwaysOn );

  vbox->addWidget(labelInfo1);
  vbox->addWidget(info1);
  vbox->addWidget(labelInfo2);
  vbox->addWidget(info2);

  hbox3->addWidget(sendCmdButton);
  hbox3->addWidget(cmdLine);
  hbox3->setMargin(5);
  
  // field image
  imageLabel = new QLabel();
  QImage image("fieldk.jpg");
  //image.save("tmp.jpg", "jpg", 100);
  //pix = new QPixmap();
  //pix->load("filedk.jpg");
  imageLabel->setPixmap(QPixmap::fromImage(image));
  //MainWindow().setImage(image)
  imageLabel->resize(FIELDW,FIELDH);
  imageLabel->setMinimumSize(FIELDW,FIELDH);
  	if (image.isNull())
	QMessageBox::information(this, tr("Error"), tr("Cannot load image."));
  image = image.scaled( FIELDW, FIELDH, Qt::KeepAspectRatio, Qt::FastTransformation ); 
  connect(imageLabel, SIGNAL(clicked()), imageLabel, SLOT(mousePressEvent(QMouseEvent *event)));
  
	//if ( kidsize )
	//{
		//QImage image ("fieldk.jpg");
  	//imageLabel->setPixmap(QPixmap::fromImage(image));
  	//if (image.isNull())
			//QMessageBox::information(this, tr("Error"), tr("Cannot load image."));
  	//image = image.scaled( FIELDWK, FIELDHK, Qt::KeepAspectRatio, Qt::FastTransformation ); 
  	//imageLabel->resize(FIELDWK,FIELDHK);
  //}
  //else
  //{
  	//QImage image ("fieldt.jpg");
  	//imageLabel->setPixmap(QPixmap::fromImage(image));
  	//if (image.isNull())
			//QMessageBox::information(this, tr("Error"), tr("Cannot load image."));
  	//image = image.scaled( FIELDWT, FIELDHT, Qt::KeepAspectRatio, Qt::FastTransformation ); 
  	//imageLabel->resize(FIELDWT,FIELDHT);
  //}
  
  //imageLabel->resize(FIELDWT,FIELDHT);
  //imageLabel->setMargin(5);
  //connect( imageLabel, SIGNAL(clicked()), this, SLOT(getxy()));
  
  
 	// widget layout
  grid->addWidget(topFiller);
  grid->addLayout(hbox, 0, 0, Qt::AlignTop);
  grid->addLayout(hbox2, 1, 0, Qt::AlignTop);
  grid->addWidget(imageLabel, 2, 0, 8, 5, Qt::AlignLeft);  
  grid->addLayout(vbox, 2, 5, Qt::AlignJustify);
  //grid->addLayout(vbox2, 7, 5, 1, 2, Qt::AlignTop);
  grid->addLayout(hbox3, 10, 0, Qt::AlignLeft);
  grid->addWidget(bottomFiller);
              
  w->setLayout(grid);
				
  createActions();
  createMenus();

  statusBar()->showMessage(tr(""));

  setWindowTitle(tr("Remote Brain"));
  setMinimumSize(850, 600);
  //resize(850, 600);
  setMaximumSize(850, 600);
  defineRobotColors();
}

/**
 * Customize the robot indicator's color.
 */
void defineRobotColors()
{
	for ( int i = 0; i < NUMBEROFROBOTS; i++ )
	{
		r[i] = g[i] = b[i] = 0;
	}
	r[1] = 237;
	g[1] = 187;
	b[1] = 51;
}

/**
 * Get the x coordinate of a Robot.
 * @param robot The Robot's number.
 */
int MainWindow::getx( int robot )
{
	return pos_x[robot];	
}

/**
 * Get the y coordinate of a Robot.
 * @param robot The Robot's number.
 */
int MainWindow::gety( int robot )
{
	return pos_y[robot];	
}

/**
 * Set the x coordinate of a Robot.
 * @param robot The Robot's number.
 * @param newx The x coordinate.
 */
void MainWindow::setx( int robot, int newx )
{
	pos_x[robot] = newx;
}

/**
 * Set the y coordinate of a Robot.
 * @param robot The Robot's number.
 * @param newy The y coordinate.
 */
void MainWindow::sety( int robot, int newy )
{
	pos_y[robot] = newy;
}

/**
 * Write a string in PC Info widget.
 * @param info The string you have to write.
 */
void MainWindow::appendPCInfo( char* info )
{
	info1->append(QString(info));
	emit(daAggiornare());
}

/**
 * Write a QString in PC Info widget.
 * @param info The string you have to write.
 */
void MainWindow::appendPCInfo( QString info )
{
	info1->append(info);
	emit(daAggiornare());
}

/**
 * Write an int in PC Info widget.
 * @param i The int you have to write.
 */
void MainWindow::appendPCInfo( int i )
{
	str = "%1";
	info1->append(str.arg("%1").arg(i));
	emit(daAggiornare());
}

/**
 * Write two strings in PC Info widget.
 * @param info The first string you have to write.
 * @param i The second string you have to write.
 */
void MainWindow::appendPCInfo( const char* info, const char* i )
{
	info1->append(QString(info).append(QString(i)));
	emit(daAggiornare());
}

/**
 * Write a string and an int in PC Info widget.
 * @param info The string you have to write.
 * @param i The int you have to write.
 */
void MainWindow::appendPCInfo( const char* info, int i )
{
	info1->append(QString(info).arg("%1").arg(i));
	emit(daAggiornare());
}

/**
 * Write a string in Robot Info widget.
 * @param info The string you have to write.
 */
void MainWindow::appendRobotInfo( const char* info )
{
	info2->append(QString(info));
	emit(daAggiornare());
}

/**
 * Write a QString in Robot Info widget.
 * @param info The string you have to write.
 */
void MainWindow::appendRobotInfo( QString info )
{
	info2->append(info);
	emit(daAggiornare());
}

/**
 * Write an int in Robot Info widget.
 * @param i The int you have to write.
 */
void MainWindow::appendRobotInfo( int i )
{
	str = "%1";
	info2->append(str.arg("%1").arg(i));
	emit(daAggiornare());
}

/**
 * Write two strings in Robot Info widget.
 * @param info The first string you have to write.
 * @param i The second string you have to write.
 */
void MainWindow::appendRobotInfo( const char* info, const char* i )
{
	info2->append(QString(info).append(QString(i)));
	emit(daAggiornare());
}

/**
 * Write a string and an int in Robot Info widget.
 * @param info The string you have to write.
 * @param i The int you have to write.
 */
void MainWindow::appendRobotInfo( const char* info, int i )
{
	info2->append(QString(info).arg(i));
	emit(daAggiornare());
}


/**
 * Draw a cross on the field image.
 * Each cross indicates a Robot's position.
 */
void MainWindow::drawPosition()
{

//	if ( (x <= FIELDW-3) && (x >= 3) && ( y >= 3) && (y <= FIELDH-3) )
//{
//		img.setPixel(x,y,R2COLOR);
//		img.setPixel(x+1,y,R2COLOR);
//		img.setPixel(x,y+1,R2COLOR);
//		img.setPixel(x-1,y,R2COLOR);
//		img.setPixel(x,y-1,R2COLOR);
//		img.setPixel(x+2,y,R2COLOR);
//		img.setPixel(x,y+2,R2COLOR);
//		img.setPixel(x-2,y,R2COLOR);
//		img.setPixel(x,y-2,R2COLOR);
//		img.setPixel(x+3,y,R2COLOR);
//		img.setPixel(x,y+3,R2COLOR);
//		img.setPixel(x-3,y,R2COLOR);
//		img.setPixel(x,y-3,R2COLOR);
//		return img;
//	}
	
	
	
//	if ( x < FIELDW-2 )
//		{
//			image.setPixel(x+3,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x+2,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x+1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( x < FIELDW-1 )
//		{
//			image.setPixel(x+2,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x+1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( x < FIELDW )
//		{
//			image.setPixel(x+1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
	
//		if ( y < FIELDH-2 )
//		{
//			image.setPixel(x,y+3,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y+2,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y+1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		if ( y < FIELDH-1 )
//		{
//			image.setPixel(x,y+2,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y+1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		if ( y < FIELDH )
//		{
//			image.setPixel(x,y+1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
		
//		if ( x > 2 )
//		{
//			image.setPixel(x-3,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x-2,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x-1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( x > 1 )
//		{
//			image.setPixel(x-2,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x-1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( x > 0 )
//		{
//			image.setPixel(x-1,y,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
		
//		if ( y > 2 )
//		{
//			image.setPixel(x,y-3,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y-2,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y-1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( y > 1 )
//		{
//			image.setPixel(x,y-2,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y-1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}
//		else if ( y > 0 )
//		{
//			image.setPixel(x,y-1,qRgb(r[robot], g[robot], b[robot]));
//			image.setPixel(x,y,qRgb(r[robot], g[robot], b[robot]));
//		}


	//appendPCInfo ( "drawing position of robot number %1", robot );
	//if ( robot < 2 ) // se non è stato selezionato "both"
	{
		//QImage image("tmp.jpg");
		QImage image("fieldk.jpg");
		int x,y;
		for ( int i = 0; i < NUMBEROFROBOTS; i++ )
		{
			x = pos_x[i];
			y = pos_y[i];

			if ( x < FIELDW-2 )
			{
				image.setPixel(x+3,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x+2,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x+1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( x < FIELDW-1 )
			{
				image.setPixel(x+2,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x+1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( x < FIELDW )
			{
				image.setPixel(x+1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
		
			if ( y < FIELDH-2 )
			{
				image.setPixel(x,y+3,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y+2,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y+1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			if ( y < FIELDH-1 )
			{
				image.setPixel(x,y+2,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y+1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			if ( y < FIELDH )
			{
				image.setPixel(x,y+1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			
			if ( x > 2 )
			{
				image.setPixel(x-3,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x-2,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x-1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( x > 1 )
			{
				image.setPixel(x-2,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x-1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( x > 0 )
			{
				image.setPixel(x-1,y,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
		
			if ( y > 2 )
			{
				image.setPixel(x,y-3,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y-2,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y-1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( y > 1 )
			{
				image.setPixel(x,y-2,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y-1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
			else if ( y > 0 )
			{
				image.setPixel(x,y-1,qRgb(r[i], g[i], b[i]));
				image.setPixel(x,y,qRgb(r[i], g[i], b[i]));
			}
		}
		imageLabel->setPixmap(QPixmap::fromImage(image));
	}
	//else appendPCInfo("can't draw both robots!");
	
}

/**
 * Method associated with Send Command button.
 * Sends the string specified in the associated text box.
 */
void MainWindow::sendCmd()
{
	str = cmdLine->toPlainText();
	//char* data = (char*)(str.toAscii()).constData();
	serialp->terminate();
	senddata(&portHandle, (str.toAscii()).constData(), str.size());
	serialp->start();
}

/**
 * Capture a mouse event in the main window
 */
void MainWindow::mousePressEvent(QMouseEvent *event)
{
	// capture the mouse left button click
	// on the field image
	if(event->button() == Qt::LeftButton)
	{
		int fx = imageLabel->pos().x();
		int fy = imageLabel->pos().y();
		
		str = "fx,fy: %1 %2";
		info1->append(str.arg("%1").arg(fx).arg(fy));
		
		// test function
		// draw a cross on mouse left click
		if ( robot < NUMBEROFROBOTS )
		{
			pos_x[robot] = event->pos().x() - imageLabel->pos().x();
			pos_y[robot] = event->pos().y() - imageLabel->pos().y() - 20;
			str = "abs pos: %1 %2";
			info1->append(str.arg("%1").arg(event->pos().x()).arg(event->pos().y()));
		
			if ( (pos_x[robot] <= FIELDW) && (pos_x[robot] >= 0) && ( pos_y[robot] >= 0) && (pos_y[robot] <= FIELDH) )
			{
				str = "pos: %1 %2";
				info1->append(str.arg("%1").arg(pos_x[robot]).arg(pos_y[robot]));
				drawPosition();
			}
		}
	}
}


/**
 * Event associated with context menu
 */
void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
	QMenu menu(this);
  //menu.addAction(cutAct);
  //menu.addAction(copyAct);
  //menu.addAction(pasteAct);
  menu.exec(event->globalPos());
}


/**
 * Method associated with the OpenPort button.
 * Open a connection on the selected COM port.
 * If the connection is created successfully,
 * a thread is started waiting for incoming strings.
 */
void MainWindow::openComPort()
{

	short x = openPort ( &portHandle, portacom );
	if ( x == 1 ) // connected
	{
		serialp->start();
		
		str = "Port %1 open";
		info1->append(str.arg("%1").arg(portacom));
		openPortButton->setEnabled(false);
		closePortButton->setEnabled(true);
		setPosButton->setEnabled(true);	
		startButton->setEnabled(true);
		stopButton->setEnabled(true);
		goButton->setEnabled(true);

		//message = "connected";
		//senddata( &portHandle, message, strlen(message) );
	}
	else if ( x == 0 ) // error: port does not exist
	{
		str = "Port %1 does not exixst";
		info1->append(str.arg("%1").arg(portacom));
	}
	else if ( x == 2 ) // error: unable to open port
	{
		str = "Unable to open port %1";
		info1->append(str.arg("%1").arg(portacom));
	}
	else if ( x == 3 ) // error in BuildCommDCB
	{
		info1->append("Error BuildCommDCB");
	}
	else if ( x == 4 ) // error in SetCommState
	{
		info1->append("Error SetCommState");
	}
	else if ( x == 5 ) // error in SetupComm
	{
		info1->append("Error SetupComm");
	}
	else if ( x == 6 ) // error setting timeout
	{
		info1->append("Error setting Timeout");
	}	
}

/**
 * Method associated with Close ComPort button
 * Close the handle of the open port
 * and terminate the thread
 */
void MainWindow::closeComPort()
{
	// terminate the thread
	serialp->terminate();
	if ( !closePort( &portHandle ) ) // error closing port
	{
		info1->append("Error closing port");
	}
	else //port closed successfully
	{
		str = "Port %1 closed";
		info1->append(str.arg("%1").arg(portacom));
		openPortButton->setEnabled(true);
		closePortButton->setEnabled(false);
		setPosButton->setEnabled(false);	
		startButton->setEnabled(false);
		stopButton->setEnabled(false);
		goButton->setEnabled(false);
	}
}

/**
 * Method associated with Set Posistion button.
 * Send the following command: RiSETPOS.X=x.Y=y.T=t where
 * i is the robot's number
 * x,y the position
 * t the orientation.
 */
void MainWindow::setPos()
{
	//int size = 17;
  //if ( pos_x[robot] < 10 )
//  	size++;
//  else if ( pos_x[robot] < 99 )
  	//size = size+2;
	//else
	//	size = size+3;  	
	//if ( pos_y[robot] < 10 )
//  	size++;
//  else if ( pos_y[robot] < 99 )
  	//size = size+2;
	//else
//		size = size+3;  			
	
	//add nuber of digits of t
//	size++;	
	char position[50];
	// position and orientation will be sent in hex format
	sprintf(position, "R%dSETPOS.X=%x.Y=%x.T=%x\n", robot+1, pos_x[robot], pos_y[robot], 0 );
  serialp->terminate();
	senddata(&portHandle, position, strlen(position) );
	serialp->start();
  appendPCInfo("Sent command (hex): ", position);
  // to control the conversion
  sprintf(position, "R%dSETPOS.X=%d.Y=%d.T=%d\n", robot+1, pos_x[robot], pos_y[robot], 0 );
  appendPCInfo("Sent command (dec): ", position);
}
 
/**
 * Method associated with Start button.
 * Sends the following command: RiSTART where i is the robot's number.
 */
void MainWindow::start()
{
	//char* message;
	if ( robot == 0 )
	{
		message = "R1START\n";
	}
	else if ( robot == 1 )
	{
		message = "R2START\n";
	}
	else if ( robot == 2 )
	{
		message = "R*START\n";
	}
	serialp->terminate();
	senddata(&portHandle, message, strlen(message) );
	serialp->start();
	appendPCInfo("Sent command: ", message);
}

/**
 * Method associated with Stop button.
 * Sends the following command: RiSTOP where i is the robot's number.
 */
void MainWindow::stop()
{
	if ( robot == 0 )
	{
		message = "R1STOP\n";
	}
	else if ( robot == 1 )
	{
		message = "R2STOP\n";
	}
	else if ( robot == 2 )
	{
		message = "R*STOP\n";
	}
	serialp->terminate();
	senddata(&portHandle, message, strlen(message) );
	serialp->start();
	appendPCInfo("Sent command: ", message);
}

void MainWindow::aggiorna()
{
    info1->append("");
}

/**
 * Method associated with Goal button.
 * Sends the following command: RiGOAL=y or RiGOAL=b where
 * i is the robot's number
 * y is the yellow goal
 * b is the blue goal
 */
void MainWindow::goal()
{
	if ( strcmp ( fieldport, "y" ) == 0 )
	{
		if ( robot == 0 )
			message = "R1GOAL=y\n";
		if ( robot == 1 )
			message = "R2GOAL=y\n";
		if ( robot == 2 )
			message = "R*GOAL=y\n";
	}
	else
	{
		if ( robot == 0 )
			message = "R1GOAL=b\n";
		if ( robot == 1 )
			message = "R2GOAL=b\n";
		if ( robot == 2 )
			message = "R*GOAL=b\n";
	}
	serialp->terminate();
	senddata(&portHandle, message, strlen(message));
	serialp->start();
	appendPCInfo("Sent command: ", message);
}
    
/**
 * Method associated with robot selection combo
 */
void MainWindow::robotSel()
{
	int index = combo->currentIndex();

	if ( robot != index )
	{
		robotchanged = true;
		//appendPCInfo("robot changed");
	}
	
	robot = index;
	appendPCInfo("Selected Robot %1", robot+1);

	
	//if ( index == 0 )
	//{
		//statusBar()->showMessage(tr("Selected Robot 1"));
		//robot = 0;
	//}
	//else if ( index == 1 )
	//{
		//statusBar()->showMessage(tr("Selected Robot 2"));
		//robot = 1;
		//robotchanged = true;
	//}
	//else if ( index == 2 )
	//{
		//statusBar()->showMessage(tr("Selected both Robots"));
		//robot = 2;
		//robotchanged = true;
	//}
	
}

/**
 * Method associated with COM port selection combo
 */
void MainWindow::portSel()
{
    //appendPCInfo("PIPPO");
	int index = comboPort->currentIndex();
	if ( index == 0 )
	{
		statusBar()->showMessage(tr("COM1 selected"));
		portacom = "COM1";
	}
	else if ( index == 1 )
	{
		statusBar()->showMessage(tr("COM2 selected"));
		portacom = "COM2";
	}
	else if ( index == 2 )
	{
		statusBar()->showMessage(tr("COM3 selected"));
		portacom = "COM3";
	}
	else if ( index == 3 )
	{
		statusBar()->showMessage(tr("COM4 selected"));
		portacom = "COM4";
	}
	else if ( index == 4 )
	{
		statusBar()->showMessage(tr("COM5 selected"));
		portacom = "COM5";
	}
	else if ( index == 5 )
	{
		statusBar()->showMessage(tr("COM6 selected"));
		portacom = "COM6";
	}
	else if ( index == 6 )
	{
		statusBar()->showMessage(tr("COM7 selected"));
		portacom = "COM7";
	}
	else if ( index == 7 )
	{
		statusBar()->showMessage(tr("COM8 selected"));
		portacom = "COM8";
	}
	else if ( index == 8 )
	{
		statusBar()->showMessage(tr("COM9 selected"));		
		portacom = "COM9";
	}
}

/**
 * Create menu actions
 */
void MainWindow::createActions()
{
  exitAct = new QAction(tr("&Exit"), this);
  exitAct->setShortcut(tr("Ctrl+Q"));
  exitAct->setStatusTip(tr("Close window"));
  connect(exitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

  reloadAct = new QAction(tr("&Reload Image"), this);
  reloadAct->setShortcut(tr("Ctrl+R"));
  reloadAct->setStatusTip(tr("Reload Field Image"));
  connect(reloadAct, SIGNAL(triggered()), this, SLOT(reloadImage()));

}

/**
 * Create menus
 */
void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu(tr("&File"));
	fileMenu->addAction(reloadAct);
	fileMenu->addAction(exitAct);
}

void listeningthread()
{
	FillMemory(&recvbuffer, sizeof(recvbuffer), 0);
	receivedata( &portHandle, recvbuffer );

		//if ( strcmp( recvbuffer[0] , "a" ) == 0 )
		//pos_x[1] = atoi(recvbuffer);
		//int ny = 50;
		//drawPosition (); //( 1, nx, ny );
		//printf("nx= %d, ny=%d\n", nx, ny);
		//appendRobotInfo ( "Sent position x = %1", atoi(recvbuffer) );
		
		//DWORD dwThrdParam = (DWORD)recvbuffer;
		//DWORD dwThreadId = 1;

		//Sleep(1000);

		//fprintf(log, "avvio del thread con parametro %s\n", dwThrdParam);
		//HANDLE thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE) funzione, &dwThrdParam, 0, &dwThreadId);	
}

DWORD WINAPI funzione (LPDWORD *lpdwParam)
{
	char *data = (char*)*lpdwParam;
	str = "Elaborazione stringa: %1";
	info1->append(str.arg("%1").arg(data));
	return 0;
}

//void MainWindow::setImage ( QImage img )
//{
		//imageLabel->setPixmap(QPixmap::fromImage(img));
//}

//QLabel MainWindow::getFieldImage()
//{
	//return imageLabel;
//}


//void saveImge( QImage img )
//{
	//if (robotchanged)
	//{
//		img.save("tmp.jpg", "jpg", 80);
		//robotchanged = false;
	//}
//}

//void MainWindow::fieldSel()
//{
//	int index = comboFieldSize->currentIndex();
	// teen size
//	if ( index == 1 )
//	{
//		kidsize = true;
//		info1->append(tr("Kid Size"));
//		QImage image ("fieldk.jpg");
//  	imageLabel->setPixmap(QPixmap::fromImage(image));
//		image = image.scaled( FIELDWK, FIELDHK, Qt::KeepAspectRatio, Qt::FastTransformation ); 
//		imageLabel->resize(FIELDWK,FIELDHK);
//	}
	// kid size
//	if ( index == 0 )
//	{
//		kidsize = false;
//		info1->append(tr("Teen Size"));
//		QImage image ("fieldt.jpg");
//  	imageLabel->setPixmap(QPixmap::fromImage(image));
//		image = image.scaled( FIELDWT, FIELDHT, Qt::KeepAspectRatio, Qt::FastTransformation ); 
//		imageLabel->resize(FIELDWT,FIELDHT);
//	}
//}

//void MainWindow::reloadImage()
//{
//	QImage image("fieldk.jpg");
//	imageLabel->setPixmap(QPixmap::fromImage(image));
//}

