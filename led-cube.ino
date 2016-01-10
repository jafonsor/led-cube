#include "animation.h"

AnimationManager manager;

RandomAllDirMovement snakeMovement1(9090);
RandomArestaMovement snakeMovement2(9898);
RandomAllDirMovement moveAnimMove(6767);
XYMovement planeMove(0,0);

RandomAllDirMovement randMove1(2323);
RandomAllDirMovement randMove2(2424);
RandomAllDirMovement randMove3(9527);
RandomAllDirMovement randMove4(7);

void setup() {
  /**/
  Serial.begin(9600);
  Serial.println("hellow");
  /**/
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
  
  AnimSeq * blinkSeq = new AnimSeq(2);
  blinkSeq->addAnim(new AllOnAnim(200));
  blinkSeq->addAnim(new AllOffAnim(200));
  
  RepeatAnim * blinkAnim = new RepeatAnim(3,blinkSeq);
  
  

  Collection<Movable*> * movesAnimMovables = new Collection<Movable*>(4);
  randMove1.pos(1,1,1);
  randMove2.pos(1,1,1);
  randMove3.pos(1,1,1);
  randMove4.pos(1,1,1);
  movesAnimMovables->add(&randMove1);
  movesAnimMovables->add(&randMove2);
  movesAnimMovables->add(&randMove3);
  movesAnimMovables->add(&randMove4);
  
  
  //manager.addAnim(new StepsAnim(100,2));
  //manager.addAnim(new SnakeAnim(&snakeMovement2, 70, 7, 100));
  //manager.addAnim(blinkAnim);
  //manager.addAnim(new SnakeAnim(&snakeMovement1, 70, 4, 100));
  //manager.addAnim(blinkAnim);
  manager.addAnim(new MovesAnim(movesAnimMovables, 200, 10));
}

/** /
void loop() {
  // do nothing
}
/**/
void loop() {
  while(true) {
    manager.runAnimations();
  }
}
/**/
