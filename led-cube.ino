#include "frame.h"

// duration - time in milliseconds that the frame lasts
void renderFrame(bool frame[3][9], int duration) {
  int nLedsOn = 0;
  for(int c = 0; c < 3; c++)
    for(int l = 0; l < 9; l++)
      if(frame[c][l])
        nLedsOn++;
  if(nLedsOn == 0) {
    //Serial.println("n leds == 0");
    delay(duration);
  } else {
    //Serial.println("n leds > 0");
    long subFrameTime = 10000l; // 100fps in microseconds
    long betweenLedDelay = 10000l / (long)nLedsOn;
    int numberOfSubFrames = duration/10;
    
    for(int i = 0; i < numberOfSubFrames; i++) {
      for(int c = 0; c < 3; c++) {
        digitalWrite(11+c, HIGH);
        for(int l = 0; l < 9; l++) {
          if(frame[c][l]) {
            digitalWrite(2+l, HIGH);
            delayMicroseconds(betweenLedDelay);
            digitalWrite(2+l, LOW);
          }
        }
        digitalWrite(11+c, LOW);
      }
    }
    
  }
}

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
}

const long timePerFrame = 1000000l/100l;

/** /
void loop() {
  // do nothing
}
/**/


Frame frame = {ALL_ON,100};
Animation* animations[] = { new MoveRandom(99999), new AllOnAnim(), new AllOffAnim() };
unsigned int currentAnim = 1;
void loop() {
  
  //animations[1]->animate();
  animations[0]->animate();
  
  /** 
  animations[currentAnim]->animate();
  currentAnim++;
  if(currentAnim > 1)
    currentAnim = 0;
  /**/
}
/**/
