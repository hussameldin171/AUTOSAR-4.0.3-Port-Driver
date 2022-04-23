/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Hussam Wael
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"

#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
		|| (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
		|| (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Det.h does not match the expected version"
#endif

#endif

STATIC const Port_ConfigPin * Port_Pins = (Port_Configuration.Pins);
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;
STATIC boolean ClockConfigured[PORT_NUMBER_OF_PORTS] = {FALSE};

/************************************************************************************
 * Service Name: Port_Init
 * Service ID[hex]: 0x00
 * Sync/Async: Synchronous
 * Reentrancy: Non reentrant
 * Parameters (in): ConfigPtr - Pointer to post-build configuration data
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function to Initialize the Port module.
 ************************************************************************************/
void Port_Init(const Port_ConfigType * ConfigPtr)
{

	boolean Error = FALSE ;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

	if(NULL_PTR == ConfigPtr){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_INIT_SID,
				PORT_E_PARAM_CONFIG);

	}

	else{

		/* No Action Required */
	}


#endif

	if(FALSE == Error){

		for(Port_PinType i = PORT_PIN0_ID ; i < PORT_NUMBER_OF_PORT_PINS ; i++){

			/*Step 1 : Configure the CLock*/
			if(ClockConfigured[Port_Pins[i].Port_Num] == FALSE){

				SET_BIT(SYSCTL_REGCGC2_REG, Port_Pins[i].Port_Num);

				ClockConfigured[Port_Pins[i].Port_Num] = TRUE;
			}


			/*Step 2 : Unlock a pin if required*/

			switch(Port_Pins[i].Port_Num){

			case PORT_PORTC_ID:
				break;

			case PORT_PORTD_ID:
				if(Port_Pins[i].Pin_Num == PORT_PIN7_ID){ /*PD7*/

					GPIO_PORTD_LOCK_REG = UNLOCKING_VALUE;

					SET_BIT(GPIO_PORTD_CR_REG,Port_Pins[i].Pin_Num);

				}
				break;

			case PORT_PORTF_ID: 
				if(Port_Pins[i].Pin_Num == PORT_PIN0_ID){ /*PF0*/

					GPIO_PORTF_LOCK_REG = UNLOCKING_VALUE;

					SET_BIT(GPIO_PORTF_CR_REG,Port_Pins[i].Pin_Num);

				}
				break;

			}

			/*Step 3 : Set digital or analog*/
			{
				volatile uint32 * DEN_Registers[] = {&GPIO_PORTA_DEN_REG ,
						&GPIO_PORTB_DEN_REG ,
						&GPIO_PORTC_DEN_REG,
						&GPIO_PORTD_DEN_REG,
						&GPIO_PORTE_DEN_REG,
						&GPIO_PORTF_DEN_REG};


				volatile uint32 * AMSEL_Registers[] = {&GPIO_PORTA_AMSEL_REG ,
						&GPIO_PORTB_AMSEL_REG ,
						&GPIO_PORTC_AMSEL_REG,
						&GPIO_PORTD_AMSEL_REG,
						&GPIO_PORTE_AMSEL_REG,
						&GPIO_PORTF_AMSEL_REG};

				if(Port_Pins[i].pin_mode == PORT_MODE_ADC){

					SET_BIT(*AMSEL_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);
					CLEAR_BIT(*DEN_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);
				}

				else{
					CLEAR_BIT(*AMSEL_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);
					SET_BIT(*DEN_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);
				}

			}


			/*Step 4 : Set the Alternate function*/
			{
				volatile uint32 * AFSEL_Registers[] = {&GPIO_PORTA_AFSEL_REG ,
						&GPIO_PORTB_AFSEL_REG ,
						&GPIO_PORTC_AFSEL_REG,
						&GPIO_PORTD_AFSEL_REG,
						&GPIO_PORTE_AFSEL_REG,
						&GPIO_PORTF_AFSEL_REG};

				volatile uint32 * PCTL_Registers[] = {&GPIO_PORTA_PCTL_REG ,
						&GPIO_PORTB_PCTL_REG ,
						&GPIO_PORTC_PCTL_REG,
						&GPIO_PORTD_PCTL_REG,
						&GPIO_PORTE_PCTL_REG,
						&GPIO_PORTF_PCTL_REG};

				*PCTL_Registers[Port_Pins[i].Port_Num] &= ~( ((uint32)0x0F) <<(4* Port_Pins[i].Pin_Num)) ;

				switch(Port_Pins[i].pin_mode){

				case PORT_MODE_DIO:
				case PORT_MODE_ADC:

					CLEAR_BIT(*AFSEL_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);

					break;

				default:

					SET_BIT(*AFSEL_Registers[Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);

					*PCTL_Registers[Port_Pins[i].Port_Num] |= ( ( (uint32)Port_Pins[i].pin_mode)<<(4*Port_Pins[i].Pin_Num) );


				}

			}

			/*Step 5 : Set the Direction of the pin*/
			{
				volatile uint32 * Direction_Registers[] = {&GPIO_PORTA_DIR_REG ,
						&GPIO_PORTB_DIR_REG ,
						&GPIO_PORTC_DIR_REG,
						&GPIO_PORTD_DIR_REG,
						&GPIO_PORTE_DIR_REG,
						&GPIO_PORTF_DIR_REG};

				WRITE_BIT(*Direction_Registers[Port_Pins[i].Port_Num] ,  Port_Pins[i].Pin_Num , Port_Pins[i].direction);
			}

			/*Step 6 : Set the initial value*/

			if(Port_Pins[i].direction == PORT_PIN_OUT){

				volatile uint32 * DATA_Registers[] = {&GPIO_PORTA_DATA_REG ,
						&GPIO_PORTB_DATA_REG ,
						&GPIO_PORTC_DATA_REG,
						&GPIO_PORTD_DATA_REG,
						&GPIO_PORTE_DATA_REG,
						&GPIO_PORTF_DATA_REG};

				WRITE_BIT(*DATA_Registers[Port_Pins[i].Port_Num] ,  Port_Pins[i].Pin_Num , Port_Pins[i].initial_value);

			}

			/*Step 7 : Configure Internal Resistances*/

			else if((Port_Pins[i].direction == PORT_PIN_IN) && (Port_Pins[i].resistance != PORT_RESISTANCE_OFF) ){

				volatile uint32 *Resistance_Registers[][PORT_NUMBER_OF_PORTS] ={
						{&GPIO_PORTA_PUR_REG ,
								&GPIO_PORTB_PUR_REG ,
								&GPIO_PORTC_PUR_REG,
								&GPIO_PORTD_PUR_REG,
								&GPIO_PORTE_PUR_REG,
								&GPIO_PORTF_PUR_REG},

								&GPIO_PORTA_PDR_REG ,
										&GPIO_PORTB_PDR_REG ,
										&GPIO_PORTC_PDR_REG,
										&GPIO_PORTD_PDR_REG,
										&GPIO_PORTE_PDR_REG,
										&GPIO_PORTF_PDR_REG
				};

				SET_BIT(*Resistance_Registers[Port_Pins[i].resistance][Port_Pins[i].Port_Num] , Port_Pins[i].Pin_Num);
			}



		}



		Port_Status = PORT_INITIALIZED;
	}


	else{

		/* No Action Required */

	}
}

/************************************************************************************
 * Service Name: Port_SetPinDirection
 * Service ID[hex]: 0x01
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number
 * 					Direction - Port Pin Direction
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function that Sets the port pin direction.
 ************************************************************************************/
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)

void Port_SetPinDirection(Port_PinType Pin,
		Port_PinDirectionType Direction){

	boolean Error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

	if(PORT_NOT_INITIALIZED == Port_Status){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID,
				PORT_E_UNINIT);

	}
	else{
		/* No Action Required */
	}


	if( (Pin >= PORT_NUMBER_OF_PORT_PINS) || (Pin < 0) ){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID,
				PORT_E_PARAM_PIN);
	}

	else{
		/* No Action Required */
	}


	if(PORT_PIN_DIRECTION_NON_CHANGEABLE == Port_Pins[Pin].changeableDirection){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_DIRECTION_SID,
				PORT_E_DIRECTION_UNCHANGEABLE);
	}

	else{
		/* No Action Required */
	}

