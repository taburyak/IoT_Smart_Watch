#include <Arduino.h>
#include "ProvisioningService.h"

void setup(void) 
{
  Serial.begin(115200);
  ServiceProvisioning.begin();
}

void loop() 
{
  ServiceProvisioning.run();
}
