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

class MovesAnim : public Animation, public WithDuration {
    Collection<Movable*> * _movables;
    const int _num_of_steps;
  public:
    MovesAnim(Collection<Movable*> * movables, int duration, int num_of_steps) :
      WithDuration(duration),
      _movables(movables),
      _num_of_steps(num_of_steps) {}

    void animate(Cube & cube) {
      cube.allOff();
      
      Iterator<Movable*> * it;
      Movable * movableAux;
      for (int i = 0; i < _num_of_steps; i++) {
        it = _movables->iterator();
        while(it->hasNext()) {
          cube.on(it->next()->pos());
        }
        delete it;
        
        cube.render(duration());
      
        it = _movables->iterator();
        while(it->hasNext()) {
          movableAux = it->next();
          cube.off(movableAux->pos());
          movableAux->move();
        }
        delete it;
      }
    }
};

class ToggleMoveAnim : public Animation, public WithDuration {
  Movable * _movable;
  const int _steps;
public:
  ToggleMoveAnim(Movable * movable, int duration, int steps):
    WithDuration(duration),
    _movable(movable),
    _steps(steps) {}
  
  // Override
  void animate(Cube & cube) {
    cube.allOff();
    
    // hot fix - don't know why first position is bugged
    Position origin(0,0,0);
    cube.toggle(origin);
    _movable->move();
    
    cube.render(duration());
    for(int i = 0; i < _steps-1; i++) {
      cube.toggle(_movable->pos());
      cube.render(duration());
      _movable->move();
      
    }
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
    Movable * _movable;
    const int _cicles;
    SnakeIterator<Position> _it;

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
    SnakeAnim(Movable * movable, int duration, int size, int cicles):
      WithDuration(duration),
      _size(size),
      _positions(_size),
      _movable(movable),
      _cicles(cicles),
      _it(&_positions)
    {
      _positions.add(_movable->pos());
      for (int i = 0; i < _size - 1; i++) {
        _movable->move();
        _positions.add(_movable->pos());
      }
    }

    void animate(Cube & cube) {
      cube.allOff();
      for (int i = 0; i < _size; i++) {
        cube.on(_positions[i]);
      }

      for (int i = 0; i < _cicles; i++) {
        cube.render(duration());

        _movable->move();
        
        cube.off(_it.tail());
        cube.on(_movable->pos());

        _it.next();
        _it.head(_movable->pos());
      }
    }
};

class StepsAnim : public Animation, public WithDuration {
  XYMovement _top;
  XYMovement _midle;
  XYMovement _bottom;
  int _laps;
public:
  StepsAnim(int duration, int laps) :
    WithDuration(duration),
    _top(0,0),
    _midle(1,1),
    _bottom(2,2),
    _laps(laps) {}
  
  void animate(Cube & cube) {
    cube.allOff();
    
    for(int i = 0; i < 8 * _laps; i++) {
      cube.toggle(_top.pos());
      cube.toggle(_midle.pos());
      cube.toggle(_bottom.pos());
      cube.render(duration());
      _top.move();
      _midle.move();
      _bottom.move();
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


