#include <stdlib.h>
#include <Arduino.h>
 
void renderFrame(bool frame[3][9], int duration);

#define ALL_ON {{true,true,true,true,true,true,true,true,true},{true,true,true,true,true,true,true,true,true},{true,true,true,true,true,true,true,true,true}}
#define ALL_OFF {{false,false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false,false},{false,false,false,false,false,false,false,false,false}}
typedef struct {
public:
  bool leds[3][9];
  int duration;
  
  void render() {
    renderFrame(leds,duration);
  }
  
  bool & led(int x, int y, int z) {
    return leds[z][3*y+x];
  }
  
  void set(int x, int y, int z, bool val) {
    led(x,y,z) = val;
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
}Frame;

class Animation {
protected:
  Frame & _frame;
  
public:
  Animation(Frame & frame) : _frame(frame)
  {
    // empty
  }
  
  virtual bool finished();
  virtual void updateFrame() = 0;
  
  void animate() {
    do {
      updateFrame();
      _frame.render();
    } while(!finished());
  }
};

class OneFrameAnim : public Animation {
public:
  OneFrameAnim(Frame &frame) : Animation(frame)
  {
    // nothing
  }
  bool finished() {
    return true;
  }
  
  void updateFrame() {
    // do nothing
  }
};

class AllOnAnim : public OneFrameAnim {
  Frame allOn = {ALL_ON,1000};
public:
  AllOnAnim() : OneFrameAnim(allOn)
  {
    // nothing
  }
};

class AllOffAnim : public OneFrameAnim {
  Frame allOff = {ALL_OFF,500};
public:
  AllOffAnim() : OneFrameAnim(allOff)
  {
    // nothing
  }
};

class NFramesAnim : public Animation {
  int _framesUntilEnd;
public:
  NFramesAnim(Frame & frame, int nFrames) :
    Animation(frame), _framesUntilEnd(nFrames)
  {
    // do nothing
  }
  
  bool finished() {
    return _framesUntilEnd <= 0;
  };
  
  void updateFrame() {
    _framesUntilEnd--;
  }
};

class MoveRandom : public NFramesAnim {
  Frame initialFrame = {ALL_OFF,250};
  int _currentDir; // <pos:-1,+1> x: 0,1; y: 2,3; z: 4,5
  int _x, _y, _z;
  
  int randomDir() {
    return random(0,6);
  }
  
  int randomPos() {
    return random(0,3);
  }
  
  int randomNewDir() {
    int newDir = oposedDir(_currentDir);
    while(newDir == oposedDir(_currentDir)) {
      newDir = randomDir();
      
      Serial.println(newDir);
    }
    return newDir;
  }
  
  int oposedDir(int dir) {
    switch(dir) {
      case 0: return 1;
      case 1: return 0;
      case 2: return 3;
      case 3: return 2;
      case 4: return 5;
      case 5: return 4;
    }
  }
  
  void moveOnDir(int & x, int & y, int & z, int dir) {
    switch(dir) {
      case 0: x--; break;
      case 1: x++; break;
      case 2: y--; break;
      case 3: y++; break;
      case 4: z--; break;
      case 5: z++; break;
    }
  }
  
  bool outSideCube(int x, int y, int z) {
    return x < 0 || x > 2 || y < 0 || y > 2 || z < 0 || z > 2;
  }
  
public:
  MoveRandom(int seed) : 
    NFramesAnim(initialFrame,20),
    _currentDir(0)
  {
    randomSeed(seed);
    _currentDir = randomDir();
    _x = randomPos();
    _y = randomPos();
    _z = randomPos();
    _frame.on(_x, _y, _z);
  }
  
  // Override
  void updateFrame() {
    NFramesAnim::updateFrame();
    /**/
    int x, y, z;
    int newDir;
    do {
      newDir = randomNewDir();
      x = _x; y = _y; z = _z;
      moveOnDir(x,y,z, newDir);
    } while(outSideCube(x,y,z));
    _frame.off(_x,_y,_z);
    _frame.on(x,y,z);
    _x = x; _y = y; _z = z;
    _currentDir = newDir;
    /**/
  }
};

