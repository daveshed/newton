#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include "CppLinuxSerial/SerialPort.hpp"

#include "serialcomms.h"

using namespace mn::CppLinuxSerial;

char const* greet()
{
   SerialPort serialPort(
      "/dev/ttyACM0",
      BaudRate::B_9600,
      NumDataBits::EIGHT,
      Parity::NONE,
      NumStopBits::ONE);
   serialPort.SetTimeout(-1);  // blocking
   serialPort.Open();
   serialPort.Close();
   return "hello, world";
}

BOOST_PYTHON_MODULE(hello_ext)
{
   using namespace boost::python;
   def("greet", greet);
}