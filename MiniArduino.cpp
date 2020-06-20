/*
	MiniArduino
	library for MiniArduino plateform
*/

#include <UTFT.h>
#include <digitalWriteFast.h>
#include <MiniArduino.h>

extern uint8_t SmallFont[];

MiniArduino::MiniArduino(void) : UTFT(ST7735_80_160,PIN_SDA,PIN_SCL,PIN_CS,PIN_RST,PIN_RS){
}

/* begin
    Initialize the display, the IOs, the ADC's ref
    
    input:
            nothing
    output:
            nothing
*/
void MiniArduino::begin(void){
    // setup Buttons
    pinMode(RED_BUTTON, INPUT_PULLUP);
    pinMode(GREEN_BUTTON, INPUT_PULLUP);

    // setup ADC reference to internal as we are working with a battery
    analogReference(INTERNAL);

    // setup display on  
    InitLCD(LANDSCAPE); // init display and sets orientation
    setFont(SmallFont); // declare the font we will use
    setOffsetXY(24,0);    // set the offset for the 80x160 TFT display with Red tab
    maxx = getDisplayXSize();
    maxy = getDisplayYSize();
    pinMode(PIN_BLK,OUTPUT); // pinBLK pin drives display backlight
    analogWrite(PIN_BLK,backlight); // full on

    clrScr();

    // setup joystick
    print(F("calibrating joystick"), CENTER, 35);
    calibrateJoystick();
}

/* setInternalRefValue
    sets value of the internal bandgap reference used to compute battery level
    Ref value is multiplied by 1023 because:
    
    Vref   ADCvalue
    ---- = --------
    VCC      1023
    
    input:
            long reference value in millivolts
    output:
            nothing
*/
void MiniArduino::setInternalRefValue(uint16_t ref){
    internalRef = (uint32_t)ref * 1023UL;
}

/* battery
    Measure battery level
    
    Change ADC's ref to Vcc and read internal bandgap reference value
    Compute battery level
    Switch back ADC's ref to internal
    Note: internalRef shall be multiplied by 1023
    
    input:
            nothing
    output:
            uint16_t : battery level in mV
*/
uint32_t MiniArduino::getBattery(void){
    uint32_t result;
    // Read 1.1V reference against AVcc
    uint8_t sav_reg = ADMUX;
    ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
    delay(5); // Wait for Vref to settle
    ADCSRA |= _BV(ADSC); // Convert
    while (bit_is_set(ADCSRA,ADSC));
    result = ADCL;
    result |= ADCH<<8;
    result = internalRef / result; // Back-calculate AVcc in mV
    ADMUX = sav_reg;              // restore previous ADC configuration
    return result;
}

/* textBatteryState
    print the voltage of the battery with this format x.xV
    
    input:
            uint8_t pos: position along x axis
            uint8_t line : position along y axis
            
    output:
            nothing

*/
void MiniArduino::textBatteryState(uint8_t pos, uint8_t line) {
    unsigned int batVal = getBattery()/100; // getBatvoltage (in mV)
    printNumI(batVal/10, pos, line);
    print(".", pos + FONTWIDTH, line);
    printNumI(batVal%10, pos + 2 * FONTWIDTH, line);
    print("V", pos + 3 * FONTWIDTH, line);
}

/* drawBatteryState
    Draws a battery symbol with the inside indicating battery level
    level is coded both by the color and the size of the filling
    
    input:
            uint8_t pos : X coordinate of top left the symbol
            uint8_t pos : Y coordinate of top left the symbol
            bool orientation : draws the battery horizontal or vertical using either one of BAT_HORIZ or BAT_VERT
    output:
            nothing
*/
void MiniArduino::drawBatteryState(uint8_t pos, uint8_t line, bool orientation) {
    // define a colormap for battery state from red for 0 to green for 10
    // colors are coded {RED, GREEN} in the table
    uint8_t const colorMap[11][2] = {{255,0},{200,16},{192,32},{160,64},{128,128},{128,128},{64,160},{64,160},{32,192},{16,200},{0,255}};
    long batVal = getBattery(); // get battery voltage (in mV)
    uint16_t level;
    // constrain and map level in the 0 to 10 range
    level = constrain(batVal, MIN_BAT, MAX_BAT );
    level = map(level, MIN_BAT, MAX_BAT, 0, 10);

    if (orientation == BAT_HORIZ){
        setColor(VGA_WHITE);
        // draw battery
        fillRect(pos, line+2, pos+2, line+5);  // Draw battery tip
        drawRect(pos+2, line, pos+16, line+7);  // Draw battery contour
        setColor(VGA_BLACK);
        fillRect(pos+3, line+1, pos+15, line+6);  // Erase battery interior
        setColor(colorMap[level][0], colorMap[level][1], 0); // set color using colorMap
        fillRect(pos+14 - level, line+2, pos+14, line+5); // Draw battery level
    } else {
        setColor(VGA_WHITE);
        // draw battery
        fillRect(pos+2, line, pos+5, line+2);  // Draw battery tip
        drawRect(pos, line+2, pos+7, line+16);  // Draw battery contour
        setColor(VGA_BLACK);
        fillRect(pos+1, line+3, pos+6, line+15);  // Erase battery interior
        setColor(colorMap[level][0], colorMap[level][1], 0); // set color using colorMap
        fillRect(pos+2, line+14 - level, pos+5, line+14); // Draw battery level
    }
}


