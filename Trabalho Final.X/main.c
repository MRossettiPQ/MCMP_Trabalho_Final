#include <xc.h>                                         //***inclusão da biblioteca do compilador
#include <pic16f877a.h>                                 //***inclusão da biblioteca do chip espec?fico
#include <stdio.h>                                      //***inclusão da biblioteca standard padr?o "C" 

#define _XTAL_FREQ 4000000                              //***Defini a frequencia do clock, 4Mhz neste caso

//****** congiguration bits  **************************************************
#pragma config WDTE = ON                                //Habilita o uso do WDT
#pragma config FOSC = HS                                //Define uso do clock externo EM 4 OU 20mHZ
//se usar o XT no m?ximo 4Mhz
#pragma config PWRTE = ON                               //Habilita o reset ao ligar
#pragma config BOREN = ON                               //Habilita o reset por Brown-out 

//*** Define pinos referentes a interface com LCD
#define RS RB2
#define EN RB1
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7

#include "lcd.h"
//%%%%%%%%%%%%%%%%%%%%%% DEFINE VARIAVEIS PARA RECEBER ANALOGICO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
float valor_entry0, valor_entry1, valor_entry2, valor_entry3, valor_entry4, valor_entry5, valor_entry6 = 0;
//%%%%%%%%%%%%%%%%%%%%%% DEFINE BUFFER LCD %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  
char buffer0[30] = "INICIA",buffer1[30] = "INICIA";     //vari?vel para o fun??o sprintf

void recolheTela(void);
void expandeTela(void);
void lerSensores(void);
void atualizaLCD(void);
void estufaUm(void);
void estufaDois(void);
void estufaTres(void);
void estufaQuatro(void);
void __interrupt() TrataINT(void);
void iniciaPinos(void);

