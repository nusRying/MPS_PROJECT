#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
void delay (int num){
for (int i=0 ; i<num ; i++){
    for (int j=0 ; j<100 ; j++){
        for (int k=0 ; k<100 ; k++){
        }
    }
}
}
}
#pragma config OSC = HS    // Set the oscillator configuration to HS
#pragma config WDT = OFF   // Disable the Watchdog Timer
#pragma config LVP = OFF   // Disable Low-Voltage Programming

void LCD_command(unsigned char cmd) {
    PORTC = (PORTC & 0x0F) | (cmd & 0xF0); // Send high nibble first
    RC2 = 0; // RS = 0 for command IR
    RC3 = 1; // E = 1 to read
    delay(1); // Delay for at least 1 ms
    RC3 = 0; // E = 0
    delay(1); // Delay for at least 1 ms
    PORTC = (PORTC & 0x0F) | ((cmd << 4) & 0xF0); // Send low nibble second
    RC3 = 1; // E = 1
    delay(1); // Delay for at least 1 ms
    RC3 = 0; // E = 0
    delay(1); // Delay for at least 1 ms
}

void LCD_data(unsigned char data) {
    PORTC = (PORTC & 0x0F) | (data & 0xF0); // Send high nibble first
    RC2 = 1; // RS = 1 for data
    RC3 = 1; // E = 1
    delay(1); // Delay for at least 1 ms
    RC3 = 0; // E = 0
    delay(1); // Delay for at least 1 ms
    PORTC = (PORTC & 0x0F) | ((data << 4) & 0xF0); // Send low nibble second
    RC3 = 1; // E = 1
    delay(1); // Delay for at least 1 ms
    RC3 = 0; // E = 0
    delay(1); // Delay for at least 1 ms
}

void LCD_init() {
    LCD_command(0x02); // Return home
    LCD_command(0x28); // 4-bit mode, 2 lines, 5x8 font
    LCD_command(0x0C); // Display on, cursor off, blink off
    LCD_command(0x06); // Increment cursor, no shift
    LCD_command(0x01); // Clear display
    delay(2); // Delay for at least 2 ms
}

void LCD_print(char *str) {
    while (*str != '\0') {
        LCD_data(*str);
        str++;
    }
}

char* SOC(int current, int time, int SOC_0) {
    int SOC = ((current * time) / 4000) + SOC_0;
    char* snum = (char*)malloc(5 * sizeof(char));
    sprintf(snum, "%d", SOC);
    return snum;
}

void main() {
    TRISC = 0x00; // Configure PORTC as output
    TRISD = 0xFF; // Configure PORTD as input
    TRISB = 0xFF; // Configure PORTB as input
    LCD_init(); // Initialize the LCD display
    char str[16]; // Define a string buffer for sprintf()
    int soc_after;
    
    while (1) {
        int time = PORTD;
        int current = PORTB;
        
        LCD_command(0x80); // Set the cursor to the first line
        LCD_print("SOC is: "); // Print the message on the LCD

        LCD_command(0xC0); // Set the cursor to the second line
        soc_after = atoi(SOC(current, time, 4)); // Convert the string to an integer
        sprintf(str, "%d", soc_after); // Format integer as decimal string
        LCD_print(str); // Print the message on the LCD

        delay(1000); // Delay for at least 1000 ms
    }
}
