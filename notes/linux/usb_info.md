# Linux下，查看USB设备信息
打印USB总线上所有USB设备的信息
```shell
root@ubuntu22:/sys/kernel/debug/usb# cat devices 

T:  Bus=01 Lev=00 Prnt=00 Port=00 Cnt=00 Dev#=  1 Spd=12   MxCh= 2
B:  Alloc= 17/900 us ( 2%), #Int=  1, #Iso=  0
D:  Ver= 1.10 Cls=09(hub  ) Sub=00 Prot=00 MxPS=64 #Cfgs=  1
P:  Vendor=1d6b ProdID=0001 Rev= 6.05
S:  Manufacturer=Linux 6.5.0-41-generic uhci_hcd
S:  Product=UHCI Host Controller
S:  SerialNumber=0000:02:00.0
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   2 Ivl=255ms

T:  Bus=01 Lev=01 Prnt=01 Port=00 Cnt=01 Dev#=  2 Spd=12   MxCh= 0
D:  Ver= 1.10 Cls=00(>ifc ) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
P:  Vendor=0e0f ProdID=0003 Rev= 1.03
S:  Manufacturer=VMware
S:  Product=VMware Virtual USB Mouse
C:* #Ifs= 1 Cfg#= 1 Atr=c0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=03(HID  ) Sub=01 Prot=02 Driver=usbhid
E:  Ad=81(I) Atr=03(Int.) MxPS=   8 Ivl=1ms

T:  Bus=01 Lev=01 Prnt=01 Port=01 Cnt=02 Dev#=  3 Spd=12   MxCh= 7
D:  Ver= 1.10 Cls=09(hub  ) Sub=00 Prot=00 MxPS= 8 #Cfgs=  1
P:  Vendor=0e0f ProdID=0002 Rev= 1.00
S:  Manufacturer=VMware, Inc.
S:  Product=VMware Virtual USB Hub
C:* #Ifs= 1 Cfg#= 1 Atr=e0 MxPwr=  0mA
I:* If#= 0 Alt= 0 #EPs= 1 Cls=09(hub  ) Sub=00 Prot=00 Driver=hub
E:  Ad=81(I) Atr=03(Int.) MxPS=   1 Ivl=255ms

```

