
#include <usbh_midi.h>
#include <usbhub.h>
#include <stdlib.h>

#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

USB Usb;
//USBHub Hub(&Usb);
USBH_MIDI  Midi(&Usb);
long strtol (const char *__nptr, char **__endptr, int __base);

void MIDI_poll();
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime);

boolean bFirst;
uint16_t pid, vid;

void setup()
{
  bFirst = true;
  vid = pid = 0;
  Serial.begin(115200);

  if (Usb.Init() == -1) {
    while (1); //halt
  }//if (Usb.Init() == -1...
  delay( 200 );
}

void loop()
{
  Usb.Task();
  //uint32_t t1 = (uint32_t)micros();
  if ( Usb.getUsbTaskState() == USB_STATE_RUNNING )
  {
    MIDI_poll();
  }
}

// Poll USB MIDI Controler and send to serial MIDI
void MIDI_poll()
{
  char buf[20];
  uint8_t bufMidi[64];
  uint16_t  rcvd;
  if (Midi.RecvData( &rcvd,  bufMidi) == 0 ) {
    sprintf(buf, " %02X", bufMidi[2]);
	int decimal_answer = strtol(buf, NULL, 16);
	convertToNote(decimal_answer);
  }
}

// Delay time (max 16383 us)
void doDelay(uint32_t t1, uint32_t t2, uint32_t delayTime)
{
  uint32_t t3;

  if ( t1 > t2 ) {
    t3 = (0xFFFFFFFF - t1 + t2);
  } else {
    t3 = t2 - t1;
  }

  if ( t3 < delayTime ) {
    delayMicroseconds(delayTime - t3);
  }
}
void convertToNote(int Dec){
String Note[100] = {
"C1","C1#","D1","D1#","E1","F1","F1#","G1","G1#","A1","A1#","B1",
"C2","C2#","D2","D2#","E2","F2","F2#","G2","G2#","A2","A2#","B2",
"C3","C3#","D3","D3#","E3","F3","F3#","G3","G3#","A3","A3#","B3",
"C4","C4#","D4","D4#","E4","F4","F4#","G4","G4#","A4","A4#","B4",
"C5","C5#","D5","D5#","E5","F5","F5#","G5","G5#","A5","A5#","B5",
"C6"
};

for (int i = 36; i <= 96; i++)
{
if (Dec == i)
{
	Serial.println(Note[i - 36]);
}
}

}