#endif

	if(FALSE == Error){


		volatile uint32 * Direction_Registers[] = {&GPIO_PORTA_DIR_REG ,
				&GPIO_PORTB_DIR_REG ,
				&GPIO_PORTC_DIR_REG,
				&GPIO_PORTD_DIR_REG,
				&GPIO_PORTE_DIR_REG,
				&GPIO_PORTF_DIR_REG};

		WRITE_BIT(*Direction_Registers[Port_Pins[Pin].Port_Num] ,  Port_Pins[Pin].Pin_Num , Direction);


	}

	else{
		/* No Action Required */
	}

}

#endif


/************************************************************************************
 * Service Name: Port_RefreshPortDirection
 * Service ID[hex]: 0x02
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Function that Refreshes port direction.
 ************************************************************************************/
void Port_RefreshPortDirection(void){

	boolean Error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

	if(PORT_NOT_INITIALIZED == Port_Status){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_REFRESH_PORT_DIRECTION_SID,
				PORT_E_UNINIT);

	}
	else{
		/* No Action Required */
	}

#endif

	if(FALSE == Error){

		volatile uint32 * Direction_Registers[] = {&GPIO_PORTA_DIR_REG ,
				&GPIO_PORTB_DIR_REG ,
				&GPIO_PORTC_DIR_REG,
				&GPIO_PORTD_DIR_REG,
				&GPIO_PORTE_DIR_REG,
				&GPIO_PORTF_DIR_REG};

		for(Port_PinType i = PORT_PIN0_ID ; i < PORT_NUMBER_OF_PORT_PINS ; i++){

			if(PORT_PIN_DIRECTION_NON_CHANGEABLE == Port_Pins[i].changeableDirection){

				WRITE_BIT(*Direction_Registers[Port_Pins[i].Port_Num] ,  Port_Pins[i].Pin_Num , Port_Pins[i].direction);

			}

			else{
				/* No Action Required */
			}
		}

	}

	else{
		/* No Action Required */
	}


}


