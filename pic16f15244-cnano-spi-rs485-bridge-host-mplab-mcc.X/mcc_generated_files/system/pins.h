/**
 * Generated Pins header File
 * 
 * @file pins.h
 * 
 * @defgroup  pinsdriver Pins Driver
 * 
 * @brief This is generated driver header for pins. 
 *        This header file provides APIs for all pins selected in the GUI.
 *
 * @version Driver Version  3.0.0
*/

/*
© [2023] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

#ifndef PINS_H
#define PINS_H

#include <xc.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0

// get/set IO_RA2 aliases
#define RE_TRIS                 TRISAbits.TRISA2
#define RE_LAT                  LATAbits.LATA2
#define RE_PORT                 PORTAbits.RA2
#define RE_WPU                  WPUAbits.WPUA2
#define RE_OD                   ODCONAbits.ODCA2
#define RE_ANS                  ANSELAbits.ANSA2
#define RE_SetHigh()            do { LATAbits.LATA2 = 1; } while(0)
#define RE_SetLow()             do { LATAbits.LATA2 = 0; } while(0)
#define RE_Toggle()             do { LATAbits.LATA2 = ~LATAbits.LATA2; } while(0)
#define RE_GetValue()           PORTAbits.RA2
#define RE_SetDigitalInput()    do { TRISAbits.TRISA2 = 1; } while(0)
#define RE_SetDigitalOutput()   do { TRISAbits.TRISA2 = 0; } while(0)
#define RE_SetPullup()          do { WPUAbits.WPUA2 = 1; } while(0)
#define RE_ResetPullup()        do { WPUAbits.WPUA2 = 0; } while(0)
#define RE_SetPushPull()        do { ODCONAbits.ODCA2 = 0; } while(0)
#define RE_SetOpenDrain()       do { ODCONAbits.ODCA2 = 1; } while(0)
#define RE_SetAnalogMode()      do { ANSELAbits.ANSA2 = 1; } while(0)
#define RE_SetDigitalMode()     do { ANSELAbits.ANSA2 = 0; } while(0)
// get/set IO_RC0 aliases
#define IO_RC0_TRIS                 TRISCbits.TRISC0
#define IO_RC0_LAT                  LATCbits.LATC0
#define IO_RC0_PORT                 PORTCbits.RC0
#define IO_RC0_WPU                  WPUCbits.WPUC0
#define IO_RC0_OD                   ODCONCbits.ODCC0
#define IO_RC0_ANS                  ANSELCbits.ANSC0
#define IO_RC0_SetHigh()            do { LATCbits.LATC0 = 1; } while(0)
#define IO_RC0_SetLow()             do { LATCbits.LATC0 = 0; } while(0)
#define IO_RC0_Toggle()             do { LATCbits.LATC0 = ~LATCbits.LATC0; } while(0)
#define IO_RC0_GetValue()           PORTCbits.RC0
#define IO_RC0_SetDigitalInput()    do { TRISCbits.TRISC0 = 1; } while(0)
#define IO_RC0_SetDigitalOutput()   do { TRISCbits.TRISC0 = 0; } while(0)
#define IO_RC0_SetPullup()          do { WPUCbits.WPUC0 = 1; } while(0)
#define IO_RC0_ResetPullup()        do { WPUCbits.WPUC0 = 0; } while(0)
#define IO_RC0_SetPushPull()        do { ODCONCbits.ODCC0 = 0; } while(0)
#define IO_RC0_SetOpenDrain()       do { ODCONCbits.ODCC0 = 1; } while(0)
#define IO_RC0_SetAnalogMode()      do { ANSELCbits.ANSC0 = 1; } while(0)
#define IO_RC0_SetDigitalMode()     do { ANSELCbits.ANSC0 = 0; } while(0)
// get/set IO_RC1 aliases
#define IO_RC1_TRIS                 TRISCbits.TRISC1
#define IO_RC1_LAT                  LATCbits.LATC1
#define IO_RC1_PORT                 PORTCbits.RC1
#define IO_RC1_WPU                  WPUCbits.WPUC1
#define IO_RC1_OD                   ODCONCbits.ODCC1
#define IO_RC1_ANS                  ANSELCbits.ANSC1
#define IO_RC1_SetHigh()            do { LATCbits.LATC1 = 1; } while(0)
#define IO_RC1_SetLow()             do { LATCbits.LATC1 = 0; } while(0)
#define IO_RC1_Toggle()             do { LATCbits.LATC1 = ~LATCbits.LATC1; } while(0)
#define IO_RC1_GetValue()           PORTCbits.RC1
#define IO_RC1_SetDigitalInput()    do { TRISCbits.TRISC1 = 1; } while(0)
#define IO_RC1_SetDigitalOutput()   do { TRISCbits.TRISC1 = 0; } while(0)
#define IO_RC1_SetPullup()          do { WPUCbits.WPUC1 = 1; } while(0)
#define IO_RC1_ResetPullup()        do { WPUCbits.WPUC1 = 0; } while(0)
#define IO_RC1_SetPushPull()        do { ODCONCbits.ODCC1 = 0; } while(0)
#define IO_RC1_SetOpenDrain()       do { ODCONCbits.ODCC1 = 1; } while(0)
#define IO_RC1_SetAnalogMode()      do { ANSELCbits.ANSC1 = 1; } while(0)
#define IO_RC1_SetDigitalMode()     do { ANSELCbits.ANSC1 = 0; } while(0)
// get/set IO_RC6 aliases
#define DE_TRIS                 TRISCbits.TRISC6
#define DE_LAT                  LATCbits.LATC6
#define DE_PORT                 PORTCbits.RC6
#define DE_WPU                  WPUCbits.WPUC6
#define DE_OD                   ODCONCbits.ODCC6
#define DE_ANS                  ANSELCbits.ANSC6
#define DE_SetHigh()            do { LATCbits.LATC6 = 1; } while(0)
#define DE_SetLow()             do { LATCbits.LATC6 = 0; } while(0)
#define DE_Toggle()             do { LATCbits.LATC6 = ~LATCbits.LATC6; } while(0)
#define DE_GetValue()           PORTCbits.RC6
#define DE_SetDigitalInput()    do { TRISCbits.TRISC6 = 1; } while(0)
#define DE_SetDigitalOutput()   do { TRISCbits.TRISC6 = 0; } while(0)
#define DE_SetPullup()          do { WPUCbits.WPUC6 = 1; } while(0)
#define DE_ResetPullup()        do { WPUCbits.WPUC6 = 0; } while(0)
#define DE_SetPushPull()        do { ODCONCbits.ODCC6 = 0; } while(0)
#define DE_SetOpenDrain()       do { ODCONCbits.ODCC6 = 1; } while(0)
#define DE_SetAnalogMode()      do { ANSELCbits.ANSC6 = 1; } while(0)
#define DE_SetDigitalMode()     do { ANSELCbits.ANSC6 = 0; } while(0)
/**
 * @ingroup  pinsdriver
 * @brief GPIO and peripheral I/O initialization
 * @param none
 * @return none
 */
void PIN_MANAGER_Initialize (void);

/**
 * @ingroup  pinsdriver
 * @brief Interrupt on Change Handling routine
 * @param none
 * @return none
 */
void PIN_MANAGER_IOC(void);


#endif // PINS_H
/**
 End of File
*/