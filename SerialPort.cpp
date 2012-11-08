/**
	* <p>Title: SerilPort.cpp</p>
	*	<p>Description: Implementation of the SerialPort class.</p>
	* @author Andrea Maglie (Matr.456188)
	* @version 1.0
	*/
	
	
#include "SerialPort.h"
#include "mainwindow.h"

MainWindow* mainWindow;

#pragma comment(lib, "uuid.lib")
using namespace std;

#define RECVBUFFERSIZE 512

DWORD read;
DWORD write;
DCB dcb;
COMMTIMEOUTS cmt;

/**
 * Constructor
 */
SerialPort::SerialPort(MainWindow* mW) 
{   mainWindow = mW;
}

/**
 * Destructor
 */
SerialPort::~SerialPort() {}


/**
 * Create the handle and open the COM port
 * @param porth The handle
 * @param portacom The COM port you have to open
 */
int openPort ( HANDLE* porth, char* portacom )
{
	printf("Opening port: %s\n", portacom);
	*porth = CreateFileA( (LPCSTR)portacom, GENERIC_READ | GENERIC_WRITE,	0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,	0);

	if ( *porth == INVALID_HANDLE_VALUE )
	{
		if ( GetLastError() == ERROR_FILE_NOT_FOUND )
		{
			printf("Port %s does not exist.\n", portacom);
			return 0;
		}
		else
		{
			printf("Unable to open port.\n");
			return 2;
		}
	}

	FillMemory(&dcb, sizeof(dcb), 0);
	dcb.DCBlength = sizeof(dcb);

	if ( !BuildCommDCBA("38400,n,8,1", &dcb) )
	{
		return 3;
	}

	if ( !SetCommState(*porth, &dcb))
	{
		return 4;
	}

	if ( !SetupComm(*porth, 1024, 1024) )
	{
		return 5;
	}

	cmt.ReadIntervalTimeout = 1000;
	cmt.ReadTotalTimeoutMultiplier = 1000;
	cmt.ReadTotalTimeoutConstant = 1000;
	cmt.WriteTotalTimeoutConstant = 1000;
	cmt.WriteTotalTimeoutMultiplier = 1000;

	if (!SetCommTimeouts(*porth, &cmt) )
	{
		printf("Error setting timeout.\n");
		return 6;
	}
	
	return 1;
}

/**
 * Close the COM port
 * @param porth The handle
 */
int closePort( HANDLE *porth )
{
	if(CloseHandle(*porth))
		return 1;
	else return 0;
}

/**
 * Send (writing on COM port)
 * @param porth The handle
 * @param d the string that you have to send
 * @param size the size of the string
 */
bool senddata( HANDLE *porth, const char* d, double size )
{
	write = -1;
	printf("Sending: %s\n", d); 
	WriteFile(*porth, d, size, &write, NULL);
	return 0;
}

/**
 * Receive (reading the COM port)
 */
void receivedata( HANDLE *porth, char* recvbuffer )
{
	read = -1;
	//printf("Listening...\n");
	ReadFile( *porth, recvbuffer, RECVBUFFERSIZE, &read, NULL);
	mainWindow->appendPCInfo("received: ", recvbuffer);
	//appendPCInfo(recvbuffer);
	printf("Received: ");
	printf(recvbuffer);
	printf("\n");
}

/**
 * Thread loop
 */
void SerialPort::run()
{
	while ( true )
	{
		listeningthread();
	}
}

