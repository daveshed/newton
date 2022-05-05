from newton.hcomms import hcomms_ext
interface = hcomms_ext.HostInterface(0x11)
print(interface.get_reading().raw_data)
