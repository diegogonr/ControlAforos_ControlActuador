/*
 * ****************************ETAPA CONTROL-ACTUADOR************************ 
 * Descripción: Se tiene un torniquete el cual estará ubicado en cada salón, este
 * dispositivo es representado por un motor paso a paso. Tenemos 2 botonos el cuál 
 * uno de ellos representa la entrada de personas cada vez que es presionado y el otro
 * representa la salida de personas. Además, se tiene un tercer botón que será 
 * usado en caso de emergencia habilitando el torniquete para la salida de personas 
 */


#include <xc.h>
#include "Config.h"

#define a EMERGENCIA
#define Total_Personas 5

void Giro_IN (void);
void Giro_OUT (void);


int i;                                        //contador de número de personas
char motor=0;
char a=0;                                     //variable de emergencia 
char const HORARIO[4] = {  0b10001000,
                           0b11000000,
                           0b10010000,
                           0b10100000};


void InitPort(void){
    ADCON1bits.PCFG = 0xFF;                        //Pines en digital
    TRISCbits.RC0=1;                               //Entrada de personas
    TRISCbits.RC1=1;                               //Salida de personas
    TRISB=0b00000111; 
    TRISE=0x00;
    TRISA=0x00;
}

void Init_Int (void)
{     
    INTCON3bits.INT1E = 1;        // Habilitar INT0 
    INTCON3bits.INT1F = 0;        //Bandera 0
    INTCON2bits.INTEDG2 = 0;      //Flanco descendete
    INTCONbits.GIE = 1;   
}

void main(void) {

    InitPort();   
    Init_Int();
    while(1){
        if(EMERGENCIA==0)
        {
           if((PORTCbits.RC0 == 0) && (i<Total_Personas )) //se ejecuta cuando detecta el button 1
            {                              
                __delay_ms (200);                        //efecto rebote
                while(PORTCbits.RC0 == 0);               //si el bit sigue siendo 1 seguirá en este bucle
                i++;                                     //aumento en 1
                Giro_IN();                               //Giro de entrada
                LATA=i;                      
            }

           else if ((PORTCbits.RC1 == 0) && (i>0) )      //se ejecuta cuando detecta el button 2
            {
                __delay_ms (200);                        //efecto rebote
                while(PORTCbits.RC1 == 0);               //si el bit sigue siendo 0 seguirá en este bucle
                i--;                                     //disminución en 1
                Giro_OUT ();                             //Giro de salida
                LATA=i;                      
            }           
         }
        else 
         {
           while((EMERGENCIA == 0xff) && (i>0) )
           {
             Giro_OUT ();
             i--;
             LATA=i;                      
             __delay_ms (500);
           }
         }
        
     }                  
}

void __interrupt() INT_isr(void)
{
    EMERGENCIA = ~EMERGENCIA;
    LATEbits.LATE0=~LATEbits.LATE0;                      
    INTCON3bits.INT1F = 0; 
}

void Giro_IN (void)
{
    if (motor<3)
    {
        motor++;
        LATB=HORARIO[motor];
    }
    else 
    {
        motor=0;
        LATB=HORARIO[motor];                        
    }

}


void Giro_OUT (void)
{
    if (motor>0)
    {
        motor--;             
        LATB=HORARIO[motor];
    }
    else 
    {
        motor=3;
        LATB=HORARIO[motor];                        
    }
}
