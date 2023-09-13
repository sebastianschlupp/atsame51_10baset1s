/* ************************************************************************** */
/* Descriptive File Name

  @Company
    Tardigrade Marine Services

  @File Name
    userapps.h

  @Summary
    Actions to be included in MPLAB Harmony Projects

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef USER_APPS_H    /* Guard against multiple inclusion */
#define USER_APPS_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "definitions.h"


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */
//#define EXAMPLE_CONSTANT 0


    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    // *****************************************************************************

    /* Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    /*typedef struct _example_struct_t {
        // Describe structure member.
        int some_number;

        // Describe structure member.
        bool some_flag;

    } example_struct_t;*/
typedef enum{
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
    SYS_ERROR_55 = 0x15U

} SYS_USER_ERROR_FLAGS;

typedef struct _ADC_Status {
        ADC_POSINPUT Pchannel;
        ADC_NEGINPUT Nchannel;
        bool busy;
    } ADC_Status;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************
    void USER_Initialize(void);
    void USER_Tasks(void);


/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* USER_APPS_H */

/* *****************************************************************************
 End of File
 */
