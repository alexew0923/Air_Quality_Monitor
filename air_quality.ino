#include <Arduino.h>
#include <SensirionI2cSps30.h>
#include <Wire.h>

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

#define BUTTON_PIN 9
#define RED_LIGHT_PIN 21

#define SERIAL_INPUT_PIN 7
#define LATCH_PIN 8
#define CLOCK_PIN 10

#define FIRST_DIGIT 2
#define SECOND_DIGIT 3
#define THIRD_DIGIT 4
#define FOURTH_DIGIT 5

int mode; //1-4 mc1-10p0, 5-9 nc0-10p5-0

SensirionI2cSps30 sensor;

static char errorMessage[64];
static int16_t error;

void setup() {
  Serial.begin(115200);
  Wire.begin();
  sensor.begin(Wire, SPS30_I2C_ADDR_69);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(RED_LIGHT_PIN, OUTPUT);
  digitalWrite(RED_LIGHT_PIN, LOW);

  sensor.stopMeasurement();
  int8_t serialNumber[32] = {0};
  int8_t productType[8] = {0};
  sensor.readSerialNumber(serialNumber, 32);
  Serial.print("serialNumber: ");
  Serial.print((const char*)serialNumber);
  Serial.println();
  sensor.readProductType(productType, 8);
  Serial.print("productType: ");
  Serial.print((const char*)productType);
  Serial.println();
  sensor.startMeasurement(SPS30_OUTPUT_FORMAT_OUTPUT_FORMAT_UINT16);
  delay(100);
}

void loop() {
  uint16_t dataReadyFlag = 0;
  uint16_t values[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  delay(1000);
  error = sensor.readDataReadyFlag(dataReadyFlag);
  if (error != NO_ERROR) {
      Serial.print("Error trying to execute readDataReadyFlag(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return;
  }
  Serial.print("dataReadyFlag: ");
  Serial.print(dataReadyFlag);
  Serial.println();
  error = sensor.readMeasurementValuesUint16(values[0], values[1], values[2], values[3], values[4], values[5], values[6], values[7], values[8], values[9]);
  if (error != NO_ERROR) {
      Serial.print("Error trying to execute readMeasurementValuesUint16(): ");
      errorToString(error, errorMessage, sizeof errorMessage);
      Serial.println(errorMessage);
      return;
  }
  Serial.print("mc1p0: ");
  Serial.print(values[0]);
  Serial.print("\t");
  Serial.print("mc2p5: ");
  Serial.print(values[1]);
  Serial.print("\t");
  Serial.print("mc4p0: ");
  Serial.print(values[2]);
  Serial.print("\t");
  Serial.print("mc10p0: ");
  Serial.print(values[3]);
  Serial.print("\t");
  Serial.print("nc0p5: ");
  Serial.print(values[4]);
  Serial.print("\t");
  Serial.print("nc1p0: ");
  Serial.print(values[5]);
  Serial.print("\t");
  Serial.print("nc2p5: ");
  Serial.print(values[6]);
  Serial.print("\t");
  Serial.print("nc4p0: ");
  Serial.print(values[7]);
  Serial.print("\t");
  Serial.print("nc10p0: ");
  Serial.print(values[8]);
  Serial.print("\t");
  Serial.print("typicalParticleSize: ");
  Serial.print(values[9]);
  Serial.println();

  if (digitalRead(BUTTON_PIN) == LOW) {
    if (mode < 9) {
      mode ++;
    } else {
      mode = 0;
    }
  }

  display(values[mode]);
}

void display(int number) {
  
}
