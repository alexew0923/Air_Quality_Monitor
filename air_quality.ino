#include <Arduino.h>
#include <SensirionI2cSps30.h>
#include <Wire.h>
//#include "WiFi.h"
#include <HTTPClient.h>

// macro definitions
// make sure that we use the proper definition of NO_ERROR
#ifdef NO_ERROR
#undef NO_ERROR
#endif
#define NO_ERROR 0

#define BUTTON_PIN 9
#define RED_LED_PIN 21

#define SERIAL_INPUT_PIN 10
#define LATCH_PIN 8
#define CLOCK_PIN 20

#define FIRST_DIGIT 2
#define SECOND_DIGIT 3
#define THIRD_DIGIT 4
#define FOURTH_DIGIT 5

int seven_segment[4] = {2, 3, 4, 5};
byte digits[5];

int mode = 1; //0-3 mc1-10p0, 4-8 nc0-10p5-0
int buttonPressedTime;
bool buttonPressed = false;

SensirionI2cSps30 sensor;
int sensorDelay;
uint16_t values[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static char errorMessage[64];
static int16_t error;

const char* ssid = "iPhone";
const char* password = "012345678";
String google_script_ID = "AKfycbyw4pTeeHpArzfgDZk8-san1PKvezJoK5gPBRp5UY2Ma01jXIgQMy90XLtZwLh0X07hsA";
int sheetDelay;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    sensor.begin(Wire, SPS30_I2C_ADDR_69);

    pinMode(BUTTON_PIN, INPUT);
    pinMode(RED_LED_PIN, OUTPUT);
    digitalWrite(RED_LED_PIN, LOW);

    pinMode(SERIAL_INPUT_PIN, OUTPUT);
    pinMode(LATCH_PIN, OUTPUT);
    digitalWrite(LATCH_PIN, LOW);
    pinMode(CLOCK_PIN, OUTPUT);


    pinMode(FIRST_DIGIT, OUTPUT);
    digitalWrite(FIRST_DIGIT, LOW);
    pinMode(SECOND_DIGIT, OUTPUT);
    digitalWrite(SECOND_DIGIT, LOW);
    pinMode(THIRD_DIGIT, OUTPUT);
    digitalWrite(THIRD_DIGIT, LOW);
    pinMode(FOURTH_DIGIT, OUTPUT);
    digitalWrite(FOURTH_DIGIT, LOW);

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

    /*WiFi.begin(ssid, password);
    for (int i = 0; i < 30; i++) {
        if (WiFi.status() == WL_CONNECTED) {
            for (int j = 0; j < 3; j++) {
                updateShiftRegister(0b10000000);
                delay(500);
                updateShiftRegister(0b00000000);
                delay(500);
            }
            break;
        }
        delay(1000);
    }
    if (WiFi.status() != WL_CONNECTED) {
        for (int j = 0; j < 3; j++) {
            digitalWrite(RED_LED_PIN, HIGH);
            delay(500);
            digitalWrite(RED_LED_PIN, LOW);
            delay(500);
        }
    }
    Serial.println(WiFi.status());*/
}

