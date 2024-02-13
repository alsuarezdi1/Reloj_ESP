//////////////////////////////////////////////////////////////////////////////////////////////////////
//                           Reloj por interrupcion para ESP 32/8266                                //
//                                Creado por Alfonso L. Suárez                                      //
//                                    alsuarezdi@gmail.com                                          //
//                                  modificado en Feb/9/2024                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////


//Debe definir estas lineas en el programa principal para configurar la interrupcion por timer:
/*
// Crear un Timer en Hardware
hw_timer_t * timer = NULL;
/---------------------------------------------
//Rutina de interrupción 
void IRAM_ATTR onTimer(){
  Clk_Interrupt();
}
/---------------------------------------------
//Esto va en el setup 
  timer = timerBegin(0, 80, true);              //  1/(80MHZ/80) = 1us 
  timerAttachInterrupt(timer, &onTimer, true);  // Attach the onTimer function to our timer 
  // *Set the alarm clock to call the onTimer function every second 1 tick is 1us => 1 second is 1000000us 
  //Repeat alarm (third parameter) 
  timerAlarmWrite(timer, 1000000, true); 
  //Start alert
  timerAlarmEnable(timer);
  Clk_Clear();  //Reset reloj
*/

//---------------------------------------------------------------------------------------------
#include "Reloj_ESP.h"
#include <Arduino.h>

byte Clk_seg;       //Segundos en base 60
byte Clk_min;       //Minutos
byte Clk_hor;       //Horas
byte Clk_Lapse;     //Incrementa en 1 por cada interrupción (cada 1 segundos), puede ser reseteado y consultado externamente para cronometrar
byte Clk_Estado;    //Banderas del reloj
    //Bit0:	;1= Avisa al exterior que una interrupción y un incremento de 1 segundo se dió. Debe ser clareado externamente.

//---------------------------------------------------------------------------------------------
//Configuración inicial del reloj
void Clk_Clear(void) {
  Clk_seg = 0;
  Clk_min = 0;
  Clk_hor = 0;

}

//---------------------------------------------------------------------------------------------
//Llamar a esta rutina dentro de la rutina principal de interrupciones
void Clk_Interrupt(void){

  Clk_Lapse = Clk_Lapse+1;        //incrementa el cronometro Lapse en 1
  Clk_seg = Clk_seg +1;           //Hace un incremento al reloj
  
  if(Clk_seg>=60){                //See if MINUTES should be incremented
      Clk_seg=0;                  //Reset SECONDS
      Clk_min++;                  //Incrementa minutos
  }

  if(Clk_min>59){                 //See if Hours should be changed
    Clk_min=0;                    //Reset minutes
    Clk_hor++;                    //Incrementa hour
  }
  
  if(Clk_hor>23){                 //See if day should be changed
    Clk_hor=0;                    //Reset hora
  }
  
  Clk_Estado = Clk_Estado & 0x01; //Avisa del cambio

}