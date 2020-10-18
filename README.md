# FreeRTOS_RAW_Sockets
### This repository contains addition of RAW Sockets in FreeRTOS+Plus stack.

Project example was done on Infineon XMC4500 Relax Kit over Ethernet using https://github.com/embeddedcrab/Linux_4_ARM/tree/master/ml_iot_freertos_base [FreeRTOS TCP/IP over Ethernet] as base with few modifications.


Followintg functionalities were added:
1. SOCK_RAW and IPPROTO_RAW flags for RAW sockets functionality
2. SOCK_RAW Sockets with IPPOTO_RAW & IPPROTO_UDP can act as client (example given in App/src/client_task.cpp)
3. SOCK_RAW Sockets with IPPROTO_UDP can act as server (example given in App/src/server_task.cpp)


You can find the functionalities in App and FreeRTOS+Plus folders. [Main focus is on RAW Sockets functionality, you can ignore other things :-)]

You can see the Network capture for different cases of application for reference.


Explore it, you might find it useful.

Happy to Help & Share 😊

Cheers!!
