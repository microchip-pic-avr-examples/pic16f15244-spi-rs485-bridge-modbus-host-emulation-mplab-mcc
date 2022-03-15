#include <stdint.h>
#include <stdbool.h>

#ifndef MODBUS_H
#define	MODBUS_H

#ifdef	__cplusplus
extern "C" {
#endif
    
//Client address
#define CLIENT_1               (0x15)
#define CLIENT_2               (0x16)

#define MODBUS_SLAVE_ONE_ADD   (0x80)
#define MODBUS_SLAVE_TWO_ADD   (0x81)

//Function codes
#define READ_COIL_STATUS       (0x01)
#define READ_INPUT_STATUS      (0x02)    
#define READ_HOLDING_REGISTERS (0x03)
#define READ_INPUT_REGISTERS   (0x04)
#define ERROR_FUNCTION         (0x81)
    
#define AO_TYPE_REGISTER_NUMBER (0x9C41)
#define TEMP_SENSOR_ADDRESS     (0x9CAC)
#define PRESS_SENSOR_ADDRESS    (0x9D10)
    
#define ADDRESS_OF_TEMP_DATA (TEMP_SENSOR_ADDRESS - AO_TYPE_REGISTER_NUMBER) 
#define ADDRESS_OF_PRESS_DATA (PRESS_SENSOR_ADDRESS - AO_TYPE_REGISTER_NUMBER)
    
typedef enum 
{
   CLIENT_ERROR = 1,
   CRC_ERROR,
   FUNCTION_CODE_ERROR
}modbus_error_e;

typedef struct
{                                   //data streamer frame creation 
    uint8_t CRC[2];
    uint8_t data[4];
    uint8_t function;
    uint8_t error;
	uint16_t address;
    uint16_t length;
}ModbusRTUFrame_t; 

typedef struct
{
    bool crcMatchFlag;
    bool slaveDetected;
    bool functionCodeDetected;
    bool errorCodeDetected;
    bool successMdBusRTUFrame;
    bool readHoldingRegisters;
    bool modbusHostFrameReady;
    bool frameDataLengthDetected;
}ModbusRxFlags_t; 

typedef union
{
  float sensorData;
  struct 
  {
    uint8_t data[4];
  }SensorData_st;
}SensorData_t;

void ModbusRTUClientResponseFrameParser(uint8_t dataLen);
bool ReceiveModbusClientResponse(void);

#ifdef	__cplusplus
}
#endif

#endif	/* MODBUS_H */


