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
uint16_t AIN_ADC_AN2_value;
uint16_t AIN_ADC_AN3_value;
uint32_t PWM_duty_value;
uint32_t PWM_period_value;
uint8_t SYS_USER_ERROR_FLAG = 0x00U;
uint8_t EIC15_loop_count = 0;
bool EIC15_triggered = false;

ADC_Status current_adc_status = {ADC_POSINPUT_PTC,ADC_NEGINPUT_GND,false};
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
  @Summary
    External interrupt handler for EIC15
  @Description
    This functions sets a flag that tells the user code that an interrupt has been triggered.
    It also checks if there is a function that was triggered that resets the flag.
  @Precondition
    EIC15 triggered
  @Parameters
    none
  @Returns
    none
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
void EIC_User_Handler15(){
    if(EIC15_triggered == 1)
        SYS_USER_ERROR_FLAG = SYS_USER_ERROR_INTFLAGNOTHANDLED;
    EIC15_triggered = 1;
}

/** 
  @Summary
    AIN_ADC_ConversionTrigger triggers a new conversion. The conversion takes a while so make sure to use ADC0_ConversionStatusGet() to check when the result is available;
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
void AIN_ADC_ConversionTrigger(ADC_POSINPUT positiveInput, ADC_NEGINPUT negativeInput){
    ADC0_ChannelSelect(positiveInput, negativeInput);
    ADC0_ConversionStart();
    current_adc_status.Pchannel = positiveInput;
    current_adc_status.Nchannel = negativeInput;
    current_adc_status.busy = true;
}

/** 
  @Summary
    -
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
bool ADC_Reading_Example(){
    //Reading two ADCs and outputting values to command line
    bool function_done = false;
    //AN2
    if((current_adc_status.busy == false) & (current_adc_status.Pchannel == ADC_POSINPUT_PTC) & (current_adc_status.Nchannel == ADC_NEGINPUT_GND)){
        AIN_ADC_ConversionTrigger(ADC_POSINPUT_AIN15, ADC_NEGINPUT_GND);
    }
    if((current_adc_status.busy == true) & (current_adc_status.Pchannel == ADC_POSINPUT_AIN15) & (current_adc_status.Nchannel == ADC_NEGINPUT_GND)){
        if(ADC0_ConversionStatusGet() == true){
            AIN_ADC_AN2_value = ADC0_ConversionResultGet();//12bit value; 0=min, 4095=max; Vref=3.3V;
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "AN2 ADC value read is: %u\n\r",AIN_ADC_AN2_value);
            current_adc_status.Pchannel = ADC_POSINPUT_PTC;
            current_adc_status.Nchannel = ADC_NEGINPUT_GND;
            current_adc_status.busy = false;
        }
    }
    //AN3
    if((current_adc_status.busy == false) & (current_adc_status.Pchannel == ADC_POSINPUT_PTC) & (current_adc_status.Nchannel == ADC_NEGINPUT_GND)){
        AIN_ADC_ConversionTrigger(ADC_POSINPUT_AIN2, ADC_NEGINPUT_GND);
    }
    if((current_adc_status.busy == true) & (current_adc_status.Pchannel == ADC_POSINPUT_AIN2) & (current_adc_status.Nchannel == ADC_NEGINPUT_GND)){
        if(ADC0_ConversionStatusGet() == true){
            AIN_ADC_AN3_value = ADC0_ConversionResultGet();//12bit value; 0=min, 4095=max; Vref=3.3V;
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "AN3 ADC value read is: %u\n\r",AIN_ADC_AN3_value);
            current_adc_status.Pchannel = ADC_POSINPUT_PTC;
            current_adc_status.Nchannel = ADC_NEGINPUT_GND;
            current_adc_status.busy = false;
            function_done = true;
        }
    }
    return function_done;
}

/** 
  @Summary
    -
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
bool TTC0_PWM24bitDutyCycleWrite(uint8_t pwmchannel, uint32_t pwmdutycycle24bit){
    bool status = false;
    if ((TCC0_REGS->TCC_STATUS & (TCC_STATUS_PERBUFV_Msk)) == 0U){
        TCC0_REGS->TCC_CC[pwmchannel] = pwmdutycycle24bit;
        status = true;
    }    
    return status;
}

/** 
  @Summary
    Reads the value configured in the compare counter register for a certain pwm channel
  @Description
    This functions sets a flag that tells the user code that an interrupt has been triggered.
    It also checks if there is a function that was triggered that resets the flag.
  @Precondition
    TCC0 initialized and configured for PWM operation
  @Parameters
    uint8_t pwmchannel
  @Returns
    uint32_t value for the compare counter register of the channel selected
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
uint32_t TTC0_PWM24bitDutyCycleRead(uint8_t pwmchannel){
    if((0<=pwmchannel) & (pwmchannel<=5))
        return TCC0_REGS->TCC_CC[pwmchannel];
    SYS_USER_ERROR_FLAG = SYS_USER_ERROR_INPNOTINRANGE;
}

/** 
  @Summary
    -
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
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
        SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Updating duty cycle counter from %u to %u\n\r", tmp_PWM_old_duty_value[i], tmp_PWM_new_duty_value[i]);
    }
    TCC0_PWM24bitPeriodSet(tmp_PWM_new_period_value);
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Updating period from %u to %u\n\r", tmp_PWM_old_period_value, tmp_PWM_new_period_value);
    return status;
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
  @Summary
    -
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
void USER_Initialize(void){
    EIC_CallbackRegister(EIC_PIN_15,EIC_User_Handler15, 0);
    ADC0_Enable();
    ADC0_InterruptsEnable(ADC_INTFLAG_RESRDY_Msk); //Enable the result ready flag in the interrupt flag register.
    TCC0_PWMStart();
}

/** 
  @Summary
    -
  @Description
    -
  @Precondition
    -
  @Parameters
    -
  @Returns
    -
  @Errors
    SYS_USER_ERROR_FLAGS as defined in userapp.h are set if needed
 */
void USER_Tasks(void){

    if (EIC15_triggered == true){
        EIC15_loop_count++;

        if(ADC_Reading_Example() == true){
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\r-----------------------------------------------------------------------\n\r");
            SYS_DEBUG_PRINT(SYS_ERROR_INFO, "Boop: %u\n\r",EIC15_loop_count);
            EIC15_triggered = false;
            EIC15_loop_count = 0;
        }
    }
    
}

/* 
    //Using AN3 value to determine if the LED should toggle or not.
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "If this value is above 2046, the LED should toggle \n\r");
    if (AIN_ADC_value > 2046)
        LED0_Toggle();
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "\n\r-----------------------------------------------------------------------\n\r");
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
    SYS_DEBUG_PRINT(SYS_ERROR_INFO, "PWM period and duty cycle are set to: %u %u\n\r",PWM_period_value, PWM_duty_value);
 */

/* *****************************************************************************
 End of File
 */