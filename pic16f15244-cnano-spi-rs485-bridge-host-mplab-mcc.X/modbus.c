#include "mcc_generated_files/uart/eusart1.h"
#include "mcc_generated_files/system/clock.h"
#include "modbus.h"
#include "modbusCRC.h"
#include "application.h"
#include "dataVisualizer.h"
#include <stdio.h>
#include <string.h>

DataStremerFrame_t dataStremerFrame_t;           //structure for data visualizer variables
ModbusRxFlags_t modBUSRxFlags_t;           //structure for modbus Rx handler 
ModbusRTUFrame_t modBUSRTUFrame_t;             //structure for modbus RTU frame.
SensorData_t sensorData_t;                       //structure to save sensor data

/*******************************************************************************
 * bool ReceiveModbusClientResponse(void)
 *
 * API to receive Modbus response from the client 
 *
 * @param  void
 * @return bool - return true if host not received any fame from the client
 *                   return false if host received frame from the client 
 ******************************************************************************/
bool ReceiveModbusClientResponse(void) 
{
    uint8_t ch, index = 0;

    if (EUSART1_IsRxReady()) 
    {
        for (index = 0; index <= (CLIENT_MODBUS_FRAME_LENGTH - 1); index++) 
        {
            if (EUSART1_IsRxReady()) 
            {
                ch = EUSART1_Read();
                modbusRTUResponseBuffer[index] = ch;                    //load received characters to response buffer  
            }
        }
        modbusRTUResponseBuffer[index] = '\0';
        return RECEIVED;
    }
    memset(modbusRTUResponseBuffer,(int16_t)'\0',sizeof (modbusRTUResponseBuffer));
    return NOT_RECEIVED;
}
/*******************************************************************************
 * void ModbusRTUClientResponseFrameParser(uint8_t dataLen)
 *
 * API to run the function to validate and parse the client response and decode the sensor data 
 *
 * @param  Data length 
 * @return None
 ******************************************************************************/
void ModbusRTUClientResponseFrameParser(uint8_t dataLen) 
{
    uint16_t calCRC = 0;              //initialize local variable with zero
    uint8_t dataString[6] = {0};
    uint8_t len = 0;
    
    calCRC = ModRTU_CRC(modbusRTUResponseBuffer,(dataLen - 2));
    modBUSRTUFrame_t.CRC[0] = (calCRC & MASK_LOWER_BYTE);
    modBUSRTUFrame_t.CRC[1] = ((calCRC & MASK_HIGHER_BYTE) >> 8);
    if (modBUSRTUFrame_t.CRC[0] == modbusRTUResponseBuffer[dataLen - 2]) 
    {
        if (modBUSRTUFrame_t.CRC[1] == modbusRTUResponseBuffer[dataLen - 1]) 
        {
            modBUSRxFlags_t.crcMatchFlag = true;
        }
        else 
        {
            modBUSRxFlags_t.crcMatchFlag = false;
        }
    }
    else 
    {
        modBUSRxFlags_t.crcMatchFlag = false;
    }

    modBUSRTUFrame_t.address = modbusRTUResponseBuffer[0];
    if (modbusRTUResponseBuffer[0] == CLIENT_1) 
    {
        modBUSRxFlags_t.slaveDetected = true;
    } else if (modbusRTUResponseBuffer[0] == CLIENT_2) 
    {
        modBUSRxFlags_t.slaveDetected = true;
    }
    else 
    {
        modBUSRxFlags_t.slaveDetected = false;
    }

    modBUSRTUFrame_t.function = modbusRTUResponseBuffer[1];
    if (modBUSRTUFrame_t.function == READ_HOLDING_REGISTERS) 
    {
        modBUSRxFlags_t.functionCodeDetected = true;
    } 
    else if(modBUSRTUFrame_t.function == ERROR_FUNCTION) 
    {
        modBUSRxFlags_t.errorCodeDetected = true;
    }
    else 
    {
       modBUSRxFlags_t.functionCodeDetected = false;
       modBUSRxFlags_t.errorCodeDetected = false;
    }
    if( modBUSRxFlags_t.errorCodeDetected == true)
    {
      modBUSRTUFrame_t.error = modbusRTUResponseBuffer[2];
      modBUSRxFlags_t.crcMatchFlag = true;                                                       //return from frame parsing if error detected
    }
    modBUSRTUFrame_t.length = modbusRTUResponseBuffer[2];
    if (modBUSRTUFrame_t.length >= 1) 
    {
        modBUSRxFlags_t.frameDataLengthDetected = true;
    } 
    else 
    {
        modBUSRxFlags_t.frameDataLengthDetected = false;
    }
    for (len = 0; len < modBUSRTUFrame_t.length; len++) 
    {
        modBUSRTUFrame_t.data[len] = modbusRTUResponseBuffer[3 + len];
    }
    sensorData_t.SensorData_st.data[3] = modBUSRTUFrame_t.data[3];
    sensorData_t.SensorData_st.data[2] = modBUSRTUFrame_t.data[2];
    sensorData_t.SensorData_st.data[1] = modBUSRTUFrame_t.data[1];
    sensorData_t.SensorData_st.data[0] = modBUSRTUFrame_t.data[0];
    if(dataStremerFrame_t.dataStreamer_t.sensorOption == PRESSURE_FRAME)
    {
       dataStremerFrame_t.dataStreamer_t.pressureSensorData = sensorData_t.sensorData;
       dataStremerFrame_t.dataStreamer_t.temperatureSensorData = CLEAR;
    }
    if(dataStremerFrame_t.dataStreamer_t.sensorOption == TEMPERATURE_FRAME)
    {
       dataStremerFrame_t.dataStreamer_t.temperatureSensorData = sensorData_t.sensorData;
       dataStremerFrame_t.dataStreamer_t.pressureSensorData = CLEAR;
    }
    My_itoa(sensorData_t.sensorData,dataString,BASE_10);
    EUSART1PinSwap(DATAVISUALIZER);                                               //Swap/re-configure PPS pins for connecting EUSART to terminal
    EUSART1_SendString((uint8_t*) "\r\nSensor Data: ",(uint8_t*)dataString,SENSOR_DATA_LENGTH);
    if (modBUSRxFlags_t.errorCodeDetected == true) 
    {
        
        modBUSRxFlags_t.errorCodeDetected = false;
        if (modBUSRTUFrame_t.error == CRC_ERROR) 
        {
           modBUSRxFlags_t.crcMatchFlag = false;
        }
        if (modBUSRTUFrame_t.error == CLIENT_ERROR) 
        {
            modBUSRxFlags_t.slaveDetected = false;
        }
        if (modBUSRTUFrame_t.error == FUNCTION_CODE_ERROR) 
        {
            modBUSRxFlags_t.functionCodeDetected = false;
        }
        memset(modBUSRTUFrame_t.data,0,sizeof(modBUSRTUFrame_t.data));                 //reseting the data to zero if error frame received
        modBUSRTUFrame_t.length = CLEAR;                                                //reseting the length to zero if error frame received
        dataStremerFrame_t.dataStreamer_t.temperatureSensorData = CLEAR;                            //reseting the sensor data to zero if error frame received
        dataStremerFrame_t.dataStreamer_t.pressureSensorData = CLEAR;
    }
}
