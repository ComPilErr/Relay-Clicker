#include <Arduino.h>

#define LED_PIN 13
#define OUT_PIN 3

volatile uint8_t update_made = 0;
volatile uint8_t array[32] = {0,1,4,8,16,28,32,48,64,72,80,110,118,120,128,128,128,128,120,118,110,80,72,64,48,32,28,16,8,4,1,0};
volatile uint8_t * volatile my_pointer = array;

ISR ( TIMER4_COMPA_vect ) // play timer period
{
  update_made++;
  if (update_made >= 1) // 1 times x 50 ms = 50 ms play time
  {
  update_made = 0;
  OCR2B = (uint8_t) (*my_pointer);
  my_pointer++;
  if (my_pointer >= array + sizeof(array)) { my_pointer = array; }
  }
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  pinMode(OUT_PIN, OUTPUT);
  digitalWrite(OUT_PIN, LOW);

  cli();
  TCCR4A=0; // нормальный режим работы таймера
  TCCR4B=0;
  OCR4A=0xC34; // it's 50 ms
  TCCR4B = 1<<CS42|0<<CS41|0<<CS40|0<<WGM43|1<<WGM42; // режим сравнения, делитель 256
  TIMSK4 = 0<<ICIE4|0<<OCIE4B|1<<OCIE4A|0<<TOIE4; // разрешение прерываний по сравнению
  TCNT4=0;
  TIMSK4 |= (1<<OCIE4A); // turn on the timer
  sei();

  cli();
  // Example: Generate ~62.50 kHz PWM (assuming a 16 MHz system clock)
  // Formula: 16,000,000 / (1 * (255 + 1)) = 62,500 Hz
  // Prescaler 1 corresponds to CS20 bit (_BV(CS20))
  TCCR2A = 0; // нормальный режим работы таймера
  TCCR2B = 0;

  // 3. Configure Fast PWM Mode 7 (TOP = OCR2A) and Non-Inverting mode on OC2B
  // COM2B1 = 1 (Clear OC2B on compare match, set at BOTTOM)
  // WGM21 = 1, WGM20 = 1 (Part 1 of Mode 7)
  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);

  // 4. Set WGM22 = 1 (Part 2 of Mode 7) and apply prescaler bits
  TCCR2B = _BV(WGM22) | (_BV(CS20) & 0x07);

  // 5. Set the frequency ceiling (TOP) and initial duty cycle (0%)
  OCR2A = 255;
  OCR2B = 0;
  sei();

}

void loop() {;}
