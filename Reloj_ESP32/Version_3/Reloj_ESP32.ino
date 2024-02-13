//////////////////////////////////////////////////////////////////////////////////////////////////////
//                           Reloj por interrupcion para ESP 32/8266                                //
//                                Creado por Alfonso L. Suárez                                      //
//                                    alsuarezdi@gmail.com                                          //
//                                  modificado en Feb/9/2024                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>


#include <NTPClient.h>
#include <WiFiUdp.h>
#include "Reloj_ESP.h"

const char* ssid = "KT_GiGA_8999";		//Usa las tuyas. Las mias no te servirán. Ni sabes donde vivo... Ni pagarás el viaje
const char* password = "21ik16ki46";

const long utcOffsetInSeconds = 9*3600; // Ajusta el offset UTC según tu zona horaria  (GMT+9)
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", utcOffsetInSeconds);

/* Crear un Timer en Hardware */
hw_timer_t * timer = NULL;

//------------------------------------------------------------------------------------------------------
//Rutina de interrupción 
void IRAM_ATTR onTimer(){
  Clk_Interrupt();
}

//------------------------------------------------------------------------------------------------------
void setup() {

  Serial.begin(115200);

  //-----------------------------------------

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

  //-----------------------------------------
   // Conexión WiFi
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }  
  Serial.println("Conectado a la red WiFi");

  // Inicialización del cliente NTP
  timeClient.begin();
  timeClient.update(); // Actualizar la hora desde el servidor NTP

  // Obtener y mostrar la hora actual
  //int year = timeClient.getYear();
  //int month = timeClient.getMonth();
  //int day = timeClient.getDay();
  Clk_hor = timeClient.getHours();
  Clk_min = timeClient.getMinutes();
  Clk_seg = timeClient.getSeconds();

  // Desconectar el cliente NTP
  timeClient.end();
  // Desconectar y apagar el módulo WiFi
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
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