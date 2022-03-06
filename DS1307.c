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
	if (HAL_I2C_IsDeviceReady(&hi2c1, 0b11010000, 1, 10)==HAL_OK){ //verifica se si colega a device temperatura
		return 1; //successo
	}
	return 0; //insuccesso
}


//Permette di settare il control register
//FREQUENZA = 1, 4, 8, 32

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

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, giuseppe, 2, 10); //Setta control Register

	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &ControlRegister, 1, 10); //Mi collego a control register

	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &data, 1, 10); //Setta control Register
}

void ClockEnable(bool CKen) { //si osservi che il clock è abilitato di default (Disabilitare il clck riduce i consumi)

	uint8_t SecondsRegister=0x00;
	uint8_t Seconds=0x00;


	if (CKen==1){
		Seconds=0x00; //Setta bit CH (ClockHalt) a 0 abilitando il clock
	}
	else{
		Seconds=0x80; //Setta bit CH (ClockHalt) a 1 disabilitando il clock
	}


	uint8_t CKSet[2]={SecondsRegister,Seconds};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, CKSet, 2, 10); //Setta control Register

}



void SettaGiorno(uint8_t giorno) {
	uint8_t DayRegister=0x04;

	uint8_t DaySet[2]={DayRegister,giorno};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, DaySet, 2, 10); //Setta control Register


//	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &DayRegister, 1, 10); //Mi collego a day register

//	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &giorno, 1, 10); //Setta control Register
}


void SettaMese(uint8_t mese) {
	uint8_t MonthRegister=0x05;

	uint8_t MonthSet[2]={MonthRegister,mese};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, MonthSet, 2, 10); //Setta control Register

	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &MonthRegister, 1, 10); //Mi collego a day register

	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &mese, 1, 10); //Setta control Register
}


void SettaAnno(uint8_t anno) {
	uint8_t YearRegister=0x06;

	uint8_t YearSet[2]={YearRegister,anno};

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, YearSet, 2, 10); //Setta control Register


	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &YearRegister, 1, 10); //Mi collego a day register

	//HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &anno, 1, 10); //Setta control Register
}

uint8_t LeggiSecondi(void) { //per testare che effettivamente il valore si aggiorna
	uint8_t SecondsRegister=0x00;
	uint8_t Secondi=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &SecondsRegister, 1, 10); //Mi collego a day register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Secondi , 1, 10); //Leggo Ambient Temp

	return Secondi;
}


uint8_t LeggiAnno(void) {
	uint8_t YearRegister=0x06;
	uint8_t Anno=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &YearRegister, 1, 10); //Mi collego a day register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Anno , 1, 10); //Leggo Ambient Temp

	return Anno;
}


uint8_t LeggiMese(void) {
	uint8_t MonthRegister=0x05;
	uint8_t Mese=0x00;

	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &MonthRegister, 1, 10); //Mi collego a day register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Mese , 1, 10); //Leggo Ambient Temp

	return Mese;
}

uint8_t LeggiGiorno(void) {
	uint8_t DayRegister=0x04;
	uint8_t Giorno=0x00;

	//uint8_t robertini[2]={DayRegister,Giorno};

	//HAL_I2C_Master_Receive(&hi2c1,SLAVE_READ_ADDRESS, robertini, 2, 10); //Setta control Register


	HAL_I2C_Master_Transmit(&hi2c1,SLAVE_WRITE_ADDRESS, &DayRegister, 1, 10); //Mi collego a day register

	HAL_I2C_Master_Receive(&hi2c1, SLAVE_READ_ADDRESS,&Giorno , 1, 10); //Leggo Ambient Temp

	return Giorno;
}


//Ci si ricordi che se la seguente funzione ritorna 1 bisogna aggiornare il valore MeseSveglia
bool Sveglia(uint8_t MeseSveglia) { //funzione che ritorna 1 se il mese corrente corrisponde al mese richiesto

	if (MeseSveglia==LeggiMese()){
		return 1;
	}
	else{
		return 0;
	}
}



