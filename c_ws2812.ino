// TODO.... 
// 1.  fix MQTT instructions... currently does not work through opState... and extra effects don't work... must integrate MODEDROP....
// 2.  consolidate text and order of effects
// 3.  Auto enable MQTT if IP is entered on wifi config. 
// 4.  
// 5.  nointurrup handles for UDP parce... maybe... 
// 6. 
// 7. rework effects to use animations class
// 8.  
// 9   

// EEPROM ALLOCATIONS:

// PixelCount_address + 0 =  
// PixelCount_address + 1 =  
// PixelCount_address + 2 =  
// PixelCount_address + 3 =  LastOpState




//int spectrumValue[7];
//int filter=80;


// what!
uint16_t 
var1 = 0,var2 = 0,var3 = 0,var4 = 0,var5 = 0,
var6 = 0,var7 = 0,var8 = 0,var9 = 0,var10 = 0;

static const char *VAR_STRING[] = {
"Ceiling           ", // var 1
"Floor             ", // var 2
"Var3              ", // var 3
"Var4              ", // var 4
"Var5              ", // var 5
"Effect Option     ", // var 6
"Total_X           ", // var 7
"Number of effects ", // var 8
"Var9              ", // var 9
"Size of effect    "};// var 10




void  cache handle_WS2812 () { // handles the web commands...


String buf, paused_string, selected; 


static int power = 0; 

bool updateLEDs = false;


 if (server.arg("mode").length() != 0) WS2812_mode_string(server.arg("mode"));
 if ((server.arg("dim") != String(CurrentBrightness)) && (server.arg("dim").length() != 0)) WS2812_dim_string(server.arg("dim"));
 if ((server.arg("timer") != String(WS2812interval)) && (server.arg("timer").length() != 0)) WS2812timer_command_string(server.arg("timer"));
 if ((server.arg("anispeed") != String(CurrentAnimationSpeed)) && (server.arg("anispeed").length() != 0))  Animationspped_command_string(server.arg("anispeed"));
 if (server.arg("paused").length() != 0) {
      paused = (server.arg("paused")).toInt();
      if (paused) animator->Pause(); 
      if (!paused) animator->Resume(); 
    }

 if (server.arg("rgbpicker").length() != 0)  { 
    //WS2812_mode_string("rgb-" + server.arg("rgbpicker"));

    WS2812_Set_New_Colour(server.arg("rgbpicker"));

    Serial.println("RGB picker command: " + server.arg("rgbpicker"));
  }

  //if (server.arg("command").length() != 0) WS2812_command_string(server.arg("command"));

  if (server.arg("leds").length() != 0) {
    pixelCount = server.arg("leds").toInt();
    updateLEDs = true;
  }

   if (server.arg("ledpin").length() != 0) {
    pixelPIN = server.arg("ledpin").toInt();
    updateLEDs = true;
  }

  if ((server.arg("modedrop").length() != 0)) 
    {

      WS2812_mode_string(server.arg("modedrop"));

      }

      //----  having this under here works better as the page gets updated before the request data is fired back!

      if (paused) { 
        paused_string = F("<a href='/ws2812?paused=0'>PLAY</a> ") ; 
      } else {
        paused_string = F("<a href='/ws2812?paused=1'>PAUSE</a>") ; 
      }

String content3 = F("\
<!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'><title> % </title></head><body><h1> % </h1>\
%\
<br> <a href='/ws2812?mode=off'>OFF</a> | <a href='/ws2812?mode=on'>ON</a>  | % | <a href='/ws2812?mode=refresh'>REFRESH</a> | <a href='/lightsconfig'>CONFIG</a>\
<form name=frmTest action='/ws2812' method='POST'>\
Select Mode <select name='modedrop' onchange='this.form.submit();'>\
");

  buf = insertvariable ( content3, String(deviceid));

  buf = insertvariable ( buf, String(deviceid));
  
  if (WiFi.status() == WL_CONNECTED) 
    { buf = insertvariable ( buf, F("<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>"));
    } else { buf = insertvariable ( buf, " "); } ;

    buf = insertvariable ( buf, paused_string);


//  if (wifimode == 1) { buf += "<script type='text/javascript' src='http://jscolor.com/jscolor/jscolor.js'></script>"; } ; // This is needed as it pulls in stuff from internet... when in AP mode causes crash.  
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();

   server.sendContent(buf);

 buf = " ";


for (int k=0; k < numberofmodes; k++ ) {
    if (HoldingOpState == k) { 
        selected = "' selected "; 
      } else selected = "' "; 
  buf += "<option value='" + String(k) + selected + ">" + String(k) + ". " + MODE_STRING[k] + "</option>";
   // httpbuf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";

  }

  buf += "</select>";
  buf += "</form></p>";


    server.sendContent(buf);

    buf = " "; 

  if(WiFi.status() == WL_CONNECTED) {
  String content0 = F("<p><form action='/ws2812' method='POST'\
  <p>Color: <input class='color' name='rgbpicker' value = '%' >\
  <br>  <input type='submit' value='Submit'/>\
  </form>\
  ");

  buf = insertvariable ( content0, WebRGBcolour); 
  
  server.sendContent(buf);

  }  // This is needed as it pulls in stuff from internet... when in AP mode causes crash.  
  

 // 1 = CurrentAnimationSpeed String(CurrentAnimationSpeed) 
 // 2 = CurrentBrightness String(CurrentBrightness)
 // 3 = WS2812interval String(WS2812interval)
 // 4 = pixelCount String(pixelCount)
 // 5 = pixelPIN String(pixelPIN)
 // 6 = String(power) 
  
  String content1 = F("\
  <form name=sliders action='/ws2812' method='POST'>\
  <br>Animation: <input type='range' name='anispeed'min='0' max='10000' value='%' onchange='this.form.submit();' >\
  <br>Brightness: <input type='range' name='dim'min='0' max='255' value='%' onchange='this.form.submit();' >\
  <br>Timer: <input type='range' name='timer'min='0' max='2000' value='%' onchange='this.form.submit();'>\
  </form>\
  <p><form action='/ws2812' method='POST'\
  <form action='/ws2812' method='POST'>\
  <p>LEDs: <input type='text' id='leds' name='leds' value='%' >\
  <br>PIN: <input type='text' id='ledpin' name='ledpin' value='%' >\
  <br>  <input type='submit' value='Submit'/>\
  </form>\
  Power = %mA\
  <br> Adalight order: GRB\
  ");
  
  buf = insertvariable ( content1, String(CurrentAnimationSpeed)); 
  buf = insertvariable ( buf, String(CurrentBrightness)); 
  buf = insertvariable ( buf, String(WS2812interval)); 
  buf = insertvariable ( buf, String(pixelCount)); 
  buf = insertvariable ( buf, String(pixelPIN)); 
  buf = insertvariable ( buf, String(power) ); 

  buf += htmlendstring; 
 
  //server.send(200, "text/html", buf);


    server.sendContent(buf);



if (updateLEDs) { initiateWS2812(); updateLEDs = false;};
//power = getPixelPower();


}

void cache Animationspped_command_string (String Value) {

  uint16_t newvalue = Value.toInt();

  CurrentAnimationSpeed = newvalue; 
  lasteffectupdate = 0; 
}



// Takes MODE selected by number, changes OpState, SAVES NEW MODE TO EEPROM, incase of reboot....
void cache WS2812_mode_number(String Value) {

      lasteffectupdate = 0;
      uint8_t chosen_mode = Value.toInt();
      Serial.println("MODE DROP recieved: " + Value);
      opState = (operatingState)chosen_mode;
      if (chosen_mode != 0) LastOpState = (operatingState)chosen_mode;
      EEPROM.write(PixelCount_address + 3, LastOpState);
      EEPROM.commit();

}

//  might need to add an effect function timeout thing....  to reset effect and call update with NEW dim setting... 


void  cache  WS2812_dim_string (String Value)
{
      lasteffectupdate = 0; // RESET EFFECT COUNTER, force refresh of effects....
      int a = Value.toInt();
      if (a > 255) a = 255;
      if (a < 0) a = 0;

      CurrentBrightness  = a;

      EEPROM.write(PixelCount_address + 2, CurrentBrightness);
      EEPROM.commit();
     
      // if (isnan(CurrentBrightness)) CurrentBrightness = 255;

      Serial.println("Brightness set to: " + String(CurrentBrightness));

      send_mqtt_msg("brightness", String(CurrentBrightness));

}


