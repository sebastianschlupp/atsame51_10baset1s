/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

#include "userapps.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
//int global_data;
uint16_t AIN_ADC_value;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
/*static int ExampleLocalFunction(int param1, int param2) {
    return 0;
}*/
uint16_t AIN_ADC_Read(ADC_POSINPUT positiveInput, ADC_NEGINPUT negativeInput){
    uint16_t tempADCvalue;
    ADC0_ChannelSelect(positiveInput, negativeInput);
    ADC0_ConversionStart();
    while(ADC0_ConversionStatusGet()==0)
        ;
    tempADCvalue=ADC0_ConversionResultGet();
    return tempADCvalue;
}
void EIC_User_Handler(){
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Normal Boop!\n\r");
    AIN_ADC_value = AIN_ADC_Read(ADC_POSINPUT_AIN2, ADC_NEGINPUT_GND);//12bit value; 0=min, 4095=max; Vref=3.3V;
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "AN3 ADC value read is: %u\n\r",AIN_ADC_value);
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "If this value is above 2046, the LED should toggle \n\r");
    if (AIN_ADC_value > 2046)
        LED0_Toggle();
    AIN_ADC_value = AIN_ADC_Read(ADC_POSINPUT_AIN15, ADC_NEGINPUT_GND);//12bit value; 0=min, 4095=max; Vref=3.3V;
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "AN2 ADC value read is: %u\n\r",AIN_ADC_value);
}
void USER_Initialize(void){
    EIC_CallbackRegister(EIC_PIN_15,EIC_User_Handler, 0);
    ADC0_Enable();
    ADC0_InterruptsEnable(ADC_INTFLAG_RESRDY_Msk);
    TCC0_PWMStart();
}
void USER_Tasks(void){
    
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
/*int ExampleInterfaceFunction(int param1, int param2) {
    return 0;
}*/


/* *****************************************************************************
 End of File
 */