#include "serial.h"

serial::serial(int baudrate,char* port){
    hComm = CreateFile(port,                //port name
		GENERIC_READ | GENERIC_WRITE, //Read/Write
		0,                            // No Sharing
		NULL,                         // No Security
		OPEN_EXISTING,// Open existing port only
		0,            // Non Overlapped I/O
		NULL);        // Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE)
		printf("Error in opening serial port %s\n",port);
	else
		printf("opening serial port %s successful\n",port);

	dcbSerialParams = {0};
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hComm, &dcbSerialParams) == FALSE)
		printf("GetCommState() failed...\n");
	
	dcbSerialParams.BaudRate = CBR_115200;// Setting BaudRate = 115200
	dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
	dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
	dcbSerialParams.Parity = NOPARITY;  // Setting Parity = None

	if(SetCommState(hComm, &dcbSerialParams)==FALSE)
		printf("SetCommState() failed...\n");

	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 50; // in milliseconds
	timeouts.ReadTotalTimeoutConstant = 50; // in milliseconds
	timeouts.ReadTotalTimeoutMultiplier = 10; // in milliseconds
	timeouts.WriteTotalTimeoutConstant = 50; // in milliseconds
	timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds
}

serial::~serial(){
    CloseHandle(hComm);//Closing the Serial Port
}

void serial::read(char* rx_buffer,int bufsize){
    char TempChar; //Temporary character used for reading
	DWORD NoBytesRead;
	int i;

	for(i=0;i<bufsize;i++){
		ReadFile(hComm,           //Handle of the Serial port
			&TempChar,       //Temporary character
			sizeof(TempChar),//Size of TempChar
			&NoBytesRead,    //Number of bytes read
			NULL);

		rx_buffer[i] = TempChar;// Store Tempchar into buffer
		i++;
	}
}

void serial::send(char* tx_data,int len){
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	dNoOFBytestoWrite = len;
	BOOL ret=WriteFile(hComm,        // Handle to the Serial port
		tx_data,     // Data to be written to the port
		dNoOFBytestoWrite,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written
		NULL);
	if (!ret)
		printf("write data to serial port failed...\n");
}