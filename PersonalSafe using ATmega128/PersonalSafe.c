#include <mega128.h>
#include <delay.h>
#include "lcd.h"

#define     Do      1908
#define     Re      1700
#define     Mi      1515
#define     Pa      1432
#define     Sol     1275
#define     La      1136
#define     Si      1012

unsigned char number[] = {0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x30};
unsigned char strpw[10] = "PASSWORD:";
unsigned char statelock[7] = "LOCKED";
unsigned char stateopen[5] = "OPEN";
unsigned char strDONE[8] = "DONE!!!";
unsigned char strADCode[10]={0x32,0x30,0x31,0x34,0x31,0x34,0x36,0x30,0x33,0x34};
unsigned char statePW[4];
unsigned char keydata;
unsigned char keynum;
unsigned char PW[4];
unsigned char pwcnt;
unsigned int int_stPW, int_PW;
unsigned char keydelete;

bit cmp;
bit blckopn;
void lckopn(void);
void myDelay_us(unsigned int delay)     //부저 구동하기위한 delay함수 
{
    int i;
    for(i=0; i<delay; i++)
    {
        delay_us(1);
    }
}
void SSound(unsigned int time)                  //음계 출력 함수 
{
    int i, tim;
    tim = 50000/time;
    for(i=0; i<tim; i++) // 음계마다 같은 시간 동안 울리도록 time 변수 사용
    {
        PORTG |= 1<<PORTG4; //부저ON, PORTG의 4번 핀 ON(out 1)
        myDelay_us(time);
        PORTG &= ~(1<<PORTG4);  //부저OFF, PORTG의 4번 핀 OFF(out 0)
        myDelay_us(time);
    }
}
void port_init()
{
    DDRC = 0x0f;
    PORTC = 0x0f;
    SREG |= 0x80; 
}

unsigned char KeyScan(void)
{
    unsigned int Key_Scan_Line_Sel = 0xf7;
    unsigned char Key_Scan_sel = 0, key_scan_num = 0;
    unsigned char Get_Key_Data = 0;

    for(Key_Scan_sel = 0; Key_Scan_sel < 4; Key_Scan_sel++)
    {
        PORTC = Key_Scan_Line_Sel;
        delay_us(10);
        Get_Key_Data = (PINC & 0xf0);

        if(Get_Key_Data != 0x00)
        {
            switch(Get_Key_Data)
            {
                case 0x10:
                    key_scan_num = Key_Scan_sel * 4 + 1;
                    break;
                case 0x20:
                    key_scan_num = Key_Scan_sel * 4 + 2;
                    break;
                case 0x40:
                    key_scan_num = Key_Scan_sel * 4 + 3;
                    break;
                case 0x80:
                    key_scan_num = Key_Scan_sel * 4 + 4;
                    break;
                default :
                    key_scan_num = 17;
                    break;
            }
            return key_scan_num;
        }
        else;
        Key_Scan_Line_Sel = (Key_Scan_Line_Sel >> 1);
    }
    return key_scan_num;
}

