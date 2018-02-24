const int leftmotorPin1 = 3;
const int leftmotorPin2 = 5;
const int rightmotorPin1 = 6;
const int rightmotorPin2 = 9;
const int trigfront=2;
const int echofront=4;
const int trigleft=7;
const int echoleft=8;
const int trigright=10;
const int echoright=11;
char path[100]=" ";
int pathLength=0;
boolean flag=false;
void stop(){                                              //S T O P FUNCTION
  digitalWrite(leftmotorPin1, LOW);
  digitalWrite(leftmotorPin2, LOW);
  digitalWrite(rightmotorPin1, LOW);
  digitalWrite(rightmotorPin2, LOW);
}

void right(){                                             //R I G H T FUNCTION
  digitalWrite(leftmotorPin1, HIGH);
  digitalWrite(leftmotorPin2, LOW);
  digitalWrite(rightmotorPin1, LOW);
  digitalWrite(rightmotorPin2, HIGH);
  delay(490);
}
void left(){                                              //L E F T FUNCTION
  digitalWrite(leftmotorPin1, LOW);
  digitalWrite(leftmotorPin2, HIGH);
  digitalWrite(rightmotorPin1, HIGH);
  digitalWrite(rightmotorPin2, LOW);
  delay(490);
}

void UTurn(){                                             //U - T U R N FUNCTION
  digitalWrite(leftmotorPin1, HIGH);
  digitalWrite(leftmotorPin2, LOW);
  digitalWrite(rightmotorPin1, LOW);
  digitalWrite(rightmotorPin2, HIGH);
  delay(970);
}
void forward(){                                           //F O R W A R D FUNCTION
  analogWrite(leftmotorPin1, 0);
  analogWrite(leftmotorPin2, 235);
  digitalWrite(rightmotorPin1, LOW);
  digitalWrite(rightmotorPin2, HIGH);
}

void pushForward(){                                       //P U S H F O R W A R D
  forward();
  delay(1200);
  stop();
}

void adjustRightGoLeft(){
  analogWrite(leftmotorPin1, 0);
  analogWrite(leftmotorPin2, 255);
  analogWrite(rightmotorPin1, 0);
  analogWrite(rightmotorPin2, 170);
  delay(32);
  forward();
}

void adjustLeftGoRight(){
  analogWrite(leftmotorPin1, 0);
  analogWrite(leftmotorPin2, 170);
  analogWrite(rightmotorPin1, 0);
  analogWrite(rightmotorPin2, 255);
  delay(32);
  forward();
}
void adjustLeftGoRightFast(){
  analogWrite(leftmotorPin1, 0);
  analogWrite(leftmotorPin2, 120);
  analogWrite(rightmotorPin1, 0);
  analogWrite(rightmotorPin2, 150);
  delay(25);
  forward();
}

int sensorValue(int trig,int echo){                                //Sensors
  digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  int duration=pulseIn(echo,HIGH,60000);
  int temp=duration/29/2;
  return temp;
}
void printpath(){
  int i;
  for(i=0;i<=pathLength;i++){
    Serial.print(path[i]);
  }
  Serial.println(pathLength);
  delay(2000);
}

boolean checkLeft(){                                            //CHECK LEFT
  int t1,t2,t3;
  t1=sensorValue(trigleft,echoleft);
  delay(20);
  t2=sensorValue(trigleft,echoleft);
  delay(20);
  t3=sensorValue(trigleft,echoleft);
  return (t1>75 && t2>75 && t3>75);
}

