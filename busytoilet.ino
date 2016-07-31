#include "neopixel.h"
#include "SparkFunMAX17043.h"

#define PHOTOSENSOR_PIN A0
#define MOTIONSENSOR_PIN D3
#define PHOTOSENSOR_POWER_PIN A5
#define MOTIONSENSOR_POWER_PIN2 D4
#define LED_PIN D2
#define PIXEL_PIN D6
#define PIXEL_COUNT 64
#define PIXEL_TYPE WS2812B

int photosensorData = 0; // Variable to keep the reading of the lightsensor
int initialPhotosensorData = 0; // Variable to keep the reading of the lightsensor on startup
int motionsensorStatus; // Variable to keep the reading of the motionsensor
bool roomOccupied = false; // Variable to keep the state of the room
int motionTime; // Keep track of when motion change was last detected
bool lightLeftOn = false; // Keep track of light forgotten in 'on' state

//double voltage = 0; // Variable to keep track of LiPo voltage
double soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
bool alert; // Variable to keep track of whether alert has been triggered

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

void setup() {
  pinMode(PHOTOSENSOR_POWER_PIN,OUTPUT); // This pin supplies power to the photosensor
  pinMode(MOTIONSENSOR_POWER_PIN2, OUTPUT); // This pin supplies power to the motionsensor
  digitalWrite(PHOTOSENSOR_POWER_PIN, HIGH);
  digitalWrite(MOTIONSENSOR_POWER_PIN2, HIGH);

  pinMode(MOTIONSENSOR_PIN,INPUT_PULLUP); // motionsensor pulls line LOW when movement detected
  pinMode(LED_PIN,OUTPUT);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  lipo.begin(); // Initialize the MAX17043 LiPo fuel gauge
  // Quick start restarts the MAX17043 in hopes of getting a more accurate
  // guess for the SOC.
  lipo.quickStart();
  // We can set an interrupt to alert when the battery SoC gets too low.
  // We can alert at anywhere between 1% - 32%:
  //lipo.setThreshold(10); // Set alert threshold to 20%.

  Particle.variable("roomOccupied", roomOccupied);
  Particle.variable("lightLeftOn", lightLeftOn);
  Particle.variable("photosensor", photosensorData);
  Particle.variable("motionsensor", motionsensorStatus);
  //Particle.variable("voltage", voltage);
  Particle.variable("soc", soc);
  //Particle.variable("alert", alert);
  Particle.function("reset", performReset);

  motionTime = Time.now();

  // Except the device turned on in light conditions
  // Will compare against this value to see if it's 'dark'
  initialPhotosensorData = analogRead(PHOTOSENSOR_PIN) - 500;
}

void loop() {
  checkLight();
  checkMotion();
  checkBattery();
}

void checkLight() {
  photosensorData = analogRead(PHOTOSENSOR_PIN);
  if (photosensorData >= initialPhotosensorData)
  {
    roomOccupied = true;
    doorClosed();
    // room occupied while no movement detected for 15 minutes ?
    if((Time.now() >= motionTime + 900))
      lightLeftOn = true;
    else
      lightLeftOn = false;
  }
  else
  {
    roomOccupied = false;
    lightLeftOn = false;
    doorOpen();
  }
}

void checkMotion() {
  if(Time.now() >= motionTime + 3) {
    motionsensorStatus = digitalRead(MOTIONSENSOR_PIN);
    if(motionsensorStatus==LOW) {
      motionTime = Time.now();
      digitalWrite(LED_PIN, HIGH);
    };
    if(motionsensorStatus==HIGH)
      digitalWrite(LED_PIN, LOW);
  };
}

void checkBattery() {
    //lipo.getVoltage() returns a voltage value (e.g. 3.93)
  	//voltage = lipo.getVoltage();
  	//lipo.getSOC() returns the estimated state of charge (e.g. 79%)
  	soc = lipo.getSOC();
  	//lipo.getAlert() returns a 0 or 1 (0=alert not triggered)
  	//alert = lipo.getAlert();
}

int performReset(String command) {
  System.reset();
  return 1;
}

