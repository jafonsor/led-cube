#include "animation.h"


AnimationManager animManager;

void setup() {
  pinMode( 2,OUTPUT);
  pinMode( 3,OUTPUT);
  pinMode( 4,OUTPUT);
  pinMode( 5,OUTPUT);
  pinMode( 6,OUTPUT);
  pinMode( 7,OUTPUT);
  pinMode( 8,OUTPUT);
  pinMode( 9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  for(int i = 2; i <= 13; i++) {
    digitalWrite(i,LOW);
  }
  Serial.begin(9600);
  Serial.println("hello");
  
  Collection<Animation*> * blinks =  new Collection<Animation*>(2);
  blinks->add(new AllOnAnim(500));
  blinks->add(new AllOffAnim(250));
  
  Animation * blinkAnim = new RepeatAnim(5, new AnimSeq(blinks));
  
  animManager.addAnim(blinkAnim);
  animManager.addAnim(new RepeatAnim(30, new RandomMoveAnim(200)));
}

/** /
void loop() {
  // do nothing
}
/**/
void loop() {
  animManager.runAnimations();
}
/**/
