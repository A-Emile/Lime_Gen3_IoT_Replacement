/*
4C 42 44 43 50 01 10   1B 00 08   03 ... three LED bytes here ... 03
Byte   Meaning
12   red
13   yellow
14   green
*/
typedef enum {
  red    = 12,
  yellow = 13,
  green  = 14,
} LEDbytePosition;
/*
LED byte has two bits = bit for blink + bit for power
LED Bits:
Bits   State
00   off
01   on
11   blink
*/
typedef enum {
  off   = 0b00,
  on    = 0b01,
  blink = 0b11,
} LEDstate;

void sendDisplayLED(LEDbytePosition pos, LEDstate state);