void cache WS2812_effect_string (String request) {

      for (uint8_t i = 0; i < numberofmodes; i++ ) {

        if (request == String(MODE_STRING[i])) {

                // uint8_t chosen_mode = Value.toInt(); // turn it to number...
        HoldingOpState = (operatingState)i; // assign selection to holdingopstate... 
        Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
        LastOpState = (operatingState)i;
        Serial.print("MQTT MODE MATCH FOUND :"); 
        Serial.println(MODE_STRING[i]); 
        break; 

        }
      } 

   if (HoldingOpState == OFF) { 
    send_mqtt_msg("mode","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }


send_mqtt_msg("effect", MODE_STRING[HoldingOpState]); 



}

void  cache WS2812_mode_string (String Value)

{

  lasteffectupdate = 0 ; // RESET EFFECT COUNTER
  //Random_func_timeout = 0; //RESET additionall timeout... 
  if (paused) paused = false; // this sets it back to play, if paused when a mode change occurs...

  if (Value.toInt() != 0) {  // if the numerical mode does not equal 0 which is off....

      uint8_t chosen_mode = Value.toInt(); // turn it to number...
      HoldingOpState = (operatingState)chosen_mode; // assign selection to holdingopstate... 
      Current_Effect_State = POST_EFFECT; // This is required to trigger the holding op state to opstate...
      LastOpState = (operatingState)chosen_mode;

  } else {


  Value.toLowerCase();

  if (Value == "off" | Value == "OFF") { 
      if (opState != OFF) { 
          Current_Effect_State = POST_EFFECT; //  Set this to TERMINATE current effect.... 
          HoldingOpState = OFF; 
      } 
  }

  if (Value == "on" | Value == "ON") { HoldingOpState = LastOpState; Current_Effect_State = POST_EFFECT; } ; 

  if (Value == "refresh" ) { 
    lasteffectupdate = 0;  
  }

  if (Value == "pause") paused = true;
  if (Value == "play") paused = false; 

}

   if (HoldingOpState == OFF) { 
    send_mqtt_msg("mode","off");
  } else { 
    send_mqtt_msg("mode", "on");
  }

  EEPROM.write(PixelCount_address + 3, LastOpState);
  EEPROM.commit();

send_mqtt_msg("effect", MODE_STRING[HoldingOpState]); 

}


void cache WS2812_Set_New_Colour (String instruction) {

      //opState = LastOpState = COLOR; //  this allows you to pick colour base for other MODES.... 
      //String instruction = Value.substring(4,Value.length()+1 );
      //Serial.println("/n RGB command recieved: " + instruction);
      lasteffectupdate = 0; 
      WebRGBcolour = instruction;
      NewColour = HEXtoRGB(instruction);
      EEPROM.write(PixelCount_address + 4, NewColour.R);
      EEPROM.write(PixelCount_address + 5, NewColour.G);
      EEPROM.write(PixelCount_address + 6, NewColour.B);
      EEPROM.commit();

      //Serial.print("NEW RGB COLOUR..");
      //Serial.print(NewColour.R,HEX); 
      //Serial.print(" "); 
      //Serial.print(NewColour.G,HEX); 
      //Serial.print(" ");       
      //Serial.println(NewColour.B,HEX); 
  
      send_mqtt_msg("colour", WebRGBcolour); 
/*
#ifdef HSL_FLOAT

              float originalHUE = (HslColor(NewColour)).H;
              float originalSAT = (HslColor(NewColour)).S;
              float originalLIG = (HslColor(NewColour)).L;
              Serial.print("HSL components (float): ");
              Serial.print(originalHUE);
              Serial.print(",");
              Serial.print(originalSAT);
              Serial.print(",");
              Serial.println(originalLIG);
#else
              uint8_t originalHUE = (HslColor(NewColour)).H;
              uint8_t originalSAT = (HslColor(NewColour)).S;
              uint8_t originalLIG = (HslColor(NewColour)).L;
              Serial.print("HSL components (int): ");
              Serial.print(originalHUE);
              Serial.print(",");
              Serial.print(originalSAT);
              Serial.print(",");
              Serial.println(originalLIG);             

#endif

              uint8_t originalR = NewColour.R;
              uint8_t originalG = NewColour.G;
              uint8_t originalB = NewColour.B;
              Serial.print("RGB components: ");
              Serial.print(originalR);
              Serial.print(",");
              Serial.print(originalG);
              Serial.print(",");
              Serial.println(originalB);  
*/

}


void  cache WS2812timer_command_string (String Value)

{

lasteffectupdate = 0;

WS2812interval = Value.toInt();

send_mqtt_msg("timer", Value); 
}


//void  WS2812_command_string (String Value) {
//}
/*

if (Value.indexOf("rgb") >= 0) 
{
  opState = COLOR;
  String instruction = Value.substring(4,Value.length()+1 );
  Serial.println("RGB command recieved: " + instruction);
  NewColour = HEXtoRGB(instruction);
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {
      //strip->LinearFadePixelColor(1000, pixel, NewColour);
    }
}

} */


RgbColor cache dim(RgbColor original) {

dim (original, CurrentBrightness); 

}


RgbColor cache dim(RgbColor original, uint8_t brightness) {

  //  int amounttodarken = 255 - CurrentBrightness;
  //  value.Darken(amounttodarken);
  //  return value;

   // if (CurrentBrightness == 255) return original; 
             // Serial.print("Original HSL = ");
           // Serial.print((HslColor(original)).L); 

/*  Serial.print("IN: RGB(");
  Serial.print(original.R);
  Serial.print(",");
  Serial.print(original.G);
  Serial.print(",");
  Serial.print(original.B);
  Serial.print(") = ");
*/

#ifdef HSL_FLOAT

     // RgbColor newvalue = dimbyhsv(value, (byte) CurrentBrightness);
              float originalHUE = HslColor(original).H;
              float originalSAT = HslColor(original).S;
              float originalLIG = HslColor(original).L;


    float newLIG =  originalLIG   * ( (float)brightness / 255.0f ) ; 
/*
  Serial.print(" HSL(");
  Serial.print(originalHUE);
  Serial.print(",");
  Serial.print(originalSAT);
  Serial.print(",");
  Serial.print(originalLIG);
  Serial.print(")");

  Serial.print(" --> ");

  Serial.print(" HSL(");
  Serial.print(originalHUE);
  Serial.print(",");
  Serial.print(originalSAT);
  Serial.print(",");
  Serial.print(newLIG);
  Serial.print(")");
*/

#else

              uint8_t originalHUE = (HslColor(original)).H;
              uint8_t originalSAT = (HslColor(original)).S;
              uint8_t originalLIG = (HslColor(original)).L;


    uint8_t newLIG = (uint8_t)  ( HslColor(original).L * ( (float)  brightness / 255.0f )); 
    //HslColor updatedColor = HslColor( HslColor(original).H, HslColor(original).S, newLIG ) );
 

#endif


 return RgbColor( HslColor(originalHUE, originalSAT, newLIG )  );

 // Serial.print(" = RGB(");
 // Serial.print(updatedColor.R);
 // Serial.print(",");
 // Serial.print(updatedColor.G);
 // Serial.print(",");
 // Serial.print(updatedColor.B);
 // Serial.println(")");

  //  Serial.print("HSL = ");
  //  Serial.print((HslColor(original)).L);
  //  Serial.print(" -- ");
  //  Serial.print(brightness);
  //  Serial.print("(");
  //  float bright = (float) (brightness)  / 255.0f ; 
  //  Serial.print(bright);
  //  Serial.print(") --> ");
  //  Serial.println(newLIG);


//return(updatedColor);
}




void cache SetRGBcolour (RgbColor value) {

SetRGBcolour(value,CurrentAnimationSpeed); 

}

void cache SetRGBcolour (RgbColor value, uint16_t speed) {


    //long  now1 = millis(); 
    //long interval = now1 - lasteffectupdate; 
    
  switch(Current_Effect_State) {


    case PRE_EFFECT:
    Pre_effect(); 
    break;
    case RUN_EFFECT:

      if (millis() - lasteffectupdate > 3000) {
          //animator->FadeTo(speed,dim(RgbColor(value))); 
          fade_to(dim(value), speed, RGB);
          }

      lasteffectupdate = millis(); 

    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }


}

RgbColor cache HEXtoRGB (String hexString) // Converts HEX to RBG object....

{

  uint32_t decValue = 0;
  int nextInt = 0;
  
  for (int i = 0; i < hexString.length(); i++) {
    
    nextInt = int(hexString.charAt(i));
    if (nextInt >= 48 && nextInt <= 57) nextInt = map(nextInt, 48, 57, 0, 9);
    if (nextInt >= 65 && nextInt <= 70) nextInt = map(nextInt, 65, 70, 10, 15);
    if (nextInt >= 97 && nextInt <= 102) nextInt = map(nextInt, 97, 102, 10, 15);
    
    nextInt = constrain(nextInt, 0, 15);
    
    decValue = (decValue * 16) + nextInt;
  }
  
    int r = decValue >> 16;
    int g = decValue >> 8 & 0xFF;
    int b = decValue & 0xFF;

    return RgbColor(r,g,b);

}


String cache RGBtoHEX (RgbColor value) {

  //char R[3]
 //sprintf( R, "%X", value );





}

void cache Pre_effect() {

Current_Effect_State = RUN_EFFECT; 
lasteffectupdate = 0; 
Random_func_timeout = 0; //RESET additionall timeout... 


}

void cache Post_effect() {

Current_Effect_State = PRE_EFFECT; 
opState = HoldingOpState; 

}

void cache StripOFF() {


  switch(Current_Effect_State) {

    case PRE_EFFECT:
    Pre_effect(); 

//
//      if (millis() - lasteffectupdate > 3000) {
//          animator->FadeTo(2000,RgbColor(0,0,0)); 
//          }
//      lasteffectupdate = millis(); 

      //if (millis() - lasteffectupdate > 3000) {
        for (uint8_t n = 0; n < pixelCount; n++)
            {
              RgbColor original = strip->GetPixelColor(n);

#ifdef HSL_FLOAT

              float originalHUE = (HslColor(original)).H;
              float originalSAT = (HslColor(original)).S;
              float originalLIG = (HslColor(original)).L;
#endif


        AnimUpdateCallback animUpdate = [=](float progress)
        {

#ifdef HSL_FLOAT
            // float originalHSL = HslColor(original).L; 
            //float NewLIG = originalLIG  * (1.0 - progress); 
            //float inverseprogress = (1.0 - progress); 
            //RgbColor updatedColor = RgbColor(HslColor(originalHUE , originalSAT, NewLIG  ))  ;  

   // WORKING         float NewLIG = ( (float)(HslColor(original)).L - ((float)(HslColor(original)).L  * progress) ); 
            float NewLIG = HslColor(original).L - (HslColor(original).L  * progress) ; 

            RgbColor updatedColor = RgbColor(HslColor( (HslColor(original)).H, (HslColor(original)).S, NewLIG  ))  ; 

#else

            uint8_t NewLIG = (uint8_t) ( HslColor(original).L - (HslColor(original).L  * progress) ); 
            RgbColor updatedColor = RgbColor(HslColor( HslColor(original).H, HslColor(original).S, NewLIG  ))  ;    

#endif

           // if (n == 0) { 
           // Serial.print("Original HSL = ");
           // Serial.print((HslColor(original)).L); 
           // Serial.print(": ");
           // Serial.print(progress);
            //Serial.print(" -- ");
            //Serial.print(inverseprogress);
           // Serial.print(" --> ");
           // Serial.println(NewLIG);



           // } // end of if...


            //Serial.print(progress);
            //Serial.print(" --> ");
            //Serial.println(valuepassed);
            strip->SetPixelColor(n, updatedColor);
        };

        animator->StartAnimation(n, 2000, animUpdate);
    }
    //}

      // lasteffectupdate = millis(); 
    break;

    case RUN_EFFECT:

    break;

    case POST_EFFECT:
    Post_effect(); 
    break;
  }

}



void cache initiateWS2812 ()

{
  opState = OFF;
  ChangeNeoPixels(pixelCount, pixelPIN); // initial setup
  strip->Begin();
  StripOFF();
  SetRandomSeed();
  animator->Resume(); 



}



void cache ws2812 ()  // put switch cases here...


{
static unsigned long update_strip_time = 0;  //  keeps track of pixel refresh rate... limits updates to 33 Hrz.....

  //Serial.print("ws2812 called case = " + String(opState));
  //wsPoint += 1;

switch (opState)
   {
   case OFF:
      //if (strip) StripOFF();
   StripOFF();
      break;
   case RAINBOW:
      rainbow();
      break;
   case COLOR:
      SetRGBcolour(NewColour);
      break;
   case ChaseRainbow:
      theatreChaseRainbow();
      break;
   case FADE:
      Fade();
      break;
   case ADALIGHT:
      Adalight();
      break;
   case TEST:
      test();
      break;
   case LOOPAROUND:
      LoopAround(192, 200);
      break;
   case PICKRANDOM:
      PickRandom(128);
      break;
   case FADEINFADEOUT:
      FadeInFadeOutRinseRepeat(192);
      break;
   case COOLBLOBS:
      CoolBlobs();
      break;    
    case UDP:
      UDPfunc();
      break;
    case RAINBOWCYCLE:
      Rainbowcycle();
      break;
    case SPIRAL:
      spiral();
      break;
    case SQUARES2:
      Squares2(0);
      break;
    case SQUARESRANDOM:
      Squares2(1);
      break;
    case TEST4:
      test4();
      break;
    case SQUARES:
      Squares();
      break;
    case EQ1:
      eq1();
      break;
    case RANDOM:
      Random_colour();
      break;
    case RANDOMFUNC:
      Random_function();
      break;
    //case ARTNET:
    //  Art_Net_func ();
    //  break;
    case RANDOM_TOP_BOTTOM:
      Effect_Top_Bottom(TWOCOLOUR,HSL);
      break;
    case RANDOM_TOP_BOTTOM_LINEAR:
      Effect_Top_Bottom(TWOCOLOUR, RGB);
      break;    
    case SINGLE_COLOUR_FADE:
      Effect_Top_Bottom(SINGLECOLOUR, HSL);
      break;  
    case RANDOM_COLOUR_FADE:
      Effect_Top_Bottom(RANDOMSINGLECOLOUR,HSL);
      break;      
    case HSICYCLE:
      HSI_Cycle();
      break;
    case NEWANIMATIONS:
      test_newanimations();
      break;
      
   }


if (millis() - update_strip_time > 30) {

    if ( animator->IsAnimating() ) animator->UpdateAnimations(100); 

    strip->Show();

    update_strip_time = millis();

} // end of switch case...

/*
static bool ani_update = false;

if (!ani_update && strip->IsAnimating()) {
  Serial.print("Animating...");
  ani_update = true;
} else if (ani_update && !strip->IsAnimating()) {
  Serial.println("Stopped");
  ani_update = false;
}
*/


if (paused) lasteffectupdate = millis(); 


} // end of ws2812 function 


void cache HSI_Cycle() {


static int j;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

if (millis() > (lasteffectupdate) ){

  //Serial.println("Fade updaed");

if (j > 360) j = 0; 

double j_double = (double)j / 360; 
double v_double = (double)CurrentBrightness / 255; 
double s_double; 

//RgbColor col = hsvToRgb(j_double,1,v_double);

/*  Serial.println();
  Serial.print(j_double);
  Serial.print("-->");
  Serial.print(" R:");
  Serial.print(col.R);
  Serial.print(" G:");
  Serial.print(col.G);
  Serial.print(" B:");
  Serial.print(col.B);  
*/

//int col = 200; 

for (int i=0; i < pixelCount; i++) {

double s_double = (double)i / pixelCount; 


RgbColor col = hsvToRgb(j_double,s_double,v_double);

    strip->SetPixelColor(i, col);   
        }

lasteffectupdate = millis() + WS2812interval ;

j++; 

}

//  END of EFFECT  

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

}




void cache Random_function() {
/*
static uint8_t random_choice = 0 ; 
static uint32_t Random_func_next_time = 0;
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


if (millis() > Random_func_next_time + 1000) Random_func_next_time = 0; // This resets things if its been turned off after 1s... 

    if (millis() > Random_func_next_time) {

    random_choice = random(0, 5);

    Random_func_next_time = millis() + random(WS2812interval*30, (WS2812interval * 300) ); 

    }


    if (random_choice == 0) rainbow();
    if (random_choice == 1) spiral();
    if (random_choice == 2) Rainbowcycle();
    if (random_choice == 3) Random_colour();
    if (random_choice == 4) Squares2(1);
    if (random_choice == 5) Random_Top_Bottom(0);



if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache Random_colour() {

 //long Random_func_timeout = 0, Random_func_lasttime = 0; 
static uint8_t current_r = 0;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

    if (millis() > lasteffectupdate) {
      //Serial.print("New random choice..."); 

      uint16_t random_animation_speed = random(2000, 20000);

      //RgbColor random_colour_random = RgbColor(random(255),random(255),random(255));  // generates lots of white colours... not balenced..
      RgbColor random_colour_random = Wheel(random(255));

      SetRGBcolour(random_colour_random,random_animation_speed);

      //Random_func_lasttime = millis(); 
      long Random_func_timeout = random(60000, 60000*5);
      //Random_func_timeout = random(10000, 60000);
      lasteffectupdate = millis() + Random_func_timeout; 
    }




if (Current_Effect_State == POST_EFFECT) Post_effect(); 

} // end of random func


void cache CoolBlobs() {




if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
    if (millis() > (lasteffectupdate ) ){
      




      lasteffectupdate = millis() + WS2812interval; 
    } // end of timer if


if (Current_Effect_State == POST_EFFECT) Post_effect(); 

} // end of coolblobs


//////// i nserted
/*

void setcolour () {

    for (uint8_t pixel = 0; pixel < pixelCount; pixel++) {

      //strip->LinearFadePixelColor(5000, pixel, NewColour);

    }

}

*/

void cache  FadeInFadeOutRinseRepeat(uint8_t peak) {

  /*
  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (effectPosition == 0)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(800,100);
      strip->LinearFadePixelColor(time, pixel, RgbColor(random(peak), random(peak), random(peak)));
    }
  }
  else if (effectPosition == 1)
  {
    for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
    {
      uint16_t time = random(600,700);
      strip->LinearFadePixelColor(time, pixel, RgbColor(0, 0, 0));
    }
  }
  effectPosition = (effectPosition + 1) % 2; // next effectPosition and keep within the number of effectPositions
  
if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache PickRandom(uint8_t peak)
{

  /*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
  // pick random set of pixels to animate
  uint8_t count = random(pixelCount);
  while (count > 0)
  {
    uint8_t pixel = random(pixelCount);
    
    // configure the animations
    RgbColor color = Wheel(random(255)); // = //strip->getPixelColor(pixel);

    //color = RgbColor(random(peak), random(peak), random(peak));

    
    uint16_t time = random(100,400);
    strip->LinearFadePixelColor( time, pixel, color);
    
    count--;
  }

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache LoopAround(uint8_t peak, uint16_t speed)
{
  /*
  // Looping around the ring sample
  uint16_t prevPixel;
  RgbColor prevColor;
  
  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 5)) % pixelCount; 
  strip->LinearFadePixelColor(speed, prevPixel, RgbColor(0, 0, 0));
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 4)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 3)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 2)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade previous one dark
  prevPixel = (effectPosition + (pixelCount - 1)) % pixelCount; 
  prevColor = strip->GetPixelColor( prevPixel );
  prevColor.Darken(prevColor.CalculateBrightness() / 2);
  strip->LinearFadePixelColor(speed, prevPixel, prevColor);
  
  // fade current one light
  strip->LinearFadePixelColor(speed, effectPosition, RgbColor(random(peak), random(peak), random(peak)));
  effectPosition = (effectPosition + 1) % pixelCount;
  

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache SetRandomSeed()
{
  uint32_t seed;
  
  // random works best with a seed that can use 31 bits
  // analogRead on a unconnected pin tends toward less than four bits
  seed = analogRead(0);
  delay(1);
  
  for (int shifts = 3; shifts < 31; shifts += 3)
  {
    seed ^= analogRead(0) << shifts;
    delay(1);
  }
  
  // Serial.println(seed);
  randomSeed(seed);
}


RgbColor cache Wheel (byte WheelPos) {

  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return  RgbColor(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
   WheelPos -= 85;    
   return RgbColor(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return  RgbColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
} 


void cache Fade() {

switch(Current_Effect_State) {
    case PRE_EFFECT:
    effectPosition = 0; 
    Pre_effect(); 
    for (uint16_t i = 0; i < pixelCount; i++)
            {
              RgbColor original = strip->GetPixelColor(i);
        RgbColor col = Wheel(effectPosition++);
        AnimUpdateCallback animUpdate = [=](float progress)
        {
           RgbColor updatedColor = RgbColor::LinearBlend(original, dim(col) ,  progress) ;
            strip->SetPixelColor(i, updatedColor);
        };
        animator->StartAnimation(i, 2000, animUpdate);
        lasteffectupdate = millis();  
    }
    break;
    case RUN_EFFECT:  
    if (animator->IsAnimating()) { break; } ; //  This line stops the effect from running if it is still in the warm up! 
      if (millis() - lasteffectupdate > WS2812interval) { 
            RgbColor col = Wheel(effectPosition++);
            for (uint16_t i=0; i < pixelCount; i++) {
                strip->SetPixelColor(i, dim(col));    
              }
          if (effectPosition==256) effectPosition=0; 
          lasteffectupdate = millis(); 
          }
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
}



void  cache Rainbowcycle() {
  switch(Current_Effect_State) {
    case PRE_EFFECT:
    effectPosition = 0; 
    Pre_effect(); 
    for (uint16_t i = 0; i < pixelCount; i++)
            {
              RgbColor original = strip->GetPixelColor(i);
        
        AnimUpdateCallback animUpdate = [=](float progress)
        {
           RgbColor updatedColor = RgbColor::LinearBlend(original, dim(Wheel(i * 256 / pixelCount + effectPosition)) ,  progress) ;
            strip->SetPixelColor(i, updatedColor);
        };
        animator->StartAnimation(i, 2000, animUpdate);
        lasteffectupdate = millis();  
    effectPosition++; 
    }
    break;
    case RUN_EFFECT:  
    if (animator->IsAnimating()) { break; }  ; //  This line stops the effect from running if it is still in the warm up! 
      if (millis() - lasteffectupdate > WS2812interval) {      
           for(uint16_t i=0; i< pixelCount; i++) {
                strip->SetPixelColor(i, dim(Wheel(i * 256 / pixelCount + effectPosition)));
            }
          if (effectPosition==256*5) effectPosition=0; 
          lasteffectupdate = millis(); 
          effectPosition++;
          }
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }
} // END OF RAINBOW CYCLE







void cache test4() {

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

//Serial.println("TEST4");
  static int wsPoint = 0;
  RgbColor color1 = Wheel(100);
  RgbColor color2 = Wheel(200);

if (wsPoint == 255) wsPoint = 0; 
 if (millis() > (lasteffectupdate ) ){





HsvFADErgb(color1,color2,wsPoint); 



lasteffectupdate = millis() + WS2812interval ;
wsPoint++; 
}

if (Current_Effect_State == POST_EFFECT) Post_effect(); 
}

void  cache test() {
switch(Current_Effect_State) {

    case PRE_EFFECT:
      Pre_effect();
        for (uint8_t i = 0; i < 1; i++ ) {
       
           RgbColor originalColor = RgbColor(0,0,0); 
           RgbColor color = RgbColor(255,0,0);
            // define the effect to apply, in this case linear blend

            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor;
                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                updatedColor = RgbColor::LinearBlend(originalColor, color, new_progress);
                strip->SetPixelColor(0, updatedColor);
            };

            animator->StartAnimation(0, 5000, animUpdate);

}

    break;
    case RUN_EFFECT:  
  
    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }






} // end of test 4

 void cache rainbow() {

  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  
// Var 1 = timer interval
// Var 2 = wsPoint Min
// Var 3 = wsPoint Max
// Var 4 = wsPoint step....  
// Var 5 = pixel jump?  ie... instead of i++, do i =+ var 3... ? might need to turn of previous pixel
// Var 6 = wheel multipler... or wheel map..  (wheel takes value.. returns color...)
// try to constrain wspoint which is color generator... 
 if(var3 == 0) var3 = 256; // MAKE sure that there is a +ve end point for the colour wheel... else wsPoint = var3;

 if (millis() > (lasteffectupdate) ){

  static int wsPoint =0;
  //uint16_t i; // , j;
  //pixelsNUM = 60;
  //for(j=0; j<256; j++) { v
    for(int i=0; i<pixelCount; i++) {
    //RgbColor tempcolour = Wheel(i+wsPoint);
    strip->SetPixelColor(i, dim(Wheel(i+wsPoint)));
    }
    
    if (wsPoint==var3) wsPoint=var2; 
    wsPoint++;
    lasteffectupdate = millis() + WS2812interval ;
}
    // Serial.println("Colours Updated..." + String(//strip->numPixels()));

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

}   // END OF RAINBOW


/*
void cache clearpixels() {

    memset(pixelsPOINT, 0, 3 * strip->PixelCount() );   // clear current without... 


}
*/
void cache spiral() {
/*
static uint16_t currentcolor = 0;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (millis() > (lasteffectupdate ) ) 
  { // effect update timer
  uint8_t pitch = 0 ;
  
  if (var7 == 0 ) pitch = 13; else pitch = var7;
  uint8_t total_y = return_total_y(pitch); // get the number of rows.  rounds up...
  uint8_t x,y;

 //clearpixels();

    for (x = 0; x < pitch; x+=2) {
      RgbColor colour = Wheel(( x * 256 / pitch + currentcolor) ); // i * 256 / pixelCount + wsPoint) 
      for (y = 0; y < total_y; y++) {
      strip->SetPixelColor(return_pixel( x, y, pitch), dim(colour));
      }
    }
  

  if (currentcolor == 256) currentcolor = 0; else currentcolor++ ;// cycle through to next colour
  lasteffectupdate = millis() + WS2812interval ;

  } // effect update timer

if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}
/*
// FACES ALGO....
void cache test3 () {

  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 5;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {

      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {

      RgbColor colour = RgbColor(random(255),random(255),random(255));

      if (square_size == 10) colour.Darken(random(0,200));

      if (square_size == 10) square_size = random(3,7);


      uint8_t x_rand = random(0, total_x - square_size ) ; 
      uint8_t y_rand = random(0, total_y - square_size ) ;

         


      // strip->LinearFadePixelColor(time, return_shape_square(x_rand, y_rand, 4, square_size, total_y ) , colour);


        

        //Serial.println();

      for (int j =0; j < (square_size * square_size) ; j++) {


        //Serial.print("GRID " + String (square_size * square_size) + "... ");

        int pixel1 = return_shape_face(x_rand, y_rand, j, square_size, total_x ); 

        //Serial.print(pixel1);
        //Serial.print(" ");

        if (pixel1 <= pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));
      
      }


    }

    //Serial.println("------------------");

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }




if (Current_Effect_State == POST_EFFECT) Post_effect(); 
} // end of test

*/


// Working 

void cache fade_to(RgbColor Colour) {
fade_to(Colour,2000,RGB); 
}

void cache fade_to(RgbColor Colour, uint16_t time ) {
fade_to(Colour,time,RGB); 
}

//void cache testblendmethod(BlendMethod(test) ) {


//}


void cache fade_to(RgbColor NewColour, uint16_t time, BlendMethod method ) {
    
     for (uint16_t i = 0; i < pixelCount; i++)
            
            {

        RgbColor original = strip->GetPixelColor(i);
        // NewColour = dim(NewColour);

        AnimUpdateCallback animUpdate = [=](float progress)
        {
          if (method == RGB) { 
            RgbColor updatedColor = RgbColor::LinearBlend(original, NewColour , progress) ;   
            strip->SetPixelColor(i, updatedColor); 
          }; 

           if (method == HSL) { 
            HslColor updatedColor = HslColor::LinearBlend(HslColor(original), HslColor(NewColour), progress); 
            strip->SetPixelColor(i, updatedColor);  
          };         
           
        };

        animator->StartAnimation(i, time, animUpdate);
   }
}

void  cache Squares2 (uint8_t mode) { // WORKING RANDOM SQUARE SIZES...

  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......
  uint8_t effect_option = var6;
  uint8_t dimoffset ; 
  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 1;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



  switch(Current_Effect_State) {

    case PRE_EFFECT:


    effectPosition = 0; 
    
    /*Serial.print("Pixels = ");
    Serial.print(pixelCount);
    Serial.print(" Total_x = ");
    Serial.print(total_x);
    Serial.print(" Total_y = ");
    Serial.print(total_y);
    Serial.print(" Square_size = ");
    Serial.print(square_size); 
    Serial.print(" Number of Points = "); 
    Serial.println(numberofpoints);
*/


    if (effect_option == 1) fade_to(RgbColor(0,0,0), HSL); 



    // dimoffset = constrain( dimoffset ,0 , 255); 

    //Serial.print("dimoffset = ");
    //Serial.println(dimoffset); 


  //  if (effect_option == 0) top_bottom_fade( dim(Wheel(random(0,255)), constrain((CurrentBrightness - 100),0,255) ), dim(Wheel(random(0,255)), constrain((CurrentBrightness - 100),0,255)  ), total_x, CurrentAnimationSpeed); 
    
     if (effect_option == 0) { 

          dimoffset = CurrentBrightness / 5;
          //Serial.print("Dimm Offset = ");
          //Serial.println(dimoffset);

          RgbColor Top =     dim ( Wheel (  random(0,255)  ), (uint8_t)dimoffset );
          RgbColor Bottom =  dim ( Wheel (  random(0,255)  ), (uint8_t)dimoffset ) ;
         
          top_bottom_fade(  Top , Bottom , total_x, CurrentAnimationSpeed); 
    }

    Pre_effect();  // PRE effect SETS LAST EFFECT UPDATE TO ZERO... ? is this requires?

    lasteffectupdate = millis(); 


    break;
    case RUN_EFFECT:  

      
      if ( lasteffectupdate == 0 ) { // This allows a refresh, or brightness change etc...  to re-set up the effect..
        Current_Effect_State = PRE_EFFECT;
        break; 
      }

      if (millis() - lasteffectupdate > WS2812interval ) {      

      //for (int i = 0; i < numberofpoints; i++) {

      RgbColor color = dim(Wheel(random(255))); // RgbColor(random(255),random(255),random(255));

      //if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(2,7);

      bool coordinates_OK = false;
      uint8_t x_rand,y_rand;
      //Serial.println("Generating coordinates.");
       uint8_t count = 0; 

      
        // Serial.print(".");
       x_rand = random(0, total_x - square_size + 1) ; 
       y_rand = random(0, total_y - square_size + 1) ;

       for (uint8_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++) {

          uint16_t pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x ); 

          if (animator->IsAnimating(pixel)) { coordinates_OK = false; break; }; 
          if (sq_pixel == (square_size * square_size)-1) { coordinates_OK = true; }; 
          }
          // if (count++ == 5) break; 
          //Serial.print("Count = "); 
          //Serial.println(count);
          //delay(1); 
       


      //for (int j =0; j < (square_size * square_size) ; j++) {
      // Serial.println();
      // Serial.print("Found: x = ");
      // Serial.print(x_rand);
      // Serial.print(" y = ");
      // Serial.println(y_rand);

    if (coordinates_OK) {
      
      uint16_t time = random(( CurrentAnimationSpeed  ), (CurrentAnimationSpeed * 10)); //generate same time for each object

    for (uint8_t sq_pixel = 0; sq_pixel < (square_size * square_size); sq_pixel++)
        {
            

            uint16_t pixel = return_shape_square(x_rand, y_rand, sq_pixel, square_size, total_x ); 
  
            RgbColor originalColor = strip->GetPixelColor(pixel);
          
            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor;
                float new_progress = progress * 2.0; 
                if (new_progress >= 1) new_progress = (2.0 - new_progress); 
                updatedColor = RgbColor::LinearBlend(originalColor, color, new_progress);
                strip->SetPixelColor(pixel, updatedColor);

            };

            animator->StartAnimation(pixel, time , animUpdate); // might change this to be a random variant...
        }



        //Serial.print("GRID " + String (square_size * square_size) + "... ");

       // int pixel = return_shape_square(x_rand, y_rand, j, square_size, total_x ); 

        //if (pixel1 < pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));          



        lasteffectupdate = millis(); 

          }
    
        }


    break;
    case POST_EFFECT:
    Post_effect(); 
    break;
  }






/*
  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


  uint8_t x,y, total_y;
  uint8_t total_x = var7; 
  uint8_t square_size = var10;
  uint8_t numberofpoints = var8; // default 5, if it = 10, then its random......


  if (square_size == 0) square_size = 3;  
  if (numberofpoints == 0) numberofpoints = 5;
  if (total_x == 0) total_x = 13; 

  total_y = return_total_y(total_x); 



    if ( (millis() > lasteffectupdate ) && (!strip->IsAnimating()) ) {

      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }


      for (int i = 0; i < numberofpoints; i++) {

      RgbColor colour = Wheel(random(255)); // RgbColor(random(255),random(255),random(255));

      //if (square_size == 10) colour.Darken(random(0,50)); // OLD METHOD
      if (mode == 1) colour = dimbyhsv(colour, (255 - random(0,50) )); // OLD METHOD
      if (mode == 1) square_size = random(2,7);


      uint8_t x_rand = random(0, total_x - square_size ) ; 
      uint8_t y_rand = random(0, total_y - square_size ) ;


      for (int j =0; j < (square_size * square_size) ; j++) {


        //Serial.print("GRID " + String (square_size * square_size) + "... ");

        int pixel1 = return_shape_square(x_rand, y_rand, j, square_size, total_x ); 

        if (pixel1 < pixelCount) strip->LinearFadePixelColor(CurrentAnimationSpeed, pixel1 , dim(colour));
      
     }


    }

    //Serial.println("------------------");

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;  // add time so that next sequence starts AFTER animations have finished...
    strip->StartAnimating(); // start animations

    }


//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 
if (Current_Effect_State == POST_EFFECT) Post_effect();  

*/
} 
// end of Squares2



void cache Squares () {

  /*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  static int wsPoint = 0;
  
  if (var8 == 0) var8 = 5;
  if (var7 == 0) var7 = 13;  

  uint8_t numberofpoints = var8; // default 5
  uint8_t x,y;
  uint8_t total_y = return_total_y(var7); 
  uint8_t total_x = var7; 


    if ( (millis() > lasteffectupdate) && (!strip->IsAnimating()) ) {
      for(int i = 0; i < pixelCount; i++) // SET everything to black!  
      {
        //strip->SetPixelColor(i,0);
        strip->LinearFadePixelColor(CurrentAnimationSpeed, i, 0);
      }

      for (int i = 0; i < numberofpoints; i++) {


      uint8_t x_rand = random(1, total_x-2) ; 
      uint8_t y_rand = random(1, total_y-2) ;

      RgbColor colour = Wheel(random(255)) ; //  RgbColor(random(255),random(255),random(255));

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand,y_rand,total_x), dim(colour));

      colour.Darken(50);

      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand - 1 , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand     , y_rand - 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand + 1 , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand     , total_x     ), dim(colour));
      strip->LinearFadePixelColor(CurrentAnimationSpeed, return_pixel(x_rand + 1 , y_rand - 1 , total_x     ), dim(colour));       


    }

    lasteffectupdate = millis() + CurrentAnimationSpeed + (WS2812interval*10) ;
    strip->StartAnimating(); // start animations

    }



//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 
if (Current_Effect_State == POST_EFFECT) Post_effect();  



*/

} // end of Squares






void cache theatreChaseRainbow() {

  if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


  static uint16_t colourpoint = 0; 
  static uint8_t animationstate = 0 ; 
  if (millis() > (lasteffectupdate ) ) {


  //for (int j=0; j < 256; j++) {       // cycle all 256 colors in the wheel

        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+animationstate, dim(Wheel(i+colourpoint)));    //turn every third pixel on
        }


        for (int i=0; i < pixelCount; i=i+3) {
          strip->SetPixelColor(i+ animationstate - 1, 0);    //turn every third pixel on
        }
        animationstate++; 

        if (animationstate == 3) animationstate = 0; // reset animation state. 


  
    colourpoint++; 
    lasteffectupdate = millis() + WS2812interval ;
  }  //  end of timer..... 


//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 
if (Current_Effect_State == POST_EFFECT) Post_effect();  

}



void cache Adalight_Flash() {

    for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(255,0,0));
          }
    strip->Show(); 
    delay(200);
        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,255,0));
          }
        strip->Show(); 
    delay(200);
        for(uint16_t i=0; i<pixelCount; i++) {
        strip->SetPixelColor(i, RgbColor(0,0,255));
          }
        strip->Show(); 
    delay(200);
    strip->ClearTo(RgbColor(0,0,0));

  }


