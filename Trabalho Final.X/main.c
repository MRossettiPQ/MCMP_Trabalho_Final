/*
 * File:   lcd_exe.c
 * Author: Roderval
 *
 */

#include <xc.h>          //***inclus?o da biblioteca do compilador
#include <pic16f877a.h>  //***inclus?o da biblioteca do chip espec?fico
#include <stdio.h>       //***inclus?o da biblioteca standard padr?o "C" 

#define _XTAL_FREQ 4000000 //***Defini a frequencia do clock, 4Mhz neste caso

//****** congiguration bits  **************************************************
#pragma config WDTE = OFF   //Desabilita o uso do WDT
#pragma config FOSC = HS     //define uso do clock externo EM 4 OU 20mHZ
//se usar o XT no m?ximo 4Mhz
#pragma config PWRTE = ON   //habilita reset ao ligar
#pragma config BOREN = ON   //Habilita o reset por Brown-out 

//*** define pinos referentes a interface com LCD
#define RS RB2
#define EN RB1
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7

//**** inclui a biblioteca do LCD escolhido
#include "lcd.h"


int main()
{
    char buffer0[20],buffer1[20],buffer2[20],buffer3[20],buffer4[20],buffer5[20],buffer6[20];     //vari?vel para o fun??o sprintf
   
  //**inicializa??es************************************************
   TRISB = 0b00000000;       //configura pinos de entrada(1)e sa?da (0)
   TRISA = 0b11111111;       //configura pinos de entrada(1)e sa?da (0)
   TRISC = 0b00000000;
   TRISD = 0b00000000;
   
   OPTION_REGbits.nRBPU = 0; //Ativa resistores de pull-ups
   
   //*** configura o conversor anal?gico/digital*******************************
   //ficar? somente com AN0 como entrada anal?gica. Tem tabela no datasheet
   ADCON1bits.PCFG0   = 0;  //configura as entradas anal?gicas
   ADCON1bits.PCFG1   = 0;  //configura as entradas anal?gicas
   ADCON1bits.PCFG2   = 0;  //configura as entradas anal?gicas
   ADCON1bits.PCFG3   = 0;  //configura as entradas anal?gicas
   
   //define o clock de convers?o
   ADCON0bits.ADCS0 = 0  ;   //confirmando default Fosc/2
   ADCON0bits.ADCS1 = 0  ;   //confirmando default Fosc/2
   
   ADCON1bits.ADFM = 0   ;   //Default ? 0. Pra mostra que pegar? os dados em 8 bits 
                             //no ADRESH, pois est? justifica ? esquerda
                             // Passando pra 1 pode pegar os dados em 10 bits 
                             //usando os dois registradores ADRESHL justificado ?
                             //direita
   
   //inicializa registradores do AD
   ADRESL = 0x00;          //inicializar valor anal?gico com 0
   ADRESH = 0x00;          
   
   ADCON0bits.ADON = 1;     //Liga AD
  
  Lcd_Init();    //necess?rio para o LCD iniciar
  
  
  while(1)
  {
     //%%%%%%%%%%%%%%%%%%%%%% Seletor Entrada %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
      float valor_entry0, valor_entry1, valor_entry2, valor_entry3, valor_entry4, valor_entry5, valor_entry6 = 0;
      
      
      //Sensor Chuva !!!!!!!! AN0
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry0 = ADRESH;     // passa valores convertido do reg para a vari?vel
              
       //Sensor Agua !!!!!!!! AN1
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry1 = ADRESH;     // passa valores convertido do reg para a vari?vel
       
       //Sensor Luz!!!!!!!! AN2
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry2 = ADRESH;     // passa valores convertido do reg para a vari?vel
       
       //&&&&&&&&&&&&&&&&&&& Funcao sensor Luz &&&&&&&&&&&&&&&&&&&&&&&&&&&& 
       if(valor_entry2 < 127)
       {
           //Enable Motor
           RD5 = 1;
           RD3 = 1;
           RD1 = 1;
           RC0 = 1;
           
           //movimenta
           RD4 = 0;
           RC1 = 1;
           RD2 = 0;
           RD6 = 1;
           
           RD0 = 0;
           RC3 = 1;
           RC2 = 0;
           RD7 = 1;
           __delay_ms(500);
           
           //para motor
           RD4 = 1;
           RC1 = 1;
           RD2 = 1;
           RD6 = 1;
           
           RD0 = 1;
           RC3 = 1;
           RC2 = 1;
           RD7 = 1;
           __delay_ms(500);
           
       }
       if(valor_entry2 > 128)
       {
           //Enable Motor
           RD5 = 1;
           RD3 = 1;
           RD1 = 1;
           RC0 = 1;
           
           //movimenta
           
           RD4 = 1;
           RC1 = 0;
           RD2 = 1;
           RD6 = 0;
           
           RD0 = 1;
           RC3 = 0;
           RC2 = 1;
           RD7 = 0;
           
           __delay_ms(500);
           
           //para motor
           RD4 = 1;
           RC1 = 1;
           RD2 = 1;
           RD6 = 1;
           
           RD0 = 1;
           RC3 = 1;
           RC2 = 1;
           RD7 = 1;
           __delay_ms(500);
           
       }
       //&&&&&&&&&&&&&&&&&&Final Sensor Luz

       
       //Sensor Solo 1 !!!!!!!! AN3
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 0; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry3 = ADRESH;     // passa valores convertido do reg para a vari?vel
              
       //Sensor Solo 2 !!!!!!!! AN4
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 1; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry4 = ADRESH;     // passa valores convertido do reg para a vari?vel
              
       //Sensor Solo 3 !!!!!!!! AN5
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 1; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry5 = ADRESH;     // passa valores convertido do reg para a vari?vel
              
       //Sensor Solo 4 !!!!!!!! AN6
      //Seleciona canal de entrada 0 como entrada anal?gica
       ADCON0bits.CHS0 = 0; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS1 = 1; //configura canal 0 como entrada anal?gica
       ADCON0bits.CHS2 = 1; //configura canal 0 como entrada anal?gica
       
       ADCON0bits.GO = 1;  //converte
       __delay_us(20);     //tempo de convers?o
       valor_entry6 = ADRESH;     // passa valores convertido do reg para a vari?vel
             
       
    //%%%%%%%%%%%%%%%%%%%%%% Final %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    
    Lcd_Clear();                    //limpa LCD
    Lcd_Set_Cursor(1,1);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("C:");  //escreve string
    sprintf(buffer0, "%.2f", valor_entry0);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(1,3);                  
    Lcd_Write_String(buffer0);             //Escreve o conte?do de buffer no LCD
    __delay_ms(20);
     
    
    Lcd_Set_Cursor(1,10);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("A:");  //escreve string
    sprintf(buffer1, "%.2f", valor_entry1);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(1,12);                  
    Lcd_Write_String(buffer1);
    __delay_ms(20);
    
    
    Lcd_Set_Cursor(1,19);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("L:");  //escreve string
    sprintf(buffer2, "%.2f", valor_entry2);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(1,21);                  
    Lcd_Write_String(buffer2);
    __delay_ms(20);
    
    
    Lcd_Set_Cursor(1,28);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("S1:");  //escreve string
    sprintf(buffer3, "%.2f", valor_entry3);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(1,31);                  
    Lcd_Write_String(buffer3);
    __delay_ms(20);
    
    
    Lcd_Set_Cursor(2,1);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("S2:");  //escreve string
    sprintf(buffer4, "%.2f", valor_entry4);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(2,4);                  
    Lcd_Write_String(buffer4);
    __delay_ms(20);
   
    
    Lcd_Set_Cursor(2,11);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("S3:");  //escreve string
    sprintf(buffer5, "%.2f", valor_entry5);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(2,14);                  
    Lcd_Write_String(buffer5);
    __delay_ms(20);
        
    Lcd_Set_Cursor(2,21);            //P?e curso linha 1 coluna 1
    Lcd_Write_String("S4:");  //escreve string
    sprintf(buffer6, "%.2f", valor_entry6);  //Armazena em buffer o conte?do da vari?vel f formatado com duas casas
    Lcd_Set_Cursor(2,24);                  
    Lcd_Write_String(buffer6);
    __delay_ms(100); 
       
  }
  return 0;
}