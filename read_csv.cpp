#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <list>
using namespace std;
int main() {
    ifstream file("Current data.csv");

    if (file.is_open()) {
        string line;
        list<double> currentList;
        double SOC_0 = 0.0;
        double time = 1.0;
        double SOC = SOC_0;

        while (getline(file, line)) {
            stringstream ss(line);
            string value;

            while (getline(ss, value, ',')) {
                try {
                    double current = stod(value);
                    currentList.push_back(current);

                    if (current >= 0) {
                        SOC += (current * time) / 4000.0;
                        SOC = (SOC > 1.0) ? 1.0 : SOC;
                    } else {
                        SOC += (current * time) / 4000.0;
                        SOC = (SOC < 0.0) ? 0.0 : SOC;
                    }

                    double SOCPercentage = SOC * 100.0;
                    cout << "SOC: " << SOCPercentage << "%" << endl;
                } catch (const invalid_argument& e) {
                    cerr << "Invalid argument error: " << e.what() << endl;
                } catch (const out_of_range& e) {
                    cerr << "Out-of-range error: " << e.what() << endl;
                }
            }
        }

        cout << "Length of list: " << currentList.size() << endl;
        cout << "Summary of CSV data" << endl;
        cout << "--------------------" << endl;
        cout << "Total rows: " << currentList.size() << endl;
        cout << "Total columns: 1" << endl;
        cout << "--------------------" << endl;

        file.close();
    } else {
        cout << "Failed to open the CSV file!" << endl;
    }

    return 0;
}