void cache Adalight () {    //  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;
  uint8_t prefix[] = {'A', 'd', 'a'}, hi, lo, chk, i;

  static boolean Adalight_configured;
  static uint16_t effectbuf_position = 0;
  enum mode { MODE_INITIALISE = 0, MODE_HEADER, MODE_CHECKSUM, MODE_DATA, MODE_SHOW, MODE_FINISH};
  static mode state = MODE_INITIALISE;
  static int effect_timeout = 0;
  static uint8_t prefixcount = 0;
  static unsigned long ada_sent = 0; 
  static unsigned long pixellatchtime = 0;
  const unsigned long serialTimeout = 15000; // turns LEDs of if nothing recieved for 15 seconds..
  //const unsigned long initializetimeout = 10000; 
  //static unsigned long initializetime = 0; 
    //if (!Adalight_configured) {
    //    Adalight_Flash(); 
    //    Adalight_configured = true;
    //}

//    if (pixellatchtime > 0 && (pixellatchtime + serialTimeout) < millis()) {
//      pixellatchtime = 0; // reset counter / latch to zero should only fire when show pixls has been called!
//      StripOFF();  // turn off pixels... 
//      state = MODE_HEADER;  // resume header search....
//    }

    // if(millis() > initializetime + initializetimeout) state = MODE_INITIALISE; // this goes to initiaase mode...

    if (Current_Effect_State == PRE_EFFECT) { state = MODE_INITIALISE; } ; 
      //Pre_effect();  
    // }

    if (Current_Effect_State == POST_EFFECT) state = MODE_FINISH;  

    //if (Current_Effect_State == POST_EFFECT) state = MODE_FINISH; 


  switch (state) {

    case MODE_INITIALISE:
      Serial.println("Begining of Adalight");
      Adalight_Flash(); 
      state = MODE_HEADER;
      Pre_effect();  
      //Current_Effect_State = RUN_EFFECT;

    case MODE_HEADER:

      effectbuf_position = 0; // reset the buffer position for DATA collection...

          if(Serial.available()) { // if there is serial available... process it... could be 1  could be 100....
               
            for (int i = 0; i < Serial.available(); i++) {  // go through every character in serial buffer looking for prefix...

              if (Serial.read() == prefix[prefixcount]) { // if character is found... then look for next...
                  prefixcount++;
              } else prefixcount = 0;  //  otherwise reset....  ////

            if (prefixcount == 3) {
            effect_timeout = millis(); // generates START TIME.....
            state = MODE_CHECKSUM;
            prefixcount =0;
            break; 
            } // end of if prefix == 3
            } // end of for loop going through serial....
            } else if (!Serial.available() && (ada_sent + 5000) < millis()) {
                  Serial.print("Ada\n"); // Send "Magic Word" string to host
                  ada_sent = millis(); 
            } // end of serial available....

    break;

    case MODE_CHECKSUM:

        if (Serial.available() >= 3) {
          hi  = Serial.read();
          lo  = Serial.read();
          chk = Serial.read();
          if(chk == (hi ^ lo ^ 0x55)) {
            state = MODE_DATA;
          } else {
            state = MODE_HEADER; // ELSE RESET.......
          }
        }

      if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.

      break;

    case MODE_DATA:

        //  this bit is what might... be causing the flashing... as it extends past memory stuctures....
        while (Serial.available() && effectbuf_position < 3 * strip->PixelCount()) {  // was <=  
          pixelsPOINT[effectbuf_position++] = Serial.read();
          //if (effectbuf_position == 3 * strip->PixelCount()) break; 
        }

      if (effectbuf_position >= 3*pixelCount) { // goto show when buffer has recieved enough data...
        state = MODE_SHOW;
        break;
      } 

        if ((effect_timeout + 1000) < millis()) state = MODE_HEADER; // RESET IF BUFFER NOT FILLED WITHIN 1 SEC.


      break;

    case MODE_SHOW:

      strip->Dirty(); // MUST USE if you're using the direct buffer copy... 
      pixellatchtime = millis();
      state = MODE_HEADER;
      break;

    case MODE_FINISH:

      Serial.print("END OF ADALIGHT..."); 
      //Current_Effect_State = PRE_EFFECT; 
      //opState = HoldingOpState; 
      //state == MODE_INITIALISE; 
      Post_effect();  

    break; 
}

      //strip->Show();


 //initializetime = millis(); // this resets the timer 


//if (Current_Effect_State == POST_EFFECT) { Current_Effect_State = PRE_EFFECT; opState = HoldingOpState; } ; 

}


