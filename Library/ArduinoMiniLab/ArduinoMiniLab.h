/*
	ArduinoMiniLab
	library for Arduino Mini Lab plateform
*/

#ifndef _MINIARDUINOH_
#define _MINIARDUINOH_
#include <UTFT.h>

#if defined(ARDUINO) && ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
  #include "pins_arduino.h"
#endif

// default reference value in mV
#define DEFAULT_REF 1100UL

//declare the IO associated with the buttons
#define GREEN_BUTTON 4
#define RED_BUTTON 2

//declare the IO associated with the joystick
#define XAXIS A7
#define YAXIS A6

//declare IO associated with the display
#define PIN_SDA 11
#define PIN_SCL 13
#define PIN_CS 10
#define PIN_RST A0
#define PIN_RS 8
#define PIN_BLK 9

// define max et min voltage for battery level display in mV
// MIN is 3.2V because under 3.2V display is very dim but system is still functionnal
// MAX is 4.1V because battery charger halts around 4.15 and battery discharges quickly from 4.2 to 4.1
// discharge curve is nearly linear from 4.1 to 3.2
#define MIN_BAT 3200
#define MAX_BAT 4100
#define BAT_HORIZ true
#define BAT_VERT false

// Declare font size we will be using
#define FONTWIDTH 6
#define FONTHEIGHT 8

struct joystick{
    uint16_t    raw;        // raw returned by analogRead()
    int16_t     centered;   // signed value -255 <--> 255, 0 = rest position
    uint16_t    center;     // value when joystick in rest position
    bool        up;         // when value > 255
    bool        down;       // when value < 255
};


class MiniArduino : public UTFT {
    public:
        MiniArduino(void);
        
        void begin(void);
        void setInternalRefValue(uint16_t ref);
        uint32_t getBattery(void);
        void calibrateJoystick(void);
        void refreshJoysticks(void);
        bool buttonState(uint8_t button);
        void setBacklight(uint8_t level);
        uint8_t getBacklight(void);
        void drawBatteryState(uint8_t pos, uint8_t line, bool orientation);
        struct joystick    XAxis, YAxis;
        
    protected:
    
    private:
        //extern uint8_t SmallFont[];
        uint32_t    internalRef = DEFAULT_REF * 1023UL; // internal reference value in mV
        uint16_t    battery;
        uint8_t     backlight = 255; // backlight level
};
#endif
