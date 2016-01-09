#include "animation.h"

AnimationManager manager;

RandomAllDirMovement snakeMovement1(9090);
RandomArestaMovement snakeMovement2(9898);
RandomAllDirMovement moveAnimMove(6767);
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
  
  
  manager.addAnim(new StepsAnim(100,2));
  manager.addAnim(new SnakeAnim(&snakeMovement2, 70, 7, 100));
  manager.addAnim(blinkAnim);
  manager.addAnim(new SnakeAnim(&snakeMovement1, 70, 4, 100));
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
