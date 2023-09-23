/*******************************************************************************
  System Definitions

  File Name:
    definitions.h

  Summary:
    project system definitions.

  Description:
    This file contains the system-wide prototypes and definitions for a project.

 *******************************************************************************/

//DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2018 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
//DOM-IGNORE-END

#ifndef DEFINITIONS_H
#define DEFINITIONS_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "crypto/crypto.h"
#include "peripheral/nvmctrl/plib_nvmctrl.h"
#include "peripheral/tc/plib_tc1.h"
#include "peripheral/tc/plib_tc0.h"
#include "driver/lan865x/drv_lan865x.h"
#include "system/time/sys_time.h"
#include "peripheral/adc/plib_adc0.h"
#include "driver/spi/drv_spi.h"
#include "library/tcpip/tcpip.h"
#include "system/sys_time_h2_adapter.h"
#include "system/sys_random_h2_adapter.h"
#include "system/int/sys_int.h"
#include "system/ports/sys_ports.h"
#include "system/cache/sys_cache.h"
#include "system/dma/sys_dma.h"
#include "system/reset/sys_reset.h"
#include "osal/osal.h"
#include "system/debug/sys_debug.h"
#include "peripheral/sercom/i2c_master/plib_sercom2_i2c_master.h"
#include "peripheral/sercom/spi_master/plib_sercom1_spi_master.h"
#include "peripheral/evsys/plib_evsys.h"
#include "system/command/sys_command.h"
#include "peripheral/port/plib_port.h"
#include "peripheral/clock/plib_clock.h"
#include "peripheral/nvic/plib_nvic.h"
#include "peripheral/dmac/plib_dmac.h"
#include "peripheral/cmcc/plib_cmcc.h"
#include "peripheral/sercom/usart/plib_sercom5_usart.h"
#include "peripheral/eic/plib_eic.h"
#include "wolfssl/wolfcrypt/port/pic32/crypt_wolfcryptcb.h"
#include "net_pres/pres/net_pres.h"
#include "net_pres/pres/net_pres_encryptionproviderapi.h"
#include "net_pres/pres/net_pres_transportapi.h"
#include "net_pres/pres/net_pres_socketapi.h"
#include "peripheral/tcc/plib_tcc0.h"
#include "system/console/sys_console.h"
#include "system/console/src/sys_console_uart_definitions.h"
#include "FreeRTOS.h"
#include "task.h"
#include "app.h"
#include "app1.h"
#include <errno.h>



// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

/* Device Information */
#define DEVICE_NAME			 "ATSAME51J20A"
#define DEVICE_ARCH			 "CORTEX-M4"
#define DEVICE_FAMILY		 "SAME"
#define DEVICE_SERIES		 "SAME51"

/* CPU clock frequency */
#define CPU_CLOCK_FREQUENCY 120000000

// *****************************************************************************
// *****************************************************************************
// Section: System Functions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Initialization Function

  Function:
    void SYS_Initialize( void *data )

  Summary:
    Function that initializes all modules in the system.

  Description:
    This function initializes all modules in the system, including any drivers,
    services, middleware, and applications.

  Precondition:
    None.

  Parameters:
    data            - Pointer to the data structure containing any data
                      necessary to initialize the module. This pointer may
                      be null if no data is required and default initialization
                      is to be used.

  Returns:
    None.

  Example:
    <code>
    SYS_Initialize ( NULL );

    while ( true )
    {
        SYS_Tasks ( );
    }
    </code>

  Remarks:
    This function will only be called once, after system reset.
*/

void SYS_Initialize( void *data );

// *****************************************************************************
/* System Tasks Function

Function:
    void SYS_Tasks ( void );

Summary:
    Function that performs all polled system tasks.

Description:
    This function performs all polled system tasks by calling the state machine
    "tasks" functions for all polled modules in the system, including drivers,
    services, middleware and applications.

Precondition:
    The SYS_Initialize function must have been called and completed.

Parameters:
    None.

Returns:
    None.

Example:
    <code>
    SYS_Initialize ( NULL );

    while ( true )
    {
        SYS_Tasks ( );
    }
    </code>

Remarks:
    If the module is interrupt driven, the system will call this routine from
    an interrupt context.
*/

void SYS_Tasks ( void );

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* System Objects

Summary:
    Structure holding the system's object handles

Description:
    This structure contains the object handles for all objects in the
    MPLAB Harmony project's system configuration.

Remarks:
    These handles are returned from the "Initialize" functions for each module
    and must be passed into the "Tasks" function for each module.
*/

typedef struct
{

    /* LAN865X System object */
    SYS_MODULE_OBJ          lan865xObject;

    SYS_MODULE_OBJ  sysTime;
    SYS_MODULE_OBJ  sysConsole0;


    SYS_MODULE_OBJ  tcpip;
    /* SPI0 Driver Object */
    SYS_MODULE_OBJ drvSPI0;

    SYS_MODULE_OBJ  sysDebug;

    SYS_MODULE_OBJ  netPres;


} SYSTEM_OBJECTS;

typedef enum
{
    //Default state
    SYS_USER_ERROR_NOERROR = 0x00U,
    //Error flags related to unsuitable input values 0x10
    SYS_USER_ERROR_GENERICINPUTVALUEFAULT = 0x10U,
    SYS_USER_ERROR_INPNOTINRANGE = 0x11U, //the input values are not within the range required for the function to work properly
    SYS_USER_ERROR_12 = 0x12U,
    SYS_USER_ERROR_13 = 0x13U,
    SYS_USER_ERROR_14 = 0x14U,
    SYS_USER_ERROR_15 = 0x15U,
    //Error flags related to ... 2x
            
    //Error flags related to interrupt handling 3x
    SYS_USER_ERROR_GENERICINTFAULT = 0x30U,
    SYS_USER_ERROR_INTFLAGNOTHANDLED = 0x31U, //the interrupt is setting a flag that has to be reset in the function that handles it

    //Error flags related to ADC handling 4x
    SYS_USER_ERROR_GENERICADCFAULT = 0x30U,
    SYS_USER_ERROR_ADCCONVERSIONFAULT = 0x31U, //after a conversion is started, this flag will be set until the conversion is done. This way we don't need a while loop blocking all code execution to know when the result is done.
    //Error flags related to ... 5x
    SYS_USER_ERROR_55 = 0x55U,
            
    //Error flags related to Networking 8x
    SYS_USER_ERROR_ = 0x80U, //Error creating socket
    //SYS_USER_ERROR_ = 0x81U, //Error creating socket
    //SYS_USER_ERROR_ = 0x82U, //Error creating socket
    //SYS_USER_ERROR_ = 0x83U, //Error creating socket
    //SYS_USER_ERROR_ = 0x84U, //Error creating socket

    //Error flags related to RTOS Tasks 9x
    SYS_USER_ERROR_UNDEFINEDSTATE = 0x90U,
            
} SYS_USER_ERROR_FLAGS;

// *****************************************************************************
// *****************************************************************************
// Section: extern declarations
// *****************************************************************************
// *****************************************************************************



extern SYSTEM_OBJECTS sysObj;

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* DEFINITIONS_H */
/*******************************************************************************
 End of File
*/

