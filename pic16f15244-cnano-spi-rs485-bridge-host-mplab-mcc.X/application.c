#include <xc.h>
#include "application.h"
#include "mcc_generated_files/uart/eusart1.h"
#include "mcc_generated_files/system/pins.h"
#include "mcc_generated_files/system/clock.h"
#include "dataVisualizer.h"
#include "modbusCRC.h"
#include "modbus.h"
#include <string.h>

extern volatile uint8_t eusart1TxHead;
extern volatile uint8_t eusart1TxTail;
extern volatile uint8_t eusart1TxBuffer[BUFFER_SIZE];
extern volatile uint8_t eusart1TxBufferRemaining;
extern volatile uint8_t eusart1RxHead;
extern volatile uint8_t eusart1RxTail;
extern volatile uint8_t eusart1RxCount;

volatile uint8_t timeCntr = 0;      
volatile uint8_t frameOption = TEMPERATURE_FRAME;     //Set temperature frame as default 
volatile uint8_t timer0OverFlowFlag = true;   // Timer 0 overflow flag set as default 

ModbusRxFlags_t modBUSRxFlags_t;     
DataStremerFrame_t dataStremerFrame_t;           //structure for data visualizer variables

void EUSART_Flush(void);
/*******************************************************************************
 * void Application(void)
 *
 * API to run the application, this API calls main application functions.
 * Function calls to send modbus frame to client, get response and create data frame for 
 * data visualizer
 * @param None
 * @return None
 ******************************************************************************/
void Application(void) 
{
    bool dataReceived = true;     
    
    if(timer0OverFlowFlag == true)                                                     //timer overflow every 10s
    {
        frameOption += NEXT_FRAME;
        if (frameOption > LAST_FRAME_OPTION) 
        {
          frameOption = TEMPERATURE_FRAME;
        }
        memset((uint8_t*)dataStremerFrame_t.DataStreamer_buffer,'\0',sizeof(dataStremerFrame_t.DataStreamer_buffer));
        ModbusRTUFrameCreation();
        dataStremerFrame_t.dataStreamer_t.h_client_ID = modbusRTUbuffer[0];           //Updating slave address to data visualizer structure
        RS485TxEnable();                                                              //Enable RS485 transmission   
        EUSART1PinSwap(RS485);                                                        //Swap/re-configure PPS pins for connecting EUSART to RS485 module
        SendModbusRTUFrame(modbusRTUbuffer, 8);
        RS485RxEnable();
        __delay_ms(DELAY_FOR_CLIENT_RESPONSE);                                        //Hard delay to wait for client response
        dataReceived = ReceiveModbusClientResponse();                                 //Receive response frame from the client 
        
        if (dataReceived == false) 
        {
            EUSART_Flush();
            dataReceived = true;
            ModbusRTUClientResponseFrameParser(CLIENT_MODBUS_FRAME_LENGTH);           //call function to validate and parse the response buffer
        }
        CreateDataStreamProtocolAndSend();
        memset((uint8_t*)modbusRTUbuffer,'\0',sizeof(modbusRTUbuffer));
        memset((uint8_t*)modbusRTUResponseBuffer,'\0',sizeof(modbusRTUResponseBuffer));
        timer0OverFlowFlag = false;
    }
}
/*******************************************************************************
 * void ModbusRTUFrameCreation(void)
 *
 * API to create the Modbus frame for reading both sensor 1 and sensor 2 alternatively   
 *
 * @param None
 * @return None
 ******************************************************************************/
void ModbusRTUFrameCreation(void) 
{
    uint8_t index = 0;
    uint16_t calCRC = 0;

    if (frameOption == TEMPERATURE_FRAME) 
    {
        dataStremerFrame_t.dataStreamer_t.sensorOption = TEMPERATURE_FRAME; 
        for (index = 0; index <= (HOST_MODBUS_FRAME_LENGTH - 1); index++)
        {
            modbusRTUbuffer[index] = modbusRTUFrameSensor1[index];
        }
    }
    if (frameOption == PRESSURE_FRAME) 
    {
        dataStremerFrame_t.dataStreamer_t.sensorOption = PRESSURE_FRAME; 
        for (index = 0; index <= (HOST_MODBUS_FRAME_LENGTH - 1); index++)
        {
            modbusRTUbuffer[index] = modbusRTUFrameSensor2[index];
        }
    }
    calCRC = ModRTU_CRC(modbusRTUbuffer, HOST_MODBUS_FRAME_LENGTH);
    modbusRTUbuffer[HOST_MODBUS_FRAME_LENGTH] = (calCRC & MASK_LOWER_BYTE);
    modbusRTUbuffer[HOST_MODBUS_FRAME_LENGTH + 1] = ((calCRC & MASK_HIGHER_BYTE) >> 8);
    dataStremerFrame_t.dataStreamer_t.h_crc = calCRC;               //Load crc value to data visualizer data array
}
/*******************************************************************************
 * void RS485TxEnable(void)
 *
 * Function to enable transmission of RS485 
 *
 * @param None
 * @return None
 ******************************************************************************/
void RS485TxEnable(void) 
{
    DE_SetHigh(); //Enable transmission 
    RE_SetHigh(); //Disable Reception 
}

/*******************************************************************************
 * void RS485RxEnable(void)
 *
 * Function to enable reception of RS485 
 *
 * @param None
 * @return None
 ******************************************************************************/
