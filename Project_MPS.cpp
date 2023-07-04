#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

using namespace std;

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the LCD I2C address and dimensions
// Define the GPIO pins connected to the SIP switch
const int switchPins[] = {2, 4, 5, 12, 13, 14, 15, 16};  // Example GPIO pin numbers
const int numSwitches = sizeof(switchPins) / sizeof(switchPins[0]);
const int decnum = 0;
void setup() {
  Serial.begin(9600);  // Initialize serial communication
  for (int i = 0; i < numSwitches; i++) {
    pinMode(switchPins[i], INPUT_PULLUP);  // Set the SIP switch pins as inputs with internal pull-up resistors
  }
}
int main() {
    ifstream file("C:\\Users\\123\\Desktop\\Current data.csv");

    if (file.is_open()) {
        string line;
        list<double> currentList;
        double SOC_0 = 0.0;
        double time = 1.0;
        double SOC = SOC_0;
        double previousSOC = SOC_0;

        // Initialize LCD
        lcd.begin(16, 2);
        lcd.backlight();

        while (getline(file, line)) {
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                try {
                    double current = stod(value);
                    currentList.push_back(current);

                    previousSOC = SOC; // Store the previous SOC value

                    if (current >= 0) {
                        SOC = previousSOC + (current * time) / 4000.0;
                        SOC = (SOC > 1.0) ? 1.0 : SOC;
                    } else {
                        SOC = previousSOC + (current * time) / 4000.0;
                        SOC = (SOC < 0.0) ? 0.0 : SOC;
                    }

                    double SOCPercentage = SOC * 100.0;

                    // Display SOC on LCD
                    lcd.clear();
                    lcd.setCursor(0, 0);
                    lcd.print("SOC: ");
                    lcd.print(SOCPercentage);
                    lcd.print("%");

                    Serial.println "Current SOC: " << SOCPercentage << "%" << endl;
                } catch (const invalid_argument& e) {
                    // cerr << "Invalid argument error: " << e.what() << endl;
                } catch (const out_of_range& e) {
                    // cerr << "Out-of-range error: " << e.what() << endl;
                }
            }
        }

        // Serial.println "Length of list: " << currentList.size() << endl;
        // Serial.println "Summary of CSV data" << endl;
        // Serial.println "--------------------" << endl;
        // Serial.println "Total rows: " << currentList.size() << endl;
        // Serial.println "Total columns: 1" << endl;
        // Serial.println "--------------------" << endl;

        file.close();
    } else {
        // Serial.println "Failed to open the CSV file!" << endl;
    }

    return 0;
}
