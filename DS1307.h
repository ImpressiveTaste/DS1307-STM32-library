/*
 * DS1307.h
 *
 *  Created on: Dec 27, 2020
 *      Author: marco
 */

#ifndef DS1307_H_
#define DS1307_H_


#include "main.h"

#include <stdbool.h>
#include <stdlib.h>


#define SLAVE_READ_ADDRESS 0b11010001 // Read Address
#define SLAVE_WRITE_ADDRESS 0b11010000 // Write Address

/**
  * @brief  Checks for I2C communication with DS1307
  * @retval OK(1) or error(0)
  */
bool HelloDS1307(void);

/**
  * @brief  Enables Clock
  * @note   To make all the other functions work clock must be enabled.
  * @param  CKen: ClockEnalble(1) or ClockDisbale(0)
  */
void ClockEnable(bool CKen);

/**
  * @brief  Set Square Wave pin
  * @note   It can be used as an external oscillator for the
  * 		microcontroller or as external interrupt.
  * @param 	OUT: If SQW not used can be used to set GPO pin
  * @param 	SQWE: 1 to have it enabled 0 to have it desabled
  * @param 	frequenza: 1, 4, 8, 32 Khz
  */
void SQW_Register(bool OUT, bool SQWE, uint8_t frequenza);

/**
  * @brief  Sets Day
  * @param 	CurrentDay
  */
void SettaGiorno(uint8_t giorno);

/**
  * @brief  Sets Month
  * @param 	CurrentMonth
  */
void SettaMese(uint8_t mese);

/**
  * @brief  Sets Year
  * @param 	CurrentYear
  */
void SettaAnno(uint8_t anno);

/**
  * @brief  Requests current Year
  * @retval CurrentYear
  */
uint8_t LeggiAnno(void);

/**
  * @brief  Requests current Month
  * @retval CurrentMonth
  */
uint8_t LeggiMese(void);

/**
  * @brief  Requests current Day
  * @retval CurrentDay
  */
uint8_t LeggiGiorno(void);

/**
  * @brief  it turns on if the month corresponds with the current month
  * @param 	Month that I would like it to get noticed
  * @retval If the month is the same as the param then returns 1
  * 		If not returns 0
  */
bool Sveglia(uint8_t MeseSveglia);

/**
  * @brief  Requests current time (seconds)
  * @retval seconds
  */
uint8_t LeggiSecondi(void);



#endif /* DS1307_H_ */
