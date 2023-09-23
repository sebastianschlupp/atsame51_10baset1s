/*******************************************************************************
  MPLAB Harmony Application Source File

  Company:
    Microchip Technology Inc.

  File Name:
    app1.c

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

#include "app1.h"

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
    This structure should be initialized by the APP1_Initialize function.

    Application strings and buffers are be defined outside this structure.
*/

APP1_DATA app1Data;

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


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP1_Initialize ( void )

  Remarks:
    See prototype in app1.h.
 */

void APP1_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    app1Data.state = APP1_TCPIP_WAIT_INIT;


    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}


/******************************************************************************
  Function:
    void APP1_Tasks ( void )

  Remarks:
    See prototype in app1.h.
 */

/*void APP1_Tasks ( void )
{

    //Check the application's current state.
    switch ( app1Data.state )
    {
        // Application's initial state.
        case APP1_STATE_INIT:
        {
            bool appInitialized = true;


            if (appInitialized)
            {

                app1Data.state = APP1_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP1_STATE_SERVICE_TASKS:
        {

            break;
        }

        // TODO: implement your application state machine.


        // The default state should never be executed.
        default:
        {
            // TODO: Handle error in application's state machine.
            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Error code: %u",SYS_USER_ERROR_UNDEFINEDSTATE);
            break;
        }
    }
}*/
void APP1_Tasks(void) {
    SYS_STATUS tcpipStat;
    const char *netName, *netBiosName;
    int i, nNets;
    TCPIP_NET_HANDLE netH;

    switch (app1Data.state) {
        case APP1_TCPIP_WAIT_INIT:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if (tcpipStat < 0) { // some error occurred
                SYS_DEBUG_PRINT(SYS_ERROR_ERROR, " APP: TCP/IP stack initialization failed!\r\n");
                app1Data.state = APP1_TCPIP_ERROR;
            } else if (tcpipStat == SYS_STATUS_READY) {
                // now that the stack is ready we can check the 
                // available interfaces 
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for (i = 0; i < nNets; i++) {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);

#if defined(TCPIP_STACK_USE_NBNS)
                    SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#else
                    SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)
                    (void)netName;          // avoid compiler warning 
                    (void)netBiosName;      // if SYS_CONSOLE_PRINT is null macro

                }
                app1Data.state = APP1_TCPIP_WAIT_FOR_IP;

            }
            break;


        case APP1_TCPIP_WAIT_FOR_IP:
            nNets = TCPIP_STACK_NumberOfNetworksGet();
            for (i = 0; i < nNets; i++) {
                netH = TCPIP_STACK_IndexToNet(i);
                if (!TCPIP_STACK_NetIsReady(netH)) {
                    return; // interface not ready yet!
                }
                IPV4_ADDR           ipAddr;
                ipAddr.Val = TCPIP_STACK_NetAddress(netH);
                SYS_DEBUG_PRINT(SYS_ERROR_ERROR, TCPIP_STACK_NetNameGet(netH));
                SYS_DEBUG_PRINT(SYS_ERROR_ERROR, " IP Address: ");
                SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "%d.%d.%d.%d \r\n", ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]); 
            }
            // all interfaces ready. Could start transactions!!!
            app1Data.state = APP1_BSD_CREATE_SOCKET;
            //... etc.
            break;
        case APP1_BSD_CREATE_SOCKET:
        {
            SOCKET udpSkt = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
            if (udpSkt == SOCKET_ERROR) {
                app1Data.state = APP1_TCPIP_WAIT_FOR_IP;
                SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Error %d", errno);
                return;
            } else {
                app1Data.socket = (SOCKET) udpSkt;
                app1Data.state = APP1_BSD_BIND;
            }
        }
            break;

        case APP1_BSD_BIND:
        {
            struct sockaddr_in addr;
            int addrlen = sizeof (struct sockaddr_in);
            addr.sin_port = SERVER_PORT;
            addr.sin_addr.S_un.S_addr = IP_ADDR_ANY;
            if (bind(app1Data.socket, (struct sockaddr*) &addr, addrlen) == SOCKET_ERROR) {
                closesocket(app1Data.socket);
                app1Data.state = APP1_TCPIP_SERVING_CONNECTION;
            } else {
                app1Data.state = APP1_TCPIP_SERVING_CONNECTION;
            }
        }
            break;

        case APP1_TCPIP_SERVING_CONNECTION:
        {
            uint8_t AppBuffer[32];

            struct sockaddr_in clientaddr;
            int len = sizeof (struct sockaddr_in);

            // Figure out how many bytes have been received and how many we can transmit.
            int i = recvfrom(app1Data.socket, (char*) AppBuffer, sizeof (AppBuffer), 0, (struct sockaddr *) &clientaddr, &len);

            if (i <= 0) {
                break;
            }

            SYS_DEBUG_PRINT(SYS_ERROR_ERROR, "Recieved '%s' size %d\r\n", AppBuffer, i);

            sendto(app1Data.socket, (char *) AppBuffer, i, 0, (struct sockaddr *) &clientaddr, len);

            app1Data.state = APP1_TCPIP_CLOSING_CONNECTION;
        }
            break;
        case APP1_TCPIP_CLOSING_CONNECTION:
        {
            closesocket(app1Data.socket);

            app1Data.state = APP1_BSD_CREATE_SOCKET;

        }
            break;
        default:
            break;
    }
}


/*******************************************************************************
 End of File
 */