void doorOpen() {
    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.setPixelColor(3, 0, 0, 0);
    strip.setPixelColor(4, 0, 0, 0);
    strip.setPixelColor(5, 0, 0, 0);
    strip.setPixelColor(6, 0, 0, 0);
    strip.setPixelColor(7, 0, 0, 0);
    strip.setPixelColor(8, 0, 0, 0);
    strip.setPixelColor(9, 0, 0, 0);
    strip.setPixelColor(10, 0, 0, 0);
    strip.setPixelColor(11, 0, 0, 0);
    strip.setPixelColor(12, 0, 0, 0);
    strip.setPixelColor(13, 0, 0, 0);
    strip.setPixelColor(14, 0, 0, 0);
    strip.setPixelColor(15, 0, 0, 0);
    strip.setPixelColor(16, 0, 0, 0);
    strip.setPixelColor(17, 0, 0, 0);
    strip.setPixelColor(18, 0, 0, 0);
    strip.setPixelColor(19, 0, 0, 0);
    strip.setPixelColor(20, 0, 0, 0);
    strip.setPixelColor(21, 0, 0, 0);
    strip.setPixelColor(22, 27, 167, 46);
    strip.setPixelColor(23, 0, 0, 0);
    strip.setPixelColor(24, 0, 0, 0);
    strip.setPixelColor(25, 0, 0, 0);
    strip.setPixelColor(26, 0, 0, 0);
    strip.setPixelColor(27, 0, 0, 0);
    strip.setPixelColor(28, 0, 0, 0);
    strip.setPixelColor(29, 27, 167, 46);
    strip.setPixelColor(30, 0, 0, 0);
    strip.setPixelColor(31, 0, 0, 0);
    strip.setPixelColor(32, 0, 0, 0);
    strip.setPixelColor(33, 0, 0, 0);
    strip.setPixelColor(34, 0, 0, 0);
    strip.setPixelColor(35, 0, 0, 0);
    strip.setPixelColor(36, 27, 167, 46);
    strip.setPixelColor(37, 0, 0, 0);
    strip.setPixelColor(38, 0, 0, 0);
    strip.setPixelColor(39, 0, 0, 0);
    strip.setPixelColor(40, 0, 0, 0);
    strip.setPixelColor(41, 27, 167, 46);
    strip.setPixelColor(42, 0, 0, 0);
    strip.setPixelColor(43, 27, 167, 46);
    strip.setPixelColor(44, 0, 0, 0);
    strip.setPixelColor(45, 0, 0, 0);
    strip.setPixelColor(46, 0, 0, 0);
    strip.setPixelColor(47, 0, 0, 0);
    strip.setPixelColor(48, 0, 0, 0);
    strip.setPixelColor(49, 0, 0, 0);
    strip.setPixelColor(50, 27, 167, 46);
    strip.setPixelColor(51, 0, 0, 0);
    strip.setPixelColor(52, 0, 0, 0);
    strip.setPixelColor(53, 0, 0, 0);
    strip.setPixelColor(54, 0, 0, 0);
    strip.setPixelColor(55, 0, 0, 0);
    strip.setPixelColor(56, 0, 0, 0);
    strip.setPixelColor(57, 0, 0, 0);
    strip.setPixelColor(58, 0, 0, 0);
    strip.setPixelColor(59, 0, 0, 0);
    strip.setPixelColor(60, 0, 0, 0);
    strip.setPixelColor(61, 0, 0, 0);
    strip.setPixelColor(62, 0, 0, 0);
    strip.setPixelColor(63, 0, 0, 0);
    strip.setBrightness(30);
    strip.show();
}

void doorClosed() {
    strip.setPixelColor(0, 0, 0, 0);
    strip.setPixelColor(1, 0, 0, 0);
    strip.setPixelColor(2, 0, 0, 0);
    strip.setPixelColor(3, 0, 0, 0);
    strip.setPixelColor(4, 0, 0, 0);
    strip.setPixelColor(5, 0, 0, 0);
    strip.setPixelColor(6, 0, 0, 0);
    strip.setPixelColor(7, 0, 0, 0);
    strip.setPixelColor(8, 0, 0, 0);
    strip.setPixelColor(9, 185, 30, 0);
    strip.setPixelColor(10, 0, 0, 0);
    strip.setPixelColor(11, 0, 0, 0);
    strip.setPixelColor(12, 0, 0, 0);
    strip.setPixelColor(13, 0, 0, 0);
    strip.setPixelColor(14, 185, 30, 0);
    strip.setPixelColor(15, 0, 0, 0);
    strip.setPixelColor(16, 0, 0, 0);
    strip.setPixelColor(17, 0, 0, 0);
    strip.setPixelColor(18, 185, 30, 0);
    strip.setPixelColor(19, 0, 0, 0);
    strip.setPixelColor(20, 0, 0, 0);
    strip.setPixelColor(21, 185, 30, 0);
    strip.setPixelColor(22, 0, 0, 0);
    strip.setPixelColor(23, 0, 0, 0);
    strip.setPixelColor(24, 0, 0, 0);
    strip.setPixelColor(25, 0, 0, 0);
    strip.setPixelColor(26, 0, 0, 0);
    strip.setPixelColor(27, 185, 30, 0);
    strip.setPixelColor(28, 185, 30, 0);
    strip.setPixelColor(29, 0, 0, 0);
    strip.setPixelColor(30, 0, 0, 0);
    strip.setPixelColor(31, 0, 0, 0);
    strip.setPixelColor(32, 0, 0, 0);
    strip.setPixelColor(33, 0, 0, 0);
    strip.setPixelColor(34, 0, 0, 0);
    strip.setPixelColor(35, 185, 30, 0);
    strip.setPixelColor(36, 185, 30, 0);
    strip.setPixelColor(37, 0, 0, 0);
    strip.setPixelColor(38, 0, 0, 0);
    strip.setPixelColor(39, 0, 0, 0);
    strip.setPixelColor(40, 0, 0, 0);
    strip.setPixelColor(41, 0, 0, 0);
    strip.setPixelColor(42, 185, 30, 0);
    strip.setPixelColor(43, 0, 0, 0);
    strip.setPixelColor(44, 0, 0, 0);
    strip.setPixelColor(45, 185, 30, 0);
    strip.setPixelColor(46, 0, 0, 0);
    strip.setPixelColor(47, 0, 0, 0);
    strip.setPixelColor(48, 0, 0, 0);
    strip.setPixelColor(49, 185, 30, 0);
    strip.setPixelColor(50, 0, 0, 0);
    strip.setPixelColor(51, 0, 0, 0);
    strip.setPixelColor(52, 0, 0, 0);
    strip.setPixelColor(53, 0, 0, 0);
    strip.setPixelColor(54, 185, 30, 0);
    strip.setPixelColor(55, 0, 0, 0);
    strip.setPixelColor(56, 0, 0, 0);
    strip.setPixelColor(57, 0, 0, 0);
    strip.setPixelColor(58, 0, 0, 0);
    strip.setPixelColor(59, 0, 0, 0);
    strip.setPixelColor(60, 0, 0, 0);
    strip.setPixelColor(61, 0, 0, 0);
    strip.setPixelColor(62, 0, 0, 0);
    strip.setPixelColor(63, 0, 0, 0);
    strip.setBrightness(30);
    strip.show();
}
