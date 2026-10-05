#pragma once

#include <Arduino.h>
#include <Wifi.h>


void printBoardInfo()
{
    Serial.println();
    Serial.println("========================================");
    Serial.println("ESP Board Information");
    Serial.println("========================================");

#ifdef ARDUINO_BOARD
    Serial.printf("Board           : %s\n", ARDUINO_BOARD);
#else
    Serial.println("Board           : Unknown");
#endif

#ifdef ARDUINO
    Serial.printf("Arduino Core    : %d\n", ARDUINO);
#endif

#ifdef ESP_ARDUINO_VERSION_MAJOR
    Serial.printf("Arduino Version : %d.%d.%d\n",
                  ESP_ARDUINO_VERSION_MAJOR,
                  ESP_ARDUINO_VERSION_MINOR,
                  ESP_ARDUINO_VERSION_PATCH);
#endif

    Serial.printf("Chip Model      : %s\n", ESP.getChipModel());
    Serial.printf("Chip Revision   : %d\n", ESP.getChipRevision());
    Serial.printf("CPU Cores       : %d\n", ESP.getChipCores());
    Serial.printf("CPU Frequency   : %d MHz\n", ESP.getCpuFreqMHz());

    Serial.printf("SDK Version     : %s\n", ESP.getSdkVersion());

    Serial.printf("Flash Size      : %.2f MB\n",
                  ESP.getFlashChipSize() / 1024.0 / 1024.0);

    Serial.printf("Flash Speed     : %u MHz\n",
                  ESP.getFlashChipSpeed() / 1000000);

    Serial.printf("Free Heap       : %u\n", ESP.getFreeHeap());
    Serial.printf("Largest Block   : %u\n", ESP.getMaxAllocHeap());
    Serial.printf("Min Free Heap   : %u\n", ESP.getMinFreeHeap());

    Serial.printf("PSRAM Present   : %s\n",
                  psramFound() ? "YES" : "NO");

    if (psramFound())
    {
        Serial.printf("PSRAM Size      : %.2f MB\n",
                      ESP.getPsramSize() / 1024.0 / 1024.0);

        Serial.printf("Free PSRAM      : %u\n",
                      ESP.getFreePsram());
    }

    Serial.printf("Sketch Size     : %u\n",
                  ESP.getSketchSize());

    Serial.printf("Free Sketch     : %u\n",
                  ESP.getFreeSketchSpace());

    Serial.printf("Build Date      : %s\n", __DATE__);
    Serial.printf("Build Time      : %s\n", __TIME__);
    Serial.printf("Compiler        : GCC %d.%d.%d\n",
                  __GNUC__,
                  __GNUC_MINOR__,
                  __GNUC_PATCHLEVEL__);

#ifdef CONFIG_IDF_TARGET_ESP32
    Serial.println("Target          : ESP32");
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S2
    Serial.println("Target          : ESP32-S2");
#endif

#ifdef CONFIG_IDF_TARGET_ESP32S3
    Serial.println("Target          : ESP32-S3");
#endif

#ifdef CONFIG_IDF_TARGET_ESP32C3
    Serial.println("Target          : ESP32-C3");
#endif

#ifdef CONFIG_IDF_TARGET_ESP32C6
    Serial.println("Target          : ESP32-C6");
#endif

    Serial.println("========================================");
}