// called to change the number of pixels
void cache ChangeNeoPixels(uint16_t count, uint8_t pin)  {
  
 bool commitchanges = false; 

    //Serial.println("Change Neopixels called"); 


        uint8_t pixelPINstored = EEPROM.read(PixelPIN_address);    
        //int pixelCountstored = EEPROM.read(PixelCount_address);

        uint8_t a = EEPROM.read(PixelCount_address);
        uint8_t b = EEPROM.read(PixelCount_address+1);
        if(isnan(a)) a = 0;
        if(isnan(b)) b = 0;
        uint16_t pixelCountstored = a*256+b;

        //int b=EEPROM.read(PixelCount_address+1);
        //nt pixelCountstored = a*256+b;


    if (count != pixelCountstored) {
    //Serial.println("Pixel count changed..."); 

      int a = pixelCount/256;
      int b = pixelCount % 256;        
      
        EEPROM.write(PixelCount_address,a);
        EEPROM.write(PixelCount_address+1,b);

        commitchanges = true;

    if (EEPROM.read(PixelCount_enablebyte) != flagvalue) EEPROM.write(PixelCount_enablebyte,flagvalue) ;
     //Serial.println("pixel count byte updated");
    }

    if (pin != pixelPINstored) {
    //Serial.println("Change Neopixels PIN called"); 

        EEPROM.write(PixelPIN_address, (byte)pin);
    if (EEPROM.read(PixelPIN_enablebyte) != flagvalue) EEPROM.write(PixelPIN_enablebyte,flagvalue) ;
     
     commitchanges = true;

    }

    if (commitchanges == true) {
      EEPROM.commit();
          Serial.println("WS2812 Settings Updated."); 
        }


 //   if (strip)
 //   {
        //StripOFF();

 //       delete strip;
 //   }

 //   strip = new NeoPixelBus(count, pin);

  if (animator != NULL)
  {
    delete animator;
  }
  if (strip != NULL)
  {
    delete strip;
  }
  strip = new NeoPixelBus(count, pin);
  animator = new NeoPixelAnimator(strip);


    pixelsPOINT = (uint8_t*)strip->Pixels(); ///  used for direct access to pixelbus buffer...
    if (strip->PixelCount() < 10) var7 = 1;


}