/************************************************************************************
 * Service Name: Port_GetVersionInfo
 * Service ID[hex]: 0x03
 * Sync/Async: Synchronous
 * Reentrancy: Non-Reentrant
 * Parameters (in): None
 * Parameters (inout): None
 * Parameters (out): versioninfo - Pointer to where to store the version information of this module.
 * Return value: None
 * Description: Returns the version information of this module.
 ************************************************************************************/

#if (PORT_VERSION_INFO_API == STD_ON)

void Port_GetVersionInfo(Std_VersionInfoType* versioninfo){

	boolean Error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

	if(PORT_NOT_INITIALIZED == Port_Status){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,
				PORT_E_UNINIT);

	}
	else{
		/* No Action Required */
	}




	if(NULL_PTR == versioninfo){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_GET_VERSION_INFO_SID,
				PORT_E_PARAM_POINTER);

	}

	else{
		/* No Action Required */
	}

#endif

	if(FALSE == Error){

		/* Copy the vendor Id */
		versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
		/* Copy the module Id */
		versioninfo->moduleID = (uint16)PORT_MODULE_ID;
		/* Copy Software Major Version */
		versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
		/* Copy Software Minor Version */
		versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
		/* Copy Software Patch Version */
		versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;

	}

	else{
		/* No Action Required */
	}


}
#endif



