#include "cube.h" // Position

class Movement {
protected:
  Position _pos;
  int _currentDir;
  
public:
  Movement(): _pos(0,0,0) {}
  Movement(int x, int y, int z, int dir) :
    _pos(x,y,z),
    _currentDir(dir) {}
  
  void setDir(int dir) {
     _currentDir = dir; 
  }
  
  void moveAhead() {
    _pos.moveOnDir(_currentDir);
  }
  
  void set(int x, int y, int z, int dir) {
    _pos.x = x; _pos.y = y; _pos.z = z;
    _currentDir = dir;
  }
  
  void set(Position & pos, int dir) {
    _pos = pos;
    _currentDir = dir;
  }
  
  void getPosition(Position & pos) {
    pos = _pos;
  }
  
  Position & getPosition() {
    return _pos;
  }
  
  void setPosition(Position & pos) {
    _pos = pos;
  }
};

class RandomMovement : public Movement {
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
public:
  RandomMovement(int seed) {
    randomSeed(seed);
    _currentDir = randomDir();
    _pos.x = randomPos();
    _pos.y = randomPos();
    _pos.z = randomPos();
  }
  
  void moveRandom() {
    Position newPos;
    int newDir;
    do {
      newDir = randomNewDir();
      newPos = _pos;
      newPos.moveOnDir(newDir);
    } while(newPos.outsideCube());
    _pos = newPos;
    _currentDir = newDir;
  }
};
