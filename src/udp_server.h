/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    udp_server.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "UDP_SERVER_Initialize" and "UDP_SERVER_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "UDP_SERVER_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _UDP_SERVER_H
#define _UDP_SERVER_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "definitions.h"
#include "configuration.h"

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
    /* Application's state machine's initial state. */
    UDP_SERVER_TCPIP_WAIT_INIT,
    /* In this state, the application waits for a IP Address */
    UDP_SERVER_TCPIP_WAIT_FOR_IP,

    UDP_SERVER_BSD_CREATE_SOCKET,

    UDP_SERVER_BSD_BIND,

    UDP_SERVER_TCPIP_WAIT_FOR_CONNECTION,

    UDP_SERVER_TCPIP_SERVING_CONNECTION,

    UDP_SERVER_TCPIP_CLOSING_CONNECTION,

    UDP_SERVER_TCPIP_ERROR,
    /* TODO: Define states used by the application state machine. */

} UDP_SERVER_STATES;


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
    UDP_SERVER_STATES state;

    /* TODO: Define any additional data used by the application. */
    SOCKET socket;
} UDP_SERVER_DATA;

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
    void UDP_SERVER_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    UDP_SERVER_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    UDP_SERVER_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void UDP_SERVER_Initialize ( void );


/*******************************************************************************
  Function:
    void UDP_SERVER_Tasks ( void )

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
    UDP_SERVER_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void UDP_SERVER_Tasks( void );

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _UDP_SERVER_H */

/*******************************************************************************
 End of File
 */