int main()
{
    iniciaPinos();
    Lcd_Init(); 
    while(1)
    {
        lerSensores();
        //FUNÇÃO PARA RECEBER AGUA DA CHUVA
        //&&&&&&&&&&&&&&&& Funcao Sensor Chuva &&&&&&&&&&&&&&&&  
        if(valor_entry0 > 192 ) //RECOLHE TELA - 
        {
            expandeTela();
        }
        //FUNÇÃO PROTEGER SOL FORTE
        //&&&&&&&&&&&&&&&&&&& Funcao sensor Luz &&&&&&&&&&&&&&&&&&&&&&&&&&&& 
        if(valor_entry2 < 127)  //RECOLHE TELA -
        {
            recolheTela();
        }   
        else                    //EXPANDE TELA -
        {
            expandeTela();
        }
       //&&&&&&&&&&&&&&&&&&Final Sensor Luz
        //FUNÇÃO DE REGAR PLANTAS
        if(valor_entry1 < 64)
        {
            //&&&&&&&&&&&&&&&&&& Funcao Sensor solo 1&&&&&&&&&&&&&&&
            //Cuida estufa 1
            if(valor_entry3 < 128)
            {
                estufaUm();
            }            
        }
        if (valor_entry1 < 128)
        {
            //&&&&&&&&&&&&&&&&&& Funcao Sensor solo 2&&&&&&&&&&&&&&&
            //Cuida estufa 2
            if(valor_entry4 < 128)
            {
                estufaDois();
            }              
        }
        if (valor_entry1 < 192)
        {
           //&&&&&&&&&&&&&&&&&& Funcao Sensor solo 3 &&&&&&&&&&&&&&&
           //Cuida estufa 3
           if(valor_entry5 < 128)
           {
               estufaTres();
           }              
        }
        if (valor_entry1 < 256)
        {
            //&&&&&&&&&&&&&&&&&& Funcao Sensor solo 4 &&&&&&&&&&&&&&&
            //Cuida estufa 4
            if(valor_entry6 < 128)
            {
                estufaQuatro();
            }        
        }
        //&&&&&&&&&&&&&&&& FIM FUNÇÃO DE REGAR &&&&&&&&&&&&&&&&&&&& 
        //%%%%%%%%%%%%%%%%%%%%%% Final %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        __delay_ms(250);
    }
    return 0;
}
void __interrupt() TrataINT(void)
{ 
    if(INTF == 1)
    {
        PORTBbits.RB3 = 1; //DEBUG
        recolheTela();
        __delay_ms(600);
        expandeTela();
        INTF = 0;
        PORTBbits.RB3 = 0; //DEBUG
    }
    else if (TMR1IF == 1)
    { 
        TMR1IF = 0;
        TMR1L = 0xDC;
        TMR1H = 0x0B;
        
        CLRWDT (); 
        
        PORTBbits.RB3 = 1;
        atualizaLCD();
        PORTBbits.RB3 = 0;
    }
    return;
}
void estufaUm(void)
{
    RC6 = 1;
    __delay_ms(500);
    RC6 = 0;
    return;
}
void estufaDois(void)
{
    RC7 = 1;
    __delay_ms(500);
    RC7 = 0;
    return;
}
void estufaTres(void)
{
    RC4 = 1;
    __delay_ms(500);
    RC4 = 0;
    return;
}
void estufaQuatro(void)
{
    RC5 = 1;
    __delay_ms(500);
    RC5 = 0;
    return;
}
void lerSensores(void)
{
    //Sensor Chuva !!!!!!!! AN0
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 0;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry0 = ADRESH;                              // passa valores convertido do reg para a vari?vel
    
    //Sensor Agua !!!!!!!! AN1
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 0;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry1 = ADRESH;                              // passa valores convertido do reg para a vari?vel    
    
    //Sensor Luz!!!!!!!! AN2
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 0;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry2 = ADRESH;                              // passa valores convertido do reg para a vari?vel    
    
    //Sensor Solo 1 !!!!!!!! AN3
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 0;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry3 = ADRESH;                              // passa valores convertido do reg para a vari?vel    
    
    //Sensor Solo 2 !!!!!!!! AN4
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 1;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry4 = ADRESH;                              // passa valores convertido do reg para a vari?vel    
    
    //Sensor Solo 3 !!!!!!!! AN5
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 1;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry5 = ADRESH;                              // passa valores convertido do reg para a vari?vel    
    
    //Sensor Solo 4 !!!!!!!! AN6
    //Seleciona canal de entrada 0 como entrada anal?gica
    ADCON0bits.CHS0 = 0;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS1 = 1;                                //configura canal 0 como entrada anal?gica
    ADCON0bits.CHS2 = 1;                                //configura canal 0 como entrada anal?gica
       
    ADCON0bits.GO = 1;                                  //converte
    __delay_us(20);                                     //tempo de convers?o
    valor_entry6 = ADRESH;                              // passa valores convertido do reg para a vari?vel

    return;   
}
void recolheTela(void)
{
    while(RE2 == 0)
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
        
        __delay_ms(20);
    }
        
    //para motor
    RD4 = 1;
    RC1 = 1;
    RD2 = 1;
    RD6 = 1;

    RD0 = 1;
    RC3 = 1;
    RC2 = 1;
    RD7 = 1;
    
    //Desliga Ponte H
    RD5 = 0;
    RD3 = 0;
    RD1 = 0;
    RC0 = 0;
    
    return;
}
void expandeTela(void)
{
    while(RA4 == 0 && RB0 == 0)
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
        __delay_ms(20);
    }

           
    //para motor
    RD4 = 1;
    RC1 = 1;
    RD2 = 1;
    RD6 = 1;
           
    RD0 = 1;
    RC3 = 1;
    RC2 = 1;
    RD7 = 1;
    
    RD5 = 0;
    RD3 = 0;
    RD1 = 0;
    RC0 = 0;
    return;
}
void atualizaLCD(void)
{
    Lcd_Clear();                                        //Limpa LCD
    Lcd_Set_Cursor(1,1); 
    sprintf(buffer0, "C:%i A:%i L:%i S1:%i", (int)valor_entry0, (int)valor_entry1, (int)valor_entry2, (int)valor_entry3); 
    __delay_ms(20);
    sprintf(buffer1, "S2:%i S3:%i S4:%i ", (int)valor_entry4, (int)valor_entry5, (int)valor_entry6);
    __delay_ms(20);
    Lcd_Write_String(buffer0); 
    Lcd_Set_Cursor(2,1);
    Lcd_Write_String(buffer1); 
    __delay_ms(50);
    return;
}
void iniciaPinos(void)
{
    OPTION_REGbits.nRBPU = 1;
    //************* CONFIGURAÇÃO PINOS PIC *************************************
    TRISBbits.TRISB0 = 1;                               //Habilita R0 como entrada
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB5 = 0;
    TRISBbits.TRISB6 = 0;
    TRISBbits.TRISB7 = 0;
    
    OPTION_REGbits.INTEDG = 1;                          //
    INTCONbits.GIE = 1;                                 //Habilita interrupção global
    INTCONbits.INTE = 1;                                //
    INTCONbits.INTF = 0;                                //
    INTCONbits.PEIE = 1;
    
    OPTION_REGbits.PS0 = 1;                               //
    OPTION_REGbits.PS1 = 1;                              //
    OPTION_REGbits.PS2 = 1;                              //
    OPTION_REGbits.PSA = 1;                              //
    
    T1CONbits.TMR1CS = 0;                               //
    T1CONbits.T1CKPS0 = 1;                              //
    T1CONbits.T1CKPS1 = 1;                              //
    
    TMR1L = 0xDC;
    TMR1H = 0x0B;
    
    PIE1bits.TMR1IE=1; 
    
    T1CONbits.TMR1ON = 1;
    
    TRISA = 0b11111111;                                 //configura pinos de entrada(1)e sa?da (0)
    TRISC = 0b00000000;
    TRISD = 0b00000000;
    //*********** CONFIGURA O CONVERSOR ANALOGICO/DIGITAL **********************
    //Ficar somente com AN0 como entrada anal?gica. Tem tabela no datasheet
    ADCON1bits.PCFG0   = 0;                             //configura as entradas anal?gicas
    ADCON1bits.PCFG1   = 0;                             //configura as entradas anal?gicas
    ADCON1bits.PCFG2   = 0;                             //configura as entradas anal?gicas
    ADCON1bits.PCFG3   = 0;                             //configura as entradas anal?gicas

    //Define o clock de conversor
    ADCON0bits.ADCS0 = 0  ;                             //confirmando default Fosc/2
    ADCON0bits.ADCS1 = 0  ;                             //confirmando default Fosc/2

    ADCON1bits.ADFM = 0   ;                             //Default ? 0. Pra mostra que pegar? os dados em 8 bits 
                                                        //no ADRESH, pois est? justifica ? esquerda
                                                        //Passando pra 1 pode pegar os dados em 10 bits 
                                                        //usando os dois registradores ADRESHL justificado ?
                                                        //direita

    //Inicializa registradores do AD
    ADRESL = 0x00;                                      //inicializar valor analogico com 0
    ADRESH = 0x00;          

    ADCON0bits.ADON = 1;                                //Liga 
    
    PORTBbits.RB0 = 0;
    PORTBbits.RB1 = 0;
    PORTBbits.RB2 = 0;
    PORTBbits.RB3 = 0;
    PORTBbits.RB4 = 0;
    PORTBbits.RB5 = 0;
    PORTBbits.RB6 = 0;
    PORTBbits.RB7 = 0;
    
    PORTCbits.RC0 = 0;
    PORTCbits.RC1 = 0;
    PORTCbits.RC2 = 0;
    PORTCbits.RC3 = 0;
    PORTCbits.RC4 = 0;
    PORTCbits.RC5 = 0;
    PORTCbits.RC6 = 0;
    PORTCbits.RC7 = 0;
    
    PORTDbits.RD0 = 0;
    PORTDbits.RD1 = 0;
    PORTDbits.RD2 = 0;
    PORTDbits.RD3 = 0;
    PORTDbits.RD4 = 0;
    PORTDbits.RD5 = 0;
    PORTDbits.RD6 = 0;
    PORTDbits.RD7 = 0;
    
    return;
}