void solveMaze(){                                                 //MAZE SOLVE
  boolean status=false;
  while(!status){
    int frontval,leftval,rightval,duration,count=0;
  //front sensor
  frontval=sensorValue(trigfront,echofront);

  //left sensor
  leftval=sensorValue(trigleft,echoleft);

  //right sensor
  rightval=sensorValue(trigright,echoright);

  Serial.print("Distance front :  ");
  Serial.print(frontval);
  Serial.print("  Distance left :  ");
  Serial.print(leftval);
  Serial.print("  Distance right :  ");
  Serial.println(rightval);
  
 if(leftval>75 ){
    if(checkLeft()){
    forward();
    delay(500);
    left();
    pushForward();
    takeEntry('L');
   // printpath();
    }
  }
  else if(frontval>45){
    forward();
    if(rightval>55 && !flag){
      takeEntry('S');
     // printpath();
      flag=true;
    }
    if(rightval<25)
    flag=false;
  }
  else if(rightval>55 && frontval<28){
    forward();
    delay(330);
    right();
    pushForward();
    takeEntry('R');
   // printpath();
 }
 else if(frontval>0 && frontval<15 && rightval<45) {
    UTurn();
    stop();
    delay(650);
    takeEntry('B');
    printpath();
 }
  if(frontval>8){
    if(leftval<21)
      adjustLeftGoRight();
    if(rightval<21)
      adjustRightGoLeft();
  }

  if(frontval>100 && leftval>100 && rightval>100)
    status=true;
  }
}

void takeEntry(char c){                                           //take entry
  path[pathLength]=c;
  pathLength++;
  findShortestPath();
}

void findShortestPath(){                                              //find shortest path
  if(pathLength < 3 || path[pathLength-2] != 'B')
    return;

  int totalAngle = 0;
  int i;
  for(i=1;i<=3;i++)
  {
    switch(path[pathLength-i])
    {
      case 'R':
        totalAngle += 90;
  break;
      case 'L':
  totalAngle += 270;
  break;
      case 'B':
  totalAngle += 180;
  break;
    }
  }

  totalAngle = totalAngle % 360;

  switch(totalAngle)
  {
    case 0:
  path[pathLength - 3] = 'S';
  break;
    case 90:
  path[pathLength - 3] = 'R';
  break;
    case 180:
  path[pathLength - 3] = 'B';
  break;
    case 270:
  path[pathLength - 3] = 'L';
  break;
  }

  pathLength -= 2;
  
}
void goSmartly(){                                           //go smartly
  int index=0;
   boolean status=false;
  while(!status){
    int frontval,leftval,rightval;
  frontval=sensorValue(trigfront,echofront);

  leftval=sensorValue(trigleft,echoleft);

  rightval=sensorValue(trigright,echoright);

  Serial.print("Distance front :  ");
  Serial.print(frontval);
  Serial.print("  Distance left :  ");
  Serial.print(leftval);
  Serial.print("  Distance right :  ");
  Serial.println(rightval);
  
 if(leftval>25 ){
   if(checkLeft()){
     if(path[index]=='L'){
     forward();
     delay(500);
     left();
     pushForward();
     forward();
     index++;
     }
   }
   else if(path[index]=='S'){
     forward();
     index++;
   }
 }
 if(rightval>25){
    if(path[index]=='R'){
    forward();
    if(frontval>40)
    delay(800);
    else
    delay(230);
    right();
    pushForward();
    forward();
    index++;
    }
 }
 else forward();

 if(frontval>4){
    if(leftval<=4){
      adjustLeftGoRightFast();
    }
    else{
    if(leftval<=17)
      adjustLeftGoRight();
    if(rightval<=17)
      adjustRightGoLeft();
    }
  }

  if(frontval>100 && leftval>100 && rightval>100)
    status=true;
  }
}


void setup() {
  pinMode(leftmotorPin1, OUTPUT);
  pinMode(leftmotorPin2, OUTPUT);
  pinMode(rightmotorPin1, OUTPUT);
  pinMode(rightmotorPin2, OUTPUT);
  pinMode(trigfront,OUTPUT);
  pinMode(echofront,INPUT);
  pinMode(trigleft,OUTPUT);
  pinMode(echoleft,INPUT);
  pinMode(trigright,OUTPUT);
  pinMode(echoright,INPUT);
  Serial.begin(9600);
  delay(5000);
}

void loop() {

  solveMaze();
  stop();
  findShortestPath();
  delay(10000);
  goSmartly();
  stop();
  delay(20000);
}
