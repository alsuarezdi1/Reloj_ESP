//////////////////////////////////////////////////////////////////////////////////////////////////////
//                           Reloj por interrupcion para ESP 32/8266                                //
//                                Creado por Alfonso L. Suárez                                      //
//                                    alsuarezdi@gmail.com                                          //
//                                  modificado en Feb/9/2024                                        //
//////////////////////////////////////////////////////////////////////////////////////////////////////

#include <Arduino.h>

extern byte Clk_seg;       // Segundos en base 60
extern byte Clk_min;       //Minutos
extern byte Clk_hor;       //Horas
extern byte Clk_Lapse;     // Incrementa en 1 por cada interrupción (cada 1 segundos), puede ser reseteado y consultado externamente para cronometrar
extern byte Clk_Estado;    // Banderas del reloj
                            // Bit0: 1= Avisa al exterior que una interrupción y un incremento de 1 segundo se dio. Debe ser clareado externamente.

void Clk_Clear(void);
void Clk_Interrupt(void);

