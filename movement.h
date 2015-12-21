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
