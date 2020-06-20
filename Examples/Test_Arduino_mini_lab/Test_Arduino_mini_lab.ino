#include <UTFT.h>
#include <MiniArduino.h>

/* Test_Arduino_mini_lab
    //
    // Small plateform based on an arduino pro mini
    // equiped with:
    //      an 80x160 TFT screen
    //      2 pushbuttons
    //      an analog joystick
    //
    //
    // This program requires the MiniArduino library.
*/

// size of display
int MINX = 0;
int MINY = 0;
int MAXX = 0;
int MAXY = 0;

unsigned long timeReadBat = 0;
unsigned long delayReadBat = 10000L;

#define APPNAME ((char*)"mini_lab test")

MiniArduino myMini = MiniArduino();


void rotatingCrosshair(int x, int y, int w) {
    static uint8_t stepNum = 0;
    uint8_t x0, x1, y0, y1, x2, x3, y2, y3;
    uint16_t color;

    x0 = x - w; x2 = x - (w - w / 4);
    x1 = x + w; x3 = x + (w - w / 4);
    y0 = y - w; y2 = y - (w - w / 4);
    y1 = y + w; y3 = y + (w - w / 4);
    color = myMini.getColor();
    myMini.setColor(myMini.getBackColor());
    if (stepNum) {
        myMini.drawLine(x0, y, x1, y);
        myMini.drawLine(x, y0, x, y1);
    } else {
        myMini.drawLine(x2, y2, x3, y3);
        myMini.drawLine(x2, y3, x3, y2);
    }
    
    stepNum = (stepNum + 1) % 2;
    myMini.setColor(color);
    if (stepNum) {
        myMini.drawLine(x0, y, x1, y);
        myMini.drawLine(x, y0, x, y1);
    } else {
        myMini.drawLine(x2, y2, x3, y3);
        myMini.drawLine(x2, y3, x3, y2);
    }
}

// about
// draws the about screen
// input nothing
// output nothing
//
void about(void) {
    myMini.clrScr();    // clear screen
    myMini.setColor(VGA_WHITE); // foreground color
    myMini.drawRect(MINX, MINY, MAXX, MAXY);
    myMini.print(APPNAME, CENTER , (MINY + MAXY) / 2 - FONTHEIGHT / 2);
}


void setup(void) {
    myMini.begin();
    MAXX = myMini.getDisplayXSize() - 1;
    MAXY = myMini.getDisplayYSize() - 1;
    about();
    myMini.setColor(VGA_AQUA); // foreground color
    myMini.drawCrosshair(MINX + 20, MINY + 20, 10, 7);
    myMini.setColor(VGA_FUCHSIA); // foreground color
    myMini.drawCrosshair(MAXX - 20, MAXY - 20, 10, 7);
    myMini.setColor(VGA_YELLOW); // foreground color
    myMini.drawCrosshair(MAXX - 20, MINY + 20, 10, 7);
    myMini.setColor(VGA_SILVER); // foreground color
    myMini.drawCrosshair(MINX + 20, MAXY - 20, 10, 7);

    myMini.setColor(VGA_GREEN); // foreground color
    for (uint8_t iter = 0; iter < 10; iter++) {
        rotatingCrosshair(MAXX / 2, 20, 10);
        delay(200);
    }
    delay(1000);

    myMini.setInternalRefValue(1109); // Bandgap reference is 1,109
    myMini.clrScr();
}

void showButtonState(uint8_t line) {
    myMini.setColor(VGA_GREEN);
    myMini.print(myMini.buttonState(GREEN_BUTTON) ? F("1") : F("0"), 120, line);

    myMini.setColor(VGA_RED);
    myMini.print(myMini.buttonState(RED_BUTTON) ? F("1") : F("0"), 140, line);
}

void showJoystick(uint8_t line) {
    myMini.setColor(VGA_WHITE);
    myMini.print(F("X"), 40, line);
    myMini.printNumI(myMini.XAxis.raw, 20, line + 10, 4, '0');
    myMini.printNumI(myMini.XAxis.center, 20, line + 20, 4, '0');
    myMini.printNumI(myMini.XAxis.centered, 20, line + 30, 4, '0');
    myMini.print(myMini.XAxis.up ? F("UP  ") : myMini.XAxis.down ? F("DOWN") : F("    "), 20, line + 40);

    myMini.print(F("Y"), 120, line);
    myMini.printNumI(myMini.YAxis.raw, 100, line + 10, 4, '0');
    myMini.printNumI(myMini.YAxis.center, 100, line + 20, 4, '0');
    myMini.printNumI(myMini.YAxis.centered, 100, line + 30, 4, '0');
    myMini.print(myMini.YAxis.up ? F("UP  ") : myMini.YAxis.down ? F("DOWN") : F("    "), 100, line + 40);
}


void loop(void) {

    showButtonState(0);

    if ( (millis() - timeReadBat) > delayReadBat) {
        myMini.drawBatteryState(130, 12,BAT_HORIZ);
        myMini.drawBatteryState(77, 30,BAT_VERT);
        timeReadBat = millis();
    }

    myMini.refreshJoysticks();
    showJoystick(20);
    int16_t val= map(myMini.XAxis.centered,-255,255,0,255);
    myMini.printNumI(val, 80, 10, 3, '0');
    //gauge(10,10,val,0,255,50,5,25);
    myMini.gauge(10,10,val,0,255,64,8,32, redraw);
    delay(50);
}
