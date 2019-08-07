
/*

 ---------------
| 1 2 | 3 4 5 6 |
 ---------------

 1 - GND    to Arduino ground
 2 - Vcc    to Arduino +5v
 3 - Motor  to Arduino +5v
 4 - Read   to Arduino pin 2
 5 - Write  N/C
 6 - Sense  N/C

http://unusedino.de/ec64/technical/formats/tap.html
*/


/* (stty raw; cat > received.log) < /dev/<your_serial_port> */

/* TAP v0 header. Size is set to 0 as we don't know at the time of sending. Tapclean accepts this and fixes it. */
const unsigned char tapv0header[] = { 0x43,0x36,0x34,0x2D,0x54,0x41,0x50,0x45,0x2D,0x52,0x41,0x57,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

void tapsendbit(){
  /* Stop timer */
  TCCR2B = 0;

  /* Send '0' if overflow else counter value */
  Serial.write((TIFR2 & _BV(TOV2)) ? 0 : TCNT2);

  /* Reset and restart counter */
  /* Resetting TCNT2 to zero does not seem to work, my guess is that setting TCNT2 = 0 triggers overflow */ 
  TCNT2 = 1;
  TIFR2 = _BV(TOV2);
  TCCR2B = _BV(CS22) | _BV(CS20);
}

void setup() {
  /* Setup serial */
  Serial.begin(115200);

  /* Send TAP header */
  Serial.write(tapv0header, sizeof(tapv0header)/sizeof(tapv0header[0]));
  Serial.flush();

  /* Start timer/counter2 to count at 16000000/128=125000Hz */
  /* That is close enough to 123156Hz that it shouldn't matter */
  TCCR2B = _BV(CS22) | _BV(CS20);        //Timer2 Control Reg B: Timer Prescaler set to 128

  /* Set up external interrupt 0 (pin 2) */
  attachInterrupt(0, tapsendbit, FALLING);
}

void loop() {
}
