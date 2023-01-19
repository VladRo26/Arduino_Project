#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const byte LINII=4;
const byte COLOANE=4;
char taste[LINII][COLOANE]=
{ {'1','2','3','A'},
  {'4','5', '6','B' },
  {'7','8','9','C'}, 
  {'*', '0', '#', 'D'}
};
byte rowPins[LINII]={9,8,7,6};
byte colPins[COLOANE]={5,4,3,2};
Keypad c4keypad = Keypad(makeKeymap(taste),rowPins,colPins,LINII,COLOANE);
#define Code_Length 8
char v[Code_Length];
char cod[Code_Length]="7355608";
byte cnt=0;
const int PIN_RED=13;
const int PIN_GREEN=12;
const int PIN_BLUE=11;
const int PIN_BUZZ=10;
void reset_vector()
{
	while(cnt!=0)
	{
		v[cnt]=0;
		cnt--;
	}
}

void setup() {
	pinMode(PIN_RED, OUTPUT);
	pinMode(PIN_GREEN, OUTPUT);
	pinMode(PIN_BLUE, OUTPUT);
	digitalWrite(PIN_RED,   0);
  digitalWrite(PIN_GREEN, 0);
  digitalWrite(PIN_BLUE,  0);
	lcd.init();
	lcd.backlight();
	delay(500);
}

int d_time;
int l_millis;
int t1;
int t2;
int tmax=1000;
void reset()
{
	digitalWrite(PIN_RED,   0);
  digitalWrite(PIN_GREEN, 0);
  digitalWrite(PIN_BLUE,  0);
	t1=0;
	t2=0;
	tmax=1000;
	reset_vector();

}
void loop() {
	
	d_time=millis()-l_millis;
	l_millis=millis();
	t1=t1+d_time;
	t2=t2+d_time;
	if(t1>tmax)
	{
	tone(PIN_BUZZ,800);
	digitalWrite(PIN_RED,   255);
	delay(100);
	digitalWrite(PIN_RED,   0);
	noTone(PIN_BUZZ);
	t1=0;
	}
	if(t2>200)
	{
		tmax=tmax-15;
		t2=0;
	}
	if(tmax<100)
	{
	digitalWrite(PIN_RED,   1);
  digitalWrite(PIN_GREEN, 1);
  digitalWrite(PIN_BLUE,  0);
	lcd.clear();
	lcd.print("BOOOOM!");
	tone(PIN_BUZZ,1000);
	delay(1000);
	noTone(PIN_BUZZ);
	delay(9000);
		reset();
		return ;
	}
	lcd.setCursor(0,0);
	lcd.print("Defuse code:");
	char tasta=c4keypad.getKey();
  if(tasta)
	{
		v[cnt]=tasta;
		lcd.setCursor(cnt,1);
		lcd.print(v[cnt]);
		//lcd.setCursor(cnt,1);
		cnt++;
	}
	if(cnt==Code_Length-1)
	{
		lcd.clear();
		if(strcmp(v,cod)==0)
		{
			tone(PIN_BUZZ,100);
			delay(300);
			noTone(PIN_BUZZ);
			lcd.print("Bomb has been");
			lcd.setCursor(0,1);
			lcd.print("Defused");
			digitalWrite(PIN_RED, 0);
      digitalWrite(PIN_GREEN, 255);
      digitalWrite(PIN_BLUE, 0);
			delay(10000);
			lcd.clear();
			reset();
			return;
		}
		else
		{
			lcd.print("Incorrect code!");
			delay(1000);
		lcd.clear();
		reset_vector();
		}
	}
}