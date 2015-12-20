#include <Arduino.h>
#include "collection.h"

typedef struct {
public:
  bool leds[3][9];
  int duration;
  
  void render() {
    int nLedsOn = 0;
    for(int c = 0; c < 3; c++)
      for(int l = 0; l < 9; l++)
        if(leds[c][l])
          nLedsOn++;
    if(nLedsOn == 0) {
      //Serial.println("n leds == 0");
      delay(duration);
    } else {
      //Serial.println("n leds > 0");
      long subFrameTime = 10000l; // 100fps in microseconds
      long betweenLedDelay = 10000l / (long)nLedsOn;
      int numberOfSubFrames = duration/10;
      
      for(int i = 0; i < numberOfSubFrames; i++) {
        for(int c = 0; c < 3; c++) {
          digitalWrite(11+c, HIGH);
          for(int l = 0; l < 9; l++) {
            if(leds[c][l]) {
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
  
  void setAll(bool val) {
    for(int c = 0; c < 3; c++) {
      for(int l = 0; l < 9; l++) {
        leds[c][l] = val;
      }
    }
  }
  
  void allOn()  { setAll(true); }
  void allOff() { setAll(false); }
  
} Frame;


class Animation {
public:
  virtual void setUpFrame(Frame & frame) = 0;
  virtual void updateFrame(Frame & frame) {
    // do nothing
  }
  
  virtual bool finished() {
    return true;
  }
};

class AnimationManager {
  const static int _max_anims = 20;
  Collection<Animation*> _anims;
  Frame _frame;
public:
  AnimationManager(): _anims(_max_anims) {}
  
  void runAnimations() {
    Iterator<Animation*> *it = _anims.iterator();
    Animation * currentAnim;
    while(it->hasNext()) {
      currentAnim = it->next();
      currentAnim->setUpFrame(_frame);
      do {
        _frame.render();
        currentAnim->updateFrame(_frame);
      } while(!currentAnim->finished());
    }
    delete it;
  }
  
  bool addAnim(Animation * animation) {
    return _anims.add(animation);
  }
};

class AllOnAnim : public Animation {
  int _duration;
public:
  AllOnAnim(int duration) : _duration(duration) {}
  void setUpFrame(Frame & frame) {
    frame.duration = _duration;
    frame.allOn();
  }
};

class AllOffAnim : public Animation {
  int _duration;
public:
  AllOffAnim(int duration) : _duration(duration) {}
  void setUpFrame(Frame & frame) {
    frame.duration = _duration;
    frame.allOff();
  }
};

class RandomMovement {
  int _x, _y, _z;
  int _currentDir;
  
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
  RandomMovement(int seed) {
    randomSeed(seed);
    _currentDir = randomDir();
    _x = randomPos();
    _y = randomPos();
    _z = randomPos();
  }
  
  void getPosition(int & x, int & y, int & z) {
    x = _x; y = _y; z = _z;
  }
  
  void move() {
    int x, y, z;
    int newDir;
    do {
      newDir = randomNewDir();
      x = _x; y = _y; z = _z;
      moveOnDir(x,y,z, newDir);
    } while(outSideCube(x,y,z));
    _x = x; _y = y; _z = z;
    _currentDir = newDir;
  }
};

class RepeatAnim : public Animation {
  const int _max_repeats;
  int _number_of_repeats;
  Animation * _animation;
public:
  RepeatAnim(int repeats, Animation * animation) :
    _max_repeats(repeats),
    _number_of_repeats(0),
    _animation(animation) {}
  
  ~RepeatAnim() {
    delete _animation;
  }
    
  void setUpFrame(Frame & frame) {
    _animation->setUpFrame(frame);
  }
  
  void updateFrame(Frame & frame) {
    _animation->updateFrame(frame);
  }
  
  bool finished() {
    if(_number_of_repeats >= _max_repeats) {
      _number_of_repeats = 0;
      return true;
    } else {
      _number_of_repeats++;
      return false;
    }
  }
};

class RandomMoveAnim : public Animation {
  int _nMoves;
  int _moveDuration;
  RandomMovement _movement;
public:
  RandomMoveAnim(int moveDuration) :
    _moveDuration(moveDuration),
    _movement(9999) {}
  
  void setUpFrame(Frame & frame) {
    int x, y, z;
    frame.duration = _moveDuration;
    _movement.getPosition(x,y,z);
    frame.allOff();
    frame.on(x,y,z);
  }
  
  void updateFrame(Frame & frame) {
    int x, y, z;
    _movement.getPosition(x,y,z);
    frame.off(x,y,z);
    _movement.move();
    _movement.getPosition(x,y,z);
    frame.on(x,y,z);
  }
};

class AnimSeq : public Animation {
  Collection<Animation*> * _anims;
  Iterator<Animation*> * _it = 0;
  Animation * _currentAnim;
public:
  AnimSeq(Collection<Animation*> * collection) :
    _anims(collection) {}
  
  ~AnimSeq() {
    delete _anims;
  }
  
  void setUpFrame(Frame & frame) {
    if(_it != 0)
      delete _it;
    _it = _anims->iterator();
    _currentAnim = _it->next();
    _currentAnim->setUpFrame(frame);
  }
  
  void updateFrame(Frame & frame) {
    if(_currentAnim->finished()) {
      _currentAnim = _it->next();  
      _currentAnim->setUpFrame(frame);
    } else {
      _currentAnim->updateFrame(frame);
    }
  }
  
  bool finished() {
    return !_it->hasNext();
  }
};