/* calibrateJoystick
    Find value of both axis of joystick while in rest position

    Makes multiple reads of both joystick axis and make an average
    keep value in joystick struct for later use
    
    input:
            nothing
    output:
            nothing
*/
void MiniArduino::calibrateJoystick(void){
    uint16_t pos;

    pos = 0;
    // makes 17 acquisition but reject the first one
    for(uint8_t i=0; i<17; i++){
        if (i>0) pos += analogRead(XAXIS); // reject first sample
        delay(50);
    }
    pos = pos >> 4;     // divide by 16
    XAxis.center = pos; // initialize structure with new values
    XAxis.raw = pos;
    XAxis.centered = 0;
    XAxis.up = false;
    XAxis.down = false;

    pos = 0;
    for(uint8_t i=0; i<17; i++){
        if (i>0) pos += analogRead(YAXIS);
        delay(50);
    }
    pos = pos >> 4;
    YAxis.center = pos;
    YAxis.raw = pos;
    YAxis.centered = 0;
    YAxis.up = false;
    YAxis.down = false;
}

/* refreshJoysticks
    refresh content of joystick structures
    
    input:
            nothing
    output:
            nothing    
*/
void MiniArduino::refreshJoysticks(void){
  uint16_t pos;
  int16_t pos2;
  
    pos = analogRead(XAXIS);

    XAxis.raw = pos;                    // update raw value in struct
    pos2 = (int16_t)pos - XAxis.center; // substract offset
    XAxis.up = false;                   // up & down to default
    XAxis.down = false;
    if (pos2>255){                      // if axis at maximum
        XAxis.centered = 255;           // saturate value
        XAxis.up = true;                // update up
    } else if (pos2<-255){              // if axis at minimum
        XAxis.centered = -255;          // saturate value
        XAxis.down = true;              // update down
    } else {
        XAxis.centered = pos2;          // else update centered value
    }

    pos = analogRead(YAXIS);

    YAxis.raw = pos;
    pos2 = (int16_t)pos - YAxis.center;
    YAxis.up = false;
    YAxis.down = false;
    if (pos2>255){
        YAxis.centered = 255;
        YAxis.up = true;
    } else if (pos2<-255){
        YAxis.centered = -255;
        YAxis.down = true;
    } else {
        YAxis.centered = pos2;
    }
}

/* buttonState
    Returns state of a button
    
    input:
            uint8_t : button to test (RED_BUTTON or GREEN_BUTTON)
    output:
            boolean : true or false (true if button is pushed)
*/
bool MiniArduino::buttonState(uint8_t button){
    if (button==RED_BUTTON)
        return digitalReadFast(RED_BUTTON)==0? true : false;
    else
        return digitalReadFast(GREEN_BUTTON)==0? true : false;
}

/* setBacklight
    Set new backlight level
    
    input:
            uint8_t : level of backlight 0 --> 255 (0: off, 255: max)
    output:
            nothing
*/
void MiniArduino::setBacklight(uint8_t level){
    backlight = level;
    analogWrite(PIN_BLK, backlight);
}

/* getBacklight
    Get current backlight level
    
    input:
            nothing
    output:
            uint8_t : level of backlight 0 --> 255 (0: off, 255: max)
*/
uint8_t MiniArduino::getBacklight(void){
    return backlight;
}

/* drawCrosshair
    Draws a crosshair with the current foreground color
    
    input:
            uint8_t x, uint8_t y : coordinate of the center of the crosshair
            uint8_t w, uint8_t h : width and height of the branches of the crosshair
            The crosshair is twice the width and the height given by the arguments
        
    ouput:
            nothing

*/
void MiniArduino::drawCrosshair(uint8_t x, uint8_t y, uint8_t w, uint8_t h) {
    uint8_t x0, y0, x1, y1;
    x0 = x - w;
    x1 = x + w;
    y0 = y - h;
    y1 = y + h;
    if (x0 < minx) x0 = minx;
    if (x1 > maxx) x1 = maxx;
    if (y0 < miny) y0 = miny;
    if (y1 > maxy) y1 = maxy;
    drawLine(x0, y, x1, y);
    drawLine(x, y0, x, y1);
}

/* gauge
    Draws an axis with divisions put a cursor on it
    overall size is len along X axis and 6 along Y axis
    smaller divisions are 1 pixel apart the X axis
    larger division are ! pixel over X axis and 3 pixels under
    Cursor is 
    
    intput:
            uint8_t x       : X coordinate of top left the symbol
            uint8_t y       : Y coordinate of top left the symbol
            uint8_t val     : value to map on the axis
            uint8_t minVal  : minimum value val can reach
            uint8_t maxVal  : maximum value val can reach
            uint8_t len     : length of the axis
            uint8_t step1   : smaller division on axis
            uint8_t step2   : larger division on axis step2 shall a multiple of step1
            boolean forceRedraw : force redrawing the axis if true

*/
void MiniArduino::gauge(uint8_t x, uint8_t y, uint8_t val, uint8_t minVal, uint8_t maxVal, uint8_t len, uint8_t step1, uint8_t step2, uint8_t forceRedraw){
    static uint8_t pos=0; // cursor position
    setColor(VGA_BLACK);
    drawLine(x+pos, y, x+pos, y+3);    // erase cursor at current position
    drawLine(x+pos-1, y, x+pos-1, y+1);
    drawLine(x+pos+1, y, x+pos+1, y+1);
    if(forceRedraw){
        setColor(VGA_WHITE);
        drawLine(x, y+5, x+len, y+5);  // draws x axis
        for(uint8_t i=0; i<len+1; i+=step1){  // draws a marker every step1
            if (i%step2==0)
                drawLine(x+i, y+4, x+i, y+8);   // draws a large marker every step2
            else
                drawLine(x+i, y+4, x+i, y+6);
        }
    }
    pos = map(val, minVal, maxVal, 0, len);
    setColor(VGA_RED);
    drawLine(x+pos, y, x+pos, y+3);
    drawLine(x+pos-1, y, x+pos-1, y+1);
    drawLine(x+pos+1, y, x+pos+1, y+1);
}

