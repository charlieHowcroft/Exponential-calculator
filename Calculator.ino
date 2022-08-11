// C++ code
// keypad constant variables
const int numColumns=3;
const int numRows=4;
const int columnPins[numColumns]= {9, 8, 7};
const int rowPins[numRows] = {13, 12, 11, 10};
const char keyMap [numRows][numColumns] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//led constant variables
#define segmentA A5
#define segmentB 6
#define segmentC 5
#define segmentD 4
#define segmentE 3
#define segmentF A4
#define segmentG A3
#define on LOW
#define off HIGH
#define displayNumberTime 1000
#define clearTime 1000

//calculator constant variables
const int maxBase=4;


//keypad variables that change

bool buttonPrevious [numRows][numColumns] = {
  {false, false, false},
  {false, false, false},
  {false, false, false},
  {false, false, false}
};

bool buttonCurrent [numRows][numColumns] = {
  {false, false, false},
  {false, false, false},
  {false, false, false},
  {false, false, false}
};

//calculator variables that change
int inputCount=0;
int exponent=0;
unsigned long answer=0;
bool done=false;
unsigned long arrayBase[maxBase];
unsigned long arrayAnswer[100];
int answerCount=0;


//main

void loop() 
{
  //Serial.print(processKeypadInput());
  
  char c=1;
  inputCount=0;
  exponent=0;
  answer=0;
  done=false;
  answerCount=0;
  resetArrayBase();
  
  while(!errorCheck(c)&&!done)
  {
  c=processKeypadInput();
    
  switch (c){
    case '*':
    	if(!readExponent(processKeypadInput()))
    		{
      			done=true;
          		displayE();
          		displayThree();
    		}else if(inputCount>1){
          		displayE();
          		displayTwo();
       		}else{
      
    		  answer=power(readBase(), exponent);
     		  done = true;
   			}
    break;
    
    case '#':
    	if(inputCount==0)
        {
          displayE();
          displayThree();
        }
    
    	answer=squareRoot(readBase());
    	done=true;
    break;
    
    default:
    addToInput(c);
    break;
  	}
    
  }
  Serial.print(" Answer is: ");
  Serial.println(answer);
  answerToArray(answer);
  displayAnswer();
  
  
  
}

void setup()
{
  Serial.begin(9600);
  
  pinMode(segmentA, OUTPUT);
  digitalWrite(segmentA, off);
  pinMode(segmentB, OUTPUT);
  digitalWrite(segmentB, off);
  pinMode(segmentC, OUTPUT);
  digitalWrite(segmentC, off);
  pinMode(segmentD, OUTPUT);
  digitalWrite(segmentD, off);
  pinMode(segmentE, OUTPUT);
  digitalWrite(segmentE, off);
  pinMode(segmentF, OUTPUT);
  digitalWrite(segmentF, off);
  pinMode(segmentG, OUTPUT);
  digitalWrite(segmentG, off);
}

//keypad functions
void scanKeypad() {
  
  
  for(int i = 0; i<numColumns; i++){
	pinMode(columnPins[i], INPUT_PULLUP); //set up the input for the column pin
  }

  //runs once, sets up all the row pins to output HIGH, this is so we can set each row to LOW one at a time and elimate rows 
  for (int r=0; r<numRows; r++){
     pinMode(rowPins[r], OUTPUT);
     digitalWrite(rowPins[r], HIGH);
   }

	
  for(int c =0; c<numColumns; c++)
  {
    for(int r = 0; r<numRows; r++)
	{
    digitalWrite(rowPins[r], LOW);
    buttonCurrent[r][c]=!digitalRead(columnPins[c]);
    digitalWrite(rowPins[r], HIGH);
	}
	}
}


char processKeypadInput(){
	
	char entry = '\0';
	int count=0;
  	
  	while(entry=='\0')
    {
    delay(50);
    scanKeypad(); 
	for(int c=0; c<numColumns; c++)
    {
		for(int r=0; r<numRows; r++)
        {
			if (buttonCurrent[r][c]&&!buttonPrevious[r][c])
            {
				buttonPrevious[r][c]=true;
				entry = keyMap[r][c];
				count ++;
			} else if(!buttonCurrent[r][c]&&buttonPrevious[r][c]) 
            {
				buttonPrevious[r][c]=false;
			}
		}
    }	
    }
      
	if(count>1)
	{
		entry = 'e';
	}
	return entry;
}

//calculator functions
int charToInt(char c)
{
  switch (c) {
    
    case '0':
    return 0;
    break;
    
    case '1':
    return 1;
    break;
    
    case'2':
    return 2;
    break;
    
    case'3':
    return 3;
    break;
    
    case'4':
    return 4;
    break;
    
    case'5':
    return 5;
    break;
    
    case'6':
    return 6;
    break;
    
    case'7':
    return 7;
    break;
    
    case'8':
    return 8;
    break;
    
    case'9':
    return 9;
    break;
  }
}
      

void addToInput(char input)
{
  arrayBase[inputCount] = charToInt(input);
  Serial.print(" base updated and is: ");
  Serial.print(arrayBase[0]);
  Serial.print(arrayBase[1]);
  Serial.print(arrayBase[2]);
  Serial.println(arrayBase[3]);
  inputCount++;
}

bool readExponent(char input)
{
  if ((input=='#')||(input=='*'))
  {
    Serial.println(" exponent entered not a number ");
    
    return false;
  }
      
  exponent=charToInt(input);
  Serial.print(" exponent is an int and is: ");
  Serial.println(exponent);
  return true;
}
      

