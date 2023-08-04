#include <AudioFileSourceLittleFS.h>
#include <AudioFileSourcePROGMEM.h>
#include <AudioGeneratorRTTTL.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <AudioLogger.h>

/* Requirements:
 *
 * http://github.com/lorol/arduino-esp32fs-plugin/releases download esp32fs.zip
 *  ~/Arduino/tools/ESP32FS/tool/esp32fs.jar               <-   place file here
 * goto "Sketch" -> "Show Sketch Folder" -> create directory "data" -> file "example.mp3"
 * Use the "Tools" -> "ESP32 Sketch Data Upload" menu to upload the MP3 files to LittleFS
 *
 * http://github.com/earlephilhower/ESP8266Audio
 * "Tools" -> "Manage Libraries..." -> "ESP8266Audio"
 */

AudioFileSourceLittleFS *file;
AudioGeneratorMP3 *mp3;
AudioOutputI2S *out;

AudioFileSourcePROGMEM *file2;
AudioGeneratorRTTTL *rtttl;
AudioOutputI2S *out2;

TaskHandle_t mp3Task;

void playMP3Task(void *pvParameters) {
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  isMP3Playing = true;
  while (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      break;
    }
  }
  isMP3Playing = false;
  delete mp3;
  vTaskDelete(NULL);
}

void playMP3(const char *mp3File) {
  out = new AudioOutputI2S(0, 1);   // built-in DAC
//  out = new AudioOutputI2S();       // MAX98357A I2S
//  out->SetPinout(BCLK_PIN, WCLK_PIN, DOUT_PIN);
  out->SetGain(1.0);
  file = new AudioFileSourceLittleFS(mp3File);
  if (!isMP3Playing) {
    xTaskCreatePinnedToCore(
    playMP3Task,   // Task function.
    "MP3Task",     // name of task.
    6144,          // Stack size of task
    NULL,          // parameter of the task
    1,             // priority of the task
    &mp3Task,      // Task handle to keep track of created task
    1);            // pin task to core 1
  }
}

