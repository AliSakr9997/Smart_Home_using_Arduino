# Smart-Home-project


# Arduino-Based Multi-Function Home Automation Project

## 🌟 Project Overview  
This project implements a multi-functional home automation system using Arduino. It integrates various sensors and actuators to automate and simplify daily tasks.

### ✨ Features  
1. **LED Brightness Control**  
   - Adjusts LED brightness inversely to light intensity using an LDR.  

2. **Servo Motor Control**  
   - Operates doors using two IR sensors and a keypad for input.  

3. **Temperature Monitoring**  
   - Activates a fan when the temperature exceeds a threshold.  
   - Triggers a buzzer alarm for high temperatures, resettable via a push button.  

4. **LCD Display**  
   - Real-time display of temperature, brightness, and password validation.  

---

### 🛠️ Components Used  
| Component                 | Quantity | Description                                 |
|---------------------------|----------|---------------------------------------------|
| Arduino UNO               | 1        | Microcontroller for all functionalities     |
| LDR (Light Sensor)        | 1        | Measures ambient light                     |
| IR Sensors                | 2        | Controls servo motors for doors            |
| Servo Motors              | 2        | Operates doors in specific directions       |
| DHT11 (Temperature Sensor)| 1        | Monitors temperature                       |
| Keypad                    | 1        | Accepts user input for password validation  |
| Buzzer                    | 2        | Alerts for wrong password or high temperature |
| LCD with I2C              | 1        | Displays system information                |
| LED                       | 2        | Demonstrates light control functionality    |

---

### ⚙️ How It Works  
1. **LDR-Based LED Control**:  
   - The LDR measures ambient light intensity, and the LED brightness is adjusted inversely using PWM signals.  

2. **IR and Servo Control**:  
   - Two IR sensors control the servo motor's movement in response to object detection.  

3. **Temperature Monitoring**:  
   - Activates a fan for temperature regulation.  
   - Triggers an alarm if the temperature exceeds a high threshold until manually reset.  

4. **LCD Display**:  
   - Displays temperature, brightness, and password-related messages.  

---

### 📜 Code Snippet  
```cpp
#include <DHT.h>
#include <Servo.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Servo doorServo;
const int ldrSensor = A0;

// Setup function
void setup() {
  Serial.begin(9600);
  pinMode(ldrSensor, INPUT);
  dht.begin();
  doorServo.attach(9);
}

// Main loop
void loop() {
  int lightValue = analogRead(ldrSensor);
  float temp = dht.readTemperature();
  // Add your logic here
}
