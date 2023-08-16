# AUDIO TO MIDI in real time

#### Convert your sound into an editable music document.

The objective of this project is to make the user able to convert real sounds, whether of instruments or voice, into a MIDI format in real time. With MIDI you can put these notes into a music editor, like Reaper, and saw them the while you are playing.

#### Made with:

[![Arduino](https://img.shields.io/badge/Arduino_IDE-00979D?style=for-the-badge&logo=Arduino&logoColor=white)](https://www.arduino.cc/)
[![Teensy](https://img.shields.io/badge/Teensy_4.0-206922?style=for-the-badge)](https://www.pjrc.com/store/teensy40.html)
[![Kicad](https://img.shields.io/badge/Kicad-1a2dc7?style=for-the-badge&logo=kicad)](https://www.kicad.org/)

# Software


The most important part of the project. We decide to do it on a teensy because of his memory and fast processment capacity, making posible to create a Fourier Transform for analize the signal and discover the notes in real time.

It tooks the audio and analize it with FFT and a windowing function for making it faster, it returns the dominant frequency. 

```
float Calculate_FFT() {

  FFT = arduinoFFT(vReal, vImag, SAMPLES, SAMPLING_FREQ);
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  double dominantFrequency = FFT.MajorPeak();
  return dominantFrequency;
}
```
<br>

Then, it looks this frequency on a defined array with the corresponding notes for each frequency range.

```
float NOTES_FREQ[] = { 103.826, 110.000, 116.541, 123.471, 130.813, 138.591, 146.832, 155.563, 164.814, 174.614,
                      184.997, 195.998, 207.652, 220.000, 233.082, 246.942, 261.626, 277.183, 293.665, 311.127,
                      329.628, 349.228, 369.994, 391.995, 415.305, 440.000, 466.164, 493.883, 523.251, 554.365,
                      587.330, 622.254, 659.255, 698.456, 739.989, 783.991, 830.609, 880.000, 932.328, 987.767 };
                      }
```

[![e572ef3c-88bd-41c5-94be-9d13599bbda8.jpg](https://i.postimg.cc/PJHggxD0/e572ef3c-88bd-41c5-94be-9d13599bbda8.jpg)](https://postimg.cc/WF9YrsX7)

```
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
```
# Electronic filter

The project has also an electronic board who contains a circuit with:
* Amplifier, for make the microphone signal stronger
* Band pass filter between 100hz and 1Khz
* Clamper circuit, for elevate the signal to the positive field

[![69ea6844-90a0-43a4-9217-2764cb34e035.jpg](https://i.postimg.cc/MTbgxwq4/69ea6844-90a0-43a4-9217-2764cb34e035.jpg)](https://postimg.cc/Kkj0rdKr)

# Simulation:

We simulate the circuit on Proteus, here the results:

[![image.png](https://i.postimg.cc/mk1MCLkp/image.png)](https://postimg.cc/p5xpttDK)


As you could see, as more amplitude on the input, the filter gets better on cutting the frequencies out of our range, but in general, it gives us a good result.

# BOM

Component|Quantity|
|:-------------|:-------------|
|Teensy 4.0|1|
|Microfono Electret|1|
|Pot 20K ohm|1|
|R de 10K ohm|5|
|R de 100K ohm|1|
|R de 100 ohm|1|
|R de 1,5K ohm|1|
|R de 1M ohm|1|
|Cap Elec 10u|1|
|Cap Elec 100n|1|
|Cap 100n|1|
|Cap 1u|1|
|Cap 10n|1|
|Cap 33u|1|
|Lm358n|2|
|Lm7805|2|
|2N4001|1|
|L7806|1|
|ld1117|1|



# Colaborate

If you want to colaborate on this project, you are invited to create a Pull Request with a descriptive text of the changes/updates.

You can also contact me: 
[jomarkow@gmail.com](mailto:jomarkow@gmail.com)

I hope you liked the project, if so, I invite you to leave a star ⭐, thanks for read :).
