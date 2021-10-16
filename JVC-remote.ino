/*
 *  ESP8266 NodeMCU MQTT JVC Projector Control
 *  
 */
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

SoftwareSerial RS232Out(12, 13); // RX, TX
 
//Enter your wifi credentials
const char* ssid = "ssid";  
const char* password =  "password";

//Enter your mqtt server configurations
const char* mqttServer = "ip or host name";   //Enter Your mqttServer address
const int mqttPort = port;                    //Port number
const char* mqttUser = "mqtt username";       //User
const char* mqttPassword = "mqtt password";   //Password

//Remote control codes
char PowerOff[] = "\x21\x89\x01\x50\x57\x30\x0A\r";
char PowerOn[] = "\x21\x89\x01\x50\x57\x31\x0A\r";
char InputHDMI1[] = "\x21\x89\x01\x49\x50\x36\x0A\r";
char InputHDMI2[] = "\x21\x89\x01\x49\x50\x37\x0A\r";
char InputComponent[] = "\x21\x89\x01\x49\x50\x32\x0A\r";
char InputSVideo[] = "\x21\x89\x01\x49\x50\x30\x0A\r";
char InputVideo[] = "\x21\x89\x01\x49\x50\x31\x0A\r";
char Inputplus[] = "\x21\x89\x01\x49\x50\x2B\x0A\r";
char Inputmin[] = "\x21\x89\x01\x49\x50\x2D\x0A\r";
char TestPatternOff[] = "\x21\x89\x01\x54\x53\x30\x0A\r";
char TestPatternColourBars[] = "\x21\x89\x01\x54\x53\x31\x0A\r";
char TestPatternStairstepblackandwhite[] = "\x21\x89\x01\x54\x53\x36\x0A\r";
char TestPatternStairstepred[] = "\x21\x89\x01\x54\x53\x37\x0A\r";
char TestPatternStairstepgreen[] = "\x21\x89\x01\x54\x53\x38\x0A\r";
char TestPatternStairstepblue[] = "\x21\x89\x01\x54\x53\x39\x0A\r";
char TestPatternCrosshatchgreen[] = "\x21\x89\x01\x54\x53\x41\x0A\r";
char GammaNormal[] = "\x21\x89\x01\x47\x54\x30\x0A\r";
char GammaA[] = "\x21\x89\x01\x47\x54\x31\x0A\r";
char GammaB[] = "\x21\x89\x01\x47\x54\x32\x0A\r";
char GammaC[] = "\x21\x89\x01\x47\x54\x33\x0A\r";
char GammaCustom1[] = "\x21\x89\x01\x47\x54\x34\x0A\r";
char GammaCustom2[] = "\x21\x89\x01\x47\x54\x35\x0A\r";
char GammaCustom3[] = "\x21\x89\x01\x47\x54\x36\x0A\r";
char GammaCorrectionValue18[] = "\x21\x89\x01\x47\x50\x30\x0A\r";
char GammaCorrectionValue19[] = "\x21\x89\x01\x47\x50\x31\x0A\r";
char GammaCorrectionValue20[] = "\x21\x89\x01\x47\x50\x32\x0A\r";
char GammaCorrectionValue21[] = "\x21\x89\x01\x47\x50\x33\x0A\r";
char GammaCorrectionValue22Default[] = "\x21\x89\x01\x47\x50\x34\x0A\r";
char GammaCorrectionValue23[] = "\x21\x89\x01\x47\x50\x35\x0A\r";
char GammaCorrectionValue24[] = "\x21\x89\x01\x47\x50\x36\x0A\r";
char GammaCorrectionValue25[] = "\x21\x89\x01\x47\x50\x37\x0A\r";
char GammaCorrectionValue26[] = "\x21\x89\x01\x47\x50\x38\x0A\r";
char PictureModeCinema1[] = "\x21\x89\x01\x50\x4D\x50\x4D\x30\x0A\r";
char PictureModeCinema2[] = "\x21\x89\x01\x50\x4D\x50\x4D\x31\x0A\r";
char PictureModeCinema3[] = "\x21\x89\x01\x50\x4D\x50\x4D\x32\x0A\r";
char PictureModeNatural[] = "\x21\x89\x01\x50\x4D\x50\x4D\x33\x0A\r";
char PictureModeStage[] = "\x21\x89\x01\x50\x4D\x50\x4D\x34\x0A\r";
char PictureModeDynamic[] = "\x21\x89\x01\x50\x4D\x50\x4D\x35\x0A\r";
char PictureModeUser1[] = "\x21\x89\x01\x50\x4D\x50\x4D\x36\x0A\r";
char PictureModeUser2[] = "\x21\x89\x01\x50\x4D\x50\x4D\x37\x0A\r";

