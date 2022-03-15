#include <stdio.h>
#include "dataVisualizer.h"
#include "application.h"
#include "mcc_generated_files/system/clock.h"
#include "mcc_generated_files/uart/eusart1.h"
#include "modbus.h"

ModbusRTUFrame_t modBUSRTUFrame_t;             //structure for modbus RTU frame.
DataStremerFrame_t dataStremerFrame_t;
ModbusRxFlags_t modBUSRxFlags_t;           //structure for modbus Rx handler 

static void SendString(uint8_t *data,uint8_t len);

/*******************************************************************************
 * void CreateDataStreamProtocolAndSend(void)
 *
 * API to create data streamer protocol buffer and send the buffer to data visualizer for viewing on custom dashboard  
 *
 * @param None 
 * @return None 
 ******************************************************************************/
void CreateDataStreamProtocolAndSend(void)
{
		dataStremerFrame_t.dataStreamer_t.start_of_frame = (char)START_OF_FRAME;      //start of frame 03 
		dataStremerFrame_t.dataStreamer_t.end_of_frame = (char)END_OF_FRAME;          //end of frame ~03 
		
        dataStremerFrame_t.dataStreamer_t.h_function_code = modbusRTUbuffer[1];
        dataStremerFrame_t.dataStreamer_t.reg_Adress = ((modbusRTUbuffer[2] << 8) | (modbusRTUbuffer[3]));
        dataStremerFrame_t.dataStreamer_t.noOfRegister = ((modbusRTUbuffer[4] << 8) | (modbusRTUbuffer[5]));
        dataStremerFrame_t.dataStreamer_t.c_client_ID = (uint8_t)modBUSRTUFrame_t.address;
        dataStremerFrame_t.dataStreamer_t.c_crc = ((modBUSRTUFrame_t.CRC[0] << 8)|(modBUSRTUFrame_t.CRC[1]));
        dataStremerFrame_t.dataStreamer_t.c_function_code = modBUSRTUFrame_t.function;
       	dataStremerFrame_t.dataStreamer_t.data[0] = modBUSRTUFrame_t.data[0];
        dataStremerFrame_t.dataStreamer_t.data[1] = modBUSRTUFrame_t.data[1];
        dataStremerFrame_t.dataStreamer_t.data[2] = modBUSRTUFrame_t.data[2];
        dataStremerFrame_t.dataStreamer_t.data[3] = modBUSRTUFrame_t.data[3];
        dataStremerFrame_t.dataStreamer_t.data_length = (uint8_t)modBUSRTUFrame_t.length;
        dataStremerFrame_t.dataStreamer_t.e_clientMatch = modBUSRxFlags_t.slaveDetected;
        dataStremerFrame_t.dataStreamer_t.e_crcMatch = modBUSRxFlags_t.crcMatchFlag;
        dataStremerFrame_t.dataStreamer_t.e_functionMatch = modBUSRxFlags_t.functionCodeDetected;
        dataStremerFrame_t.dataStreamer_t.e_lengthMatch = 1;
	    SendString(dataStremerFrame_t.DataStreamer_buffer,DATA_STREAMER_FRAME_LENGTH);                //sending data frame to data visualizer
        __delay_ms(50);
}

/*******************************************************************************
 * static void SendString(uint8_t *data,uint8_t len)
 *
 * API to send string via USART  
 *
 * @param string which we like to transmit and size of the string 
 * @return None 
 ******************************************************************************/
static void SendString(uint8_t *data,uint8_t len)
{
	uint16_t  i = 0;
	for (i=0;i<=len;i++)
	{
      while(!EUSART1_IsTxReady());  
      EUSART1_Write(*data++);
	}
}