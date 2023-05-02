#include <Wire.h>           // For I2C communication
#include <Adafruit_VL53L0X.h>
#include <Adafruit_IS31FL3731.h>
#include "animationFlicker.h"      // Flame animation data
#include "animationNormal.h"       // Import the data-flame-normal file

#define LED_DRIVER_ADDR 0x74       // I2C address of Charlieplex matrix
#define SDA_PIN 0
#define SCL_PIN 1

// Instantiate objects
Adafruit_VL53L0X lox = Adafruit_VL53L0X();
Adafruit_IS31FL3731 ledmatrix = Adafruit_IS31FL3731();

uint8_t page = 0;    // Front/back buffer control
char animation = 0;     // We're only using this as an indicator. 0 means animationNormal, 1 means animationFlicker
const uint8_t *ptr  = animationFlicker; // Current pointer into animation data
uint8_t  img[9 * 16]; // Buffer for rendering image

// UTILITY FUNCTIONS -------------------------------------------------------

// Begin I2C transmission and write register address (data then follows)
void writeRegister(uint8_t n) {
  Wire.beginTransmission(LED_DRIVER_ADDR);
  Wire.write(n);
  // Transmission is left open for additional writes
}

// Select one of eight IS31FL3731 pages, or Function Registers
void pageSelect(uint8_t n) {
  writeRegister(0xFD); // Command Register
  Wire.write(n);       // Page number (or 0xB = Function Registers)
  Wire.endTransmission();
}

void runAnimation(const uint8_t *animationType) {
    uint8_t  a, x1, y1, x2, y2, x, y;
  // Display frame rendered on prior pass.  This is done at function start
  // (rather than after rendering) to ensire more uniform animation timing.
  pageSelect(0x0B);    // Function registers
  writeRegister(0x01); // Picture Display reg
  Wire.write(page);    // Page #
  Wire.endTransmission();

  page ^= 1; // Flip front/back buffer index

  // Then render NEXT frame.  Start by getting bounding rect for new frame:
  a = pgm_read_byte(ptr++);     // New frame X1/Y1
  if(a >= 0x90) {               // EOD marker? (valid X1 never exceeds 8)
    ptr = animationType;     // Reset animation data pointer to start
    a   = pgm_read_byte(ptr++); // and take first value
  }
  x1 = a >> 4;                  // X1 = high 4 bits
  y1 = a & 0x0F;                // Y1 = low 4 bits
  a  = pgm_read_byte(ptr++);    // New frame X2/Y2
  x2 = a >> 4;                  // X2 = high 4 bits
  y2 = a & 0x0F;                // Y2 = low 4 bits

  // Read rectangle of data from anim[] into portion of img[] buffer
  for(x=x1; x<=x2; x++) { // Column-major
    for(y=y1; y<=y2; y++) img[(x << 4) + y] = pgm_read_byte(ptr++);
  }

  // Write img[] to matrix (not actually displayed until next pass)
  pageSelect(page);    // Select background buffer
  writeRegister(0x24); // First byte of PWM data
  uint8_t i = 0, byteCounter = 1;
  for(uint8_t x=0; x<9; x++) {
    for(uint8_t y=0; y<16; y++) {
      Wire.write(img[i++]);      // Write each byte to matrix
      if(++byteCounter >= 32) {  // Every 32 bytes...
        Wire.endTransmission();  // end transmission and
        writeRegister(0x24 + i); // start a new one (Wire lib limits)
      }
    }
  }
  Wire.endTransmission();
}


// SETUP FUNCTION - RUNS ONCE AT STARTUP -----------------------------------

void setup() {
  uint8_t i, p, byteCounter;

  // The Arduino Wire library runs I2C at 100 KHz by default.
  // IS31FL3731 can run at 400 KHz.  To ensure fast animation,
  // override the I2C speed settings after init...
  Wire.begin();                            // Initialize I2C
  Wire.setClock(400000);                   // 400 KHz I2C
  pageSelect(0x0B);                        // Access the Function Registers
  writeRegister(0);                        // Starting from first...
  for(i=0; i<13; i++) Wire.write(10 == i); // Clear all except Shutdown
  Wire.endTransmission();
  for(p=0; p<2; p++) {                     // For each page used (0 & 1)...
    pageSelect(p);                         // Access the Frame Registers
    writeRegister(0);                      // Start from 1st LED control reg
    for(i=0; i<18; i++) Wire.write(0xFF);  // Enable all LEDs (18*8=144)
    for(byteCounter = i+1; i<0xB4; i++) {  // For blink & PWM registers...
      Wire.write(0);                       // Clear all
      if(++byteCounter >= 32) {            // Every 32 bytes...
        byteCounter = 1;                   // End I2C transmission and
        Wire.endTransmission();            // start a new one because
        writeRegister(i);                  // Wire buf is only 32 bytes.
      }
    }
    Wire.endTransmission();
  }

  //  TimeofFlight setup
  // Setup serial communication
  Serial.begin(115200);

  // Setup I2C
  Wire.begin(SDA_PIN, SCL_PIN);

  // Start the VL53L0X sensor
  if (!lox.begin()) {
    Serial.println(F("Failed to initialize the VL53L0X sensor"));
    while (1);
  }

  // Initialize the LED matrix
  if (!ledmatrix.begin(LED_DRIVER_ADDR)) {
    Serial.println(F("Failed to initialize the LED matrix"));
    while (1);
  }

  ledmatrix.clear(); // Clear the LED matrix buffer
}

// LOOP FUNCTION - RUNS EVERY FRAME ----------------------------------------
void loop() {

  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false); // Get a ranging measurement

  if (measure.RangeStatus != 4) { // Check if the measurement is valid
    uint16_t distance = measure.RangeMilliMeter;


    // Distance is less than or equal to 30 cm
    // Display animationFlicker
    if (distance <= 300) {
      digitalWrite(LED_BUILTIN, HIGH);   // Tiny red LED on for testing
      
      if (animation == 0) {
        animation = 1;
        ptr = animationFlicker;
      }
      runAnimation(animationFlicker);

      
    // Distance is more than 30 cm
    // Display animationNormal
    } else {
      digitalWrite(LED_BUILTIN, LOW);   // Tiny red LED off for testing
      animation = 0;
      runAnimation(animationNormal);
    }
  }

}