void RS485RxEnable(void) 
{
    RE_SetLow(); //Enable reception
    DE_SetLow(); //Disable transmission
}

/*******************************************************************************
 * void EUSART1PinSwap(uint8_t option)
 *
 * Function to swap the tx/rx lines using PPS
 *
 * @param parameter to choose what need to be swapped
 * @return temperature None
 *****************************************************************************/
void EUSART1PinSwap(uint8_t option) 
{
    PIE1bits.RC1IE = CLEAR; //Disable interrupt before re-configuring the PPS registers 
    switch (option) 
    {
        case DATAVISUALIZER:
            TRISBbits.TRISB5 = SET;
            TRISCbits.TRISC0 = CLEAR;
            ANSELBbits.ANSB7 = SET;
            
            RX1PPS = RX_RC1; //RC1->EUSART1:RX1;
            RC0PPS = TX_RC0; //RC0->EUSART1:TX1;
            RB5PPS = 0x00;   //Clear the RB5PPS since it is used as Tx for RS485 communication
            break;
        case RS485:
            TRISBbits.TRISB5 = CLEAR;
            TRISCbits.TRISC0 = SET;
            ANSELBbits.ANSB7 = CLEAR;
            
            RX1PPS = RX_RB7; //RB7->EUSART1:RX1
            RB5PPS = TX_RB5; //RB5->EUSART1:TX1
            RC0PPS = 0x00;   //Clear the RC0PPS since it is used as Tx for CDC communication
            break;
        default:
            break;
    }
    PIE1bits.RC1IE = SET; //Enable interrupt after re-configuring the PPS registers 
}

/*******************************************************************************
 * void SendModbusRTUFrame(uint8_t *dat, uint8_t len)
 *
 * Function to send Modbus frame to Host/Client
 *
 * @param uint8_t *dat, uint16_t len
 * @return None
 ******************************************************************************/
void SendModbusRTUFrame(uint8_t *dat, uint8_t len) 
{
    uint8_t index = 0;

    for (index = 0; index <= (len - 1); index++) 
    {
        EUSART1_Write(dat[index]); //Send data through RS485 module
    }
    __delay_ms(DELAY_FOR_PIN_SWAP); //Delay for completing the string transmission
}

/*******************************************************************************
 * void EUSART1_SendString(uint8_t *dat, const uint8_t *str, uint8_t len) 
 *
 * Function to send data to Datavisualizer
 *
 * @param Data, string, length
 * @return None
 ******************************************************************************/
void EUSART1_SendString(uint8_t *dat, const uint8_t *str, uint8_t len) 
{
#if(DEBUG_ON)
    uint8_t index = 0;

    for (index = 0; index < strlen((char*) dat); index++) 
    {
        while (!EUSART1_IsTxReady());
        EUSART1_Write(dat[index]);
    }
    for (index = 0; index < len; index++) 
    {
        while (!EUSART1_IsTxReady());
        EUSART1_Write(str[index]);
    }
#endif
    __delay_ms(DELAY_FOR_PIN_SWAP); //Delay for completing the string transmission
}

/*******************************************************************************
 * void My_reverse(uint8_t str[], uint8_t len)
 *
 * Function to reverse a string
 * @param uint8_t str[], uint16_t len
 * @return void
 ******************************************************************************/
void My_reverse(uint8_t str[], uint8_t len) 
{
    uint8_t start, end = 0;
    uint8_t temp = 0;
    for (start = 0, end = len - 1; start < end; start++, end--) 
    {
        temp = *(str + start);
        *(str + start) = *(str + end);
        *(str + end) = temp;
    }
}

/*******************************************************************************
 * uint8_t* My_itoa(uint32_t num, uint8_t str[], uint8_t baseNumber)
 *
 * Function to convert integer to ASCII character
 * 
 * @param uint32_t num, uint8_t str[], uint16_t baseNumber
 * @return (uint8_t) str pointer
 ******************************************************************************/
uint8_t* My_itoa(uint32_t num, uint8_t str[], uint8_t baseNumber) 
{
    uint8_t index = 0;
    uint16_t rem = 0;

    /* A zero is same "0" string in all base */
    if (num == 0) 
    {
        str[index] = '0';
        str[index + 1] = '\0';
        return str;
    }

    while (num != 0) 
    {
        rem = num % baseNumber;
        str[index++] = (uint8_t) ((rem > 9) ? (rem - 10) + 'A' : rem + '0');
        num = num / baseNumber;
    }

    str[index] = '\0';
    My_reverse(str, index);

    return str;
}

/*******************************************************************************
 * void EUSART_Flush(void)
 *
 * Reset the queue 
 *
 * @param None
 * @return None
 ******************************************************************************/
void EUSART_Flush(void)
{
  eusart1TxHead = 0;
  eusart1TxTail = 0;
  eusart1TxBufferRemaining = sizeof(eusart1TxBuffer);

  eusart1RxHead = 0;
  eusart1RxTail = 0;
  eusart1RxCount = 0;
}

/*******************************************************************************
 * void Timer0_UserOverflowCallback(void)
 *
 * Interrupt handler function for Timer 0 overflow interrupt"
 *
 * @param None
 * @return None
 ******************************************************************************/
void Timer0_UserOverflowCallback(void)
{
    timeCntr++;
    if(timeCntr >= TIME_OUT)
    {
        timeCntr = CLEAR;
        timer0OverFlowFlag = true;
    }
}