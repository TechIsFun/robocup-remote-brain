/**
	* <p>Title: MainWindow.h</p>
	*	<p>Description: Header file of the graphical interface for the Remote Brain application.</p>
	* @author Andrea Maglie (Matr.456188)
	* @version 1.0
	*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>
#include "SerialPort.h"
#include "QPushButton.h"

class QAction;
class QActionGroup;
class QLabel;
class QMenu;
class SerialPort;

class MainWindow : public QMainWindow
{
	Q_OBJECT

  public:
  	MainWindow();
		//char* portacom;
		
    int getx( int robot );
    int gety( int robot );
    void setx( int robot, int newx );
    void sety( int robot, int newy );
    void drawPosition();// int robot, int x, int y);

	SerialPort *serialp;   

  signals:
    void daAggiornare();
    
  public slots:
	void aggiorna();
    void appendPCInfo( char* info );
    void appendPCInfo( QString info );
    void appendPCInfo( int i );
    void appendPCInfo( const char* info, const char* i );
    void appendPCInfo( const char* info, int i );
    void appendRobotInfo( const char* info );
    void appendRobotInfo( QString info );
    void appendRobotInfo( int i );
    void appendRobotInfo( const char* info, const char* i );
    void appendRobotInfo( const char* info, int i );
  	
  protected:
  	void contextMenuEvent(QContextMenuEvent *event);

  private slots:
  	void openComPort();
  	void closeComPort();
  	void portSel();
  	void setPos();
    void robotSel();
    void mousePressEvent(QMouseEvent *event);
    void start();
    void stop();
    void goal();
	void sendCmd();
		//void reloadImage();
	
  private:
  	void createActions();
    void createMenus();
    QMenu *fileMenu;
    QActionGroup *alignmentGroup;
    QAction *exitAct;
	QAction *reloadAct;
    QLabel *infoLabel;
    QPushButton *refresh;
 
  
};

    void listeningthread();
    void defineRobotColors();

#endif
