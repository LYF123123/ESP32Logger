#include "configuration.h"
#include "task.hpp"
#include "blink.hpp"
#include "aesBenchmark.hpp"
#include "memoryStatus.hpp"
void testTask()
{
  typedef void (*TaskFunc_t)(void *);
  TaskFunc_t logTaskFunctions[] = {taskE, taskW, taskI, taskD, taskV};

  // taskHandle
  TaskHandle_t logTaskHandles[10];
  for (int i = 0; i < 10; i++)
  {
    int typeIndex = i % 5;

    char taskName[16];
    const char *levelChars[] = {"E", "W", "I", "D", "V"};
    sprintf(taskName, "Task%s_%d", levelChars[typeIndex], i);

    xTaskCreatePinnedToCore(
        logTaskFunctions[typeIndex], 
        taskName,
        configMINIMAL_STACK_SIZE + 4096,
        (void *)i, 
        configMAX_PRIORITIES - 3,
        &logTaskHandles[i],
        (i < 5 ? 0 : 1));
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  xTaskCreate(taskLogger, "logger", configMINIMAL_STACK_SIZE + 4096, NULL, configMAX_PRIORITIES - 5, NULL);
  vTaskDelay(pdMS_TO_TICKS(2000));
  xTaskCreate(taskLedBlink, "ledBlink", configMINIMAL_STACK_SIZE + 2048, NULL, configMAX_PRIORITIES - 4, NULL);
  xTaskCreate(taskAES, "AES", configMINIMAL_STACK_SIZE + 4096, NULL, configMAX_PRIORITIES - 4, NULL);
  xTaskCreate(taskMemoryStatus, "MemoryStatus", configMINIMAL_STACK_SIZE + 4096, NULL, configMAX_PRIORITIES - 4, NULL);

  testTask();
}

void loop()
{
}
