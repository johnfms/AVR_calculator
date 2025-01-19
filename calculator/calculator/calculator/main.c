/*calculator.c Created: 13/8/2024 10:13:09 PM Author : john*/ 
#include <avr/io.h>
#include <string.h>
#include "lcd.h"
#include "keypad.h"
#define F_CPU 8000000UL

float tonum(char arr[]){
	float num=0;
	for (char i =0;arr[i]!='\0';i++){
		 num =num*10+(arr[i]-'0');
	}
	return num;
}
int main(void){
	lcd_init();
	char num1arr[10];
	char num2arr[10];
	int change = 0;
	char i=0;
	float result=0;
	char result_str[10];
	char op;
	char firsttime=1;
	float num1=0;
	float num2=0;
    while (1) {	
		char key = keypad_scan();
		if (firsttime!=1 && (key =='+' || key=='-'  || key=='*'  || key=='/')){
				change=1;
				num1arr[i]='\0';
				num1=result;
				i=0;
				op = key;
		}else if ( (key =='+' || key=='-'  || key=='*'  || key=='/')){
			if (firsttime == 1) {
				num1arr[i] = '\0';
				num1 = tonum(num1arr);
				firsttime = 0;
			}
			change=1;
			i=0;
			op = key;
		}else if(key == '='){
			change=0;
			num2arr[i]='\0';
			i=0;
			num2=tonum(num2arr);
			switch(op) {
				case '+':result = num1 + num2;break;
				case '-':result = num1 - num2;break;
				case '*':result = num1 * num2;break;
				case '/':
				if (num2 != 0) {
					result = num1 / num2;
					} else {
					lcd_cmd(0xc0);
					lcd_str("Error: Div by 0");
					for (int j = 0; j < 10; j++) {
						num1arr[j] = 0;
						num2arr[j] = 0;
					}
					change = 0;
					i = 0; 
					_delay_ms(500);
					lcd_cmd(0x01);
					lcd_cmd(0x80);
					firsttime=1;
					continue;
				}
				break;
			}
			lcd_cmd(0xc0);
			dtostrf(result, 0, 2, result_str);//double to string
			lcd_cmd(0xC0);
			lcd_str(result_str);
			
			for (int j = 0; j < 10; j++) {
				num1arr[j] = 0;
				num2arr[j] = 0;
			}
			change = 0;
			i = 0;
			_delay_ms(500);
			lcd_cmd(0x01);
			lcd_cmd(0x80);
			lcd_str(result_str);
			}else if(key=='c'){
				lcd_cmd(0x01);
				firsttime=1;
				result=0;
				num1=0;
				num2=0;
				change=0;
				i=0;
				}else{
			if (change==0){
				num1arr[i]=key;
				i++;
			}else if (change==1){
				num2arr[i]=key;
				i++;
			}
		}
    }
}