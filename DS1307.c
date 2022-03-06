/*
 * DS1307.c
 *
 *  Created on: Nov 10, 2020
 *      Author: marco
 */

#include "main.h"
#include "ds1307.h"

extern I2C_HandleTypeDef hi2c1;


bool HelloDS1307(void){
	if (HAL_I2C_IsDeviceReady(&hi2c1, 0b11010000, 1, 10)==HAL_OK){ //Is it connected?
		return 1; //success
	}
	return 0; //no success
}


//Set the control register
//Frequency[Hz] = 1, 4, 8, 32

void SQW_Register(bool OUT, bool SQWE, uint8_t frequenza) {

	bool RS0, RS1;
	uint8_t data=0x00;
	uint8_t ControlRegister=0x07;

	switch (frequenza) {
		case 1:
			RS0=0;
			RS1=0;
			break;

		case 4:
			RS0=1;
			RS1=0;
			break;

		case 8:
			RS0=0;
			RS1=1;
			break;

		case 32:
			RS0=1;
			RS1=1;
			break;

		default: //errore
			RS0=0;
			RS1=0;
			break;
	}

	if (OUT==1){
		data |= (1 << 7);
	}
	else{
		data &= ~(1 << 7);
	}

	if (SQWE==1){
		data |= (1 << 4);
	}
	else{
		data &= ~(1 << 4);
	}

	if (RS0==1){
		data |= (1 << 0);
	}
	else{
		data &= ~(1 << 0);
	}

	if (RS1==1){
		data |= (1 << 1);
	}
	else{
		data &= ~(1 << 1);
	}


	uint8_t giuseppe[2]={ControlRegister,data};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, giuseppe, 2, 10); //Set control Register
}

void ClockEnable(bool CKen) { //clock enabled by default (Disable clock can reduce power consumption)

	uint8_t SecondsRegister=0x00;
	uint8_t Seconds=0x00;


	if (CKen==1){
		Seconds=0x00; //Set bit CH (ClockHalt) to 0 enabling the clock
	}
	else{
		Seconds=0x80; //Set bit CH (ClockHalt) to 1 disabling the clock
	}


	uint8_t CKSet[2]={SecondsRegister,Seconds};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, CKSet, 2, 10); //Sets control Register

}



void SettaGiorno(uint8_t giorno) {//sets day
	uint8_t DayRegister=0x04;

	uint8_t DaySet[2]={DayRegister,giorno};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, DaySet, 2, 10); //Sets control Register
}


void SettaMese(uint8_t mese) { //sets month
	uint8_t MonthRegister=0x05;

	uint8_t MonthSet[2]={MonthRegister,mese};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, MonthSet, 2, 10); //Sets control Register
}


void SettaAnno(uint8_t anno) { //sets year
	uint8_t YearRegister=0x06;

	uint8_t YearSet[2]={YearRegister,anno};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, YearSet, 2, 10); //Sets control Register
}

uint8_t LeggiSecondi(void) { //Reads seconds
	uint8_t SecondsRegister=0x00;
	uint8_t Secondi=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &SecondsRegister, 1, 10); //Connects to seconds register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Secondi , 1, 10); 

	return Secondi;
}


uint8_t LeggiAnno(void) { //reads year
	uint8_t YearRegister=0x06;
	uint8_t Anno=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &YearRegister, 1, 10); //connects to year register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Anno , 1, 10); //rads year

	return Anno;
}


uint8_t LeggiMese(void) {
	uint8_t MonthRegister=0x05;
	uint8_t Mese=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &MonthRegister, 1, 10); //Connects to month register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Mese , 1, 10); //reads month 

	return Mese;
}

uint8_t LeggiGiorno(void) {
	uint8_t DayRegister=0x04;
	uint8_t Giorno=0x00;


	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &DayRegister, 1, 10); //Connects to day register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Giorno , 1, 10); //Reads day register

	return Giorno;
}


//If the following function returns 1 do something (uses the timer)
bool Sveglia(uint8_t MeseSveglia) { //returns 1 if the current month is the month requested

	if (MeseSveglia==LeggiMese()){
		return 1;
	}
	else{
		return 0;
	}
}



