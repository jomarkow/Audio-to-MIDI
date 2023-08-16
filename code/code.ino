#define SINTHETIC 1  // 1 for sinthetic signal, 0 for real audio input

// --------------- Libraries ------------------
#include <MIDI.h>
#include "arduinoFFT.h"
arduinoFFT FFT;

// INPUT
#define Audio_input A2  // Replace "A2" with your current input pin

// ------------ global valriables & defines --------------
#define SAMPLES 8192
#define SAMPLING_FREQ 41100
#define AMPLITUDE 100
double vReal[SAMPLES];
double vImag[SAMPLES];
float FFT_frequency;
int signalFrequency = 0;
double cycles;
int reading;
float Signal;

// FFT to MIDI
#define NOTES_LENGHT 30

float NOTES_FREQ[] = { 103.826, 110.000, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614,
                      184.997, 195.998, 207.652, 220.000, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127,
                      329.628, 349.228, 369.994, 391.995, 415.305, 440.000, 466.164, 493.883, 523.251, 554.365,
                      587.330, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.000, 932.328, 987.767 };

// MIDI
int note;
int last_note;


/**
 * Creates a sinthetic wave
 */

void Sinthetic_signal() {

  signalFrequency = random(0, 1000);
  delay(300);

  cycles = (((SAMPLES - 1) * signalFrequency) / SAMPLING_FREQ);

  for (int i = 0; i < SAMPLES; i++) {
    Signal = int((AMPLITUDE * (sin((i * (twoPi * cycles)) / SAMPLES))) / 2.0); // create the sine wave
    vReal[i] = Signal;
    vImag[i] = 0.0;
  }
}

/**
 * Reads the signal 
 *
 * it reads the input periodically and stores it.
 */

unsigned int SAMPLING_PERIOD = round(1000000 * (1.0 / SAMPLING_FREQ));
unsigned long elapsed_time;

void Audio_signal() {

  for (int i = 0; i < SAMPLES; i++) {
    reading = analogRead(Audio_input);
    vReal[i] = reading;  
    vImag[i] = 0.0;   
    while (micros() - elapsed_time < SAMPLING_PERIOD) {
      //empty loop
    }
    elapsed_time += SAMPLING_PERIOD;
  }
}


/**
 * Calculates the dominant frequency of a signal.
 *
 * It uses FFT method with windowing for more accuracy.
 * 
 * @return the dominant frecuency of the signal.
 */

float Calculate_FFT() {

  FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  double dominantFrequency = FFT.MajorPeak();
  return dominantFrequency;
}

/**
 * Convert the dominant frequency to a musical note.
 *
 * Finds the closest note for a given frequency, looking
 * on a previous defined array with every note.
 * 
 * @param freq Dominant frequency of the signal.
 * @return the note of a frequency.
 */

#define MAX_NOTE_FREQ 987.767
#define MIN_NOTE_FREQ 103.826
#define OCTAVES_FITTING 55

float searchForNote(float freq) {

  if (freq > MAX_NOTE_FREQ || freq < MIN_NOTE_FREQ) return 0;

  for (int i = 1; i < NOTES_LENGHT - 1; i++) {

    if (freq > NOTES_FREQ[i - 1] && freq < NOTES_FREQ[i]) {
      if (freq - NOTES_FREQ[i - 1] < NOTES_FREQ[i] - freq) return i + OCTAVES_FITTING;
      else return i + OCTAVES_FITTING + 1;
    }
  }
}

/**
 * Send MIDI message to PC through USB
 * 
 * @param note value to send.
 * @param last_note previous value sent.
 */

#define MIDI_VELOCITY 64
#define MIDI_CHANNEL 1

void send_MIDI(int note, int last_note) {

  if (note >= 0) {

    usbMIDI.sendNoteOn(note, MIDI_VELOCITY, MIDI_CHANNEL);

    if (note != last_note) {
      usbMIDI.sendNoteOff(last_note, 0, MIDI_CHANNEL);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(Audio_input, INPUT);
}

void loop() {

  if (SINTHETIC) Sinthetic_signal();
  else Audio_signal();

  FFT_frequency = Calculate_FFT();
  note = searchForNote(FFT_frequency);
  send_MIDI(note, last_note);
  last_note = note;

  if (DEBUG) {
    if (SINTHETIC) {
      Serial.print("The frequency is: ");
      Serial.print(signalFrequency);
    } else {
      Serial.println(reading); 
    }
    Serial.print(" Detected note: ");
    Serial.print(note);
    Serial.println("Dominant Frequency: ");
    Serial.print(FFT_frequency, 6);  // 6 decimal print
    Serial.println(" Hz");
  }
}
