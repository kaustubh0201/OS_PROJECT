#include <drivers/driver.h>
#include <common/types.h>
using namespace osd::common;
using namespace osd::drivers;

Driver :: Driver()
{}

Driver :: ~Driver()
{}

void Driver :: Activate()
{}

int Driver :: Reset()
{
    return 0;
}

void Driver :: Deactivate()
{

}

DriverManager :: DriverManager()
{
    numDrivers = 0;
}

void DriverManager :: AddDriver(Driver* drv)
{
    // if an index is set with the driver then we go to next 
    drivers[numDrivers] = drv;
    numDrivers++;
}

void DriverManager :: ActivateAll()
{
    for(int i = 0; i < numDrivers; i++)
    {
        drivers[i] -> Activate(); 
    }
    // here iterating through and activating all the drivers
}