void cache UDPfunc () {

//static boolean Adalight_configured = false;

 if (Current_Effect_State == PRE_EFFECT) {
    Serial.println("UDP mode enabled\n"); // Send "Magic Word" string to host
    Adalight_Flash();
    Udp.beginMulticast(WiFi.localIP(), multicast_ip_addr, localPort); 
    Pre_effect();  
    } 

if (Current_Effect_State == RUN_EFFECT) {

int packetSize = Udp.parsePacket();

  if(Udp.available())
  {
        Serial.println("UDP packet recieved...");

    RgbColor col = RgbColor(0,0,0);
    int currentpixel = 0;
    for (int i = 0; i < packetSize; i = i + 3) {
      if (currentpixel > pixelCount) break;
        col.R = Udp.read();
        col.G = Udp.read();
        col.B = Udp.read();
        strip->SetPixelColor(currentpixel,col);
        currentpixel++;
      }

//strip->Show();

    }

}



if (Current_Effect_State == POST_EFFECT) { 
  //Serial.println("UDP END"); 
  //Current_Effect_State = PRE_EFFECT; 
  //opState = HoldingOpState; 
  Udp.stop(); 
  Post_effect(); 
  } ; 
}

int cache getPixelPower () {
 int brightnesstally = 0;
 for (int i = 0;i < pixelCount; i++) {
  RgbColor colour = strip->GetPixelColor(i);
  int brightness = colour.CalculateBrightness();
  brightness = map(brightness,0,255,0,60);
  brightnesstally = brightnesstally + brightness;
}

int brightness = brightnesstally;

return brightness;
} 





