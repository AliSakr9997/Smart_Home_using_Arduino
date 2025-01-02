// some comments were made as draft or for testing different situations 


//needed Libiraires
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal_I2C.h>
#define DHTTYPE DHT11 //dht type
#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal.h>

#define DHTPIN 2     // The pin connected to the DHT11 sensor (digital pin)


Servo doorServo; //garden door
Servo door1Servo; //inner door


DHT dht(DHTPIN, DHTTYPE);

const int greenLED = 7;        // Green LED for correct password
const int irSensor1 = 3;       // IR Sensor 1 for garden
const int irSensor2 = 4;       // IR Sensor 2 for garden
const int buzzer1 = 6;         // Buzzer for wrong password
const int buzzer2 = 5;         // Alarm for high temperature
bool buzzerOn = false;        //state of buzzer
const int pushButton = 8;      // Push button to stop alarm
const int led1 = 10 ;           // LED 1 connected with ldr
const int fan = 13;            // DC motor for fan
const int ldrSensor = A0;      //ldr sensor


// Keypad Setup
const byte rows = 2;
const byte cols = 2;
char keys[rows][cols] = {
  {'1', '2'},
  {'3', '4'}
};
byte rowPins[rows] = {A4, A5};
byte colPins[cols] = {A3, A2};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);

// LCD Setup
LiquidCrystal_I2C lcd(0x27,16,2);
bool M = false; //password state

// Variables
const String password = "1234"; // Correct password
String inputPassword = "";
int tempThreshold = 24;        // Temperature threshold
int highTempThreshold = 26;    // High Temperature threshold
int brightness;

void setup() {
  // Initialize pins
  Serial.begin(9600);
  dht.begin();

  lcd.init();
  lcd.backlight();
  pinMode(irSensor1, INPUT);
  pinMode(irSensor2, INPUT);
  pinMode(buzzer1, OUTPUT);
  pinMode(buzzer2, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  dht.begin();
  doorServo.attach(9);
  doorServo.write(90); // Keep the door closed initially
  door1Servo.attach(11);
  door1Servo.write(90); // Keep the door closed initially
  lcd.print("Enter pass:");
}

void loop() {
//void servo_garden_door() {
  
  if (digitalRead(irSensor1) == LOW) {
    delay(500);
    doorServo.write(0); // Open the door in one direction
    delay(500);
  } else if (digitalRead(irSensor2) == LOW){
    delay(500);
    doorServo.write(260); // Keep the door closed
    delay(500);
  }else{
    delay(500);
    doorServo.write(90); // Keep the door closed
    
  }

  if(!M){

  handleSecurity();

  }
  //temperature and light system dont work unless the password is correct 
  if(M){

//  handleLDRControl();

  int ldrValue = analogRead(ldrSensor); // Read LDR value (0-1023)
  // Correct mapping of LDR value to LED brightness (0-255)
  brightness = map(ldrValue, 0, 900, 255, 0); // Map LDR value to reverse brightness range
  delay(100);

  // Write brightness to LEDs
  analogWrite(led1, brightness);
  //analogWrite(led2, brightness);
  delay(100);
  // Display brightness on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Brightness: ");
  lcd.print(brightness);
  lcd.print("   "); // Clear any extra digits

  handleTemperatureControl();


  }
  }




//functions part 



void servo_garden_door() {
  if (digitalRead(irSensor1) == LOW) {
    delay(500);
    doorServo.write(0); // Open the door in one direction
    delay(500);
  } else if (digitalRead(irSensor2) == LOW){
    delay(500);
    doorServo.write(260); // Keep the door closed
    delay(500);
  }else{
    delay(500);
    doorServo.write(90); // Keep the door closed
    
    }
  }


void handleSecurity() {
  char key = keypad.getKey();
  if (key) {
    inputPassword += key;
    lcd.setCursor(11,0);
    lcd.print(inputPassword);

    if (inputPassword.length() == password.length()) {
      if (inputPassword == password) {
        lcd.clear();
        lcd.print("Welcome Home");
        digitalWrite(greenLED, HIGH);
        door1Servo.write(0); // Open the door in one direction
        delay(5000);
        door1Servo.write(90); // Open the door in one direction
        digitalWrite(greenLED, LOW);
        //pass =1;
        M = true;

      } else {
        lcd.clear();
        lcd.print("Wrong password");
        digitalWrite(buzzer1, HIGH);
        door1Servo.write(90); // Open the door in one direction
        delay(2000);
        digitalWrite(buzzer1, LOW);
      }
      inputPassword = ""; // Reset input
      lcd.clear();
      lcd.print("Enter pass");
    }
  }
}

// Function to handle temperature control
void handleTemperatureControl() {
    // Read temperature value from the DHT sensor
    float tempValue = dht.readTemperature();

    if (tempValue <highTempThreshold ){
      buzzerOn = false; 
    }

    // Check if the reading is valid (not NaN)
    if (!isnan(tempValue)) {
        // Display the temperature on the LCD
        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(tempValue);
        lcd.print(" C"); // Adding unit for clarity
    } else {
        // Handle invalid temperature reading
        lcd.setCursor(0, 1);
        lcd.print("Temp Error");
        return; // Exit the function if the reading is invalid
    }

    // Control fan based on temperature threshold
    if (tempValue > tempThreshold) {
        digitalWrite(fan, HIGH); // Turn on fan
    } else {
        digitalWrite(fan, LOW); // Turn off fan
    }

    // Activate buzzer if temperature exceeds high temperature threshold
    if (tempValue > highTempThreshold  && !buzzerOn )   {
        digitalWrite(buzzer2, HIGH); // Turn on alarm
        buzzerOn = true;
    if(buzzerOn){
      while (digitalRead(pushButton) == HIGH) {
        delay(50); // Debounce delay
      }

    // If the button is pressed, stop the buzzer and exit the function
      digitalWrite(buzzer2, LOW); // Turn off the buzzer
      return;
      //buzzerOn = false; 
      
    }
    

}
}

// Function to control LEDs based on LDR value
void handleLDRControl() {

  int ldrValue = analogRead(ldrSensor); // Read LDR value (0-1023)
  // Correct mapping of LDR value to LED brightness (0-255)
  brightness = map(ldrValue, 0, 900, 255, 0); // Map LDR value to reverse brightness range
  delay(100);

  // Write brightness to LEDs
  analogWrite(led1, brightness);
  //analogWrite(led2, brightness);
  delay(100);
  // Display brightness on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Brightness: ");
  lcd.print(brightness);
  lcd.print("   "); // Clear any extra digits

}
