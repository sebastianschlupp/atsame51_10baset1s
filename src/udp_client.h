/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    udp_client.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "UDP_CLIENT_Initialize" and "UDP_CLIENT_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "UDP_CLIENT_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _UDP_CLIENT_H
#define _UDP_CLIENT_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "configuration.h"
#include "definitions.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

typedef enum
{
    UDP_CLIENT_TCPIP_WAIT_INIT,
    /* In this state, the application waits for a IP Address */
    UDP_CLIENT_TCPIP_WAIT_FOR_IP,

    UDP_CLIENT_TCPIP_WAITING_FOR_COMMAND,

    UDP_CLIENT_TCPIP_WAIT_ON_DNS,

    UDP_CLIENT_BSD_CREATE_SOCKET,

    UDP_CLIENT_BSD_BIND,

    UDP_CLIENT_TCPIP_SEND,

    UDP_CLIENT_TCPIP_RECV,

    UDP_CLIENT_TCPIP_CLOSING_CONNECTION,

    UDP_CLIENT_TCPIP_ERROR,
} UDP_CLIENT_STATES;


// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    UDP_CLIENT_STATES state;

    /* TODO: Define any additional data used by the application. */
    SOCKET              socket;
    struct addrinfo     hints;
    struct addrinfo *   addressInfo;
#ifndef TCPIP_STACK_USE_IPV6
    struct sockaddr_in6  rxaddr;
#else
    struct sockaddr_in  rxaddr;
#endif
    uint64_t systemCount;

} UDP_CLIENT_DATA;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void UDP_CLIENT_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    UDP_CLIENT_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    UDP_CLIENT_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void UDP_CLIENT_Initialize ( void );


/*******************************************************************************
  Function:
    void UDP_CLIENT_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    UDP_CLIENT_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void UDP_CLIENT_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _UDP_CLIENT_H */

/*******************************************************************************
 End of File
 */

