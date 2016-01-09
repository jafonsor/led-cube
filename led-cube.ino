#include "animation.h"

AnimationManager manager;

RandomMovement snakeMovement(9090);
RandomMovement moveAnimMove(6767);
XYMovement planeMove(0,0);

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
  
  manager.addAnim(new SnakeAnim(&snakeMovement, 70, 4, 200));
  //manager.addAnim(new ArestaMov(300));
  //manager.addAnim(blinkAnim);
  //manager.addAnim(new AllOnAnim(250));
  //manager.addAnim(new MoveAnim(&moveAnimMove,100,16));
  //manager.addAnim(new MoveAnim(&planeMove,300, 8));
  //manager.addAnim(new MoveTestAnim2(500));
  //manager.addAnim(new StepsAnim(100,2));
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
