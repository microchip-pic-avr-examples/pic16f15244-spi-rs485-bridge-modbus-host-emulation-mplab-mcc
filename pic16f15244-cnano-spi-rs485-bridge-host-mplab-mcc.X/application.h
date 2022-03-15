#include <stdbool.h>

#ifndef APPLICATION_H
#define	APPLICATION_H

#define DEBUG_ON                         (0)
#define TIME_OUT                         (10)            //in minutes
#define SET                              (true)
#define CLEAR                            (false)
#define NOT_RECEIVED                     (true)
#define RECEIVED                         (false)
#define DELAY_FOR_PIN_SWAP               (50)
#define DELAY_COUNT                      (100)
#define DELAY_FOR_RESPONSE               (1000)
#define DELAY_FOR_CLIENT_RESPONSE        (2000)
#define BUFFER_SIZE                      (32)
#define SENSOR_DATA_LENGTH               (5)
#define BASE_10                          (10)
#define LEN                              (4)

#define NEXT_FRAME                       (1)
#define LAST_FRAME_OPTION                (2)

#define MASK_LOWER_BYTE                  (0x00ff)
#define MASK_HIGHER_BYTE                 (0xff00)

#define CLIENT_MODBUS_FRAME_LENGTH       (9)                 //length of the client response modbus frame
#define HOST_MODBUS_FRAME_LENGTH         (6)                   //length of the query frame

#define RX_RC1                           (0x11) 
#define TX_RC0                           (0x05)
#define RX_RB7                           (0xF)
#define TX_RB5                           (0x05)

#ifdef	__cplusplus
extern "C" {
#endif

uint8_t modbusRTUFrameSensor1[HOST_MODBUS_FRAME_LENGTH] = {0x15, 0x03, 0x00, 0x6B, 0x00, 0x02};
uint8_t modbusRTUFrameSensor2[HOST_MODBUS_FRAME_LENGTH] = {0x15, 0x03, 0x00, 0xCF, 0x00, 0x02};
uint8_t modbusRTUFrameSensorError1[HOST_MODBUS_FRAME_LENGTH] = {0x18, 0x03, 0x00, 0xCF, 0x00, 0x02};
uint8_t modbusRTUFrameSensorError2[HOST_MODBUS_FRAME_LENGTH] = {0x15, 0x06, 0x00, 0xCF, 0x00, 0x02};

uint8_t modbusRTUbuffer[5 + LEN];
uint8_t modbusRTUResponseBuffer[12];

typedef enum 
{
    DATAVISUALIZER = 1, 
    RS485 = 2
} EusartType_t;

typedef enum 
{
    TEMPERATURE_FRAME = 1, 
    PRESSURE_FRAME = 2
} FrameType_t;

void My_reverse(uint8_t str[], uint8_t len);
uint8_t* My_itoa(uint32_t num, uint8_t str[], uint8_t baseNumber);
void RS485TxEnable(void);
void RS485RxEnable(void);
void EUSART1PinSwap(uint8_t option);
void SendModbusRTUFrame(uint8_t *dat, uint8_t len);
void EUSART1_SendString(uint8_t *dat, const uint8_t *str, uint8_t len);
void Application(void);
void Timer0_UserOverflowCallback(void);
void ModbusRTUFrameCreation(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APPLICATION_H */

