#include <stdint.h>

#ifndef DATAVISUALIZER_H_
#define DATAVISUALIZER_H_

#define START_OF_FRAME                (0x03)
#define END_OF_FRAME                  (0xFC)
#define DATA_STREAMER_FRAME_LENGTH    (31)

typedef struct
{                                  //Data streamer field 
	uint8_t start_of_frame;  
    uint8_t h_client_ID;   
    uint8_t h_function_code;  
    uint16_t reg_Adress;   
    uint16_t noOfRegister;  
    uint16_t h_crc;        
    uint8_t c_client_ID;   
    uint8_t c_function_code;
    uint8_t data_length;   
    uint8_t data[4];    
    uint16_t c_crc;    
    uint8_t e_clientMatch;      
    uint8_t e_crcMatch;       
    uint8_t e_functionMatch;    
    uint8_t e_lengthMatch;      
    float temperatureSensorData; 
    float pressureSensorData; 
    uint8_t sensorOption;     
    uint8_t end_of_frame;   
}DataStreamer_t;

typedef union 
{                                    //data streamer frame creation 
	uint8_t  DataStreamer_buffer[DATA_STREAMER_FRAME_LENGTH + 2]; 
    DataStreamer_t dataStreamer_t;
}DataStremerFrame_t;

void CreateDataStreamProtocolAndSend(void);

#endif /* DATAVISUALIZER_H_ */