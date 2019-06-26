/*
  Motocycle Break Flash  V1.0
  Coşkun ERGAN
  26.06.2019
*/

#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define Set_prescaler(x)   (CLKPR = (1 << CLKPCE), CLKPR = x)

volatile int enable_count = 0;
volatile int f_wdt = 1;

/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
ISR(WDT_vect)
{
  if (f_wdt == 0)
  {
    f_wdt = 1;
  }
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void enterSleep(void)
{
  ADCSRA &= ~(1 << ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  cli();
  sleep_enable();
  sleep_bod_disable();
  sei();
  sleep_cpu();
  sleep_disable();
  sei();
  ADCSRA |= (1 << ADEN);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void wakeUp()
{
  enable_count=225; // 30 dk
  //enable_count = 3; // 24sn test
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void setup() {

  //Set_prescaler(8);// cpu speed low

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(2, INPUT_PULLUP);

  for (int i = 0; i < 10; i++)
  {
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                       // wait for a second
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(100);
  }

  attachInterrupt(0, wakeUp, CHANGE);

  /* Clear the reset flag. */
  MCUSR &= ~(1 << WDRF);
  /* In order to change WDE or the prescaler, we need to
     set WDCE (This will allow updates for 4 clock cycles).
  */
  WDTCSR |= (1 << WDCE) | (1 << WDE);
  /* set new watchdog timeout prescaler value */
  WDTCSR = 1 << WDP0 | 1 << WDP3; /* 8.0 seconds */
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
void loop() {
  if (f_wdt == 1)
  {
    f_wdt = 0;
    if (enable_count)
    {
      enable_count--;
      digitalWrite(3, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(30);                       // wait for a second
      digitalWrite(3, LOW);    // turn the LED off by making the voltage LOW
    }
  }
  enterSleep();
}
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