void cache handle_lights_config() {

String buf; 
   if (server.args() != 0) { lasteffectupdate = 0; Random_func_timeout = 0; }; 
   if (server.arg("var1").length() != 0) var1 = server.arg("var1").toInt();
   if (server.arg("var2").length() != 0) var2 = server.arg("var2").toInt(); // colour point min
   if (server.arg("var3").length() != 0) var3 = server.arg("var3").toInt(); // colour point max
   if (server.arg("var4").length() != 0) var4 = server.arg("var4").toInt();
   if (server.arg("var5").length() != 0) var5 = server.arg("var5").toInt();
   if (server.arg("var6").length() != 0) var6 = server.arg("var6").toInt();
   if (server.arg("var7").length() != 0) var7 = server.arg("var7").toInt();
   if (server.arg("var8").length() != 0) var8 = server.arg("var8").toInt();
   if (server.arg("var9").length() != 0) var9 = server.arg("var9").toInt();
   if (server.arg("var10").length() != 0) { var10 = server.arg("var10").toInt(); Serial.println("Var 10 updated"); }; 

   // String a = ESP.getFlashChipSizeByChipId(); 
   if (server.arg("reset") == "true") { var1 = 0; var2 = 0; var3 = 0; var4 = 0; var5 = 0; var6 = 0; var7 = 0; var8 = 0; var9 = 0; var10 = 0;};


  String content = F("\
    <!DOCTYPE HTML><html><body bgcolor='#E6E6FA'><head> <meta name='viewport' content='initial-scale=1'>\
    <title> % </title></head><body><h1> % </h1>\
    <br> <a href='/lightsconfig?reset=true'>RESET TO DEFAULTS</a>\
    <form name=form action='/lightsconfig' method='POST'>");

    buf = insertvariable ( content, String(deviceid));

  buf = insertvariable ( buf, String(deviceid));
    
    server.setContentLength(CONTENT_LENGTH_UNKNOWN);
    server.send(200, "text/html", "");
    WiFiClient client = server.client();
    server.sendContent(buf);
    buf = " "; 

  //buf += "Select Mode <select name='modedrop' onchange='this.form.submit();'>";

  for (int k=0; k < 10; k++ ) {
  //buf += "<option value='" + String(k) + selected + ">" + String(MODE_STRING[k]) + "</option>";
   // buf += "<option value='" + String(k) + "'" + ">" + String(k) + "</option>";
    buf += String(VAR_STRING[k]) + " : <input type='text' id='var" + String(k+1) + "' name='var" + String(k+1) + "' value=''><br>";
  }

  buf += F("<input type='submit' value='Submit'>\
          </form></p>"); 
  buf += htmlendstring; 


  server.sendContent(buf);

  //server.send(200, "text/html", buf);



}



