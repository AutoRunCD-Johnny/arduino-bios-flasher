#include<SPI.h>
#include "rom.h"

#define CE 99
#define ROM_SIZE 2097152‬

SPISettings bios(8000000,MSBFIRST,SPI_MODE3);
byte state=0;
byte readbuf[1024]={0};
int i;

void BIOS_WE(){
  digitalWrite(CE,LOW);
  SPI.transfer(0x06);
  digitalWrite(CE,HIGH);
}

void BIOS_RD(int address,int len,byte *buf){
  int i;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF}; 
  
  digitalWrite(CE,LOW);
  SPI.transfer(0x03);
  SPI.transfer(addr,3);
  for(i=0;i<len;i++){
    buf[i]=SPI.transfer(0x0);
  }
  digitalWrite(CE,HIGH);
}

//256byte each page
void BIOS_WR(int address,int page_no,const byte *buf){
  int i,j;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF},*p; 
  
  for(i=0;i<page_no;i++){
    digitalWrite(CE,LOW);
    SPI.transfer(0x02);
    SPI.transfer(addr,3);
    p=(byte*)(buf)+256*i;
    for(j=0;j<256;j++){
      SPI.transfer(p[j]);
    }
    digitalWrite(CE,HIGH);
  }

}

//64Kbyte block erase
void BIOS_CLR(int address,int blk_no){
  int i,j;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF}; 
  for(i=0;i<blk_no;i++){
    digitalWrite(CE,LOW);
    SPI.transfer(0xD8);
    SPI.transfer(addr,3);
    digitalWrite(CE,HIGH);
  }
}
//ROM_size in byte
void BIOS_flash(int ROM_size,int begin_addr,const byte *buf){
  int blk_no=ROM_size>>16,page_no=ROM_size>>8;
  Serial.println("start flashing...\n");
  BIOS_CLR(begin_addr,blk_no);
  BIOS_WE();
  BIOS_WR(begin_addr,page_no,buf);
  Serial.println("done...\n");
  //verification
  //...
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
  SPI.begin();
  SPI.beginTransaction(bios);
  pinMode(CE,OUTPUT);
  digitalWrite(CE,HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  Serial.println("0 for flashing,1 for read binary data from bios,");
  state=Serial.read();
  switch (state){
    case (char)'0':
      BIOS_flash(2048*1024,0x200000,ROM);
      break;
    case (char)'1':
      for(i=0;i<2048;i++){
        BIOS_RD(i*1024,1024,readbuf);
        Serial.println((char*)readbuf);
        while(Serial.read()!=(char)'A'){
          delayMicroseconds(1000);
        }
      }
      break;
  }
  delay(1000);
}
