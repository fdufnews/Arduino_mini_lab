#include <UTFT.h>
#include <MiniArduino.h>

/* Template for Arduino_mini_lab
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
const uint8_t posBat = 130;

boolean redraw = true;

// variables used to manage battery level display
unsigned long timeReadBat = 0;        // last time battery level was read
unsigned long delayReadBat = 10000L;  // delay between consecutive battery read

// Define your app name here it will be displayed at startup
#define APPNAME ((char*)"mini_lab test")

// create an instance of MiniArduino
MiniArduino myMini = MiniArduino();


/* about
      draws the about screen

      input:
            nothing
            
      output:
            nothing
*/
void about(void) {
    myMini.clrScr();    // clear screen
    myMini.setColor(VGA_WHITE); // foreground color
    myMini.drawRect(MINX, MINY, MAXX, MAXY);
    myMini.print(APPNAME, CENTER , (MINY + MAXY) / 2 - FONTHEIGHT / 2);
}

/* setup

    Initializes display and peripherals
    Show an about screen

*/
void setup(void) {
    myMini.begin();   // display and peripheral initialization
    MAXX = myMini.getDisplayXSize() - 1; // display size
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

    delay(3000);

    myMini.setInternalRefValue(1109); // Set bandgap reference real value ( 1,109V )
    myMini.clrScr();
    myMini.setColor(VGA_WHITE);
    myMini.textBatteryState(posBat,10);  // displays battery value
    myMini.drawBatteryState(posBat, 0, BAT_HORIZ);// draws battery symbol at top right corner
    timeReadBat = millis();
}

void loop(void) {

    // update battery state every now and then
    if ( (millis() - timeReadBat) > delayReadBat) {
        myMini.setColor(VGA_WHITE);
        myMini.textBatteryState(posBat,10);
        myMini.drawBatteryState(posBat, 0, BAT_HORIZ);
        timeReadBat = millis();
    }

    myMini.refreshJoysticks(); // comment this line if the joystick is not used
    // put your code here
}