uint16_t cache return_pixel(uint8_t x, uint8_t y, uint8_t pitch) {
  uint16_t a = (pitch * y) + x; 
  return a; 
}


uint8_t cache return_total_y(uint8_t pitch) {
 uint8_t total_y = pixelCount / pitch; 
 float remainder = pixelCount % pitch;
 if (remainder > 0) total_y++;
  return total_y;

}

//  Returns a pixel number... starting in bottom left...
uint16_t cache return_shape_square(uint8_t first_pixel_x, uint8_t first_pixel_y , uint8_t desired_pixel, uint8_t grid_size, uint8_t total_in_row) {

  uint16_t pixel; 
  uint8_t pixel_x, pixel_y, row, row_left ;
  row = desired_pixel / grid_size; 
  row_left = desired_pixel % grid_size;
  pixel_y = first_pixel_y + row;  
  pixel_x = first_pixel_x + row_left; 
  pixel = return_pixel(pixel_x, pixel_y, total_in_row);
  return pixel; 
}



// function to shift pixels in blocks one way or another...

void cache pixelshift(uint16_t start, uint16_t end) {

  pixelshift( start, end,100); 
}


void cache pixelshift(uint16_t start, uint16_t end, uint16_t pixelshift_timer) {

static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
//uint16_t pixelshift_timer = 100; 

if (millis() > (last_pixelshift + pixelshift_timer)) {

  if (end > start) { // move direction right

    for (uint16_t i = end; i > start ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);

    }
  }   else if (start > end) {  // Move direction = left

    for (uint16_t i = end; i < start; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);

    }

  }
  strip->SetPixelColor(start, 0);
  last_pixelshift = millis(); 
}


}


//// function to shift pixels in blocks one way or another...
void cache pixelshift_middle() {


pixelshift_middle(100); 

}


void cache pixelshift_middle(uint16_t pixelshift_timer) {
/*
static long last_pixelshift = 0;
RgbColor pix_colour = RgbColor(0,0,0); // holds pixel data... 
uint16_t middle = strip->PixelCount() / 2; 
uint8_t remainderodd = 0; 
if  (strip->PixelCount() % 2 != 0) remainderodd = 1 ; 



if (millis() > (last_pixelshift + pixelshift_timer)) {


    for (uint16_t i = 0; i < middle ; i++) {
      pix_colour = strip->GetPixelColor(i+1);
      strip->SetPixelColor(i, pix_colour);
    }

    for (uint16_t i = strip->PixelCount(); i > middle + 1 ; i--) {
      pix_colour = strip->GetPixelColor(i-1);
      strip->SetPixelColor(i, pix_colour);
    } 

  
  strip->SetPixelColor(middle, 0);
  if (remainderodd != 0) strip->SetPixelColor(middle+1, 0);


  last_pixelshift = millis(); 

}

*/
}
 // END of pixel shift




