#include <Arduino.h>
#include "ProvisioningService.h"

void setup(void) 
{
  ServiceProvisioning.begin();
}

void loop() 
{
  ServiceProvisioning.run();
}
