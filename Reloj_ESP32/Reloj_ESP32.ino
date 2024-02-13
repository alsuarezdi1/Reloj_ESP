//////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Reloj RTC para ESP 32                                          //
//                                Creado por Alfonso L. Suárez                                      //
//                                    alsuarezdi@gmail.com                                          //
//                                  modificado en Feb/13/2024                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <time.h>

const char* ssid = "KT_GiGA_8999";		//Usa las tuyas. Las mias no te servirán. Ni sabes donde vivo... Ni pagarás el viaje
const char* password = "21ik16ki46";
bool timeSet = false;

void setup() {
  Serial.begin(115200);
  connectToWiFi();
  if (initRTC()) {
    timeSet = true;
    // Apagar la conexión WiFi
    WiFi.disconnect(true);
  }
}

void loop() {
  if (timeSet) {
    printLocalTime();
  }
  delay(1000);
}

void connectToWiFi() {
  Serial.println("Conectando a WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conexión fallida. Volviendo a intentar...");
  }
  Serial.println("Conectado a la red WiFi.");
}

bool initRTC() {
  configTime(9 * 3600, 0, "pool.ntp.org");
  Serial.println("RTC configurado");
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return false;
  }
  return true;
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Error al obtener la hora");
    return;
  }
  Serial.print("Local Time: ");
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  // Serial.print(". Wday: ");
  // Serial.println(timeinfo.tm_wday);
}