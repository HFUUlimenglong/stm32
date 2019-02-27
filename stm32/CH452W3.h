#ifndef __CH452W3_H
#define __CH452W3_H

void Pin_init_CH452(void);
void CH452_Write_Init(unsigned char m,unsigned short cmd);
void CH452_Write_Float_Data(unsigned char m,float date);
void CH452_Write_Int_Data(unsigned char m,int date);
void CH452_Write_ERROR(unsigned char m);
void CH452_Write_UnderLine(unsigned char m);
void CH452_Write_Glint(unsigned char m);
void CH452_Write_NO_Glint(unsigned char m);
void CH452_Write_Way(unsigned char m);

#endif
