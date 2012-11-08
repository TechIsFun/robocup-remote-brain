/**
	* <p>Title: SerilPort.h</p>
	*	<p>Description: Header file of the SerialPort class.</p>
	* @author Andrea Maglie (Matr.456188)
	* @version 1.0
	*/

#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <iostream>
#include <fstream>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <QThread>
#include "mainwindow.h"

class MainWindow;

class SerialPort : public QThread
{
	//HANDLE portHandle;
    
	public:
		SerialPort(MainWindow* mW);
		~SerialPort();
		void run();
};


int openPort ( HANDLE *porth, char* portacom );
int closePort( HANDLE *porth );
DWORD WINAPI funzione (LPDWORD *lpdwParam);
bool senddata( HANDLE *porth, const char* d, double size );
void receivedata( HANDLE *porth,char* recvbuffer );

		
#endif
