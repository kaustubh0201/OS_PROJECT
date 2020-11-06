#ifndef __OSD__DRIVERS__DRIVER_H
#define __OSD__DRIVERS__DRIVER_H

namespace osd
{
    namespace drivers
    {
        // using drivers class so that we can define many types of drivers
        class Driver{
        public:
        // this is the base class, we will inherit from it
            Driver(); 
            ~Driver();

            virtual void Activate(); // used to activate the driver
            virtual int Reset(); // used to reset the driver to default
            virtual void Deactivate(); // used to deactivate the driver
        };

        class DriverManager{
        private:
            Driver* drivers[255]; // this is the number of drivers that we can put 
            int numDrivers; // driver that we currently working in

        public:
            DriverManager(); 
            void AddDriver(Driver* );    

            void ActivateAll(); 
            // this function will activate all the device drivers 
        };
    }
}

#endif