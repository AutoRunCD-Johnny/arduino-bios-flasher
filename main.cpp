#include<windows.h>
#include<stdio.h>

char port[10]="COM";

int main(int argc, char* argv[])
{
	HANDLE hComm;
	
	if(argc>1){
		strcat(port,argv[1]);
	}

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

	DCB dcbSerialParams = {0};
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

	char lpBuffer[] = "HelloWorld";
	DWORD dNoOFBytestoWrite;         // No of bytes to write into the port
	DWORD dNoOfBytesWritten = 0;     // No of bytes written to the port
	dNoOFBytestoWrite = sizeof(lpBuffer);

	BOOL ret=WriteFile(hComm,        // Handle to the Serial port
		lpBuffer,     // Data to be written to the port
		dNoOFBytestoWrite,  //No of bytes to write
		&dNoOfBytesWritten, //Bytes written
		NULL);
	if (!ret)
		printf("write data to serial port failed...\n");



	char TempChar; //Temporary character used for reading
	char SerialBuffer[1024];//Buffer for storing Rxed Data
	DWORD NoBytesRead;
	int i = 0;

	while (1) {
		ReadFile(hComm,           //Handle of the Serial port
			&TempChar,       //Temporary character
			sizeof(TempChar),//Size of TempChar
			&NoBytesRead,    //Number of bytes read
			NULL);

		SerialBuffer[i] = TempChar;// Store Tempchar into buffer
		i++;
	}

	CloseHandle(hComm);//Closing the Serial Port

	return 0;
}