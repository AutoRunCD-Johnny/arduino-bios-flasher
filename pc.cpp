#include <iostream>



void manager(serialcomm *serial){
    int i=0;
    char c,msg[10]="",rx[1024]="";
    while(1){
        std::cout<<"0 for flashing, 1 for read binary data from bios"<<std::endl;
        do {
			c = getchar();
			msg[i++] = c;
		} while (c != '\n');
        i=0;

        switch (msg[0]){
            case (char)'0':
                serial->send(msg[0],1);
                do{
                    serial->read(rx,100);
                    // Sleep(700);
                }while(rx!="done");
            break;
            case (char)'1':
                serial->send(msg[0],1);
                do{
                    serial->read(rx,1024);
                    serial->send("A",1);
                }while(rx!="done");
            break;

            default:
                std::cout<<"invalid option...pls retry"<<std::endl;
            break;
        }
    }
}