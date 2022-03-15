#include "modbusCRC.h"

#define MAX (0xFFFF)
/*******************************************************************************
 * uint16_t ModRTU_CRC(uint8_t buf[], uint8_t len)
 *
 * API to run the function to calculate 2-byte CRC for the given data buffer and 
 * return
 *
 * @param uint8_t buf[], uint8_t len  // Data and length 
 * @return uint16_t // 2-byte CRC value 
 ******************************************************************************/
uint16_t ModRTU_CRC(uint8_t buf[], uint8_t len)
{
  uint16_t crc = MAX;
  uint16_t pos = 0;
  uint8_t index = 0;
  
  for (pos = 0; pos < len; pos++) 
  {
    crc ^= (unsigned int)buf[pos];          // XOR byte into least //sig. byte of crc
  
    for (index = 8; index != 0; index--)     // Loop over each bit
    {
        if ((crc & 0x0001) != 0)       // If the LSB is set
        {
          crc >>= 1;                    // Shift right and XOR 0xA001
          crc ^= CRC16_POLYNOMIAL;
        }
        else                            // Else LSB is not set
        { 
         crc >>= 1;                    // Just shift right
        }
    }
  }
  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  return crc;  
}
