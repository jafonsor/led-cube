#include <Arduino.h> // digitalWrite, delay, delayMicroseconds

class Cube {
  bool _leds[3][9];
  
  void setAll(bool val) {
    for(int c = 0; c < 3; c++) {
      for(int l = 0; l < 9; l++) {
        _leds[c][l] = val;
      }
    }
  }
  
  bool & led(int x, int y, int z) {
    return _leds[z][3*y+x];
  }
  
  void set(int x, int y, int z, bool val) {
    led(x,y,z) = val;
  } 
  
public:

  void render(int duration) {
    int nLedsOn = 0;
    for(int c = 0; c < 3; c++)
      for(int l = 0; l < 9; l++)
        if(_leds[c][l])
          nLedsOn++;
    if(nLedsOn == 0) {
      delay(duration);
    } else {
      long subFrameTime = 10000l; // 100fps in microseconds
      long betweenLedDelay = 10000l / (long)nLedsOn;
      int numberOfSubFrames = duration/10;
      
      for(int i = 0; i < numberOfSubFrames; i++) {
        for(int c = 0; c < 3; c++) {
          digitalWrite(11+c, HIGH);
          for(int l = 0; l < 9; l++) {
            if(_leds[c][l]) {
              digitalWrite(2+l, HIGH);
              delayMicroseconds(betweenLedDelay);
              digitalWrite(2+l, LOW);
            }
          }
          digitalWrite(11+c, LOW);
        }
      }
      
    }
  }
  
  void on(int x, int y, int z) {
    set(x,y,z, true); 
  }
  
  void off(int x, int y, int z) {
    set(x,y,z, false);
  }
  
  bool isOn(int x, int y, int z) {
    return led(x,y,z);
  }
  
  void allOn()  { setAll(true); }
  void allOff() { setAll(false); }
  
};
