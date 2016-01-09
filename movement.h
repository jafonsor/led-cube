#include "cube.h" // Position
#include "Arduino.h"

class Movable {
public:
  virtual void move() = 0;
  virtual Position & pos() = 0;
};

class Movement : public Movable {
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
};

class AheadMovement : public Movement {
public:
  AheadMovement(int x, int y, int z, Direction dir):
    Movement(x,y,z,dir) {}
  
  void move() {
    moveAhead();
  }
};

class XYMovement : public Movable {
  Collection<Position> _positions;
  Iterator<Position> * _it;
  int _z;
  Position _currentPos;
  
public:
  XYMovement(int index, int z=0):
    _positions(8),
    _it(_positions.ciclicalIterator()),
    _z(z)
  {
    // init the positions collection
    Position auxPos(0,0,_z);
    
    for(int x = 0; x < 3; x++) {
      auxPos.x = x;
      _positions.add(auxPos);
    }
    
    for(int y = 1; y < 3; y++) {
      auxPos.y = y;
      _positions.add(auxPos);
    }
    
    for(int x = 1; x >= 0; x--) {
      auxPos.x = x;
      _positions.add(auxPos);
    }
    
    auxPos.y = 1;
    _positions.add(auxPos);
    
    
    _currentPos = _it->next();
    for(int i = 0; i < index; i++)
      _currentPos = _it->next();
  }
  
  void move() {
    _currentPos = _it->next();
    
    /**/
    Serial.print("(");
    Serial.print(_currentPos.x);
    Serial.print(",");
    Serial.print(_currentPos.y);
    Serial.print(",");
    Serial.print(_currentPos.z);
    Serial.println(")");
    /**/
  }
  
  Position & pos() {
    return _currentPos;
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
    Position newPos(_pos);
    Direction newDir = _dir;
     
    newPos.moveOnDir(newDir);
    while(newPos.outsideCube()) {
      newDir = randomNewDir();
      newPos = _pos;
      newPos.moveOnDir(newDir);
    }
    _pos = newPos;
    _dir = newDir;
  }
  
  //Override
  void move() {
    moveRandom();
  }
};
