typedef enum {
  red    = 12,
  yellow = 13, // or blue
  green  = 14
} LEDbyteOffset;

typedef enum {
  off   = 0b00,
  on    = 0b01,
  blink = 0b11
} LEDstate;

void sendDisplayLED(LEDbyteOffset offset, LEDstate state);
/*
Note: on some Displays send yellow for blue LED
0x00 0b000000 // off
0x01 0b000001 // green on
0x03 0b000011 // green blink
0x04 0b000100 // yellow on
0x05 0b000101 // yellow on + green on
0x07 0b000111 // yellow on + green blink
0x0C 0b001100 // yellow blink
0x0D 0b001101 // yellow blink + green on
0x0F 0b001111 // yellow blink + green blink
0x10 0b010000 // red on
0x11 0b010001 // red on + green on
0x13 0b010011 // red on + green blink
0x14 0b010100 // red on + yellow on
0x15 0b010101 // red on + yellow on + green on
0x17 0b010111 // red on + yellow on + green blink
0x1C 0b011100 // red on + yellow blink
0x1D 0b011101 // red on + yellow blink + green on
0x1F 0b011111 // red on + yellow blink + green blink
0x30 0b110000 // red blink
0x31 0b110001 // red blink + green on
0x33 0b110011 // red blink + green blink
0x34 0b110100 // red blink + yellow on
0x35 0b110101 // red blink + yellow on + green on
0x37 0b110111 // red blink + yellow on + green blink
0x3C 0b111100 // red blink + yellow blink
0x3D 0b111101 // red blink + yellow blink + green on
0x3F 0b111111 // red blink + yellow blink + green blink
*/
