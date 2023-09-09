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
uint32_t PWM_duty_value;
uint32_t PWM_period_value;

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
bool TTC0_PWM24bitDutyCycleWrite(uint8_t pwmchannel, uint32_t pwmdutycycle24bit){
    bool status = false;
    if ((TCC0_REGS->TCC_STATUS & (TCC_STATUS_PERBUFV_Msk)) == 0U)
    {
        TCC0_REGS->TCC_CC[pwmchannel] = pwmdutycycle24bit;
    }    
    return status;
}
uint32_t TTC0_PWM24bitDutyCycleRead(uint8_t pwmchannel){
    return TCC0_REGS->TCC_CC[pwmchannel];
}
bool TCC0_PWM24bitPeriodWrite(uint32_t tmp_PWM_new_period_value){ //handling of wrong input value; replace bool return by enums containing error description
    bool status = false;
    uint32_t tmp_PWM_old_duty_value[6];
    uint32_t tmp_PWM_old_period_value;
    uint32_t tmp_PWM_new_duty_value[6];
    tmp_PWM_old_period_value=TCC0_PWM24bitPeriodGet();
    for(uint8_t i=0;i<6;i++){
        tmp_PWM_old_duty_value[i]=TTC0_PWM24bitDutyCycleRead(i);
        tmp_PWM_new_duty_value[i] = ((float)tmp_PWM_new_period_value / (float)tmp_PWM_old_period_value) * (float)tmp_PWM_old_duty_value[i];
        TTC0_PWM24bitDutyCycleWrite(i, tmp_PWM_new_duty_value[i]);
        SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Updating duty cycle counter from %u to %u\n\r", tmp_PWM_old_duty_value[i], tmp_PWM_new_duty_value[i]);
    }
    TCC0_PWM24bitPeriodSet(tmp_PWM_new_period_value);
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Updating period from %u to %u\n\r", tmp_PWM_old_period_value, tmp_PWM_new_period_value);
    return status;
}


uint16_t AIN_ADC_Read(ADC_POSINPUT positiveInput, ADC_NEGINPUT negativeInput){
    uint16_t tempADCvalue;
    ADC0_ChannelSelect(positiveInput, negativeInput);
    ADC0_ConversionStart();
    while(ADC0_ConversionStatusGet()==0) //remove while loop and replace by periodic checking of status to not block the processor for that time
        ;
    tempADCvalue=ADC0_ConversionResultGet();
    return tempADCvalue;
}
void EIC_User_Handler15(){
    //Simple Debug Message indicating interrupt is being executed
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "Normal Boop!\n\r\n\r");
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n\r-----------------------------------------------------------------------\n\r");
    //Reading two ADCs and outputting values to command line
        //AN2
    AIN_ADC_value = AIN_ADC_Read(ADC_POSINPUT_AIN15, ADC_NEGINPUT_GND);//12bit value; 0=min, 4095=max; Vref=3.3V;
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "AN2 ADC value read is: %u\n\r",AIN_ADC_value);
        //AN3
    AIN_ADC_value = AIN_ADC_Read(ADC_POSINPUT_AIN2, ADC_NEGINPUT_GND);//12bit value; 0=min, 4095=max; Vref=3.3V;
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "AN3 ADC value read is: %u\n\r",AIN_ADC_value);
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n\r-----------------------------------------------------------------------\n\r");
    //Using AN3 value to determine if the LED should toggle or not.
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "If this value is above 2046, the LED should toggle \n\r");
    if (AIN_ADC_value > 2046)
        LED0_Toggle();
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "\n\r-----------------------------------------------------------------------\n\r");
    //Using AN3 value to determine if the PWM period or duty cycle should be increased
    if (AIN_ADC_value > 2046){
        PWM_period_value=TCC0_PWM24bitPeriodGet();
        if(PWM_period_value>20000)
            TCC0_PWM24bitPeriodWrite(1000);
        else
            TCC0_PWM24bitPeriodWrite(PWM_period_value+1000);
    } else {
        PWM_duty_value=TTC0_PWM24bitDutyCycleRead(3);
        if(PWM_duty_value<(PWM_period_value-100))
            PWM_duty_value+=100;
        else
            PWM_duty_value=100;
        TTC0_PWM24bitDutyCycleWrite(3,PWM_duty_value);
        PWM_period_value=TCC0_PWM24bitPeriodGet();
    }
    PWM_period_value=TCC0_PWM24bitPeriodGet();
    PWM_duty_value=TTC0_PWM24bitDutyCycleRead(3);
    SYS_DEBUG_PRINT(SYS_ERROR_DEBUG, "PWM period and duty cycle are set to: %u %u\n\r",PWM_period_value, PWM_duty_value);
}
void USER_Initialize(void){
    EIC_CallbackRegister(EIC_PIN_15,EIC_User_Handler15, 0);
    ADC0_Enable();
    ADC0_InterruptsEnable(ADC_INTFLAG_RESRDY_Msk);
    //TCC0_PWM24bitPeriodSet(PWM_period_value);
    //TCC0_PWM24bitCounterSet(PWM_duty_value);
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