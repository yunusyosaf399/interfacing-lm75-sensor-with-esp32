/*
* LM75.cpp - Library to get time from LM75B
* Created by Manjunath CV. April 04 2019, 07:00 PM
* Released into the public domain.
*/

#include <Arduino.h>
#include <Wire.h>
#include <LM75.h>

LM75::LM75()
{
    _i2c_address = LM75_DFLT_ADDR;
}

LM75::LM75(uint8_t address)
{
    _i2c_address = address;
}

// LM75::~LM75()
// {

// }

uint8_t LM75::begin()
{
    Wire.begin();
    return(_i2c_address);
	//Wire.endTransmission();
}

bool LM75::isConnected()
{
    Wire.begin();
	Wire.beginTransmission(_i2c_address);
	return(Wire.endTransmission() == 0 ? true : false);
}


float LM75::getTemperature(void)
{
    uint16_t  temperature_reg = _read_two_registers(TEMPERATURE);
    return(temperature_reg / 256.0f);
}

float LM75::getMinTemperature(void)
{
    uint16_t  temperature_reg = _read_two_registers(THYST_REG);
    return(temperature_reg / 256.0f);
}

void LM75::setMinTemperature(float temperature)
{
    uint16_t temperature_reg = temperature * 256.0f;
    _write_two_registers(THYST_REG,temperature_reg);
}

float LM75::getMaxTemperature(void)
{
    uint16_t  temperature_reg = _read_two_registers(TOS_REG);
    return(temperature_reg / 256.0f);
}

void LM75::setMaxTemperature(float temperature)
{
    uint16_t temperature_reg = temperature * 256.0f;
    _write_two_registers(TOS_REG,temperature_reg);
}

uint8_t LM75::getID()
{
    return(_read_one_register(PRODUCT_ID));
}

uint8_t LM75:: getStatus()
{
    uint8_t reg_data;

    reg_data = _read_one_register(CONFIGURATION);
    return(bitRead(reg_data,0));
}

void LM75::turnUp()
{
	uint8_t reg_data;

    reg_data = _read_one_register(CONFIGURATION);
    bitClear(reg_data,0);
    _write_one_register(CONFIGURATION,reg_data);
}

void LM75::shutDown()
{
	uint8_t reg_data;

    reg_data = _read_one_register(CONFIGURATION);
    bitSet(reg_data,0);
    _write_one_register(CONFIGURATION,reg_data);

}

void LM75::setOperationMode(bool mode)
{
	uint8_t reg_data;

    reg_data = _read_one_register(CONFIGURATION);
	if (mode == COMPARATOR)
		bitClear(reg_data,1);
	else
		bitSet(reg_data,1);
	_write_one_register(CONFIGURATION,reg_data);
}

bool LM75::getOperationMode()
{
	uint8_t reg_data;
    reg_data = _read_one_register(CONFIGURATION);
	return(bitRead(reg_data,1));
}

void LM75::setPolarity(bool polarity)
{
	uint8_t reg_data;
    reg_data = _read_one_register(CONFIGURATION);

	if (polarity == LOW)
		bitClear(reg_data,2);
	else
		bitSet(reg_data,2);

    _write_one_register(CONFIGURATION,reg_data);
}

bool LM75::getPolarity()
{
    uint8_t reg_data;
    reg_data = _read_one_register(CONFIGURATION);
	reg_data = bitRead(reg_data,2);
	return(reg_data);
}

void LM75::setFaultQueue(uint8_t value)
{
	uint8_t reg_data;
    reg_data = _read_one_register(CONFIGURATION);
    
    switch(value)
    {
		case 1:
			bitClear(reg_data,3);
			bitClear(reg_data,4);
			break;
		case 2:
			bitSet(reg_data,3);
			bitClear(reg_data,4);
			break;
		case 4:
			bitClear(reg_data,3);
			bitSet(reg_data,4);
			break;
		case 8:
			bitSet(reg_data,3);
			bitSet(reg_data,4);
			break;
		default:
			bitClear(reg_data,3);
			bitClear(reg_data,4);
			break;
	}
	_write_one_register(CONFIGURATION,reg_data);
}

uint8_t LM75::getFaultQueue(void)
{
	uint8_t reg_data;
    reg_data = _read_one_register(CONFIGURATION);
    
    reg_data = reg_data >> 3;
    switch(reg_data)
    {
		case 0:
			return 1;
			break;
		case 1:
			return 2;
			break;
		case 2:
			return 4;
			break;
		case 3:
			return 8;
			break;
		default:
			return 1;
			break;
	}
	return reg_data;
}

/* Private Functions */

uint8_t LM75::_read_one_register(uint8_t reg_address)
{
    uint8_t reg_data;
    
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();

    Wire.requestFrom((int)_i2c_address,(int) 1);
    reg_data = Wire.read();
    return(reg_data);

}

void LM75::_write_one_register(uint8_t reg_address, uint8_t reg_data)
{
    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_data);
    Wire.endTransmission();
}

uint16_t LM75::_read_two_registers(uint8_t reg_address)
{
    uint8_t reg_msb, reg_lsb;
    uint16_t two_registers;

    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.endTransmission();
    Wire.requestFrom(_i2c_address, 2);

    reg_msb = Wire.read();
    reg_lsb = Wire.read();

    two_registers = (reg_msb << 8) | reg_lsb;
    
    return(two_registers);
}

void LM75:: _write_two_registers(uint8_t reg_address, uint16_t reg_data)
{
    uint8_t reg_msb, reg_lsb;
    uint16_t two_registers;

    reg_msb = reg_data >> 8;
    reg_lsb = reg_data & 0x00FF;

    Wire.beginTransmission(_i2c_address);
    Wire.write(reg_address);
    Wire.write(reg_msb);
    Wire.write(reg_lsb);
    Wire.endTransmission();
}
