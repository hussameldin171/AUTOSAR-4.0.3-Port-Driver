/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Hussam Wael
 ******************************************************************************/

#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)


/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT                (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API                (STD_ON)

/* Pre-compile option for Set Pin Direction API */
#define  PORT_SET_PIN_DIRECTION_API	     (STD_ON)

/* Pre-compile option for Set Pin Mode API */
#define PORT_SET_PIN_MODE_API		     (STD_ON)
/******************************************/

/* Number of the configured Port Pins */
#define PORT_NUMBER_OF_PORT_PINS	            (43u)

/* Number of the configured Ports */
#define PORT_NUMBER_OF_PORTS			    (6u)

/* PORT IDs */
#define PORT_PORTA_ID         0
#define PORT_PORTB_ID         1
#define PORT_PORTC_ID         2
#define PORT_PORTD_ID         3
#define PORT_PORTE_ID         4
#define PORT_PORTF_ID         5

/* Pin IDs */
#define PORT_PIN0_ID          0
#define PORT_PIN1_ID          1
#define PORT_PIN2_ID          2
#define PORT_PIN3_ID          3
#define PORT_PIN4_ID          4
#define PORT_PIN5_ID          5
#define PORT_PIN6_ID          6
#define PORT_PIN7_ID          7

#endif /* PORT_CFG_H_ */
