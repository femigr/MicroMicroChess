/*
 * Supposed to be a useable chess software at some point.
 * Running on an Infineon XMC2GO.
 * In and output via serial interface.
 * 
 * Author: Felix Griesau
 */
#include <LED.h>

// For library macro printing
#define str(x)  Serial.println( #x )
#define str1(x) str(x)
#define str2(x) Serial.print( #x )

int counter=0;
char inputString[5] = {};   // a char array to hold incoming data
bool stringComplete = false;  // whether the string is complete
char white = 'W';
  char black = 'B';
  char none = 'N';
  
  char nothing = ' ';
  char king = 'K';
  char queen = 'Q';
  char rook = 'R';
  char knight = 'H';//h for horse because k was already in use for king
  char pawn = 'P';
  char bishop = 'B';


XMCClass devCtrl;
LED Led;
uint32_t temperature = 0;
char noinput = Serial.read();
char field[8][8][2]={{{'R','B'}, {'H','B'}, {'B','B'}, {'Q','B'}, {'K','B'}, {'B','B'}, {'H','B'}, {'R','B'}},
  {{'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}, {'P','B'}},
  {{' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}},
  {{' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}},
  {{' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}},
  {{' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}, {' ','N'}},
  {{'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}, {'P','W'}},
  {{'R','W'}, {'H','W'}, {'B','W'}, {'Q','W'}, {'K','W'}, {'B','W'}, {'H','W'}, {'R','W'}}};


void setup( )
{
  
  Led.Add( LED1 );            // Configure the LEDs
  Led.Add( LED2 );
  
  //Serial.print(field);
  
  
Serial.begin( 9600 );
delay( 3000 );

Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");
Serial.println("");

while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB
  }
Serial.println( "XMChess" );
Serial.print( "Running on " );
str2( XMC_BOARD );
Serial.println("");
delay( 2000 );
// dummy read to ensure first value which may be erroneous is ignored
temperature = devCtrl.getTemperature();
}


bool move(char field1[], char field2[]){
    Serial.println("Please wait for the supercomputer to compute the supreme computations...");
    
    int y1= field1[0] -'A';
    int y2= field2[0] -'A';
    int x1 = field1[1] - '0';
    int x2 = field2[1] - '0';
    x1=8-x1;
    x2=8-x2;
    
    if(!((x1<8)&&(x2<8)&&(y1<8)&&(y2<8)&&(x1>(-1))&&(x2>(-1))&&(y1>(-1))&&(y2>(-1)))){
      Serial.println("You mistapped!");
      return false;
    }
    
    if(field[x1][y1][1]!=white){
        Serial.println("No figure of you to move there");
        return false;
      }

    
    if(field1 == field2){//Nicht sicher ob das klappt wegen Pointern und so TODO: TESSTTTT
        Serial.println("You have to move!");
        return false;
      }
    
    //Status hier: Es befindet sich eine weisse Figur auf dem 1. Feld, 2. Feld ist auf dem Spielfeld. 
    //Jetzt Zug auf Validität überprüfen. Vereinfachung, man kann ins Schach ziehen, der Gegner kann dann den König schlagen, erst dann ist das Spiel vorbei
   if(ismovevalid(x1,y1,x2,y2,white)){
        field[x2][y2][0] = field[x1][y1][0];//Schlagen ist damit gleich schon integriert :)
        field[x2][y2][1] = field[x1][y1][1];
        field[x1][y1][0] = nothing;
        field[x1][y1][1] = none;
    }
    
    

    
    return false;
  }


bool ismovevalid(int x1, int y1, int x2, int y2, char color){
   char figuretype = field[x1][x2][0];
   if(figuretype == king){
      if((abs(x1-x2)<2) && (abs(y1-y2)<2)){
        if(field[x2][y2][1]!=color){
          return true; 
        }
      }
      
   } 
   
   if(figuretype == queen){
      if((abs(x1-x2)-abs(y1-y2)==0) or ((x1==x2) or (y1==y2))){
        if(field[x2][y2][1]!=color){
          return true; 
        }
      }
      
   }
   
   if(figuretype == rook){
      if((x1==x2) or (y1==y2)){
        if(field[x2][y2][1]!=color){
          return true; 
        }
      } 
   }
   
   if(figuretype == bishop){
      if((abs(x1-x2)-abs(y1-y2)==0)){
        if(field[x2][y2][1]!=color){
          return true; 
        }
      }
      
   }

   if(figuretype == knight){
      if((abs(x1-x2)==2)&&(abs(y1-y2)==1) or (abs(x1-x2)==1)&&(abs(y1-y2)==2)){
        if(field[x2][y2][1]!=color){
          return true; 
        }
      }
      
   }

   if(figuretype == pawn){
      if((((x2-x1==1)&&(color=black))or((x2-x1==(-1))&&(color=white)))&&(abs(y1-y2)==0) && (field[x2][y2][1]==none)){
        //straight forward
        return true; 
        
      }
      if((((x2-x1==1)&&(color=black))or((x2-x1==-1)&&(color=white))) && (abs(y1-y2)==1) && (field[x2][y2][1]!=none)){
        if(field[x2][y2][1]!=color){
          //pawn takes something
          return true; 
        }
      }
      
   }
   return false;
}

void loop( )
{
 
 Led.On( LED2 );
 if (stringComplete) {
  counter=0;
  Led.Off( LED2 );
  stringComplete = false;
  
  
  char field1[3];
  char field2[3];
  
  field1[0] = inputString[0];
  field1[1] = inputString[1];

  
  field2[0] = inputString[3];
  field2[1] = inputString[4];
  if (move(field1, field2)==false)
  {
    Serial.println("Illegal move, try again!");
  }
  else{
    Serial.println("It's your turn :)");
  }
  printfield();
  printcolor();
  
}}


void serialEvent() {
  
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString[counter] = inChar;
    counter += 1;
    Serial.print(inChar);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (counter == 5) {
      Serial.println("");
      stringComplete = true;
      
    }
  }
}

void printfield(){
  Serial.println("");

  for(int x=0; x<8; x+=1){
    for(int y=0; y<8; y+=1){
      Serial.print(field[x][y][0]);
    }
    Serial.println("");
  }
}
void printcolor(){
  Serial.println("");

  for(int x=0; x<8; x+=1){
    for(int y=0; y<8; y+=1){
      Serial.print(field[x][y][1]);
    }
    Serial.println("");
  }


}
