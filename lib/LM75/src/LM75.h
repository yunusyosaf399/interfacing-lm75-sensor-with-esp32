/* ------------------------------------------------------------
 * "THE BEERWARE LICENSE" (Revision 42):
 * <cvmanjoo@gmail.com> wrote this code. As long as you retain this
 * notice, you can do whatever you want with this stuff. If we
 * meet someday, and you think this stuff is worth it, you can
 * buy me a beer in return.
 * ------------------------------------------------------------
 * ------------------------------------------------------------
 * LM75.h - Library to get time from LM75B
 * Created by Manjunath CV. April 04 2019, 07:00 PM
 * Released into the public domain.
 * -----------------------------------------------------------*/

#ifndef LM75_H
#define LM75_H

#include <Arduino.h>
#include <Wire.h>

#define LM75_DFLT_ADDR 0x48

#define TEMPERATURE 0x00
#define CONFIGURATION 0x01
#define THYST_REG 0x02	// Min Temp
#define TOS_REG 0x03  	// Max Temp
#define PRODUCT_ID 0x07

#define ON 0
#define OFF 1

#define COMPARATOR 0
#define INTERRUPT 1

class LM75
{
	public:
		
		// Constructors
		LM75();
		LM75(uint8_t address);
		//~LM75();
		
		// Functions
		uint8_t begin();
		bool isConnected();
		uint8_t getID();

        float getTemperature(void);

		float getMinTemperature(void);
		void setMinTemperature(float temperature);
		
		float getMaxTemperature(void);
		void setMaxTemperature(float temperature);

		void setFaultQueue(uint8_t);
		uint8_t getFaultQueue(void);

		void setPolarity(bool);
		bool getPolarity(void);
		
		void setOperationMode(bool mode);
		bool getOperationMode(void);
		
		uint8_t getStatus();
		void turnUp();
		void shutDown();

	private:
		int _i2c_address;
		uint8_t _read_one_register(uint8_t reg_address);
		void _write_one_register(uint8_t reg_address, uint8_t reg_data);

		uint16_t _read_two_registers(uint8_t reg_address);
		void _write_two_registers(uint8_t reg_address, uint16_t reg_data);
	};

/* Instantiate class
static LM75 LM75;
*/


#endif   /* LM75_H */
