/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Hussam Wael
 ******************************************************************************/

#ifndef PORT_H_
#define PORT_H_

/* Id for the company in the AUTOSAR
 * for example Hussam Wael's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* Port Module Id */
#define PORT_MODULE_ID    (124U)

/* Port Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for Port Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)

#define UNLOCKING_VALUE		     (0x4C4F434BU)

/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and Port Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
		||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
		||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Std_Types.h does not match the expected version"
#endif

#include "Port_Cfg.h"


/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
		||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
		||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
#error "The AR version of Port_Cfg.h does not match the expected version"
#endif

/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
		||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
		||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
#error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files */
#include "Common_Macros.h"


/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port_Init */
#define PORT_INIT_SID           		(uint8)0x00

/* Service ID for Port_SetPinDirection */
#define PORT_SET_PIN_DIRECTION_SID  		(uint8)0x01

/* Service ID for Port_RefreshPortDirection */
#define PORT_REFRESH_PORT_DIRECTION_SID  	(uint8)0x02

/* Service ID for Port_GetVersionInfo*/
#define PORT_GET_VERSION_INFO_SID       	(uint8)0x03

/* Service ID for Port_SetPinMode*/
#define PORT_SET_PIN_MODE_SID       		(uint8)0x04


/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Pin ID */
#define PORT_E_PARAM_PIN 	            (uint8)0x0A

/*DET code to report Port Pin not configured as changeable*/
#define PORT_E_DIRECTION_UNCHANGEABLE       (uint8)0x0B

/* DET code to report Port_Init API service called with NULL pointer parameter */
#define PORT_E_PARAM_CONFIG 		    (uint8)0x0C

/* DET code to report Invalid Mode */
#define PORT_E_PARAM_INVALID_MODE	    (uint8)0x0D

/*DET code to report API Port_SetPinMode service called when mode is unchangeable*/
#define PORT_E_MODE_UNCHANGEABLE	    (uint8)0x0E

/*
 * API service used without module initialization is reported using following
 * error code
 */
#define PORT_E_UNINIT 			    (uint8)0x0F

/*
 * APIs called with a Null Pointer
 */
#define PORT_E_PARAM_POINTER 		    (uint8)0x10

/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/
/* Type definition for Port_PinType used by the PORT APIs */
typedef uint8 Port_PinType;

/* Type definition for Port_PinDirectionType used by the PORT APIs */
typedef enum{

	PORT_PIN_IN ,
	PORT_PIN_OUT

}Port_PinDirectionType;

/* Type definition for Port_PinModeType used by the PORT APIs */
typedef uint8 Port_PinModeType;

/* Non AUTOSAR Data-types */
typedef enum
{
	PORT_RESISTANCE_PULL_UP,
	PORT_RESISTANCE_PULL_DOWN,
	PORT_RESISTANCE_OFF
}Port_PinInternalResistor;


typedef enum{

	PORT_PIN_DIRECTION_NON_CHANGEABLE ,
	PORT_PIN_DIRECTION_CHANGEABLE

}Port_PinDirectionChangeable;

typedef enum{

	PORT_PIN_MODE_NON_CHANGEABLE ,
	PORT_PIN_MODE_CHANGEABLE

}Port_PinModeChangeable;

typedef enum{

	PORT_PIN_LEVEL_LOW ,
	PORT_PIN_LEVEL_HIGH

}Port_PinLevelValue;

typedef enum{

	PORT_MODE_DIO,
	PORT_MODE_ALT1,
	PORT_MODE_ALT2,
	PORT_MODE_ALT3,
	PORT_MODE_ALT4,
	PORT_MODE_ALT5,
	PORT_MODE_ALT6,
	PORT_MODE_ALT7,
	PORT_MODE_ALT8,
	PORT_MODE_ALT9,
	PORT_MODE_ALT10,
	PORT_MODE_ALT11,
	PORT_MODE_ALT12,
	PORT_MODE_ALT13,
	PORT_MODE_ALT14,
	PORT_MODE_ADC,

}Port_PinInitialMode;

typedef struct
{

	uint8 Port_Num;

	uint8 Pin_Num;

	Port_PinModeType pin_mode;

	Port_PinLevelValue initial_value;

	Port_PinDirectionType direction;

	Port_PinInternalResistor resistance;

	Port_PinModeChangeable changeableMode;

	Port_PinDirectionChangeable changeableDirection;


}Port_ConfigPin;


/* Data Structure required for initializing the Port Driver */
typedef struct Port_ConfigType
{
	Port_ConfigPin Pins[PORT_NUMBER_OF_PORT_PINS];

} Port_ConfigType;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/

/* Function for PORT Init API */
void Port_Init(const Port_ConfigType* ConfigPtr) ;

#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
/* Function for PORT Set Pin Direction API */
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction);
#endif

/* Function for PORT Refresh Port Direction API */
void Port_RefreshPortDirection(void) ;

#if (PORT_VERSION_INFO_API == STD_ON)
/* Function for PORT Get Version Info API */
void Port_GetVersionInfo(Std_VersionInfoType* versioninfo) ;
#endif



#if (PORT_SET_PIN_MODE_API == STD_ON)
/* Function for PORT Set Pin Mode API */
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode );
#endif

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/

/* Extern PB structures to be used by Port and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H_ */