void cache eq1 () {
/*
if (Current_Effect_State == PRE_EFFECT) Pre_effect();  

  if (millis() > lasteffectupdate  )  {

      strip->SetPixelColor(3, RgbColor(255,0,0));
      strip->SetPixelColor(4, RgbColor(0,255,0));
      //strip->SetPixelColor(4, RgbColor(0,0,255));
      lasteffectupdate = millis() + WS2812interval;
  } 
          
uint8_t direction = random(0,2);

if (direction == 0 ) {

pixelshift(6,0);

} else if (direction == 1) {

pixelshift(0,6);

} else if (direction == 2) {

pixelshift_middle(); 


};
 
// pixelshift(6,0);


if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
}

void cache Set_Colour_ToptoBottom() {







}





// Overloaded func for topbottom fade...
void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint8_t count_x) {

  top_bottom_fade(Top,Bottom, count_x, CurrentAnimationSpeed, HSL);
}

void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint8_t count_x,uint16_t time) {

  top_bottom_fade(Top,Bottom, count_x, time, HSL);
}



void cache top_bottom_fade( RgbColor Top, RgbColor Bottom, uint8_t count_x, uint16_t time, BlendMethod Method) {

uint8_t x,y;
float colour_steps; 
// float colour_stepsF; 
uint8_t total_y = return_total_y(count_x); // get total number of rows
RgbColor colourRGB;
HslColor colourHSL; 
//if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


for (y = 0; y < total_y; y++) {

    colour_steps = (float) y / (float) total_y; 
    if (y == 0) colour_steps = 0.0f; // this stets the top blend.  
    if (y == total_y - 1) colour_steps = 1.0f;  // this sets the bottom blend
  // colour_stepsF = (float)colour_steps / 255.0;  // this converts it to float for new lib...

    if (Method == RGB) { colourRGB = RgbColor::LinearBlend(Top, Bottom, colour_steps); };
    if (Method == HSL) { colourHSL = HslColor::LinearBlend(HslColor(Top), HslColor(Bottom), colour_steps); 
                         colourRGB = RgbColor(colourHSL) ;
                        };
 
    for( x = 0; x < count_x; x++) {
        uint16_t pixel = return_pixel(x,y, count_x);  // which pixel
        RgbColor original = strip->GetPixelColor(pixel); // get the orginal colour of it, RGB
        if (pixel >= strip->PixelCount()) break; // escape if out of bounds...

        AnimUpdateCallback animUpdate = [=](float progress)
            {
             // if (Method == HSL) {
             //   HslColor updatedColor = RgbColor::LinearBlend(HslColor(original), colourHSL ,  progress) ;
             //   strip->SetPixelColor(pixel, updatedColor);
             // } else if (Method == RGB) {
                RgbColor updatedColor = RgbColor::LinearBlend(original, colourRGB, progress) ;
                strip->SetPixelColor(pixel, updatedColor);
             // }; 
            };
            animator->StartAnimation(pixel, time, animUpdate);
    } // END of x ......

}  // END of y......
    
} // END of tom_bottom_fade


  //  case RANDOM_TOP_BOTTOM:
  //    Effect_Top_Bottom(TWOCOLOUR,HSL);
  //    break;
  //  case RANDOM_TOP_BOTTOM_LINEAR:
  //    Effect_Top_Bottom(TWOCOLOUR, RGB);
  //    break;    
  //  case SINGLE_COLOUR_FADE:
  //    Effect_Top_Bottom(SINGLECOLOUR, HSL);
  //    break;  
  //  case RANDOM_COLOUR_FADE:
  //    Effect_Top_Bottom(RANDOMSINGLECOLOUR,HSL);

RgbColor Manipulate_Hue(RgbColor original, float amount ) {

HslColor OriginalHSL = HslColor(original); 

float NewHue = OriginalHSL.H + amount; 

return RgbColor( HslColor(NewHue, OriginalHSL.S, OriginalHSL.L) ); 

}

// "Ceiling           ", // var 1
// "Floor             ", // var 2


void cache Effect_Top_Bottom(EffectSetting Setting, BlendMethod Method) { 
  uint8_t total_x; 
  uint32_t random_time; 
  float Ceiling, Floor; 
  RgbColor colour_top, colour_bottom; 

  switch(Current_Effect_State) {


    case PRE_EFFECT:
    Pre_effect(); 
    Serial.println("Top Bottom PRE...");
    lasteffectupdate = 0; 
    break;
    case RUN_EFFECT: // 
    random_time = (random(WS2812interval*30, (WS2812interval * 300) ));
      if (  
            ( 
              ( millis() - lasteffectupdate > random_time ) ||  ( lasteffectupdate == 0 )
            ) 
                &&  (animator->IsAnimating() == false ) 
          ) 

          { // only generate new effect if NOT blending..
       
          if (var1 == 0) { Ceiling = 0.2f; } else { Ceiling = (float)var1 / 255.0f ; }; 
          if (var2 == 0) { Floor = -0.2f; } else { Floor = ((float)var2 / 255.0f) * -1 ; } ;
          if (var7 == 0 && pixelCount > 10) { 
            total_x = 13; 
          } else if (var7 == 0 && pixelCount < 10) {
            total_x = 1; 
          } else { 
            total_x = var7; 
          }; 

          //Serial.print("Total X = ");
          //Serial.println(total_x);

if (Setting == TWOCOLOUR) {
       colour_top    = Wheel(random(0,255)) ; //  RgbColor(255,0,0); 
       colour_bottom = Wheel(random(0,255)) ; //  RgbColor(255,0,0); 
} else if (Setting == SINGLECOLOUR) {
       colour_top    = Manipulate_Hue(NewColour, Ceiling); 
       colour_bottom = Manipulate_Hue(NewColour, Floor);
} else if (Setting == RANDOMSINGLECOLOUR) {
       RgbColor random_colour_choice = Wheel(random(0,255));     
       colour_top    = Manipulate_Hue(random_colour_choice, Ceiling); 
       colour_bottom = Manipulate_Hue(random_colour_choice, Floor);
}; 
      colour_top = dim(colour_top);
      colour_bottom = dim(colour_bottom); 

      top_bottom_fade(colour_top,colour_bottom, total_x, CurrentAnimationSpeed, Method); 
      lasteffectupdate = millis(); 
          }

    break;
    case POST_EFFECT:
    Post_effect(); 
        Serial.println("Top Bottom POST...");
    break;
  }















/*
//static uint32_t Random_func_timeout = 0, Random_func_lasttime = 0; 
static uint8_t current_r = 0, total_x = 0;

if (Current_Effect_State == PRE_EFFECT) Pre_effect();  


if (var7 == 0 ) {total_x = 13;} else total_x = var7;
    
    if ( (millis() > lasteffectupdate  ) && (!strip->IsAnimating()) ) {


 //     Serial.println();
 //     Serial.print("Effect updated..(");
 //     Serial.print(strip->IsAnimating());
//      Serial.print(") ");

      uint16_t random_animation_speed = random(2000, 20000);
      uint8_t vvv,bbb,dif = 0 ; 
      vvv = random(255);

do {
      bbb = random(255); 
      dif = vvv - bbb; 
      dif = abs(dif);
      } while (dif < 50);


      Serial.print("top: ");
      Serial.print(vvv);
      Serial.print(" bottom: ");
      Serial.print(bbb);
      Serial.print(" Abs diff: ");
      Serial.print(abs(dif)); 

      RgbColor random_colour_top = Wheel(vvv); //  RgbColor(255,0,0); 
      RgbColor random_colour_bottom = Wheel(bbb); //  RgbColor(255,0,0); 

      //Serial.print("  (PRE)");
      
      top_bottom_fade(random_colour_top, random_colour_bottom, total_x, fadetype);
      


      //Serial.println("  (POST) ");
      

      //Random_func_lasttime = millis() + CurrentAnimationSpeed;  // was working...  changed to get updates working....

      lasteffectupdate = millis() + random(WS2812interval*30, (WS2812interval * 300) ) + CurrentAnimationSpeed;
      if (fadetype == 2) lasteffectupdate = millis() + CurrentAnimationSpeed + 500; // This effect does not require changing.... 

      //Serial.print("Random_func_lasttime: ");
      //Serial.print(Random_func_lasttime); 
      // Random_func_timeout = random(60000, 60000*5);
      

      //Random_func_timeout = random(WS2812interval*30, (WS2812interval * 300) ) + CurrentAnimationSpeed; 
      //Serial.print("   Random_func_timeout: ");
      //Serial.println(Random_func_timeout); 

    }
  //}


if (Current_Effect_State == POST_EFFECT) Post_effect(); 

*/
} // end of actual function.....


void test_newanimations() {

uint8_t peak = 128; 


if (Current_Effect_State == PRE_EFFECT) Pre_effect(); 

if (Current_Effect_State == RUN_EFFECT) {

if (millis() - lasteffectupdate > WS2812interval) {

if (effectPosition == 0)
    {
        for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
        {
            uint16_t time = random(800, 1000);
            RgbColor originalColor = strip->GetPixelColor(pixel);
            RgbColor color = RgbColor(random(peak), random(peak), random(peak));

            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, color, progress);
                strip->SetPixelColor(pixel, updatedColor);
            };
            animator->StartAnimation(pixel, time, animUpdate);
        }
    }
    else if (effectPosition == 1)
    {
        for (uint8_t pixel = 0; pixel < pixelCount; pixel++)
        {
            uint16_t time = random(600, 700);
            RgbColor originalColor = strip->GetPixelColor(pixel);

            // define the effect to apply, in this case linear blend
            AnimUpdateCallback animUpdate = [=](float progress)
            {
                // progress will start at 0.0 and end at 1.0
                RgbColor updatedColor = RgbColor::LinearBlend(originalColor, RgbColor(0, 0, 0), progress);
                strip->SetPixelColor(pixel, updatedColor);
            };
            // start the animation
            animator->StartAnimation(pixel, time, animUpdate);
        }
    }
    effectPosition = (effectPosition + 1) % 2; // next effectPosition and keep within the number of effectPositions

lasteffectupdate = millis(); 
}
}

if (Current_Effect_State == POST_EFFECT) Post_effect(); 
}

