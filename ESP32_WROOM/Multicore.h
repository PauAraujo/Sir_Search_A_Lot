
void runOnCore(void (*fnToRun)(void*), String task, int core){
  TaskHandle_t Task;

  char taskName[task.length()];
  task.toCharArray(taskName, task.length());
  
  xTaskCreatePinnedToCore(
    fnToRun,
    taskName,
    10000,
    NULL,
    1,
    &Task,
    0);
}
