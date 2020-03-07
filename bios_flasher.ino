#include<SPI.h>
#include "rom.h"

SPISettings bios(8000000,MSBFIRST,SPI_MODE3);
byte state=0;

void BIOS_WE(){
  //CE low
  SPI.transfer(0x06);
  //CE high
}

void BIOS_RD(int address,int len,byte *buf){
  int i;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF}; 
  //CE low
  SPI.transfer(0x03);
  SPI.transfer(addr,3);
  for(i=0;i<len;i++){
    buf[i]=SPI.transfer(0x0);
  }
  //CE high
}

//256byte each page
void BIOS_WR(int address,int page_no,const byte *buf){
  int i,j;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF},*p; 
  
  for(i=0;i<page_no;i++){
    //CE low
    SPI.transfer(0x02);
    SPI.transfer(addr,3);
    p=(byte*)(buf)+256*i;
    for(j=0;j<256;j++){
      SPI.transfer(p[j]);
    }
    //CE high
  }

}

//64Kbyte block erase
void BIOS_CLR(int address,int blk_no){
  int i,j;
  byte addr[3]={address>>16,(address>>8)&0xFF,address&0xFF}; 
  for(i=0;i<blk_no;i++){
    //CE low
    SPI.transfer(0xD8);
    SPI.transfer(addr,3);
    //CE high
  }
}
//ROM_size in byte
void BIOS_flash(int ROM_size,int begin_addr,const byte *buf){
  int blk_no=ROM_size>>16,page_no=ROM_size>>8;
  BIOS_CLR(begin_addr,blk_no);
  BIOS_WE();
  BIOS_WR(begin_addr,page_no,buf);
  //verification
  //...
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(921600);
  SPI.begin();
  SPI.beginTransaction(bios);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(analogRead(A0));
  switch (state){
    case 0:
      BIOS_flash(2048*1024,0x200000,ROM);
      state++;
      break;
    case 1:
      Serial.println("ssgsgsg");
      break;
  }
  delay(1000);
}
