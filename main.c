#include <16F886.h>
#device ADC=10 *=16  

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES PUT                      //Power Up Timer
#FUSES NOMCLR                   //Master Clear pin not enabled
#FUSES NOPROTECT                //Code not protected from reading
#FUSES NOCPD                    //No EE protection
#FUSES BROWNOUT                 //Brownout reset
#FUSES IESO                     //Internal External Switch Over mode enabled
#FUSES FCMEN                    //Fail-safe clock monitor enabled
#FUSES NOLVP                    //No low voltage prgming, B3(PIC16) or B5(PIC18) used for I/O
#FUSES NODEBUG                  //No Debug mode for ICD
#FUSES NOWRT                    //Program memory not write protected
#FUSES BORV40                   //Brownout reset at 4.0V
#FUSES RESERVED                 //Used to set the reserved FUSE bits
#FUSES INTRC_IO 

#use delay(clock=8M)

#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8)
#include <time.h>
#include<stdlib.h>
#include<math.h>
#include "lcd.c"
#define RESET_BUTTON    PIN_d0
#DEFINE n 4


void main()
{  
//!   int primeNum[10] = {1091,1427,1753,1571,1291,2671,701,373,661,281};
   int board[n][n] = {0};
   int freePos[n*n]={0};
//!   int gen1 = 211, gen2 = 257;
   int score = 0;
   char keyInput;//get input.
   int randPos;//random number position to fill a new item(number 2) in board.
   int newPos;
   int value;
   boolean canMove;
   
   lcd_init();
   delay_ms(6);
   
   printf ("\t\t< Welcome to 2048 game >\r\n");
   printf ("'a' = Left  'd' = Right  'w' = Up  's' = Down\r\n\n");
   srand(get_timer0());
   while(TRUE) {
      printf(lcd_putc,"\fYour score\n%u", score);
      delay_ms(1);
      
      //---------------------------game over check-----------------------------------------//
      boolean canMove = false;
      int count = 0;
      for(int i=0;i<n;i++){
         for(int j=0;j<n;j++){
            if(board[i][j] == 0){
               freePos[count++] = (i * n) + j;
               canMove = true;
            }
            if((i != 0 && (board[i-1][j] == 0 || board[i-1][j] == board[i][j])) ||
               (i != n-1 && (board[i+1][j] == 0 || board[i+1][j] == board[i][j])) ||
               (j != 0 && (board[i][j-1] == 0 || board[i][j-1] == board[i][j])) ||
               (j != n-1 && (board[i][j+1] == 0 || board[i][j+1] == board[i][j]))) canMove = true;
         }
      }
      if(!canMove) {
         printf("\t\t\t!Game Over!\r\n");
         break;
      }
       //---------------------------game over check-----------------------------------------//
      
      //---------------------------random Generator-----------------------------------------//
//!      randPos = 0;
//!      if(gen2 == 0) gen2 = primeNum[count%10];
//!      for(int i=0;i<count;i++){
//!         randPos += (freePos[i]*freePos[i])%43;
//!      }
//!      randPos = abs((int)((gen1%41) * randPos))%count;
//!      int gen1Tmp = gen1;
//!      gen1 = gen2;
//!      gen2 = (int)(gen2 * gen1Tmp * randPos + 1427);
      
      randPos = rand() %count;
      board[freePos[randPos]/n][freePos[randPos]%n] = 2; //fill a new item(number 2) in board.
      //---------------------------random Generator-----------------------------------------//
      
      //---------------------------show Board-----------------------------------------//
      for(int i=0;i<n;i++){
         printf("\t");
         for(int j=0;j<n;j++){
            printf("\t%d", board[i][j]);
         }
         printf("\r\n");
      }
      printf("\t\tadd new [2] in (%d,%d)\r\n",freePos[randPos]/n,freePos[randPos]%n);
      printf("\r\n");
      printf("\r\n");
      //---------------------------show Board-----------------------------------------//
      
      //---------------------------get input and move-----------------------------------------//
      keyInput = getc();
      switch(keyInput){
         case 'a': //move items to the left if can.
            printf("\t\t\t----Left----\r\n");
            for(int i=0;i<n;i++){
               for(int j=1;j<n;j++){
                  newPos = j;
                  value = board[i][j];
                  while(newPos > 0){
                     if(board[i][newPos-1] == 0) newPos--; //move when position in left hand empty.
                     else{
                        if(board[i][newPos-1] == value){ //move when value can sum with same number.
                           value*=2; 
                           newPos--;
                        }
                        break;//end while loop.
                     }
                  }
                  if(newPos!=j){
                     if(value > 2) score += value;
                     board[i][newPos] = value;
                     board[i][j] = 0;
                  }
               }
            }
            
            break;
         case 'd': //move items to the right if can.
            printf("\t\t\t---Right---\r\n");
            for(int i=0;i<n;i++){
               for(int j=1;j<n;j++){
                  newPos = n-1-j;
                  value = board[i][n-1-j];
                  while(newPos < n-1){
                     if(board[i][newPos+1] == 0) newPos++; //move when position in right hand empty.
                     else{
                        if(board[i][newPos+1] == value){ //move when value can sum with same number.
                           value*=2; 
                           newPos++;
                        }
                        break;//end while loop.
                     }
                  }
                  if(newPos!=n-1-j){
                     if(value > 2) score += value;
                     board[i][newPos] = value;
                     board[i][n-1-j] = 0;
                  }
               }
            }
            break;
         case 'w': //move items to the top if can.
            printf("\t\t\t----Top----\r\n");
            for(int i=0;i<n;i++){
               for(int j=1;j<n;j++){
                  newPos = j;
                  value = board[j][i];
                  while(newPos > 0){
                     if(board[newPos-1][i] == 0) newPos--; //move when position in top hand empty.
                     else{
                        if(board[newPos-1][i] == value){ //move when value can sum with same number.
                           value*=2; 
                           newPos--;
                        }
                        break;//end while loop.
                     }
                  }
                  if(newPos!=j){
                     if(value > 2) score += value;
                     board[newPos][i] = value;
                     board[j][i] = 0;
                  }
               }
            }
            break;
         case 's': //move items to the bottom if can.
            printf("\t\t\t---Bottom---\r\n");
            for(int i=0;i<n;i++){
               for(int j=1;j<n;j++){
                  newPos = n-1-j;
                  value = board[n-1-j][i];
                  while(newPos < n-1){
                     if(board[newPos+1][i] == 0) newPos++; //move when position in bottom hand empty.
                     else{
                        if(board[newPos+1][i] == value){ //move when value can sum with same number.
                           value*=2; 
                           newPos++;
                        }
                        break;//end while loop.
                     }
                  }
                  if(newPos!=n-1-j){
                     if(value > 2) score += value;
                     board[newPos][i] = value;
                     board[n-1-j][i] = 0;
                  }
               }
            }
            break; 
         default:
      }
      //---------------------------get input and move-----------------------------------------//
      
   }
   
}
