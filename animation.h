#include "collection.h"
#include "cube.h"
#include "movement.h"

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

class RandMoveAnim : public Animation, public WithDuration {
    RandomMovement _movement;
    const int _num_of_steps;
  public:
    RandMoveAnim(int duration, int num_of_steps) :
      WithDuration(duration),
      _movement(999),
      _num_of_steps(num_of_steps) {}

    void animate(Cube & cube) {
      Position pos;
      cube.allOff();
      for (int i = 0; i < _num_of_steps; i++) {
        _movement.getPosition(pos);
        cube.on(pos);
        cube.render(duration());
        _movement.moveRandom();
        cube.off(pos);
      }
    }
};

class ArestaMov : public Animation, public WithDuration {
    Movement _xMov;
    Movement _yMov;
    Movement _zMov;

    void moveAllAndRender(Cube & cube) {
      cube.off(_xMov.getPosition());
      _xMov.moveAhead();
      cube.on(_xMov.getPosition());

      cube.off(_yMov.getPosition());
      _yMov.moveAhead();
      cube.on(_yMov.getPosition());
      
      cube.off(_zMov.getPosition());
      _zMov.moveAhead();
      cube.on(_zMov.getPosition());

      cube.render(duration());
    }
  public:
    ArestaMov(int duration) :
      WithDuration(duration),
      _xMov(0, 0, 0, 1),
      _yMov(0, 0, 0, 3),
      _zMov(0, 0, 0, 5) {}

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
      _xMov.set(0, 0, 0, 1);
      _yMov.set(0, 0, 0, 3);
      _zMov.set(0, 0, 0, 5);
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
  RandomMovement _movement;
  const int _cicles;
   
  bool insideBody(Position & pos) {
    Iterator<Position> * it = _positions.iterator();
    bool result = false;
    while(!result || it->hasNext()) {
      if(it->next() == pos) 
        result = true;
    }
    delete it;
  }
  
public:
  SnakeAnim(int duration, int size, int cicles):
    WithDuration(duration),
    _size(size),
    _positions(_size),
    _movement(9090),
    _cicles(cicles)
  {
    _positions.add(_movement.getPosition());
    for (int i = 0; i < _size-1; i++) {
      _movement.moveRandom();
      _positions.add(_movement.getPosition());
    }
  }

  void animate(Cube & cube) {
    cube.allOff();
    for (int i = 0; i < _size; i++) {
      cube.on(_positions[i]);
    }
    
    static SnakeIterator<Position> it(&_positions);
    Position newHead;
    Position oldHead;
    for (int i = 0; i < _cicles; i++) {
      cube.render(duration());

      // calculate new head outside body
      _movement.getPosition(newHead);
      /**/
      _movement.moveRandom();
      /** /
      do {
        _movement.setPosition(oldHead);
        _movement.moveRandom();
        _movement.getPosition(newHead);
      } while(insideBody(newHead));     
      /**/
      cube.off(it.tail());
      cube.on(newHead);

      it.next();
      it.head(newHead);
    }
  }
};