[Documentation/usb/proc_usb_info.txt](https://www.kernel.org/doc/Documentation/usb/proc_usb_info.txt) 文件。
```txt
Each line is tagged with a one-character ID for that line:

T = Topology (etc.)
B = Bandwidth (applies only to USB host controllers, which are
    virtualized as root hubs)
D = Device descriptor info.
P = Product ID info. (from Device descriptor, but they won't fit
    together on one line)
S = String descriptors.
C = Configuration descriptor info. (* = active configuration)
I = Interface descriptor info.
E = Endpoint descriptor info.

 
Legend:
  d = decimal number (may have leading spaces or 0's)
  x = hexadecimal number (may have leading spaces or 0's)
  s = string
 
Topology info:

T:  Bus=dd Lev=dd Prnt=dd Port=dd Cnt=dd Dev#=ddd Spd=dddd MxCh=dd
|   |      |      |       |       |      |        |        |__MaxChildren
|   |      |      |       |       |      |        |__Device Speed in Mbps
|   |      |      |       |       |      |__DeviceNumber
|   |      |      |       |       |__Count of devices at this level
|   |      |      |       |__Connector/Port on Parent for this device
|   |      |      |__Parent DeviceNumber
|   |      |__Level in topology for this bus
|   |__Bus number
|__Topology info tag

    Speed may be:
    	1.5	Mbit/s for low speed USB
	12	Mbit/s for full speed USB
	480	Mbit/s for high speed USB (added for USB 2.0);
		  also used for Wireless USB, which has no fixed speed
	5000	Mbit/s for SuperSpeed USB (added for USB 3.0)

    For reasons lost in the mists of time, the Port number is always
    too low by 1.  For example, a device plugged into port 4 will
    show up with "Port=03".

Bandwidth info:
B:  Alloc=ddd/ddd us (xx%), #Int=ddd, #Iso=ddd
|   |                       |         |__Number of isochronous requests
|   |                       |__Number of interrupt requests
|   |__Total Bandwidth allocated to this bus
|__Bandwidth info tag

    Bandwidth allocation is an approximation of how much of one frame
    (millisecond) is in use.  It reflects only periodic transfers, which
    are the only transfers that reserve bandwidth.  Control and bulk
    transfers use all other bandwidth, including reserved bandwidth that
    is not used for transfers (such as for short packets).

    The percentage is how much of the "reserved" bandwidth is scheduled by
    those transfers.  For a low or full speed bus (loosely, "USB 1.1"),
    90% of the bus bandwidth is reserved.  For a high speed bus (loosely,
    "USB 2.0") 80% is reserved.


Device descriptor info & Product ID info:

D:  Ver=x.xx Cls=xx(s) Sub=xx Prot=xx MxPS=dd #Cfgs=dd
P:  Vendor=xxxx ProdID=xxxx Rev=xx.xx

where
D:  Ver=x.xx Cls=xx(sssss) Sub=xx Prot=xx MxPS=dd #Cfgs=dd
|   |        |             |      |       |       |__NumberConfigurations
|   |        |             |      |       |__MaxPacketSize of Default Endpoint
|   |        |             |      |__DeviceProtocol
|   |        |             |__DeviceSubClass
|   |        |__DeviceClass
|   |__Device USB version
|__Device info tag #1

where
P:  Vendor=xxxx ProdID=xxxx Rev=xx.xx
|   |           |           |__Product revision number
|   |           |__Product ID code
|   |__Vendor ID code
|__Device info tag #2


String descriptor info:

S:  Manufacturer=ssss
|   |__Manufacturer of this device as read from the device.
|      For USB host controller drivers (virtual root hubs) this may
|      be omitted, or (for newer drivers) will identify the kernel
|      version and the driver which provides this hub emulation.
|__String info tag

S:  Product=ssss
|   |__Product description of this device as read from the device.
|      For older USB host controller drivers (virtual root hubs) this
|      indicates the driver; for newer ones, it's a product (and vendor)
|      description that often comes from the kernel's PCI ID database.
|__String info tag

S:  SerialNumber=ssss
|   |__Serial Number of this device as read from the device.
|      For USB host controller drivers (virtual root hubs) this is
|      some unique ID, normally a bus ID (address or slot name) that
|      can't be shared with any other device.
|__String info tag



Configuration descriptor info:

C:* #Ifs=dd Cfg#=dd Atr=xx MPwr=dddmA
| | |       |       |      |__MaxPower in mA
| | |       |       |__Attributes
| | |       |__ConfiguratioNumber
| | |__NumberOfInterfaces
| |__ "*" indicates the active configuration (others are " ")
|__Config info tag

    USB devices may have multiple configurations, each of which act
    rather differently.  For example, a bus-powered configuration
    might be much less capable than one that is self-powered.  Only
    one device configuration can be active at a time; most devices
    have only one configuration.

    Each configuration consists of one or more interfaces.  Each
    interface serves a distinct "function", which is typically bound
    to a different USB device driver.  One common example is a USB
    speaker with an audio interface for playback, and a HID interface
    for use with software volume control.


Interface descriptor info (can be multiple per Config):

I:* If#=dd Alt=dd #EPs=dd Cls=xx(sssss) Sub=xx Prot=xx Driver=ssss
| | |      |      |       |             |      |       |__Driver name
| | |      |      |       |             |      |          or "(none)"
| | |      |      |       |             |      |__InterfaceProtocol
| | |      |      |       |             |__InterfaceSubClass
| | |      |      |       |__InterfaceClass
| | |      |      |__NumberOfEndpoints
| | |      |__AlternateSettingNumber
| | |__InterfaceNumber
| |__ "*" indicates the active altsetting (others are " ")
|__Interface info tag

    A given interface may have one or more "alternate" settings.
    For example, default settings may not use more than a small
    amount of periodic bandwidth.  To use significant fractions
    of bus bandwidth, drivers must select a non-default altsetting.

    Only one setting for an interface may be active at a time, and
    only one driver may bind to an interface at a time.  Most devices
    have only one alternate setting per interface.


Endpoint descriptor info (can be multiple per Interface):

E:  Ad=xx(s) Atr=xx(ssss) MxPS=dddd Ivl=dddss
|   |        |            |         |__Interval (max) between transfers
|   |        |            |__EndpointMaxPacketSize
|   |        |__Attributes(EndpointType)
|   |__EndpointAddress(I=In,O=Out)
|__Endpoint info tag

    The interval is nonzero for all periodic (interrupt or isochronous)
    endpoints.  For high speed endpoints the transfer interval may be
    measured in microseconds rather than milliseconds.

    For high speed periodic endpoints, the "MaxPacketSize" reflects
    the per-microframe data transfer size.  For "high bandwidth"
    endpoints, that can reflect two or three packets (for up to
    3KBytes every 125 usec) per endpoint.

    With the Linux-USB stack, periodic bandwidth reservations use the
    transfer intervals and sizes provided by URBs, which can be less
    than those found in endpoint descriptor.
```