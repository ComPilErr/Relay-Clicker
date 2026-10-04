#include <Arduino.h>

#define LED_PIN 13
#define OUT_PIN 3

volatile bool dark = true;
volatile uint8_t update_made = 0;
volatile uint8_t disable_update_made = 0;

ISR ( TIMER3_COMPA_vect )
{
  disable_update_made++;
  if (disable_update_made > 1*3){
  digitalWrite(LED_PIN, LOW);
  digitalWrite(OUT_PIN, LOW);
  disable_update_made=0;
  TIMSK3 &= ~(1<<OCIE3A);TCNT3=0;
  }
}

ISR ( TIMER4_COMPA_vect )
{
  update_made++;
  if (update_made >= 1*2)
  {
  update_made = 0;
  digitalWrite(LED_PIN, HIGH);
  digitalWrite(OUT_PIN, HIGH);
  TCNT3=0; TIMSK3 |= (1<<OCIE3A);
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
  OCR3A=0x270; // it's 10 ms measure
  TCCR3B = 1<<CS32|0<<CS31|0<<CS30|0<<WGM33|1<<WGM32; // режим сравнения, делитель 256
  TIMSK3 = 0<<ICIE3|0<<OCIE3B|1<<OCIE3A|0<<TOIE3; // разрешение прерываний по сравнению
  TCNT3=0;
  TIMSK3 &= ~(1<<OCIE3A); // turn off the timer
  //TIMSK3 |= (1<<OCIE3A); // // turn on the timer
  sei();

  cli();
  TCCR4A=0; // нормальный режим работы таймера
  TCCR4B=0;
  OCR4A=0xC34; // it's 50 ms measure
  TCCR4B = 1<<CS42|0<<CS41|0<<CS40|0<<WGM43|1<<WGM42; // режим сравнения, делитель 256
  TIMSK4 = 0<<ICIE4|0<<OCIE4B|1<<OCIE4A|0<<TOIE4; // разрешение прерываний по сравнению
  TCNT4=0;
  //TIMSK4 &= ~(1<<OCIE4A); // turn off the timer
  TIMSK4 |= (1<<OCIE4A); // // turn on the timer
  sei();

}

void loop() {;}
