#define HALL_SENSOR A10
#define LEVI_MAGNET 2
#define RIGHT_MAGNET 3
#define LEFT_MAGNET 4
#define AUDIO 9

constexpr int PERIOD = 50;
constexpr unsigned int SCALE = 256; 
constexpr unsigned int targetValue = 300;

unsigned int dutyCycle = 0;
unsigned int counter = 0;
unsigned int measuredValue = 0;


// PID
//float integral = 0;
int lastError = 0;
int error = 0;
int correction = 0;
int derivative = 0;
// Parameters
constexpr int Kp = 1 * SCALE;
//constexpr float Ki = 1;
constexpr int Kd = 5 * SCALE;



void setup()
{
  Serial.begin(57600);
  Serial.println("----------------------------");
  Serial.println("    Levitation Metronome");
  Serial.println("    S.E. project by M. Leo #931051");
  Serial.println("----------------------------");

  // Setting pins
  pinMode(LEVI_MAGNET, OUTPUT);
  //pinMode(RIGHT_MAGNET, OUTPUT);
  //pinMode(LEFT_MAGNET, OUTPUT);
  //pinMode(AUDIO, OUTPUT);


  //**
  //* analogReadResolution()
  //* Sets the size (in bits) of the value returned by analogRead()
  //* The Due board has 12-bit ADC capabilities that can be accessed
  //* by changing the resolution to 12.
  //* This will return values from analogRead() between 0 and 4095.
  //**
  //analogReadResolution(12);

}

void loop()
{
  // FEEDBACK ---
  // Signal for the Magnet 
  
  
  dutyCycle = ComputeCyclePid();
  
  /*
  if (++counter == 10000)
  {
    counter = 0;
    Serial.print("Read: ");
    Serial.print(measuredValue);
    Serial.print(", Error: ");
    Serial.print(error);
    Serial.print(", Derivate: ");
    Serial.print(derivative);
    Serial.print(", Correction: ");
    Serial.print(correction);
    Serial.print(", Delay HIGH: ");
    Serial.println(dutyCycle);
    Serial.println("");
  }
  */

 
  digitalWrite(LEVI_MAGNET, HIGH);
  
  delayMicroseconds(dutyCycle);

  digitalWrite(LEVI_MAGNET, LOW);

  delayMicroseconds(PERIOD - dutyCycle + 10);
  
}


unsigned int ComputeCyclePid()
{
  int tempDutyCycle;
  
  measuredValue = analogRead(HALL_SENSOR);
  error = (int)targetValue - (int)measuredValue;
  derivative = error - lastError;
  correction = (( -Kp * error ) + (-Kd * derivative)) / (int)SCALE;
  tempDutyCycle = (int)dutyCycle + correction;

  if  (tempDutyCycle < 0)
    tempDutyCycle = 0;
  if  (tempDutyCycle > PERIOD)
    tempDutyCycle = PERIOD;
  
  lastError = error;
  
  return dutyCycle = tempDutyCycle;
}
