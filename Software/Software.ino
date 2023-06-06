
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// Create the Audio components.  These should be created in the
// order data flows, inputs/sources -> processing -> outputs
//
AudioInputI2S          audioInput;         // audio shield: mic or line-in
AudioSynthWaveformSine sinewave;
AudioAnalyzeFFT1024    myFFT;
AudioOutputI2S         audioOutput;        // audio shield: headphones & line-out

// Connect either the live input or synthesized sine wave
AudioConnection patchCord1(audioInput, 0, myFFT, 0);
//AudioConnection patchCord1(sinewave, 0, myFFT, 0);

AudioControlSGTL5000 audioShield;

float n;
int maxIndex = 0;
float maxValue = 0.0;

void setup() {
  // Audio connections require memory to work.  For more
  // detailed information, see the MemoryAndCpuUsage example
  AudioMemory(12);

  // Enable the audio shield and set the output volume.
  audioShield.enable();
  audioShield.inputSelect(myInput);
  audioShield.volume(0.5);

  // Configure the window algorithm to use
  myFFT.windowFunction(AudioWindowHanning1024);
  //myFFT.windowFunction(NULL);

Serial.begin(9600); // Iniciar comunicación 
}

void loop() {
  int freq_random = random(100, 1000); 
  float amp = random(50,150)/100; 
  // Create a synthetic sine wave, for testing
  // To use this, edit the connections above
  sinewave.amplitude(amp);
  sinewave.frequency(freq_random);
  
 
  if (myFFT.available()) {
    // each time new FFT data is available
    // print it all to the Arduino Serial Monitor
    Serial.print("FFT: ");
    for (int i=0; i<41; i++) {
      n = myFFT.read(i);
      Serial.print(n);
      Serial.print(" ");
      if (n >= maxValue) {
        maxValue = n;
        maxIndex = i;
      }
    }
    Serial.println();
 unsigned int sampleRate;
 sampleRate = 44100;//La frecuencia de muestreo predeterminada para el Audio Shield SGTL5000 es de 44.1 kHz 
    float frequency = (maxIndex * sampleRate) / 1024;
    /*
    Serial.print(maxIndex);
    Serial.print (" Frecuencia base: "); 
    Serial.print (freq_random); 
    Serial.print(" Hz. ");
    Serial.print("Frecuencia principal: ");
    Serial.print(frequency);
    Serial.println(" Hz");
    */
  }
}
