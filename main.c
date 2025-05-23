#include <reg51.h>

sbit rs = P2^0;
sbit rw = P2^1;
sbit en = P2^2;

sbit sw1 = P0^0; // Button for Student1
sbit sw2 = P0^1; // Button for Student2

unsigned char vote1 = 0;
unsigned char vote2 = 0;
unsigned char total_votes = 0;

void delay(unsigned int);
void lcd_cmd(unsigned char);
void lcd_data(unsigned char);
void lcd_string(unsigned char *);
void show_votes();

void main() {
    // LCD initialization
    lcd_cmd(0x38); // 2 lines, 5x7 matrix
    lcd_cmd(0x0C); // Display ON, cursor OFF
    lcd_cmd(0x06); // Increment cursor
    lcd_cmd(0x01); // Clear screen

    lcd_cmd(0x80);
    lcd_string(" Welcome to Class ");
    lcd_cmd(0xC0);
    lcd_string("    Election     ");
    delay(2);

    lcd_cmd(0x01);
    show_votes();

    while(total_votes < 18) {
        if(sw1 == 1) 
					{
            delay(3);
            while(sw1 == 0); // Wait for release
            vote1++;
            total_votes++;
            show_votes();
        }

        if(sw2 == 1) 
					{
            delay(3);
            while(sw2 == 0);
            vote2++;
            total_votes++;
            show_votes();
        }
    }

    // Voting complete – display winner
    lcd_cmd(0x01); // Clear screen
    lcd_cmd(0x80);
    lcd_string("  Voting Done!   ");
    delay(5);
    lcd_cmd(0x01);
    lcd_cmd(0x80);

    if(vote1 > vote2) {
        lcd_string("Winner: Student1 ");
    }
    else if(vote2 > vote1) {
        lcd_string("Winner: Student2 ");
    }
    else {
        // Tie – Random selection
        if(TL0 % 2 == 0)
            lcd_string("Winner: Student1 ");
        else
            lcd_string("Winner: Student2 ");
    }

    while(1); // Stop execution
}

// === Utility Functions ===

void delay(unsigned int t) {
    unsigned int i, j;
    for(i=0; i<t; i++)
        for(j=0; j<1275; j++);
}

void lcd_cmd(unsigned char cmd) {
    rs = 0; rw = 0; en = 1;
    P1 = cmd;
    en = 0;
    delay(2);
}

void lcd_data(unsigned char dat) {
    rs = 1; rw = 0; en = 1;
    P1 = dat;
    en = 0;
    delay(2);
}

void lcd_string(unsigned char *str) {
    while(*str)
        lcd_data(*str++);
}

void show_votes() {
    lcd_cmd(0x80);
    lcd_string("Stud1 Vote = ");
    lcd_data((vote1 / 10) + '0');
    lcd_data((vote1 % 10) + '0');

    lcd_cmd(0xC0);
    lcd_string("Stud2 Vote = ");
    lcd_data((vote2 / 10) + '0');
    lcd_data((vote2 % 10) + '0');
}