void getpassword(void) //패스워드 받는 함수 
{
    unsigned char i = 0;
    
    LCD_Pos(0,0);
    LCD_Str(strpw); // "PASSWORD:"

    while(1)
    {
        keydata = KeyScan();
        keydelete = KeyScan();
        delay_ms(250);
        
        if((keydelete == 8) && (i > 0))
        {   
            i--;                        // 딜리트알고리즘 
            LCD_Pos(0,i+9);                                                              
            LCD_Char(' ');
            LCD_Cursor_Shift(LEFT);
        }
        if(keydata && keydata !=8)
        {    
            if(keydata % 4 != 0)
                keynum = (keydata / 4) * 3 + (keydata % 4);
            else
                keynum = (keydata / 4) + 9;
                
            PW[i] = number[keynum];

            if(i >= 0 && i < 4)
            {
                LCD_Pos(0,i+9);
                LCD_Char(PW[i]);
            }
    
            int_PW  = ((PW[0] - 0x30) * 1000) + ((PW[1] - 0x30) * 100) + ((PW[2] - 0x30) * 10) + (PW[3] - 0x30);   //저장한 비밀번호를 int로 
            i++;
            if(keydata == 15)
            {    
                cmp = 1;
                i=0;
                if(cmp == 1)
                {
                    cmp = 0;
                    LCD_Clear();      
                    LCD_Pos(0,0);
                    LCD_Str(strDONE);
                    delay_ms(1000);
                    LCD_Clear();
                    blckopn = 0 ;
                    lckopn();
                    LCD_Pos(1,0);
                    LCD_Str(strpw); // "PASSWORD:"
                    LCD_Pos(1,9);
                    break;
                }
            }
        } 
    }     
}
void lckopn(void)
{
    if(blckopn)
    {
        LCD_Pos(0,0);
        LCD_Str(stateopen);         //비트가 1이면 open 텍스트 
    }
    else if(blckopn == 0)
    {                            //비트가 0이면 lock 텍스트
        LCD_Pos(0,0);
        LCD_Str(statelock);
    }
    else;
}
       
void InsertAdminPW(void)
{

    unsigned char j= 0;
    unsigned char strAdmin[17] = "Enter Admin Code";
    unsigned char strgetADCode[10];
    unsigned char m = 0;
    LCD_Clear();
    LCD_Pos(0,0);
    LCD_Str(strAdmin);
    LCD_Pos(1,2);
    LCD_Char('<');
    LCD_Pos(1,13);
    LCD_Char('>');
    LCD_Pos(1,3);
    
    while(1)
    {
        keydata = KeyScan();
        keydelete = KeyScan();
        delay_ms(250);
        if((keydelete == 8) && (j > 0))
        {   
            j--;
            LCD_Pos(1,j+3);                  //딜리트 함수                                             
            LCD_Char(' ');
            LCD_Cursor_Shift(LEFT);
                
        }
        
        if(keydata && (keydata !=8) && (keydata != 15))
        {   
            if(keydata % 4 != 0)
                keynum = (keydata / 4) * 3 + (keydata % 4);
            else
                keynum = (keydata / 4) + 9;
                
            strgetADCode[j] = number[keynum];
            
            if(j >= 0 && j < 10)
            {
                LCD_Pos(1, j+3);
                LCD_Char(strgetADCode[j]);    
            }
            
            j++;          
        }
        if(keydata == 15)                 //엔터 하면 비교 
        {    
            cmp = 1;
            j=0;
            if(cmp == 1)
            {
                if( (strADCode[0] == strgetADCode[0]) &&
                    (strADCode[1] == strgetADCode[1]) &&
                    (strADCode[2] == strgetADCode[2]) && 
                    (strADCode[3] == strgetADCode[3]) && 
                    (strADCode[4] == strgetADCode[4]) && 
                    (strADCode[5] == strgetADCode[5]) &&
                    (strADCode[6] == strgetADCode[6]) && 
                    (strADCode[7] == strgetADCode[7]) &&
                    (strADCode[8] == strgetADCode[8]) && 
                    (strADCode[9] == strgetADCode[9]))
                {  
                    LCD_Clear();
                    blckopn = 1;
                    pwcnt = 0;
                    SSound(Do);
                    SSound(Mi);
                    SSound(Sol);                           
                    break;
                }
                else
                { 
                    j=0;
                    for(m = 0; m<3;m++)
                    {
                    SSound(Pa);
                    delay_ms(70);
                    }
                    LCD_Clear();
                    LCD_Pos(0,0);                
                    LCD_Str(strAdmin);
                    LCD_Pos(1,2);
                    LCD_Char('<');
                    LCD_Pos(1,13);
                    LCD_Char('>');
                    LCD_Pos(1,3); 
                }
            }
        }
    }
}

