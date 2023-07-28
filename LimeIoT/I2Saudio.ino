#include <AudioFileSourceLittleFS.h>
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

TaskHandle_t mp3Task;

void playMP3Task(void *pvParameters) {
  mp3 = new AudioGeneratorMP3();
  mp3->begin(file, out);
  while (mp3->isRunning()) {
    if (!mp3->loop()) {
      mp3->stop();
      break;
    }
  }
  vTaskDelete(NULL);
}

void playMP3(const char *mp3File) {
  file = new AudioFileSourceLittleFS(mp3File);
  out = new AudioOutputI2S(0, 1);
  out->SetGain(1.0);
  xTaskCreatePinnedToCore(
  playMP3Task,   // Task function.
  "MP3Task",     // name of task.
  6144,          // Stack size of task
  NULL,          // parameter of the task
  1,             // priority of the task
  &mp3Task,      // Task handle to keep track of created task
  1);            // pin task to core 1
}
