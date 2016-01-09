#include "collection.h"
#include "cube.h"
#include "movement.h"

#include "Arduino.h"

class Animation {
  public:
    virtual void animate(Cube & cube) = 0;
};

class AnimSeq : public Animation {
    Collection<Animation*> _anims;
  public:
    AnimSeq(int max_anims) : _anims(max_anims) {}

    void animate(Cube & cube) {
      Iterator<Animation*> * it = _anims.iterator();
      while (it->hasNext()) {
        it->next()->animate(cube);
      }
      delete it;
    }

    bool addAnim(Animation * anim) {
      return _anims.add(anim);
    }
};

class AnimationManager {
    const static int MAX_ANIM_NUMBER = 20;
    AnimSeq _anims;
    Cube _cube;
  public:
    AnimationManager() : _anims(MAX_ANIM_NUMBER) {}

    void runAnimations() {
      _anims.animate(_cube);
    }

    bool addAnim(Animation * anim) {
      return _anims.addAnim(anim);
    }
};

class WithDuration {
    const int _duration;
  public:
    WithDuration(int duration) : _duration(duration) {}
    inline int duration() {
      return _duration;
    }
};

class AllOnAnim : public Animation, public WithDuration {
  public:
    AllOnAnim(int duration) : WithDuration(duration) {}
    void animate(Cube & cube) {
      cube.allOn();
      cube.render(duration());
    }
};

class AllOffAnim : public Animation, public WithDuration {
  public:
    AllOffAnim(int duration) : WithDuration(duration) {}
    void animate(Cube & cube) {
      cube.allOff();
      cube.render(duration());
    }
};

class RepeatAnim : public Animation {
    const int _times_to_repeat;
    Animation * _animation;
  public:
    RepeatAnim(int times_to_repeat, Animation * animation):
      _times_to_repeat(times_to_repeat),
      _animation(animation) {}

    void animate(Cube & cube) {
      for (int i = 0; i < _times_to_repeat; i++) {
        _animation->animate(cube);
      }
    }
};

class MoveAnim : public Animation, public WithDuration {
    Movement * _movement;
    const int _num_of_steps;
  public:
    MoveAnim(Movement * movement, int duration, int num_of_steps) :
      WithDuration(duration),
      _movement(movement),
      _num_of_steps(num_of_steps) {}

    void animate(Cube & cube) {
      Position pos;
      cube.allOff();

      _movement->pos(pos);
      cube.on(pos);
      cube.render(duration());
      cube.off(pos);
      for (int i = 0; i < _num_of_steps; i++) {
        _movement->move();
        _movement->pos(pos);
        cube.on(pos);
        cube.render(duration());
        cube.off(pos);
      }
    }
};

class ArestaMov : public Animation, public WithDuration {
    AheadMovement _xMov;
    AheadMovement _yMov;
    AheadMovement _zMov;

    void moveAllAndRender(Cube & cube) {
      cube.off(_xMov.pos());
      _xMov.move();
      cube.on(_xMov.pos());

      cube.off(_yMov.pos());
      _yMov.move();
      cube.on(_yMov.pos());

      cube.off(_zMov.pos());
      _zMov.move();
      cube.on(_zMov.pos());

      cube.render(duration());
    }
  public:
    ArestaMov(int duration) :
      WithDuration(duration),
      _xMov(0, 0, 0, XPlus),
      _yMov(0, 0, 0, YPlus),
      _zMov(0, 0, 0, ZPlus) {}

    void animate(Cube & cube) {
      cube.allOff();
      cube.on(0, 0, 0);
      cube.render(duration());

      for (int i = 0; i < 2; i++) {
        moveAllAndRender(cube);
      }
      

      /** /
      // redirect movements
      _xMov.setDir(0);
      _yMov.setDir(2);
      _zMov.setDir(4);

      for(int i = 0; i < 1; i++)
        moveAllAndRender(cube);
      /**/

      // reset for next round
      _xMov.set(0, 0, 0, XPlus);
      _yMov.set(0, 0, 0, YPlus);
      _zMov.set(0, 0, 0, ZPlus);
    }
};

