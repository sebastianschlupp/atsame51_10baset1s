/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    udp_server.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It
    implements the logic of the application's state machine and it may call
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "udp_server.h"
#include "definitions.h"                // SYS function prototypes
#include "configuration.h"                // SYS function prototypes

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the UDP_SERVER_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

#define SERVER_PORT 9760

UDP_SERVER_DATA udp_serverData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
*/
void SetLED(uint8_t token)
{
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Token to be checked is: %u\n\r",token);
    //We will only go throught the function if the first four bits of the token
    //are 0x2 (0010)
    if((token&0xF0) == 0x30)
        SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "It does start with 0x3\n\r");
    {
        //we check the last for bits and implement
        // 0x0 LED goes off
        // 0x1 LED goes on
        // 0x2 LED toggles
        if((token&0x0F) == 0x00)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Case 0\n\r");
            LED0_Set();
        } 
        if((token&0x0F) == 0x01)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Case 1\n\r");
            LED0_Clear();
        }
        if((token&0x0F) == 0x02)
        {
            SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Case 2\n\r");
            LED0_Toggle();
        }
    }
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void UDP_SERVER_Initialize ( void )

  Remarks:
    See prototype in udp_server.h.
 */

void UDP_SERVER_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    udp_serverData.state = UDP_SERVER_TCPIP_WAIT_INIT;



    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void UDP_SERVER_Tasks ( void )

  Remarks:
    See prototype in udp_server.h.
 */

void UDP_SERVER_Tasks ( void )
{
    SYS_STATUS tcpipStat;
    const char *netName, *netBiosName;
    int i, nNets;
    TCPIP_NET_HANDLE netH;
    /* Check the application's current state. */
    switch ( udp_serverData.state )
    {
    case UDP_SERVER_TCPIP_WAIT_INIT:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if (tcpipStat < 0) { // some error occurred
                SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization failed!\r\n");
                udp_serverData.state = UDP_SERVER_TCPIP_ERROR;
            } else if (tcpipStat == SYS_STATUS_READY) {
                // now that the stack is ready we can check the 
                // available interfaces 
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for (i = 0; i < nNets; i++) {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);

#if defined(TCPIP_STACK_USE_NBNS)
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)
                    (void)netName;          // avoid compiler warning 
                    (void)netBiosName;      // if SYS_CONSOLE_PRINT is null macro

                }
                udp_serverData.state = UDP_SERVER_TCPIP_WAIT_FOR_IP;

            }
            break;


        case UDP_SERVER_TCPIP_WAIT_FOR_IP:
            nNets = TCPIP_STACK_NumberOfNetworksGet();
            for (i = 0; i < nNets; i++) {
                netH = TCPIP_STACK_IndexToNet(i);
                if (!TCPIP_STACK_NetIsReady(netH)) {
                    return; // interface not ready yet!
                }
                IPV4_ADDR           ipAddr;
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                SYS_CONSOLE_MESSAGE(TCPIP_STACK_NetNameGet(netH));
                SYS_CONSOLE_MESSAGE(" IP Address: ");
                SYS_CONSOLE_PRINT("%d.%d.%d.%d \r\n", ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]); 
            }
            // all interfaces ready. Could start transactions!!!
            udp_serverData.state = UDP_SERVER_BSD_CREATE_SOCKET;
            //... etc.
            break;
        case UDP_SERVER_BSD_CREATE_SOCKET:
        {
            SOCKET udpSkt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (udpSkt == SOCKET_ERROR) {
                udp_serverData.state = UDP_SERVER_TCPIP_WAIT_FOR_IP;
                return;
            } else {
                udp_serverData.socket = (SOCKET) udpSkt;
                udp_serverData.state = UDP_SERVER_BSD_BIND;
            }
        }
            break;

        case UDP_SERVER_BSD_BIND:
        {
            struct sockaddr_in addr;
            int addrlen = sizeof (struct sockaddr_in);
            addr.sin_port = SERVER_PORT;
            addr.sin_addr.S_un.S_addr = IP_ADDR_ANY;
            if (bind(udp_serverData.socket, (struct sockaddr*) &addr, addrlen) == SOCKET_ERROR) {
                closesocket(udp_serverData.socket);
                udp_serverData.state = UDP_SERVER_TCPIP_SERVING_CONNECTION;
            } else {
                udp_serverData.state = UDP_SERVER_TCPIP_SERVING_CONNECTION;
            }
        }
            break;

        case UDP_SERVER_TCPIP_SERVING_CONNECTION:
        {
            uint8_t AppBuffer[32];

            struct sockaddr_in clientaddr;
            int len = sizeof (struct sockaddr_in);

            // Figure out how many bytes have been received and how many we can transmit.
            int i = recvfrom(udp_serverData.socket, (char*) AppBuffer, sizeof (AppBuffer), 0, (struct sockaddr *) &clientaddr, &len);

            if (i <= 0) {
                break;
            }

            SYS_CONSOLE_PRINT("Recieved '%s' size %d\r\n", AppBuffer, i);
            SetLED(AppBuffer[0]);
            sendto(udp_serverData.socket, (char *) AppBuffer, i, 0, (struct sockaddr *) &clientaddr, len);

            udp_serverData.state = UDP_SERVER_TCPIP_CLOSING_CONNECTION;
        }
            break;
        case UDP_SERVER_TCPIP_CLOSING_CONNECTION:
        {
            closesocket(udp_serverData.socket);

            udp_serverData.state = UDP_SERVER_BSD_CREATE_SOCKET;

        }
            break;
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
