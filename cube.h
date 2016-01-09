#ifndef CUBE_H
#define CUBE_H

#include <Arduino.h> // digitalWrite, delay, delayMicroseconds


enum Direction { XMinus, XPlus, YMinus, YPlus, ZMinus, ZPlus }; 

struct Position;
typedef struct Position Position;

struct Position {
  int x,y,z;
public:
  Position() {}
  Position(int xi, int yi, int zi) {
   x = xi; y = yi; z = zi;
  } 

  void readTo(int & xIn, int & yIn, int & zIn) {
    xIn = x; yIn = y; zIn = z;
  }
  
  void readTo(Position & p) {
    p.x = x; p.y = y; p.z = z;
  }
  
  bool outsideCube() {
    return x < 0 || x > 2 || y < 0 || y > 2 || z < 0 || z > 2;
  }
  
  void moveOnDir(Direction dir) {
    switch(dir) {
      case XMinus: x--; break;
      case XPlus:  x++; break;
      case YMinus: y--; break;
      case YPlus:  y++; break;
      case ZMinus: z--; break;
      case ZPlus:  z++; break;
    }
  }
};

bool operator==(const Position & p1, const Position & p2) {
  return p1.x == p2.x && p1.y == p2.y && p1.z == p2.z;
}

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
    if(!outsideCube(x,y,z))
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
  
  void on(Position & pos) {
    on(pos.x, pos.y, pos.z);
  }
  
  void off(int x, int y, int z) {
    set(x,y,z, false);
  }
  
  void off(Position & pos) {
    off(pos.x, pos.y, pos.z);
  }
  
  void toggle(Position & pos) {
    set(pos.x, pos.y, pos.z, !isOn(pos.x, pos.y, pos.z));
  }
  
  bool isOn(int x, int y, int z) {
    return led(x,y,z);
  }
  
  void allOn()  { setAll(true); }
  void allOff() { setAll(false); }
  
  static bool outsideCube(int x, int y, int z) {
    return x < 0 || x > 2 || y < 0 || y > 2 || z < 0 || z > 2;
  }
  
};

#endif
