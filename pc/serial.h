#include<windows.h>
#include<stdio.h>


class serial{
    public:
    serial(int,char*);
    ~serial();
    void read(char*,int);
    void send(char*,int);
    private:
    HANDLE hComm;
    DCB dcbSerialParams;
    COMMTIMEOUTS timeouts;

};