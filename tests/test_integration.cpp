#include "mocks.h"
#include "../forcesensor.h"
FakeSensor sensor_x, sensor_y, sensor_z;
ForceSensor::Plate force_plate(sensor_x, sensor_y, sensor_z);

int main(int argc, char const *argv[])
{
    // open serial handle from argv
    // TODO
    // write some canned data to the forceplate
    millis(1UL);
    sensor_x.raw_data(1L);
    sensor_y.raw_data(1L);
    sensor_z.raw_data(1L);
    // keep transmitting data
    while true
    {
        force_plate.update();
        force_plate.transmit(serial);
        sleep(1sec);
    }
    return 0;
}