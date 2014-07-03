/*
  Singing Mannequin
  listen to audio, flash some LEDs
  
  This is intended to flash along with an audio sample, then black
  out when the sample stops. It's not great at flashing along with
  continuous music...
*/

#define WHITEPIN 3

int incomingAudio;
int maxAudio;
int brightness;
int oldBrightness;
int zeros;

void setup() {
  pinMode(WHITEPIN, OUTPUT);
  pinMode(A0, INPUT);
  Serial.begin(9600);
}


void loop() {
  
  oldBrightness = brightness;
  
  incomingAudio = analogRead(A0);
  
  // record the max audio signal for debugging and calibration
  if (incomingAudio > maxAudio) {
    maxAudio = incomingAudio;
    Serial.print("Max audio: ");
    Serial.println(maxAudio);
  }
  
  // incoming signal is 0-1023, outgoing is 0-255
  brightness = (int)((float)incomingAudio / 4);
  
  // Serial.print(incomingAudio);
  // Serial.print(" / ");
  // Serial.println(brightness);
  // Serial.println("---");
  
  if (brightness > 255) {
    brightness = 255;
  }
  
  // fade down a bit (less strobe)
  if (brightness < (oldBrightness * 0.8)) {
    brightness = oldBrightness * 0.8;
  }
  
  // ignore random low-level signals
  if (brightness < 10) {
    brightness = 0;
  }
  
  // On average half of the incoming values are going to be negative,
  // which is recorded as zero. To deal with this in a vague and hacky
  // manner, wait until we've had 15 zero signals before sending a zero.
  // I guess it's like a 150ms hold on a compressor.
  // 
  if (brightness == 0) {
    if (zeros++ > 15) {
      analogWrite(WHITEPIN, brightness);
      zeros = 0;
    }
  } else {
    zeros = 0;
    analogWrite(WHITEPIN, brightness);
  }
  delay(10);
}

