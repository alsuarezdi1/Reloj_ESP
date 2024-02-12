#include "soc/rtc_cntl_reg.h"
#include "Reloj_ESP.h"

/* Crear un Timer en Hardware */
hw_timer_t * timer = NULL;
//portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//------------------------------------------------------------------------------------------------------
//Rutina de interrupción 
void IRAM_ATTR onTimer(){
  Clk_Interrupt();
}

//------------------------------------------------------------------------------------------------------
void setup() {
  // Desactiva la detección de reinicio por bajo voltaje (BOD)
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); // Desactiva el BOD

    // Desactiva el Watchdog Timer para el núcleo 0
  disableCore0WDT();

  // O desactiva el Watchdog Timer para el núcleo 1
  disableCore1WDT();
  //esp_task_wdt_disable(); // Desactivar el temporizador de vigilancia

  Serial.begin(115200);

  /*  1/(80MHZ/80) = 1us  */
  timer = timerBegin(0, 80, true);
 
  /* Attach the onTimer function to our timer */
  timerAttachInterrupt(timer, &onTimer, true);
 
  /* *Set the alarm clock to call the onTimer function every second 1 tick is 1us => 1 second is 1000000us * / 
  / *Repeat alarm (third parameter)*/
 
  timerAlarmWrite(timer, 1000000, true);
 
  /*Start alert*/
  timerAlarmEnable(timer);

  Clk_Clear();  //Reset reloj

  Serial.println("start timer");
}

//------------------------------------------------------------------------------------------------------ 
void loop() {
  byte a;

  while(true){
    while(a==Clk_Lapse){
      delay(100);
    }

    Serial.printf("%02d:%02d:%02d\n", Clk_hor, Clk_min, Clk_seg);    //no puede hacerse print dentro de la rutina de interrupciones porque el WDT resetea el módulo. por eso se hace aquí
    a=Clk_Lapse;
  }
 
}