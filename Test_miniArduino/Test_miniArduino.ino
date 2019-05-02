// Test_miniArduino
//
// Small plateform based on arduino pro mini
// equiped with:
//    an 80x160 TFT screen
//    2 pushbuttons
//    an analog joystick
//
//
// This program requires the UTFT library.
//

#include <UTFT.h>

// size of display
int MINX=0;
int MINY=0;
int MAXX=0;
int MAXY=0;

#define pinSDA 11
#define pinSCL 13
#define pinCS 10
#define pinRST A0
#define pinRS 8
#define pinBKL 9

#define GREEN_BUTTON 4
#define RED_BUTTON 2
#define pinX A7
#define pinY A6

// Declare which fonts we will be using
#define FONTWIDTH 6
#define FONTHEIGHT 8
extern uint8_t SmallFont[];

#define APPNAME ((char*)"miniArduino test")

// Declare an instance of the class
// syntax : UTFT(Model, SDA, SCL, CS, RST[, RS]);
UTFT myGLCD(ST7735_80_160,pinSDA,pinSCL,pinCS,pinRST,pinRS);   // Display module uses SPI

uint8_t greenButton, redButton;
uint16_t xAxis, yAxis, xMin=1023, xMax=0, yMin=1023, yMax=0;

// drawCrosshair
// draws a crosshair
// input:
//  x : x position of the crosshair
//  y : y position of the crosshair
//  w : half width of the crosshair (width of the branch)
//  h : half height of the crosshair
//
// output
//  returns nothing
//
void drawCrosshair(int x, int y, int w,int h){
uint8_t x0, y0, x1, y1;
  x0 = x - w;
  x1 = x + w;
  y0 = y - h;
  y1 = y + h;
  if (x0 < MINX) x0 = MINX;
  if (x1 > MAXX) x1 = MAXX;
  if (y0 < MINY) y0 = MINY;
  if (y1 > MAXY) y1 = MAXY;
  myGLCD.drawLine(x0,y,x1,y);
  myGLCD.drawLine(x,y0,x,y1);
}

void rotatingCrosshair(int x, int y, int w,int timeStep){
  static uint8_t stepNum=0;
  uint8_t x0,x1,y0,y1,x2,x3,y2,y3;
  uint16_t color;

  color=myGLCD.getColor();
  x0=x-w; x2=x-(w-w/4);
  x1=x+w; x3=x+(w-w/4);
  y0=y-w; y2=y-(w-w/4);
  y1=y+w; y3=y+(w-w/4);
  if(stepNum){
    myGLCD.drawLine(x0,y,x1,y);
    myGLCD.drawLine(x,y0,x,y1);
  }else{
    myGLCD.drawLine(x2,y2,x3,y3);
    myGLCD.drawLine(x2,y3,x3,y2);
  }
  delay(timeStep);
  myGLCD.setColor(myGLCD.getBackColor());
  if(stepNum){
    myGLCD.drawLine(x0,y,x1,y);
    myGLCD.drawLine(x,y0,x,y1);
  }else{
    myGLCD.drawLine(x2,y2,x3,y3);
    myGLCD.drawLine(x2,y3,x3,y2);
  }
  stepNum= (stepNum+1)%2;
  myGLCD.setColor(color);
}

// about
// draws the about screen
// input nothing
// output nothing
//
void about(void){
  myGLCD.clrScr();  // clear screen
  myGLCD.setColor(VGA_WHITE); // foreground color
  myGLCD.drawRect(MINX, MINY, MAXX, MAXY);
  //myGLCD.print(APPNAME, MINX+1 , (MINY+MAXY)/2-FONTHEIGHT);
  myGLCD.print(APPNAME, CENTER , (MINY+MAXY)/2-FONTHEIGHT/2);
}

void corners(void){
  myGLCD.clrScr();  // clear screen
  // identify top
  myGLCD.setColor(VGA_WHITE); // foreground color
  myGLCD.drawPixel(0,0);
  myGLCD.setColor(VGA_RED); // foreground color
  myGLCD.drawPixel(0,1);
  myGLCD.setColor(VGA_LIME); // foreground color
  myGLCD.drawPixel(1,0);
  myGLCD.setColor(VGA_BLUE); // foreground color
  myGLCD.drawPixel(1,1);

  //identify bottom
  myGLCD.setColor(VGA_WHITE); // foreground color
  myGLCD.drawPixel(MAXX,MAXY);
  myGLCD.setColor(VGA_RED); // foreground color
  myGLCD.drawPixel(MAXX,MAXY-1);
  myGLCD.setColor(VGA_LIME); // foreground color
  myGLCD.drawPixel(MAXX-1,MAXY);
  myGLCD.setColor(VGA_BLUE); // foreground color
  myGLCD.drawPixel(MAXX-1,MAXY-1);
}

void setup()
{
  Serial.begin(115200);
  // setup Buttons
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);

  // setup ADC
  analogReference(INTERNAL);
// Setup the LCD
  myGLCD.InitLCD(LANDSCAPE); // init display and sets orientation
  MAXX= myGLCD.getDisplayXSize()-1;
  MAXY= myGLCD.getDisplayYSize()-1;
  myGLCD.setFont(SmallFont); // declare the font we will use
  myGLCD.setOffsetXY(24,0);    // offset for 80x160 TFT display with Red tab
  pinMode(pinBKL,OUTPUT); // pinBKL pin drives display backlight
  analogWrite(pinBKL,255); // full on
  about();
  myGLCD.setColor(VGA_AQUA); // foreground color
  drawCrosshair(MINX+20, MINY+20, 10,7);
  myGLCD.setColor(VGA_FUCHSIA); // foreground color
  drawCrosshair(MAXX-20, MAXY-20, 10,7);
  myGLCD.setColor(VGA_YELLOW); // foreground color
  drawCrosshair(MAXX-20, MINY+20, 10,7);
  myGLCD.setColor(VGA_SILVER); // foreground color
  drawCrosshair(MINX+20, MAXY-20, 10,7);

  myGLCD.setColor(VGA_GREEN); // foreground color
  for(uint8_t iter=0;iter<20;iter++){
    rotatingCrosshair(MAXX/2,20,10,200);
  }
  delay(2000);
  myGLCD.clrScr();
}

void loop()
{

  greenButton = digitalRead(GREEN_BUTTON);
  redButton = digitalRead(RED_BUTTON);
  xAxis= analogRead(pinX);
  delay(10);
  xAxis= analogRead(pinX);
  yAxis= analogRead(pinY);
  delay(10);
  xAxis= analogRead(pinX);

  if (xAxis < xMin) xMin = xAxis;
  if (xAxis > xMax) xMax = xAxis;

  if (yAxis < yMin) yMin = yAxis;
  if (yAxis > yMax) yMax = yAxis;

  myGLCD.setColor(VGA_GREEN);
  myGLCD.print("Green: ",1,0);
  myGLCD.print(greenButton?"Open  ":"Closed", 50, 0);
  myGLCD.setColor(VGA_RED);
  myGLCD.print("Red  : ",1,10);
  myGLCD.print(redButton?"Open  ":"Closed", 50, 10);
  myGLCD.setColor(VGA_WHITE);
  myGLCD.print("xAxis: ",1,20);
  myGLCD.printNumI(xAxis, 50, 20);
  myGLCD.printNumI(xMin, 90, 20);
  myGLCD.printNumI(xMax, 130, 20);
  myGLCD.print("yAxis: ",1,30);
  myGLCD.printNumI(yAxis, 50, 30);
  myGLCD.printNumI(yMin, 90, 30);
  myGLCD.printNumI(yMax, 130, 30);
  delay(100);
}