void main(void)
{
   
    unsigned char screen[11]= "KPU Safe..";
    unsigned char open[17] = "The Safe is open";
    unsigned char initPW[17] ="--NEW PassWord--";
    unsigned char mode[17] = "--Insert  MODE--";
    unsigned char closed[12] = "PW fail....";
    unsigned char keymod;
    unsigned char keypw;
    unsigned char k=0,m=0;
    unsigned char trpw;
    unsigned char strwarning[9] = "WARNING:";
   
    port_init();
    LCD_Init();
    DDRG |= (1<<4); 

    LCD_Pos(0,1);
    LCD_Str(screen);
    delay_ms(1500);
    LCD_Pos(0,0);
    LCD_Str(open);
    blckopn = 1;
    while(1)
    {   
        keymod = KeyScan();
        delay_ms(200);
        if( (keymod != 4) && (keymod != 8) && (keymod != 12) && (keymod != 16) && (keymod != 13)) keypw = keymod;       
        if(blckopn)
        {
            if(keymod == 13)       //OPEN 상태에서 *키 누르면 다시 LOCK됨 
            {
                blckopn = 0 ;
                lckopn();
                SSound(Sol);
                SSound(Mi);
                SSound(Do);
            }
        }
        if(blckopn == 1)
        {
            if(keymod == 4)       // OPEN 상태에서 M1 누르면 새 패스워드 압력함수로 
            {
                LCD_Pos(0,0);
                LCD_Str(initPW); //  "--NEW PassWord--"
                LCD_Pos(1,0);
                LCD_Str(mode);   //  "--Insert  MODE--"
                delay_ms(2000);
                LCD_Clear();
                getpassword();    
            } 
        }
        if(blckopn == 0)
        {   
            
            if((keymod == 8)&& (k > 0))
            {   
                k--;
                LCD_Pos(1,k+9);                                                              
                LCD_Char(' ');
                LCD_Cursor_Shift(LEFT);
            }
            if(keypw)
            {
                if(keypw % 4 != 0)
                        trpw = (keypw / 4) * 3 + (keypw % 4);  //변환된 pw값
                    else
                        trpw = (keypw / 4) + 9;
                 statePW[k] = number[trpw];
                 
                if(k >= 0 && k < 4)
                {
                    LCD_Pos(1, k + 9);
                    LCD_Char('*');
                   
                }
                                
                int_stPW  = ((statePW[0] - 0x30) * 1000) + ((statePW[1] - 0x30) * 100) + ((statePW[2] - 0x30) * 10) + (statePW[3] - 0x30);
                k++;
                
                if(keymod == 15) // 엔터 누르면 비교
                {
                    cmp = 1;
                }
                if(cmp == 1)
                {
                    cmp = 0;
                    k=0;
                    if(int_stPW == int_PW)
                    {
                        blckopn = 1; //pos(0,0)에 "OPEN" 출력
                        pwcnt = 0;
                        LCD_Clear();
                        LCD_Pos(1,0);
                        LCD_Str(strpw);
                        lckopn();
                        SSound(Do);
                        SSound(Mi);
                        SSound(Sol);
                    }
                    else if(int_stPW != int_PW)
                    {
                        LCD_Clear();
                        lckopn();
                        LCD_Pos(0,7);
                        LCD_Str(strwarning);
                        LCD_Char(pwcnt+'1');
                        LCD_Pos(1,0);
                        LCD_Str(strpw);
                        LCD_Pos(1,9);
                        pwcnt++;
                        for(m = 0; m<3;m++)
                        {
                        SSound(Pa);
                        delay_ms(70);
                        }
                    }
                }        
            }
            if(pwcnt == 3)
            {
                LCD_Pos(1,0);
                LCD_Str(closed);   //"PW fail....";
                delay_ms(1500);
                InsertAdminPW();
                delay_ms(10);
                lckopn();
                LCD_Pos(1,0);
                LCD_Str(strpw); // "PASSWORD:"
                LCD_Pos(1,9);
            }
        }
    }
}