/************************************************************************************
 * Service Name: Port_SetPinMode
 * Service ID[hex]: 0x04
 * Sync/Async: Synchronous
 * Reentrancy: Reentrant
 * Parameters (in): Pin - Port Pin ID number
 * 					Mode - New Port Pin mode to be set on port pin.
 * Parameters (inout): None
 * Parameters (out): None
 * Return value: None
 * Description: Sets the port pin mode.
 ************************************************************************************/

#if (PORT_SET_PIN_MODE_API == STD_ON)

void Port_SetPinMode( Port_PinType Pin,
		Port_PinModeType Mode ){

	boolean Error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)

	if(PORT_NOT_INITIALIZED == Port_Status){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID,
				PORT_E_UNINIT);

	}
	else{
		/* No Action Required */
	}


	if((Pin >= PORT_NUMBER_OF_PORT_PINS) || Pin<0){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID,
				PORT_E_PARAM_PIN);
	}

	else{
		/* No Action Required */
	}




	if((Mode < PORT_MODE_DIO) || (Mode > PORT_MODE_ADC)){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID,
				PORT_E_PARAM_INVALID_MODE);

	}

	else{
		/* No Action Required */
	}

	if(PORT_PIN_MODE_NON_CHANGEABLE == Port_Pins[Pin].changeableMode){

		Error = TRUE;

		Det_ReportError(PORT_MODULE_ID,
				PORT_INSTANCE_ID,
				PORT_SET_PIN_MODE_SID,
				PORT_E_MODE_UNCHANGEABLE);
	}

	else{
		/* No Action Required */
	}


#endif

	if(FALSE == Error){

		{
			volatile uint32 * DEN_Registers[] = {&GPIO_PORTA_DEN_REG ,
					&GPIO_PORTB_DEN_REG ,
					&GPIO_PORTC_DEN_REG,
					&GPIO_PORTD_DEN_REG,
					&GPIO_PORTE_DEN_REG,
					&GPIO_PORTF_DEN_REG};


			volatile uint32 * AMSEL_Registers[] = {&GPIO_PORTA_AMSEL_REG ,
					&GPIO_PORTB_AMSEL_REG ,
					&GPIO_PORTC_AMSEL_REG,
					&GPIO_PORTD_AMSEL_REG,
					&GPIO_PORTE_AMSEL_REG,
					&GPIO_PORTF_AMSEL_REG};

			if(Mode == PORT_MODE_ADC){

				SET_BIT(*AMSEL_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);
				CLEAR_BIT(*DEN_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);
			}

			else{
				CLEAR_BIT(*AMSEL_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);
				SET_BIT(*DEN_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);
			}

		}

		{
			volatile uint32 * AFSEL_Registers[] = {&GPIO_PORTA_AFSEL_REG ,

					&GPIO_PORTB_AFSEL_REG ,
					&GPIO_PORTC_AFSEL_REG,
					&GPIO_PORTD_AFSEL_REG,
					&GPIO_PORTE_AFSEL_REG,
					&GPIO_PORTF_AFSEL_REG};

			volatile uint32 * PCTL_Registers[] = {&GPIO_PORTA_PCTL_REG ,
					&GPIO_PORTB_PCTL_REG ,
					&GPIO_PORTC_PCTL_REG,
					&GPIO_PORTD_PCTL_REG,
					&GPIO_PORTE_PCTL_REG,
					&GPIO_PORTF_PCTL_REG};

			*PCTL_Registers[Port_Pins[Pin].Port_Num] &= ~( ((uint32)0x0F) <<(4* Port_Pins[Pin].Pin_Num) ) ;

			switch(Mode){

			case PORT_MODE_DIO:
			case PORT_MODE_ADC:

				CLEAR_BIT(*AFSEL_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);

				break;

			default:

				SET_BIT(*AFSEL_Registers[Port_Pins[Pin].Port_Num] , Port_Pins[Pin].Pin_Num);

				*PCTL_Registers[Port_Pins[Pin].Port_Num] |= ((uint32)Mode)<<(4*Port_Pins[Pin].Pin_Num);


			}
		}
	}

	else{
		/* No Action Required */
	}

}

#endif
