// Arduino Shutter Timer inspired by CAMERADACTYL Cameras this extended and revised version of the code created by Ian Wallace (Instagram & Twitter: @ian_onahillroad) june 2020
// Original CAMERADACTYL hardware construction video  https://youtu.be/UwOh3da_Y8s

volatile long Start = 0; // this is the time in microseconds that the shutter opens (the arduino runs a microsecond clock in the background always - it is reasonably accurate for this purpose)
volatile long Stop = 0;  // this is the time in microseconds that the shutter closes
volatile bool ShutterFired = false; // Communication from Interrupt that the shutter just fired.

void setup() {                                                                 //This part of the program is run exactly once on boot

  Serial.begin(9600);                                                          //opens a serial connection.
  attachInterrupt(digitalPinToInterrupt(2), ShutterChangeDetector, CHANGE);    //run the function ShutterChangeDetector, every time the voltage on pin 2 changes.
  Serial.println("Shutter Timer Programme Started" );                          // Let the user know we started.
  Serial.println();
  ShutterFired = false;

}

void CloseToSpeed(long closeto, long truespeed) {

  // Supporting utility function
  // used to easily compare to common shutter speeds and show error in shutter speed if a close match
  // Only intended to display fractional speeds

  // PARAM closeto      is the denominator of the common speed to check eg.  2  for half a sec 125 for 1/125
  // PARAM truespeed    is the measured speed in **microsconds**

  // show true speeds that are within these brackets of the typical shutter camera speed

  const float LOWERLIMIT = 0.75; // 75%
  const float UPPERLIMIT = 1.25; // 125%

  long closetomicros;

  // sanity check Positive number less than 1 sec

  if ((truespeed < 1000000) && (truespeed > 0) && (closeto > 1 ) && ( closeto < 1000000) ) {

    closetomicros = 1000000 / closeto;  //convert closeto to microseconds

    // is it within limits (typically) 25% of the closeto speed?  - 25% is an arbitrary amount but seems to work quite well

    if ( ((float)truespeed / (float)closetomicros > LOWERLIMIT ) && ((float)truespeed / (float)closetomicros < UPPERLIMIT)) {

      //Its close so print a comparison

      Serial.print("Close to Speed : 1/");
      Serial.print(closeto);
      Serial.print(" Error of " );
      if ((truespeed - closetomicros) > 0) {
        Serial.print("+");
      }
      Serial.print((float)((truespeed - closetomicros)) / 1000);
      Serial.print(" ms " ) ;
      if (truespeed == closetomicros) {
        Serial.println (" Accurate!!! ");
      } else {

        if ((truespeed - closetomicros) > 0) {
          Serial.print("+");
        }
        Serial.print( (((float)(truespeed - closetomicros) * 100) / closetomicros) );
        Serial.println(" %" ) ;
      }
    }

  } else {

    // bad params
    Serial.println("Bad CloseToSpeed function Params");
    Serial.print("closeto : ");
    Serial.println(closeto);
    Serial.print("truespeed : ");
    Serial.println(truespeed);

  }

}

void loop() {

  // this main loop runds continiously looking to see if the ISR flagged that the shutter just fired.
  // most user outputs are presented in miliseconds ms (ms seem esier to understand than microseconds!)

  const float SHOWASSECONDSOVER = 0.7; // display shutter speeds over this number of seconds as a number of second speed not a fractional speed.

  if (ShutterFired ) {

    noInterrupts(); // block new spurious clock inerrupts while we deal with this - we dont want our clock times messed with!

    // we were recording - show the raw data
    Serial.println();
    Serial.print("SHUTTER FIRED!! Timer Start: ");
    Serial.print(Start);
    Serial.print("  Timer Stop: ");
    Serial.println(Stop);
    Serial.println();

    long Speed = (Stop - Start);             // make a variable called speed, which is the total number of microseconds that the shutter is open for
    Serial.print("Duration : ");
    Serial.print(Speed / 1000);             //display total miliseconds in shutter interval
    Serial.print(" ms , ");
    Serial.print(Speed);
    Serial.println(" microsec") ;
    Serial.println();

    float SS = (float)Speed / 1000000;  // make a variable SS, which is how many fractioanl seconds that the shutter open for
    // approximate anything over 700ms to be assumed to be a 1sec Plus exposure attemt and less as a fractional shutter speed
    if (SS > SHOWASSECONDSOVER) {

      Serial.print("Exposure Duration: ");
      Serial.print(SS);
      Serial.print(" Seconds");
      Serial.println();

    } else {

      float SS2 = 1 / SS;                 // make a variable SS2, which is the inverse of the SS, or 1/ the shutter speed
      Serial.print("Shutter Speed: 1/");
      Serial.println(SS2);                //display the shutter speed
      Serial.println();

      // JUST ADD EXTRA CALLS HERE IF YOU NEED A SPEED THAT IS NOT LISTED.

      CloseToSpeed(2, Speed);  // eg. 1/2 a second
      CloseToSpeed(3, Speed);
      CloseToSpeed(4, Speed);
      CloseToSpeed(5, Speed);
      CloseToSpeed(8, Speed);
      CloseToSpeed(10, Speed);
      CloseToSpeed(15, Speed);
      CloseToSpeed(20, Speed);
      CloseToSpeed(25, Speed);
      CloseToSpeed(30, Speed);
      CloseToSpeed(45, Speed);
      CloseToSpeed(60, Speed);
      CloseToSpeed(100, Speed);
      CloseToSpeed(125, Speed);// eg. 1/125 a second
      CloseToSpeed(150, Speed);
      CloseToSpeed(200, Speed);
      CloseToSpeed(250, Speed);
      CloseToSpeed(300, Speed);
      CloseToSpeed(500, Speed);
      CloseToSpeed(750, Speed);
      CloseToSpeed(1000, Speed);
      CloseToSpeed(2000, Speed);
      CloseToSpeed(3000, Speed);
      CloseToSpeed(4000, Speed);
      CloseToSpeed(5000, Speed);
    }

    Serial.println();

    // reset
    Start = 0;                         // reset Start to 0
    Stop = 0;                           //reset Stop to 0
    ShutterFired = false;

    interrupts();  // read to take the next oone

  }

}

void ShutterChangeDetector() {

  // this is the interrupt function, which is called everytime the voltage on pin 2 changes - this follows the principle of minimising work in the ISR
  // the only job of this ISR is to detect the shutter firing and collect the start end times.

  ShutterFired = false; // assume not fired

  if (digitalRead(2) == HIGH && Start == 0) {

    // shutter opened - we just start a new timing
    Start = micros();       //set the variable Start to current microseconds

  } else if ((digitalRead(2) == LOW ) && (Start != 0)) {

    // Low - the shutter just closed
    Stop = micros();      // set the variable Stop to current microseconds

    if (Stop > Start) {

      ShutterFired = true;

    } else {

      //Audrino timer resets to 0 every 70 mins so these can get muddled/
      Start = 0;                         // reset Start to 0
      Stop = 0;                           //reset Stop to 0

    }

  } else {

    // a bit unexpected - starting stopping perhaps b- just reset
    Start = 0;                         // reset Start to 0
    Stop = 0;                           //reset Stop to 0

  }

}