bool errorCheck(char input)
{
  if (inputCount>maxBase)
  	{
    	Serial.println(" first error catch, base too large ");
    	done=true;
    	displayE();
    	displayOne();
   		return true;
 	} 
  
  else if((inputCount==0)&&((input=='*')||(input=='#')))
  {
       Serial.println(" second error catch, no base entered ");
       return true;
	 }
  Serial.println(" passed error check ");
   return false;
}

void resetArrayBase()
{
  for(int i=0; i<maxBase; i++)
  {
    arrayBase[i]=0;
  }  
}
     
unsigned long power(unsigned long base, int exponent)
{
  unsigned long answer=1;
  
  for(int i=0; i<exponent; i++)
  {
    answer=answer*base;
  }
  return answer;
}

unsigned long squareRoot(int radicand)
{
    float temp, sqrt;

    // store the half of the given number e.g from 256 => 128
    sqrt = radicand / 2;
    temp = 0;

    // Iterate until sqrt is different of temp, that is updated on the loop
    while(sqrt != temp){
        // initially 0, is updated with the initial value of 128
        // (on second iteration = 65)
        // and so on
        temp = sqrt;

        // Then, replace values (256 / 128 + 128 ) / 2 = 65
        // (on second iteration 34.46923076923077)
        // and so on
        sqrt = ( radicand/temp + temp) / 2;
    }

    return sqrt;
}

int readBase()
{
	int base=0;
  
	for(int i=0; i<inputCount; i++)
	{
      	if(arrayBase[i]==0)
        {
          base=base;
        }else{
		  base=base+arrayBase[i]*power(10, inputCount-1-i);
        }
	}
  Serial.print(" Base read: ");
  Serial.println(base);
	return base;
}

void answerIntegerCount(unsigned long answer)
{
  unsigned long answerTemp = answer;
  Serial.println(" entered answer Integer count");
  while(answerTemp!=0)
  {
    Serial.println("inside while");
    answerTemp=answerTemp/10;
    answerCount++;
  }
}
    

void answerToArray(unsigned long ans)
{ 
  answerIntegerCount(ans);
  Serial.println("Entered answer to array");
  Serial.print("answer Count is: ");
    Serial.println(answerCount);
  for (int i=answerCount-1; i>=0; i--)
  {
    arrayAnswer[i]=answer%10;
    answer=answer/10;
  }
  Serial.print("Array answer is: ");
  for(int i=0; i<answerCount; i++)
  {
  Serial.print(arrayAnswer[i]);
  }
  Serial.println();
}
         

//seven segment display functions

void displayAnswer()
{
  for(int i=0;i<answerCount;i++)
  {
    displayNum(arrayAnswer[i]);
  }
}

void displayNum(int num)
{
  switch (num){
    
    case 0:
  	  displayZero();
    break;
    
    case 1:
      displayOne();
    break;
    
    case 2:
      displayTwo();
    break;
    
    case 3:
      displayThree();
    break;
    
    case 4:
      displayFour();
    break;
    
    case 5:
      displayFive();
    break;
    
    case 6:
      displaySix();
    break;
    
    case 7:
      displaySeven();
    break;
    
    case 8:
      displayEight();
    break;
    
    case 9:
      displayNine();
    break;
  }
}

void displayZero(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, on);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, off);
  delay(displayNumberTime);
  
  clearDisplay();
}
void displayOne(){
  digitalWrite(segmentA, off);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, off);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, off);
  digitalWrite(segmentG, off);
  delay(displayNumberTime);
  
  clearDisplay();
}
void displayTwo(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, off);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, on);
  digitalWrite(segmentF, off);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}
void displayThree(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, off);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displayFour(){
  digitalWrite(segmentA, off);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, off);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displayFive(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, off);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displaySix(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, off);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, on);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displaySeven(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, off);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, off);
  digitalWrite(segmentG, off);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displayEight(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, on);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displayNine(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, on);
  digitalWrite(segmentC, on);
  digitalWrite(segmentD, off);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
}

void displayE(){
  digitalWrite(segmentA, on);
  digitalWrite(segmentB, off);
  digitalWrite(segmentC, off);
  digitalWrite(segmentD, on);
  digitalWrite(segmentE, on);
  digitalWrite(segmentF, on);
  digitalWrite(segmentG, on);
  delay(displayNumberTime);
  
  clearDisplay();
  
}

void clearDisplay(){
  digitalWrite(segmentA, off);
  digitalWrite(segmentB, off);
  digitalWrite(segmentC, off);
  digitalWrite(segmentD, off);
  digitalWrite(segmentE, off);
  digitalWrite(segmentF, off);
  digitalWrite(segmentG, off);
  delay(clearTime);
}

/*
     
     void loop() 
{
  //Serial.print(processKeypadInput());
  
  switch(processKeypadInput()){
    
    case '0':
  	  displayZero();
    break;
    
    case '1':
      displayOne();
    break;
    
    case '2':
      displayTwo();
    break;
    
    case '3':
      displayThree();
    break;
    
    case '4':
      displayFour();
    break;
    
    case '5':
      displayFive();
    break;
    
    case '6':
      displaySix();
    break;
    
    case '7':
      displaySeven();
    break;
    
    case '8':
      displayEight();
    break;
    
    case '9':
      displayNine();
    break;
    
    default :
      displayError();
    break;
    
  }
  
}
  */   
     
         


