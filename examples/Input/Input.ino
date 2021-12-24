#include <XE_MIDI.h>

MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI1);

void setup() {
    pinMode(LED, OUTPUT);
    MIDI1.begin();                          // Init MIDI, by default listening to channel 1.
}

void loop() {
    if (MIDI1.read()) {
        switch(MIDI1.getType()) {
            case midi::ProgramChange:       // If it is a Program Change,
                BlinkLed(MIDI1.getData1()); // blink the LED a number of times
                                            // correponding to the program number
                                            // (0 to 127, it can last a while..)
                break;
            default: break;
        }
    }
}

void BlinkLed(uint8_t num) {
    for (uint8_t i=0;i<num;i++) {
        digitalWrite(LED_BUILTIN,HIGH);
        delay(50);
        digitalWrite(LED_BUILTIN,LOW);
        delay(50);
    }
}