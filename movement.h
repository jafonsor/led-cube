#include "cube.h" // Position

class Movement {
protected:
  Position _pos;
  Direction _dir;
  
  Direction oposedDir(Direction dir) {
    switch(dir) {
      case XMinus: return XPlus;
      case XPlus:  return XMinus;
      case YMinus: return YPlus;
      case YPlus:  return YMinus;
      case ZMinus: return ZPlus;
      case ZPlus:  return ZMinus;
    }
  }
  
public:
  Movement(): _pos(0,0,0) {}
  Movement(int x, int y, int z, Direction dir) :
    _pos(x,y,z),
    _dir(dir) {}
  
  void setDir(Direction dir) {
    if(dir >= 0 && dir <= 5)
     _dir = dir;
  }
  
  Direction setDir() { return _dir; }
  
  void moveAhead() {
    Position testPos = _pos;
    testPos.moveOnDir(_dir);
    if(!testPos.outsideCube()) {
      _pos = testPos;
    }
  }
  
  void moveBackwards() {
    Position testPos = _pos;
    testPos.moveOnDir(oposedDir(_dir));
    if(!testPos.outsideCube())
      _pos = testPos;
  }
  
  void set(int x, int y, int z, Direction dir) {
    Position testPos;
    testPos.x = x; testPos.y = y; testPos.z = z;
    if(!testPos.outsideCube())
      _pos = testPos;
    setDir(dir);
  }
  
  void set(Position & pos, Direction dir) {
    if(!pos.outsideCube())
      _pos = pos;
    setDir(dir);
  }
  
  Position & pos() {
    return _pos;
  }
  
  void pos(Position & pos) {
    if(!pos.outsideCube())
      _pos = pos;
  }
  
  virtual void move() = 0;
};

class AheadMovement : public Movement {
public:
  AheadMovement(int x, int y, int z, Direction dir):
    Movement(x,y,z,dir) {}
  
  void move() {
    moveAhead();
  }
};

class XYMovement : public Movement {
  const Direction _dirSeq[4] = {XPlus,YPlus,XMinus,YMinus};
  int _dirIndex = 0;
public:
  XYMovement(int x=0, int y=0, int z=0):
    Movement(x,y,z,_dirSeq[_dirIndex]) {}
  
  void move() {
    Position testPos(_pos);
    testPos.moveOnDir(_dir);
    if(testPos.outsideCube()) {
      testPos = _pos;
      _dirIndex = (_dirIndex + 1) % 4;
      setDir(_dirSeq[_dirIndex]);
      testPos.moveOnDir(_dir);
    }
    _pos = testPos;
  }
};

// Moves on a random direction inside the cube.
class RandomMovement : public Movement {
  Direction randomDir() {
    return (Direction)random(0,6);
  }
  
  int randomPos() {
    return random(0,3);
  }
  
  Direction randomNewDir() {
    Direction newDir = oposedDir(_dir);
    while(newDir == oposedDir(_dir)) {
      newDir = randomDir();
    }
    return newDir;
  }
  
public:
  RandomMovement(int seed) {
    randomSeed(seed);
    _dir = randomDir();
    _pos.x = randomPos();
    _pos.y = randomPos();
    _pos.z = randomPos();
  }
  
  void moveRandom() {
    Position newPos;
    Direction newDir;
    do {
      newDir = randomNewDir();
      newPos = _pos;
      newPos.moveOnDir(newDir);
    } while(newPos.outsideCube());
    _pos = newPos;
    _dir = newDir;
  }
  
  //Override
  void move() {
    moveRandom();
  }
};
