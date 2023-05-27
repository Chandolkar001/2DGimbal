#include <Wire.h>
#include <MPU6050.h>
#include <Servo.h>

MPU6050 mpu;
Servo s1;
Servo s2;
int psp, psr;
float pitch, roll, yaw;

int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;

void setup() {
  Wire.begin();
  mpu.initialize();
  Serial.begin(115200);
  s1.attach(8);
  s2.attach(4);
}

void loop() {
  float roll_sum = 0;
  float pitch_sum = 0;
  int roll_count = 0;
  int pitch_count = 0;
  unsigned long start_time = millis();
  while ((millis() - start_time) < 100) {
    readVal();
    roll_sum += roll;
    roll_count++;
    pitch_sum += pitch;
    pitch_count++;
    delay(1);
  }
  float avg_roll = roll_sum / roll_count;
  float avg_pitch = pitch_sum / pitch_count;
  int sp = map(avg_pitch, -90, 90, 0, 180);
  int sr = map(avg_roll, -90, 90, 180, 0);
  s1.write(sr);
  s2.write(sp);
  delay(20);
}

void readVal(){
  mpu.getMotion6(&accelX, &accelY, &accelZ, &gyroX, &gyroY, &gyroZ);
  roll = atan2(accelY, accelZ) * 180 / PI;
  pitch = atan2(-accelX, sqrt(accelY * accelY + accelZ * accelZ)) * 180 / PI;
  yaw = atan2(gyroY, gyroX) * 180 / PI;
}