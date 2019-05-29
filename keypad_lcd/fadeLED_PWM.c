#include <msp430.h>
#include "driverlib.h"
#include "Board.h"



char hexaKeys[4][3] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

void Key();
int row;
char pressedKey;


void main (void)
{


    WDT_A_hold(WDT_A_BASE);     // Stop watchdog timer

    ////
    //Port select XT1
    GPIO_setAsPeripheralModuleFunctionInputPin(
        GPIO_PORT_P4,
        GPIO_PIN1 + GPIO_PIN2,
        GPIO_PRIMARY_MODULE_FUNCTION
        );

    //Set external frequency for XT1
    CS_setExternalClockSource(32768);

    //Select XT1 as the clock source for ACLK with no frequency divider
    CS_initClockSignal(CS_ACLK, CS_XT1CLK_SELECT, CS_CLOCK_DIVIDER_1);

    //Start XT1 with no time out
    CS_turnOnXT1(CS_XT1_DRIVE_0);

    //clear all OSC fault flag
    CS_clearAllOscFlagsWithTimeout(1000);

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();

    // L0~L26 & L36~L39 pins selected
    LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_SEGMENT_LINE_26);
    LCD_E_setPinAsLCDFunctionEx(LCD_E_BASE, LCD_E_SEGMENT_LINE_36, LCD_E_SEGMENT_LINE_39);

    LCD_E_initParam initParams = LCD_E_INIT_PARAM;
    initParams.clockDivider = LCD_E_CLOCKDIVIDER_8;
    initParams.muxRate = LCD_E_4_MUX;
    initParams.segments = LCD_E_SEGMENTS_ENABLED;

    // Init LCD as 4-mux mode
    LCD_E_init(LCD_E_BASE, &initParams);

    // LCD Operation - Mode 3, internal 3.08v, charge pump 256Hz
    LCD_E_setVLCDSource(LCD_E_BASE, LCD_E_INTERNAL_REFERENCE_VOLTAGE, LCD_E_EXTERNAL_SUPPLY_VOLTAGE);
    LCD_E_setVLCDVoltage(LCD_E_BASE, LCD_E_REFERENCE_VOLTAGE_3_08V);

    LCD_E_enableChargePump(LCD_E_BASE);
    LCD_E_setChargePumpFreq(LCD_E_BASE, LCD_E_CHARGEPUMP_FREQ_16);

    // Clear LCD memory
    LCD_E_clearAllMemory(LCD_E_BASE);

    // Configure COMs and SEGs
    // L0, L1, L2, L3: COM pins
    // L0 = COM0, L1 = COM1, L2 = COM2, L3 = COM3
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_0, LCD_E_MEMORY_COM0);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_1, LCD_E_MEMORY_COM1);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_2, LCD_E_MEMORY_COM2);
    LCD_E_setPinAsCOM(LCD_E_BASE, LCD_E_SEGMENT_LINE_3, LCD_E_MEMORY_COM3);

    LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xEF);

    //LCD_E_on(LCD_E_BASE);

    /////

    GPIO_setAsOutputPin(GPIO_PORT_P8, GPIO_PIN3);                  // test 8.3
    GPIO_setOutputHighOnPin(GPIO_PORT_P8, GPIO_PIN3);

    // ROWS ARE OUTPUTS

    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN4);                  // Row 1: Output direction
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN5);                  // Row 2: Output direction
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN7);                  // Row 3: Output direction
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7);

    GPIO_setAsOutputPin(GPIO_PORT_P5, GPIO_PIN0);                  // Row 4: Output direction
    GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0);

    // COLUMNS ARE ISR TRIGGERS

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN5, GPIO_PRIMARY_MODULE_FUNCTION);     // Column 1: Input direction
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN5, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN5);
    row = 1;
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN5);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN6, GPIO_PRIMARY_MODULE_FUNCTION);     // Column 2: Input direction
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN6, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN6);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN6);

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);     // Column 3: Input direction
    GPIO_selectInterruptEdge(GPIO_PORT_P1, GPIO_PIN3, GPIO_HIGH_TO_LOW_TRANSITION);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN3);

    _EINT();        // Start interrupt

    PMM_unlockLPM5();           // Need this for LED to turn on- in case of "abnormal off state"
    __bis_SR_register(LPM4_bits + GIE);     // Need this for interrupts or else "abnormal termination"
    __no_operation();           //For debugger

}

void Key()
{

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH



        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)  ){     // Column 1 to GND
            pressedKey = hexaKeys[0][0];        // Shows "1"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0x60);

        }
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW

        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == GPIO_INPUT_PIN_LOW) {    // Column 2
            pressedKey = hexaKeys[0][1];       // Shows "2"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xDB);
        }
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW



        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW) {    // Column 3
            pressedKey = hexaKeys[0][2];       // Shows "3"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xF3);
        }
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5); // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW



        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)  ){     // Column 1 to GND
            pressedKey = hexaKeys[0][0];        // Shows "4"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0x67);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)  ){     // Column 2 to GND
            pressedKey = hexaKeys[0][0];        // Shows "5"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xB7);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW)  ){     // Column 3 to GND
            pressedKey = hexaKeys[0][0];        // Shows "6"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xBF);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW

        //row 3
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)  ){     // Column 1 to GND
            pressedKey = hexaKeys[0][0];        // Shows "7"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xE0);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)  ){     // Column 2 to GND
            pressedKey = hexaKeys[0][0];        // Shows "8"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xFF);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- HIGH

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW)  ){     // Column 3 to GND
            pressedKey = hexaKeys[0][0];        // Shows "9"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xE7);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  LOW


        //row 4
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN5) == GPIO_INPUT_PIN_LOW)  ){     // Column 1 to GND
            pressedKey = hexaKeys[0][0];        // Shows "*"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xEE);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  HIGH

        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN6) == GPIO_INPUT_PIN_LOW)  ){     // Column 2 to GND
            pressedKey = hexaKeys[0][0];        // Shows "0"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xFA);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  HIGH


        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- HIGH
        GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- HIGH
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW

        if ((GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN3) == GPIO_INPUT_PIN_LOW)  ){     // Column 3 to GND
            pressedKey = hexaKeys[0][0];        // Shows "#"
            LCD_E_setMemory(LCD_E_BASE,LCD_E_MEMORY_BLINKINGMEMORY_18, 0xAA);

        }
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputHighOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4  HIGH
        //reset
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN4); // Row 1- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN5);  // Row 2- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN7); // Row 3- LOW
        GPIO_setOutputLowOnPin(GPIO_PORT_P5, GPIO_PIN0); // Row 4- LOW



        LCD_E_on(LCD_E_BASE);
}

#pragma vector = PORT1_VECTOR       // Using PORT1_VECTOR interrupt because P1.4 and P1.5 are in port 1
__interrupt void PORT1_ISR()
{
    Key();
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN3); // Column 3
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN6); // Column 2
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN4); // Row 1
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN5); // Column 1
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN5); // Row 2
    GPIO_clearInterrupt(GPIO_PORT_P2, GPIO_PIN7); // Row 3
    GPIO_clearInterrupt(GPIO_PORT_P5, GPIO_PIN0); // Row 4
}
