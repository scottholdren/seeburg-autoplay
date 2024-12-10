int sigQ = 2;
int sigAA = 3;
int sigBB = 4;
int dataA = 5;
int dataB = 6;
int dataC = 7;
int dataD = 8;
int trig = 9;
int mode = 1;
int shuffle = 10;
int norepeat = 11;
int aside = 12;
int bside = 13;
int delayPin = A0;
int delayValue= 0;
int seedPin = A5;
int used[320];

#define ON LOW      //Active low
#define OFF HIGH

// random autoplay for Seeburg STD-3 and probably others
// Modes:
//   On/Off - toggle power to arduino
//   Allow repeats - toggle
//   A/B/Both - 3 position, or two toggles.
//   Pause - pause between songs, potentiometer?  or several presets, like 0-5 minutes


void setup() {
  // put your setup code here, to run once:
  digitalWrite(sigQ, OFF);
  digitalWrite(sigAA, OFF);
  digitalWrite(sigBB, OFF);
  digitalWrite(dataA, OFF);
  digitalWrite(dataB, OFF);
  digitalWrite(dataC, OFF);
  digitalWrite(dataD, OFF);
  digitalWrite(trig, OFF);
  delay(50);
  pinMode(sigQ, OUTPUT);
  pinMode(sigAA, OUTPUT);
  pinMode(sigBB, OUTPUT);
  pinMode(dataA, OUTPUT);
  pinMode(dataB, OUTPUT);
  pinMode(dataC, OUTPUT);
  pinMode(dataD, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(shuffle, INPUT);
  pinMode(norepeat, INPUT);
  pinMode(aside, INPUT);
  pinMode(bside, INPUT);
  pinMode(bside, INPUT);
  randomSeed(analogRead(delayPin));
  delay(2000);
  Serial.begin(38400);
  Serial.print("initializing...\n");
}

void loop() {
  enterSelection(112);
  delay(15000); 
  if (false) {
    delayValue = readDelay();
    int shuffleVal = digitalRead(shuffle);
    int norepeatVal = digitalRead(norepeat);
    int asideVal = digitalRead(aside);
    int bsideVal = digitalRead(bside);
//    Serial.print("delay: ");
//    Serial.print(delayValue);
//    Serial.print("\n");
//    Serial.print("shuffle: ");
//    Serial.print(shuffleVal);
//    Serial.print("\n");
//    Serial.print("norepeat: ");
//    Serial.print(norepeatVal);
//    Serial.print("\n");
//    Serial.print("aside: ");
//    Serial.print(asideVal);
//    Serial.print("\n");
//    Serial.print("bside: ");
//    Serial.print(bsideVal);
//    Serial.print("\n");
    switch (mode) {
      case 0:
        //playRandom();
        delay(1000);
        break;
      case 1:
        playRandomNoRepeats();
        delay(10000);
        break;
    }
  }
}

int readDelay() {
  int tmpValue = analogRead(delayPin);
  if (tmpValue > 1000) {
    delayValue = 0;
  } else if (tmpValue > 800) {
    delayValue = 1;
  } else if (tmpValue > 600) {
    delayValue = 2;
  } else if (tmpValue > 400) {
    delayValue = 3;
  } else if (tmpValue > 200) {
    delayValue = 4;
  } else {
    delayValue = 5;
  }
  return delayValue;
}

void playRandomNoRepeats() {
  Serial.print("Playing random no repeats...");
  Serial.print("\n");
  int selection = getRandomSelection();
  int cnt = 0;
  while (used[selection] == true && ++cnt < 10) {
    selection = getRandomSelection();
  }
  int y = 0;
  for(int x=100; x<280; ++x) {
    if (used[x]==true) {
      ++y;
    }
  }
  if (y>=160) {
    memset(used, 0, sizeof(used));
  }
  used[selection] = true;
  Serial.print("Selection: ");
  Serial.print(selection);
  Serial.print("\n\n");
  enterSelection(selection);
}

void playRandom() {
  Serial.print("Playing random...\n");
  int selection = getRandomSelection();
  Serial.print("Selection: ");
  Serial.print(selection);
  Serial.print("\n\n");
  enterSelection(selection);
}

int getRandomSelection() {
  int side = random(0,2);
  int selection;
  if (side>0) {
    selection = random(100,180);
  } else {
    selection = random(200,280);
  }
  return selection;
}

void enterSelection(int x) {
  int firstDigit = x / 100;
  int secondDigit = x / 10 % 10;
  int thirdDigit = x % 10;
  buttonPress(firstDigit);
  buttonPress(secondDigit);
  buttonPress(thirdDigit);
}


void buttonPress(int x) {
  int sig = 0;
  int data1 = 0;
  int data2 = 0;
  switch (x) {
    case 0:
      sig = sigBB;
      data1 = dataD;
      break;
    case 1:
      sig = sigQ;
      data1 = dataC;
      break;
    case 2:
      sig = sigQ;
      data1 = dataB;
      break;          
    case 3:
      sig = sigBB;
      data1 = dataA;
      break;
    case 4:
      sig = sigBB;
      data1 = dataA;
      data2 = dataD;
      break;
    case 5:
      sig = sigBB;
      data1 = dataA;
      data2 = dataB;
      break;                  
    case 6:
      sig = sigBB;
      data1 = dataA;
      data2 = dataC;
      break;
    case 7:
      sig = sigBB;
      data1 = dataC;
      data2 = dataD;
      break;
    case 8:
      sig = sigAA;
      data1 = dataB;
      data2 = dataC;
      break;
    case 9:
      sig = sigAA;
      data1 = dataB;
      data2 = dataD;
      break;
  }
  if (sig>0) {
    digitalWrite(sig, ON);
  }
  if (data1>0) {
    digitalWrite(data1, ON);
  }
  if (data2>0) {
    digitalWrite(data2, ON);
  }
  Serial.print(sig);
  Serial.print(" ");
  Serial.print(data1);
  Serial.print(" ");
  Serial.print(data2);
  Serial.print("\n");

  delay(200);
  digitalWrite(trig, ON);
  delay(200);

  digitalWrite(trig, OFF);
  digitalWrite(sig, OFF);
  digitalWrite(data1, OFF);
  digitalWrite(data2, OFF);
  delay(500);              
}