void loop() {
    if (millis() - sensorDelay > 1000) {
        sensorDelay = millis();
        uint16_t dataReadyFlag = 0;
        uint16_t mc1p0 = 0;
        uint16_t mc2p5 = 0;
        uint16_t mc4p0 = 0;
        uint16_t mc10p0 = 0;
        uint16_t nc0p5 = 0;
        uint16_t nc1p0 = 0;
        uint16_t nc2p5 = 0;
        uint16_t nc4p0 = 0;
        uint16_t nc10p0 = 0;
        uint16_t typicalParticleSize = 0;
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
        error = sensor.readMeasurementValuesUint16(mc1p0, mc2p5, mc4p0, mc10p0,
                                                    nc0p5, nc1p0, nc2p5, nc4p0,
                                                    nc10p0, typicalParticleSize);
        if (error != NO_ERROR) {
            Serial.print("Error trying to execute readMeasurementValuesUint16(): ");
            errorToString(error, errorMessage, sizeof errorMessage);
            Serial.println(errorMessage);
            return;
        }

        /*if (WiFi.status() == WL_CONNECTED && millis() - sheetDelay > 5000) {
            sheetDelay = millis();
            Serial.println(millis());
            static bool flag = false;
            String urlFinal = "https://script.google.com/macros/s/" + google_script_ID + "/exec?" + "mc1p0=" + mc1p0 + "&mc2p5=" + mc2p5 + "&mc4p0=" + mc4p0 + "&mc10p0=" + mc10p0 + "&nc0p5=" + nc0p5 + "&nc1p0=" + nc1p0 + "&nc2p5=" + nc2p5 + "&nc4p0=" + nc4p0 + "&nc10p0=" + nc10p0 + "&size=" + typicalParticleSize;
            HTTPClient http;
            http.begin(urlFinal.c_str());
            http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
            int httpCode = http.GET();
            Serial.println("HTTP Status Code: " + httpCode);

            String payload;
            if (httpCode > 0) {
                payload = http.getString();
                Serial.println("Payload: " + payload);
            }

            http.end();
            Serial.println(millis());
        }*/

        values[0] = mc1p0;
        values[1] = mc2p5;
        values[2] = mc4p0;
        values[3] = mc10p0;
        values[4] = nc0p5;
        values[5] = nc1p0;
        values[6] = nc2p5;
        values[7] = nc4p0;
        values[8] = nc10p0;
        values[9] = typicalParticleSize;

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
    }
    
    if (digitalRead(BUTTON_PIN) == LOW) {
        buttonPressed = true;
    } else {
        if (buttonPressed == true) {
            buttonPressed = false;
            int duration = millis() - buttonPressedTime;
            if (duration > 5000) {
                Serial.println("Fan Cleaning");
                sensor.startFanCleaning();
                Serial.println("Finished Fan Cleaning");
                delay(15000);
            } else if (duration > 50) {
                if (mode < 9) {
                    mode ++;
                } else {
                    mode = 0;
                }
                buttonPressedTime = millis();
                while (millis() - buttonPressedTime < 1000) {
                    display(mode + 1);
                }
                Serial.print("Mode: ");
                Serial.println(mode);
            }
        }
        buttonPressedTime = millis();
    }
    display(values[mode]);
}

void display(int data) {
    for (int i = 3; i >= 0; i--) {
        int power = 1;
        for (int j = 0; j < i; j++) {
            power = power * 10;
        }
        digits[i] = data / power;
        displayLED(i);
        data = data % power;
    }
    for (int i = 3; i >= 0; i--) {
        updateShiftRegister(digits[i]);
        digitalWrite(seven_segment[i], HIGH);
        delay(1);
        digitalWrite(seven_segment[i], LOW);
    }
}

void displayLED(int i) {
    if (digits[i] == 0) {
        digits[i] = 0b00100000;
    } else if (digits[i] == 1) {
        digits[i] = 0b00101111;
    } else if (digits[i] == 2) {
        digits[i] = 0b00010001;
    } else if (digits[i] == 3) {
        digits[i] = 0b00000101;
    } else if (digits[i] == 4) {
        digits[i] = 0b00001110;
    } else if (digits[i] == 5) {
        digits[i] = 0b01000100;
    } else if (digits[i] == 6) {
        digits[i] = 0b01000000;
    } else if (digits[i] == 7) {
        digits[i] = 0b00101101;
    } else if (digits[i] == 8) {
        digits[i] = 0b00000000;
    } else {
        digits[i] = 0b00000100;
    }
}

void updateShiftRegister(byte digit) {
    digitalWrite(LATCH_PIN, LOW);
    ledColor(digit);
    shiftOut(SERIAL_INPUT_PIN, CLOCK_PIN, LSBFIRST, digit);
    digitalWrite(LATCH_PIN, HIGH);
}

void ledColor(byte &value) {
    if (mode == 1) { //Good 0-55 Okay 55-250 Bad 250+
        if (values[1] < 250) {
            bitSet(value, 7);
        }
        if (values[1] > 55) {
            digitalWrite(RED_LED_PIN, HIGH);
        } else {
            digitalWrite(RED_LED_PIN, LOW);
        }
    } else if (mode == 3) { //Good 0-255 Okay 255-425 Bad 425+
        if (values[1] < 425) {
            bitSet(value, 7);
        }
        if (values[1] > 255) {
            digitalWrite(RED_LED_PIN, HIGH);
        }
    } else {
        digitalWrite(RED_LED_PIN, LOW);
    }
}
