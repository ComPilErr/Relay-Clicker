#include <Arduino.h>

#define LED_PIN 13
#define OUT_PIN 3

volatile bool dark = true;
volatile uint8_t update_made = 0;

ISR ( TIMER3_COMPA_vect )
{
  update_made++;
  if (update_made >= 2)
  {
  dark = !dark;
  update_made = 0;
  digitalWrite(LED_PIN, dark);
  digitalWrite(OUT_PIN, dark);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  cli();
  TCCR3A=0; // нормальный режим работы таймера
  TCCR3B=0;
  OCR3A=0x1869; // it's 100 ms measure
  TCCR3B = 1<<CS32|0<<CS31|0<<CS30|0<<WGM33|1<<WGM32; // режим сравнения, делитель 256
  TIMSK3 = 0<<ICIE3|0<<OCIE3B|1<<OCIE3A|0<<TOIE3; // разрешение прерываний по сравнению
  TCNT3=0;
  //TIMSK3 &= ~(1<<OCIE3A); // turn off the timer
  TIMSK3 |= (1<<OCIE3A); // // turn on the timer
  sei();

}

void loop() {;}
