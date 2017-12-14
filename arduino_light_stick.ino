#include "Wire.h"

#include <I2Cdev.h>
#include "MPU9250.h"

#include <Adafruit_NeoPixel.h>

#define PIN 19
#define NUMPIXEL 16

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXEL, PIN, NEO_GRB + NEO_KHZ800);

MPU9250 accelgyro;
I2Cdev   I2C_M;

void getAccel_Data(void);
void getGyro_Data(void);


uint8_t buffer_m[6];

int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

float heading;
float tiltheading;

long first_p, first_r;
long a_p, a_r;

float Axyz[3];
float Gxyz[3];

#define sample_num_mdate  5000

int redPin = 18;
int greenPin = 17;
int bluePin = 16;
int tonePin = 11;
int switchPin = 49;
long timer;
bool button = false;
bool effect = false;

void setup() {
  // put your setup code here, to run once:
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif

  Wire.begin();
  Serial.begin(38400);                        // 통신속도 38400 bps

  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");


  delay(1000);
  Serial.println("     ");

  pixels.begin();
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(tonePin, OUTPUT);
  pinMode(switchPin, INPUT_PULLUP);

  timer = micros();
}

void loop() {
  int red = 0;
  int green = 0;
  int blue = 0;
  if (digitalRead(switchPin) == LOW && button == false) {
    getAccel_Data();
    first_p = 180 * atan(Axyz[1] / sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2])) / M_PI;
    first_r = 180 * atan(Axyz[0] / sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2])) / M_PI;
    red = 255;
    blue = 0;
    green = 0;
    setColor(red, green, blue);
    delay(100);
    red = 0;
    blue = 255;
    green = 0;
    setColor(red, green, blue);
    delay(100);
    red = 0;
    blue = 0;
    green = 255;
    setColor(red, green, blue);
    delay(100);
    setColor(0, 0, 0);
    button = true;
  }
  // put your main code here, to run repeatedly:
  else if (button == true) {
    getAccel_Data();
    getGyro_Data();

    Serial.println("Acceleration(g) of X,Y,Z:");
    Serial.print(Axyz[0]);
    Serial.print(",");
    Serial.print(Axyz[1]);
    Serial.print(",");
    Serial.println(Axyz[2]);

    Serial.println("Gyro(degress/s) of X,Y,Z:");
    Serial.print(Gxyz[0]);
    Serial.print(",");
    Serial.print(Gxyz[1]);
    Serial.print(",");
    Serial.println(Gxyz[2]);
    a_p = 180 * atan(Axyz[1] / sqrt(Axyz[0] * Axyz[0] + Axyz[2] * Axyz[2])) / M_PI;
    a_r = 180 * atan(Axyz[0] / sqrt(Axyz[1] * Axyz[1] + Axyz[2] * Axyz[2])) / M_PI;
    Serial.print(first_p);
    Serial.print(":");
    Serial.println(a_p);
    Serial.print(first_r);
    Serial.print(":");
    Serial.println(a_r);

    if (!effect) {
      if (sqrt(Axyz[1]*Axyz[1] + Axyz[0]*Axyz[0]) > 1.2 && a_p > 40 && a_r < 40 && a_r > -40) {
        red = 255; tone(tonePin, 293.6648, 200); effect = true; //front
      }
      else if (sqrt(Axyz[1]*Axyz[1] + Axyz[0]*Axyz[0]) > 1.2 && a_p > -40 && a_p < 40 && a_r > 40) {
        green = 255; tone(tonePin, 329.6276, 200); effect = true; //Left
      }
      else if (sqrt(Axyz[1]*Axyz[1] + Axyz[0]*Axyz[0]) > 1.2 && a_p < -40 && a_r < 40 && a_r > -40) {
        red = 255; green = 255; blue = 255;//backyard
        tone(tonePin, 349.2282, 200); effect = true;
      }
      else if (sqrt(Axyz[1]*Axyz[1] + Axyz[0]*Axyz[0]) > 1.2 && a_p > -40 && a_p < 40  && a_r < -40) {
        blue = 255;
        tone(tonePin, 261.6256, 200); effect = true; //RIGHT
      }
      else {
        red = 0; green = 0; blue = 0;
      }
    } else if (effect) effect = false;

    for (int i = 0; i < NUMPIXEL; i++) {
      pixels.setPixelColor(i, pixels.Color(red, green, blue));
    }
    setColor(red, green, blue);
    pixels.show();
    //
    Serial.println("power on");
    if (digitalRead(switchPin) == LOW) {
      button = false;
      Serial.println("power off");
      red = 0;
      blue = 0;
      green = 0;
    }
  }
  delay(200);


  while (micros() - timer < 4000); timer = micros();
}

void getAccel_Data(void)
{
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
}
void getGyro_Data(void)
{
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);

  Gxyz[0] = (double) gx * 250 / 32768;
  Gxyz[1] = (double) gy * 250 / 32768;
  Gxyz[2] = (double) gz * 250 / 32768;
}

void setColor(int red, int green, int blue) {
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}

