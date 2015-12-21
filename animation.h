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
    while(it->hasNext()) {
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
  inline int duration() { return _duration; }
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
    for(int i = 0; i < _times_to_repeat; i++) {
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
    int x, y, z;
    cube.allOff();
    for(int i = 0; i < _num_of_steps; i++) {
      _movement.getPosition(x,y,z);
      cube.on(x,y,z);
      cube.render(duration());
      _movement.move();
      cube.off(x,y,z);
    }
  }
};
  