void beep(int freq, unsigned int duration) {
  out2 = new AudioOutputI2S(0, 1);   // built-in DAC
//  out2 = new AudioOutputI2S();       // MAX98357A I2S
//  out2->SetPinout(BCLK_PIN, WCLK_PIN, DOUT_PIN);
  out2->SetGain(1.0);
  unsigned long tempo = 240000 / duration;
  char rtttlString[26];
  String note;
  if (freq <= 262) note = "c4";
  else if (262 < freq && freq <= 277) note = abs(freq - 262) < abs(277 - freq) ? "c4" : "c#4";
  else if (277 < freq && freq <= 294) note = abs(freq - 277) < abs(294 - freq) ? "c#4" : "d4";
  else if (294 < freq && freq <= 311) note = abs(freq - 294) < abs(311 - freq) ? "d4" : "d#4";
  else if (311 < freq && freq <= 330) note = abs(freq - 311) < abs(330 - freq) ? "d#4" : "e4";
  else if (330 < freq && freq <= 349) note = abs(freq - 330) < abs(349 - freq) ? "e4" : "f4";
  else if (349 < freq && freq <= 370) note = abs(freq - 349) < abs(370 - freq) ? "f4" : "f#4";
  else if (370 < freq && freq <= 392) note = abs(freq - 370) < abs(392 - freq) ? "f#4" : "g4";
  else if (392 < freq && freq <= 415) note = abs(freq - 392) < abs(415 - freq) ? "g4" : "g#4";
  else if (415 < freq && freq <= 440) note = abs(freq - 415) < abs(440 - freq) ? "g#4" : "a4";
  else if (440 < freq && freq <= 466) note = abs(freq - 440) < abs(466 - freq) ? "a4" : "a#4";
  else if (466 < freq && freq <= 494) note = abs(freq - 466) < abs(494 - freq) ? "a#4" : "b4";
  else if (494 < freq && freq <= 523) note = abs(freq - 494) < abs(523 - freq) ? "b4" : "c5";
  else if (523 < freq && freq <= 554) note = abs(freq - 523) < abs(554 - freq) ? "c5" : "c#5";
  else if (554 < freq && freq <= 587) note = abs(freq - 554) < abs(587 - freq) ? "c#5" : "d5";
  else if (587 < freq && freq <= 622) note = abs(freq - 587) < abs(622 - freq) ? "d5" : "d#5";
  else if (622 < freq && freq <= 659) note = abs(freq - 622) < abs(659 - freq) ? "d#5" : "e5";
  else if (659 < freq && freq <= 698) note = abs(freq - 659) < abs(698 - freq) ? "e5" : "f5";
  else if (698 < freq && freq <= 740) note = abs(freq - 698) < abs(740 - freq) ? "f5" : "f#5";
  else if (740 < freq && freq <= 784) note = abs(freq - 740) < abs(784 - freq) ? "f#5" : "g5";
  else if (784 < freq && freq <= 831) note = abs(freq - 784) < abs(831 - freq) ? "g5" : "g#5";
  else if (831 < freq && freq <= 880) note = abs(freq - 831) < abs(880 - freq) ? "g#5" : "a5";
  else if (880 < freq && freq <= 932) note = abs(freq - 880) < abs(932 - freq) ? "a5" : "a#5";
  else if (932 < freq && freq <= 988) note = abs(freq - 932) < abs(988 - freq) ? "a#5" : "b5";
  else if (988 < freq && freq <= 1047) note = abs(freq - 988) < abs(1047 - freq) ? "b5" : "c6";
  else if (1047 < freq && freq <= 1109) note = abs(freq - 1047) < abs(1109 - freq) ? "c6" : "c#6";
  else if (1109 < freq && freq <= 1175) note = abs(freq - 1109) < abs(1175 - freq) ? "c#6" : "d6";
  else if (1175 < freq && freq <= 1245) note = abs(freq - 1175) < abs(1245 - freq) ? "d6" : "d#6";
  else if (1245 < freq && freq <= 1319) note = abs(freq - 1245) < abs(1319 - freq) ? "d#6" : "e6";
  else if (1319 < freq && freq <= 1397) note = abs(freq - 1319) < abs(1397 - freq) ? "e6" : "f6";
  else if (1397 < freq && freq <= 1480) note = abs(freq - 1397) < abs(1480 - freq) ? "f6" : "f#6";
  else if (1480 < freq && freq <= 1568) note = abs(freq - 1480) < abs(1568 - freq) ? "f#6" : "g6";
  else if (1568 < freq && freq <= 1661) note = abs(freq - 1568) < abs(1661 - freq) ? "g6" : "g#6";
  else if (1661 < freq && freq <= 1760) note = abs(freq - 1661) < abs(1760 - freq) ? "g#6" : "a6";
  else if (1760 < freq && freq <= 1865) note = abs(freq - 1760) < abs(1865 - freq) ? "a6" : "a#6";
  else if (1865 < freq && freq <= 1976) note = abs(freq - 1865) < abs(1976 - freq) ? "a#6" : "b6";
  else if (1976 < freq && freq <= 2093) note = abs(freq - 1976) < abs(2093 - freq) ? "b6" : "c7";
  else if (2093 < freq && freq <= 2217) note = abs(freq - 2093) < abs(2217 - freq) ? "c7" : "c#7";
  else if (2217 < freq && freq <= 2349) note = abs(freq - 2217) < abs(2349 - freq) ? "c#7" : "d7";
  else if (2349 < freq && freq <= 2489) note = abs(freq - 2349) < abs(2489 - freq) ? "d7" : "d#7";
  else if (2489 < freq && freq <= 2637) note = abs(freq - 2489) < abs(2637 - freq) ? "d#7" : "e7";
  else if (2637 < freq && freq <= 2794) note = abs(freq - 2637) < abs(2794 - freq) ? "e7" : "f7";
  else if (2794 < freq && freq <= 2960) note = abs(freq - 2794) < abs(2960 - freq) ? "f7" : "f#7";
  else if (2960 < freq && freq <= 3136) note = abs(freq - 2960) < abs(3136 - freq) ? "f#7" : "g7";
  else if (3136 < freq && freq <= 3322) note = abs(freq - 3136) < abs(3322 - freq) ? "g7" : "g#7";
  else if (3322 < freq && freq <= 3520) note = abs(freq - 3322) < abs(3520 - freq) ? "g#7" : "a7";
  else if (3520 < freq && freq <= 3729) note = abs(freq - 3520) < abs(3729 - freq) ? "a7" : "a#7";
  else if (3729 < freq && freq <= 3951) note = abs(freq - 3729) < abs(3951 - freq) ? "a#7" : "b7";
  else if (freq > 3951) note = "b7";
  snprintf(rtttlString, sizeof(rtttlString), "note:d=1,o=5,b=%d:%s", tempo, note);
  file2 = new AudioFileSourcePROGMEM(rtttlString, strlen_P(rtttlString));
  rtttl = new AudioGeneratorRTTTL();
  rtttl->begin(file2, out2);
  while (rtttl->isRunning()) {
    if (!rtttl->loop()) {
      rtttl->stop();
      break;
    }
  } 
  delete rtttl;
  delete file2;
  delete out2;
}
