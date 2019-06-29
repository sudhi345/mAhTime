#include "config.h"
#include "DischargeBattery.h"

float Vbat = 0, saved = 0;
float last = -1;

AdafruitIO_Feed *analog = io.feed("analog"); //Set up the feed. Replace "analog" in this line with the name of the feed that you created

void setup() {
  //set pinMode
  pinMode(LOAD1,OUTPUT);  //load resistance of 464ohm
  pinMode(LOAD2,OUTPUT);  //load resistance of 232ohm
  pinMode(LOAD3,OUTPUT);  //load resistance of 116ohm
  pinMode(DUT,OUTPUT);       //load is the Device Under Test (DUT)
  pinMode(SW,INPUT_PULLUP);         //push button (active high input)
  pinMode(EN_VOLTAGE_DIVIDER,OUTPUT);  //pin connected to gate of the mosfet which enables the voltage divider
  pinMode(Vin,INPUT);
  
  turnOffAllLoad();   //make sure all loads are off initially
  Wire.begin(SDA_PIN,SCL_PIN);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);            // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {      // Address 0x3C for 128x32
    for(;;);                                            // SSD1306 allocation failed, Don't proceed, loop forever
  }
  display.clearDisplay();                       //clear the display buffer
  display.setRotation(2);                       //rotate 180 degerees
  display.drawBitmap(                           //draw bitmap image
    (display.width()  - logo_width ) / 2,       //align it at the center
    (display.height() - logo_height) / 2,
    logo, logo_width, logo_height, 1);
  display.display();                            //actually display the logo
  delay(2000);
  display.clearDisplay();        //clear the display buffer
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(3,0);
  display.println("    Connecting to   ");
  display.setFont(&FreeSerif9pt7b);
  display.setCursor(0,24);
  display.println("     AdafruitIO    ");
  display.display(); // actually display all of the above
  
  io.connect();   // connect to io.adafruit.com
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }
  display.clearDisplay();
  display.setCursor(25,18);
  display.println("Connected");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setCursor(21,12);
  display.print("Select Load\n");
  display.setFont();    //set default font
  display.setCursor(4,14);
  display.print("Short press: Options\n");
  display.setCursor(10,23);
  display.print("Long press: Select");
  display.display();
  queueInit();     //initialize queue
  loadSelect();           //let the user select load / DUT
  display.setFont(&FreeSans9pt7b);  //change font to Sans
  StartTime = millis();     //save the starting time
}

void loop() {
  _time = millis();
  // io.run(); is required for all sketches. It should always be present at the top of your loop function. It keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  if( !ONCE_DISCHARGED )  
    Vbat = readVoltage();
  if( !(Vbat < VLowerCutOff) )
    enqueue(Vbat);    //enqueue the values
  if( (!(io.status() < AIO_CONNECTED)) && (rear != front) ) {     //connected but queue is not empty
    saved = dequeue();  //read value from the queue
    analog->save(saved); // save the value to the feed
  } 
  if(Vbat < VLowerCutOff)  {   //if the battery Vbat falls below the lower cut off
      ONCE_DISCHARGED = 1;         //clear the ONCE_DISCHARGED so that it does not oscillate when terminal Vbat rises slightly.
      turnOffAllLoad();    //turn OFF ALL loads when batteries got fully discharged.
      EndTime = millis();  //store end time
      DISP_ON = 1; //display that the batteries got discharged
      displayedAt = millis ();  //this is required when display is turned on using DISP_ON
      while(1){
        if( (!(io.status() < AIO_CONNECTED)) && (rear != front) ) {   //connected but queue is not empty
          saved = dequeue();          //read value from the queue
          analog->save(Vbat);         // save the value to the feed
        }
        checkForUpdate();
        yield();  
     }
  }
  
  while( (millis() - _time) < 2000){    //wait for 2 seconds
      checkForUpdate();
      yield();
  }
}