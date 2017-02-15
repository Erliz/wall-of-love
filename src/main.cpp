#include "Arduino.h"
#include "Servo.h"

#define SERIAL_PORT 9600
#define POS_HIDE 179
#define POS_SHOW 1
#define ROTATION_TIMEOUT 20

int DrivesIdle[3] = {76, 78, 77};

int rotatorsOrder[20] = {10,9,11,8,12,7,13,6,14,5,15,4,16,3,17,2,18,1,19,0};
Servo Rotators[20];
Servo Drives[3];

int posStart = 1;
int posFin = 178;

unsigned long getDelay() {
  return ((unsigned long)ROTATION_TIMEOUT * 1000);
}

void setDriveToIdle(int id) {
  Drives[id].write(DrivesIdle[id]);
}

void setDrive(int id, int pin) {
  Drives[id].attach(pin);
  setDriveToIdle(id);
  Serial.print("Servo drive initialised: "); Serial.println(id);
}

int getPinForId(int id) {
  // return id + 2;
  return rotatorsOrder[id] + 2;
}

void setup() {
  Serial.begin(SERIAL_PORT);
  for(int i = 0; i < 20; i++) {
    Rotators[i].attach(getPinForId(i));
    Rotators[i].write(POS_HIDE);
    Serial.print("Servo rotate initialised: "); Serial.println(i);
    delay(100);
  }
  setDrive(0, 26);
  setDrive(1, 27);
  setDrive(2, 28);
}

void rotate(int count, int to) {
  for(int i = 0; i < count; i++) {
   Rotators[i].write(to);
  }
}

void showHearts(int count) {
  rotate(count, POS_SHOW);
  delay(getDelay());
  rotate(count, POS_HIDE);
  delay(1000);
}

void showHint(int id) {
  Drives[id].write(POS_SHOW);
  delay(getDelay());
  Drives[id].write(POS_HIDE);
  delay(getDelay());
  setDriveToIdle(id);
  delay(1000);
}

void searchDriveIdle() {
    for (int i = 50; i < 120; i++) {
      Drives[0].write(i);
      Serial.println(i);
      delay(1000);
    }
}

void test() {
    for (int i=0; i < 20; i++) {
      Rotators[i].write(POS_SHOW);
    }
    delay(30 * 1000);
    for (int i=0; i < 20; i++) {
      Rotators[i].write(POS_HIDE);
    }
    delay(30 * 1000);
}

void testOneByOne() {
    for (int i=0; i < 20; i++) {
      Rotators[i].write(POS_SHOW);
      Serial.print("Show id "); Serial.print(i); Serial.print(" pin "); Serial.println(getPinForId(i));
      delay(1000);
      Rotators[i].write(POS_HIDE);
      delay(1000);
    }
    delay(1000);
}

void shacke() {
  for (int i=0; i < 20; i++) {
    Rotators[i].write(POS_SHOW);
  }
  delay(5 * 1000);
  for (int i=0; i < 20; i++) {
    Rotators[i].write(POS_HIDE);
  }
  delay(5 * 1000);
}

void start() {
  if (Serial.available()) {
    String serialData = Serial.readStringUntil('\n');
    Serial.print("Data from searial: "); Serial.println(serialData);

    int servsCount = serialData.toInt();

    if (servsCount == 0) {
      shacke();
    } else if (servsCount > 0 and servsCount <= 20) {
      showHearts(servsCount);
    } else if (servsCount > 20) {
      showHearts(20);
    }

    // if (servsCount == 0) {
    //   showHint(0);
    // } else if (servsCount > 20 and servsCount < 50) {
    //   showHint(1);
    // } else if (servsCount >= 50) {
    //   showHint(2);
    // }
    Serial.println("Done");
  }
}

void loop() {
  // testOneByOne();
  // test();
  start();
}