template<typename Item>
class SnakeIterator {
    Collection<Item> * _collection;
    int _headIndex;
    int _tailIndex;
  public:
    SnakeIterator(Collection<Item> * collection):
      _collection(collection),
      _tailIndex(0),
      _headIndex(collection->nElems() - 1) {}

    void next() {
      _headIndex = (_headIndex + 1) % _collection->nElems();
      _tailIndex = (_tailIndex + 1) % _collection->nElems();
    }

    void head(Item & item) {
      _collection->set(_headIndex, item);
    }

    Item & tail() {
      return _collection->get(_tailIndex);
    }
};

class SnakeAnim : public Animation, public WithDuration {
    const int _size;
    Collection<Position> _positions;
    Movement * _movement;
    const int _cicles;

    bool insideBody(Position & pos) {
      Iterator<Position> * it = _positions.iterator();
      bool result = false;
      while (!result || it->hasNext()) {
        if (it->next() == pos)
          result = true;
      }
      delete it;
    }

  public:
    SnakeAnim(Movement * movement, int duration, int size, int cicles):
      WithDuration(duration),
      _size(size),
      _positions(_size),
      _movement(movement),
      _cicles(cicles)
    {
      _positions.add(_movement->pos());
      for (int i = 0; i < _size - 1; i++) {
        _movement->move();
        _positions.add(_movement->pos());
      }
    }

    void animate(Cube & cube) {
      cube.allOff();
      for (int i = 0; i < _size; i++) {
        cube.on(_positions[i]);
      }

      // saves the position of the snake for the next call
      static SnakeIterator<Position> it(&_positions);

      for (int i = 0; i < _cicles; i++) {
        cube.render(duration());

        _movement->move();
        /** /
        Serial.print("snake head: (");
        Serial.print(_movement->pos().x);
        Serial.print(",");
        Serial.print(_movement->pos().y);
        Serial.print(",");
        Serial.print(_movement->pos().z);
        Serial.println(")");
        /**/
        cube.off(it.tail());
        cube.on(_movement->pos());

        it.next();
        it.head(_movement->pos());
      }
    }
};

class MoveTestAnim : public Animation, public WithDuration {
    AheadMovement _movement;
  public:
    MoveTestAnim(int duration):
      WithDuration(duration),
      _movement(0, 0, 0, XMinus) {}
      
    void animate(Cube & cube) {
      cube.allOff();
      cube.on(1,1,1);
      cube.render(duration());
      
      for (int i = 0; i < 6; i++) {
        _movement.set(1, 1, 1, (Direction)i);
        _movement.move();
        cube.on(_movement.pos());
        cube.render(duration());
        cube.off(_movement.pos());
      }
    }
};

class MoveTestAnim2 : public Animation, public WithDuration {
  AheadMovement _movement;
public:
  MoveTestAnim2(int duration):
    WithDuration(duration),
    _movement(0, 0, 0, XPlus) {}
    
  void animate(Cube & cube) {
    cube.allOff();
    cube.on(_movement.pos());
    cube.render(duration());
    
    for(int i = 0; i < 2; i++) {
      cube.off(_movement.pos());
      _movement.move();
      cube.on(_movement.pos());
      cube.render(duration());
    }
    
    _movement.setDir(YPlus);
    
    for(int i = 0; i < 2; i++) {
      cube.off(_movement.pos());
      _movement.move();
      cube.on(_movement.pos());
      cube.render(duration());
    }
  }
};



class StepsAnim : public Animation, public WithDuration {
  XYMovement _top;
  XYMovement _midle;
  XYMovement _bottom;
public:
  StepsAnim(int duration) :
    WithDuration(duration),
    _top(2,0,0),
    _midle(1,0,1),
    _bottom(0,0,2) {}
  
  void animate(Cube & cube) {
    cube.allOff();
    
    for(int i = 0; i < 8; i++) {
      //cube.on(_top.pos());
      cube.on(_midle.pos());
      //cube.on(_bottom.pos());
      cube.render(duration());
      _top.move();
      _midle.move();
      _bottom.move();
    }
  }
};