//Extended controls
char VerticalStretchOff[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x34\x0A\r";
char VerticalStretchOn[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x33\x0A\r";
char Aspect169[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x36\x0A\r";
char Aspect43[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x35\x0A\r";
char AspectZoom[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x37\x0A\r";
char Aspectcyclesthroughallavailablemodes[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x37\x0A\r";
char BackStepsbackwardsthroughmenusandremovesanyOSDmessages[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x33\x0A\r";
char BNRBlockNoiseReductionOff[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x30\x0A\r";
char BNRBlockNoiseReductionOn[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x46\x0A\r";
char BrightnessMin[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x42\x0A\r";
char BrightnessPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x41\x0A\r";
char BrightnessAdjAdjustmentBarOnOfftoggle[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x39\x0A\r";
char CECOff[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x37\x0A\r";
char CECOn[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x36\x0A\r";
char ColourMin[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x44\x0A\r";
char ColourPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x43\x0A\r";
char ColourAdjAdjustmentBarOnOfftoggle[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x35\x0A\r";
char ColourTemp5800K[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x45\x0A\r";
char ColourTemp6500K[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x46\x0A\r";
char ColourTemp7500K[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x30\x0A\r";
char ColourTemp9300K[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x31\x0A\r";
char ColourTempCustom1[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x33\x0A\r";
char ColourTempCustom2[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x34\x0A\r";
char ColourTempCustom3[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x35\x0A\r";
char ColourTempHighBright[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x32\x0A\r";
char ColourTempcyclesthroughalloptions[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x36\x0A\r";
char ContrastMin[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x39\x0A\r";
char ContrastPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x38\x0A\r";
char ContrastAdjAdjustmentBarOnOfftoggle[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x41\x0A\r";
char CTIColourTransientImprovementOff[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x43\x0A\r";
char CTIColourTransientImprovementLow[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x44\x0A\r";
char CTIColourTransientImprovementMiddle[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x45\x0A\r";
char CTIColourTransientimprovementHigh[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x46\x0A\r";
char CursorDown[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x32\x0A\r";
char CursorLeft[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x36\x0A\r";
char CursorRight[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x34\x0A\r";
char CursorUp[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x31\x0A\r";
char DetailEnhanceMin[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x32\x0A\r";
char DetailEnhancePlus[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x31\x0A\r";
char Gammacyclesthroughalloptions[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x35\x0A\r";
char HideOnOfftoggle[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x44\x0A\r";
char InformationdisplaysInformationtabofmenu[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x34\x0A\r";
char Inputcyclesthroughallavailableinputs[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x38\x0A\r";
char KeystoneCorrectionHorizontalMin[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x31\x0A\r";
char KeystoneCorrectionHorizontalPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x30\x0A\r";
char KeystoneCorrectionVerticalMin[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x43\x0A\r";
char KeystoneCorrectionVerticalPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x42\x0A\r";
char LensAperture1[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x38\x0A\r";
char LensAperture2[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x39\x0A\r";
char LensAperture3[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x41\x0A\r";
char LensApertureAdj[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x30\x0A\r";
char LensControlcyclesthroughalloptions[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x30\x0A\r";
char LensFocusMin[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x32\x0A\r";
char LensFocusPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x31\x0A\r";
char LensShiftDown[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x32\x0A\r";
char LensShiftLeft[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x34\x0A\r";
char LensShiftRight[] = "\x21\x89\x01\x52\x43\x37\x33\x34\x33\x0A\r";
char LensShiftUp[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x31\x0A\r";
char LensZooPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x35\x0A\r";
char LensZoomOut[] = "\x21\x89\x01\x52\x43\x37\x33\x33\x37\x0A\r";
char MenuOnOfftoggle[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x45\x0A\r";
char MNRMosquitoNoiseReductionMin[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x45\x0A\r";
char MNRMosquitoNoiseReductionPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x44\x0A\r";
char NRtogglesdisplayofRNRMNR[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x38\x0A\r";
char OKtoacceptcurrentlyselectedoption[] = "\x21\x89\x01\x52\x43\x37\x33\x32\x46\x0A\r";
char PowerOffsendtwicewithshortdelaybetweentoswitchoff[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x36\x0A\r";
char RNRRandomNoiseReductionMin[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x43\x0A\r";
char RNRRandomNoiseReductionPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x30\x42\x0A\r";
char SharpnessMin[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x46\x0A\r";
char SharpnessPlus[] = "\x21\x89\x01\x52\x43\x37\x33\x37\x45\x0A\r";
char SharpnessAdj[] = "\x21\x89\x01\x52\x43\x37\x33\x31\x34\x0A\r";
char TestPatterncyclesthroughallpatterns[] = "\x21\x89\x01\x52\x43\x37\x33\x35\x39\x0A\r";

WiFiClient espClient;
PubSubClient client(espClient);
 
void setup() {
  delay(1000);
  Serial.begin(19200);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  Serial.print("Connected to WiFi :");
  Serial.println(WiFi.SSID());
 
  client.setServer(mqttServer, mqttPort);
  client.setCallback(MQTTcallback);
 
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");

    if (client.connect("JVC DLA-HD350", mqttUser, mqttPassword )) {
 
      Serial.println("connected");  
 
    } else {
 
      Serial.print("failed with state ");
      Serial.println(client.state());  //If you get state 5: mismatch in configuration
      delay(2000);
 
    }
  }
 
  client.publish("projector/state", "JVC DLA-HD350 ready");
  client.subscribe("projector/commands");
 
}
 
void MQTTcallback(char* topic, byte* payload, unsigned int length) {
 
  RS232Out.begin(19200);
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");

  String message;
  for (int i = 0; i < length; i++) {
    message = message + (char)payload[i];  //Conver *byte to String
  }
   Serial.print(message);
  if(message == "PowerOff") {RS232Out.write(PowerOff);}
  if(message == "PowerOn") {RS232Out.write(PowerOn);}
  if(message == "InputHDMI1") {RS232Out.write(InputHDMI1);}
  if(message == "InputHDMI2") {RS232Out.write(InputHDMI2);}
  if(message == "InputComponent") {RS232Out.write(InputComponent);}
  if(message == "InputSVideo") {RS232Out.write(InputSVideo);}
  if(message == "InputVideo") {RS232Out.write(InputVideo);}
  if(message == "Inputplus") {RS232Out.write(Inputplus);}
  if(message == "Inputmin") {RS232Out.write(Inputmin);}
  if(message == "TestPatternOff") {RS232Out.write(TestPatternOff);}
  if(message == "TestPatternColourBars") {RS232Out.write(TestPatternColourBars);}
  if(message == "TestPatternStairstepblackandwhite") {RS232Out.write(TestPatternStairstepblackandwhite);}
  if(message == "TestPatternStairstepred") {RS232Out.write(TestPatternStairstepred);}
  if(message == "TestPatternStairstepgreen") {RS232Out.write(TestPatternStairstepgreen);}
  if(message == "TestPatternStairstepblue") {RS232Out.write(TestPatternStairstepblue);}
  if(message == "TestPatternCrosshatchgreen") {RS232Out.write(TestPatternCrosshatchgreen);}
  if(message == "GammaNormal") {RS232Out.write(GammaNormal);}
  if(message == "GammaA") {RS232Out.write(GammaA);}
  if(message == "GammaB") {RS232Out.write(GammaB);}
  if(message == "GammaC") {RS232Out.write(GammaC);}
  if(message == "GammaCustom1") {RS232Out.write(GammaCustom1);}
  if(message == "GammaCustom2") {RS232Out.write(GammaCustom2);}
  if(message == "GammaCustom3") {RS232Out.write(GammaCustom3);}
  if(message == "GammaCorrectionValue18") {RS232Out.write(GammaCorrectionValue18);}
  if(message == "GammaCorrectionValue19") {RS232Out.write(GammaCorrectionValue19);}
  if(message == "GammaCorrectionValue20") {RS232Out.write(GammaCorrectionValue20);}
  if(message == "GammaCorrectionValue21") {RS232Out.write(GammaCorrectionValue21);}
  if(message == "GammaCorrectionValue22Default") {RS232Out.write(GammaCorrectionValue22Default);}
  if(message == "GammaCorrectionValue23") {RS232Out.write(GammaCorrectionValue23);}
  if(message == "GammaCorrectionValue24") {RS232Out.write(GammaCorrectionValue24);}
  if(message == "GammaCorrectionValue25") {RS232Out.write(GammaCorrectionValue25);}
  if(message == "GammaCorrectionValue26") {RS232Out.write(GammaCorrectionValue26);}
  if(message == "PictureModeCinema1") {RS232Out.write(PictureModeCinema1);}
  if(message == "PictureModeCinema2") {RS232Out.write(PictureModeCinema2);}
  if(message == "PictureModeCinema3") {RS232Out.write(PictureModeCinema3);}
  if(message == "PictureModeNatural") {RS232Out.write(PictureModeNatural);}
  if(message == "PictureModeStage") {RS232Out.write(PictureModeStage);}
  if(message == "PictureModeDynamic") {RS232Out.write(PictureModeDynamic);}
  if(message == "PictureModeUser1") {RS232Out.write(PictureModeUser1);}
  if(message == "PictureModeUser2") {RS232Out.write(PictureModeUser2);}
  if(message == "VerticalStretchOff") {RS232Out.write(VerticalStretchOff);}
  if(message == "VerticalStretchOn") {RS232Out.write(VerticalStretchOn);}
  if(message == "Aspect169") {RS232Out.write(Aspect169);}
  if(message == "Aspect43") {RS232Out.write(Aspect43);}
  if(message == "AspectZoom") {RS232Out.write(AspectZoom);}
  if(message == "Aspectcyclesthroughallavailablemodes") {RS232Out.write(Aspectcyclesthroughallavailablemodes);}
  if(message == "BackStepsbackwardsthroughmenusandremovesanyOSDmessages") {RS232Out.write(BackStepsbackwardsthroughmenusandremovesanyOSDmessages);}
  if(message == "BNRBlockNoiseReductionOff") {RS232Out.write(BNRBlockNoiseReductionOff);}
  if(message == "BNRBlockNoiseReductionOn") {RS232Out.write(BNRBlockNoiseReductionOn);}
  if(message == "BrightnessMin") {RS232Out.write(BrightnessMin);}
  if(message == "BrightnessPlus") {RS232Out.write(BrightnessPlus);}
  if(message == "BrightnessAdjAdjustmentBarOnOfftoggle") {RS232Out.write(BrightnessAdjAdjustmentBarOnOfftoggle);}
  if(message == "CECOff") {RS232Out.write(CECOff);}
  if(message == "CECOn") {RS232Out.write(CECOn);}
  if(message == "ColourMin") {RS232Out.write(ColourMin);}
  if(message == "ColourPlus") {RS232Out.write(ColourPlus);}
  if(message == "ColourAdjAdjustmentBarOnOfftoggle") {RS232Out.write(ColourAdjAdjustmentBarOnOfftoggle);}
  if(message == "ColourTemp5800K") {RS232Out.write(ColourTemp5800K);}
  if(message == "ColourTemp6500K") {RS232Out.write(ColourTemp6500K);}
  if(message == "ColourTemp7500K") {RS232Out.write(ColourTemp7500K);}
  if(message == "ColourTemp9300K") {RS232Out.write(ColourTemp9300K);}
  if(message == "ColourTempCustom1") {RS232Out.write(ColourTempCustom1);}
  if(message == "ColourTempCustom2") {RS232Out.write(ColourTempCustom2);}
  if(message == "ColourTempCustom3") {RS232Out.write(ColourTempCustom3);}
  if(message == "ColourTempHighBright") {RS232Out.write(ColourTempHighBright);}
  if(message == "ColourTempcyclesthroughalloptions") {RS232Out.write(ColourTempcyclesthroughalloptions);}
  if(message == "ContrastMin") {RS232Out.write(ContrastMin);}
  if(message == "ContrastPlus") {RS232Out.write(ContrastPlus);}
  if(message == "ContrastAdjAdjustmentBarOnOfftoggle") {RS232Out.write(ContrastAdjAdjustmentBarOnOfftoggle);}
  if(message == "CTIColourTransientImprovementOff") {RS232Out.write(CTIColourTransientImprovementOff);}
  if(message == "CTIColourTransientImprovementLow") {RS232Out.write(CTIColourTransientImprovementLow);}
  if(message == "CTIColourTransientImprovementMiddle") {RS232Out.write(CTIColourTransientImprovementMiddle);}
  if(message == "CTIColourTransientimprovementHigh") {RS232Out.write(CTIColourTransientimprovementHigh);}
  if(message == "CursorDown") {RS232Out.write(CursorDown);}
  if(message == "CursorLeft") {RS232Out.write(CursorLeft);}
  if(message == "CursorRight") {RS232Out.write(CursorRight);}
  if(message == "CursorUp") {RS232Out.write(CursorUp);}
  if(message == "DetailEnhanceMin") {RS232Out.write(DetailEnhanceMin);}
  if(message == "DetailEnhancePlus") {RS232Out.write(DetailEnhancePlus);}
  if(message == "Gammacyclesthroughalloptions") {RS232Out.write(Gammacyclesthroughalloptions);}
  if(message == "HideOnOfftoggle") {RS232Out.write(HideOnOfftoggle);}
  if(message == "InformationdisplaysInformationtabofmenu") {RS232Out.write(InformationdisplaysInformationtabofmenu);}
  if(message == "Inputcyclesthroughallavailableinputs") {RS232Out.write(Inputcyclesthroughallavailableinputs);}
  if(message == "KeystoneCorrectionHorizontalMin") {RS232Out.write(KeystoneCorrectionHorizontalMin);}
  if(message == "KeystoneCorrectionHorizontalPlus") {RS232Out.write(KeystoneCorrectionHorizontalPlus);}
  if(message == "KeystoneCorrectionVerticalMin") {RS232Out.write(KeystoneCorrectionVerticalMin);}
  if(message == "KeystoneCorrectionVerticalPlus") {RS232Out.write(KeystoneCorrectionVerticalPlus);}
  if(message == "LensAperture1") {RS232Out.write(LensAperture1);}
  if(message == "LensAperture2") {RS232Out.write(LensAperture2);}
  if(message == "LensAperture3") {RS232Out.write(LensAperture3);}
  if(message == "LensApertureAdj") {RS232Out.write(LensApertureAdj);}
  if(message == "LensControlcyclesthroughalloptions") {RS232Out.write(LensControlcyclesthroughalloptions);}
  if(message == "LensFocusMin") {RS232Out.write(LensFocusMin);}
  if(message == "LensFocusPlus") {RS232Out.write(LensFocusPlus);}
  if(message == "LensShiftDown") {RS232Out.write(LensShiftDown);}
  if(message == "LensShiftLeft") {RS232Out.write(LensShiftLeft);}
  if(message == "LensShiftRight") {RS232Out.write(LensShiftRight);}
  if(message == "LensShiftUp") {RS232Out.write(LensShiftUp);}
  if(message == "LensZooPlus") {RS232Out.write(LensZooPlus);}
  if(message == "LensZoomOut") {RS232Out.write(LensZoomOut);}
  if(message == "MenuOnOfftoggle") {RS232Out.write(MenuOnOfftoggle);}
  if(message == "MNRMosquitoNoiseReductionMin") {RS232Out.write(MNRMosquitoNoiseReductionMin);}
  if(message == "MNRMosquitoNoiseReductionPlus") {RS232Out.write(MNRMosquitoNoiseReductionPlus);}
  if(message == "NRtogglesdisplayofRNRMNR") {RS232Out.write(NRtogglesdisplayofRNRMNR);}
  if(message == "OKtoacceptcurrentlyselectedoption") {RS232Out.write(OKtoacceptcurrentlyselectedoption);}
  if(message == "PowerOffsendtwicewithshortdelaybetweentoswitchoff") {RS232Out.write(PowerOffsendtwicewithshortdelaybetweentoswitchoff);}
  if(message == "RNRRandomNoiseReductionMin") {RS232Out.write(RNRRandomNoiseReductionMin);}
  if(message == "RNRRandomNoiseReductionPlus") {RS232Out.write(RNRRandomNoiseReductionPlus);}
  if(message == "SharpnessMin") {RS232Out.write(SharpnessMin);}
  if(message == "SharpnessPlus") {RS232Out.write(SharpnessPlus);}
  if(message == "SharpnessAdj") {RS232Out.write(SharpnessAdj);}
  if(message == "TestPatterncyclesthroughallpatterns") {RS232Out.write(TestPatterncyclesthroughallpatterns);}
 
  Serial.println();
  Serial.println("-----------------------");  
}
 
void loop() {
  client.loop();
}
