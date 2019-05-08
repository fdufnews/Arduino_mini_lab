# Arduino_mini_lab

## Library
That library inherit from UTFT so you shall install UTFT libray first. You will find the UTFT library in the library manager of the Arduino IDE.

### Library installation
The content of the Library directory should be copied into the "libraries" directory of your sketchbook.

### Library details
A structure is defined to handle joystick information.
For each axis the software can use:
* the raw value (result of the analogRead). The poor quality of the joystick limits the value to the range 200 to 800 which makes it difficult to use.
* a centered value which is a signed value with 0 as the center position of the joystick. The range goes from -255 to 255. There is a dead zone on both ends of each axis.
* up and down are true when the centered positon of the joystick is higher than 255 or lower than -255 (when in the dead zone))

```
struct joystick{
    uint16_t    raw;        // raw returned by analogRead()
    int16_t     centered;   // signed value -255 <--> 255, 0 = rest position
    uint16_t    center;     // value when joystick in rest position
    bool        up;         // when value > 255
    bool        down;       // when value < 255
};
```

Some methods are added to the ones inherited from UTFT. Concerning the UTFT usage informations are located in the doc files of the UTFT library. For the added method, their names are self explanatory and the code is commented.

```
class MiniArduino : public UTFT {
    public:
        MiniArduino(void);
        
        void 		begin(void);
        void 		setInternalRefValue(uint16_t ref);
        uint32_t 	getBattery(void);
        void 		calibrateJoystick(void);
        void 		refreshJoysticks(void);
        bool 		buttonState(uint8_t button);
        void 		setBacklight(uint8_t level);
        uint8_t 	getBacklight(void);
        void 		drawBatteryState(uint8_t pos, uint8_t line, bool orientation);
        struct joystick    XAxis, YAxis;
```

