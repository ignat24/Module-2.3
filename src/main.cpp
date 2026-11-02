#include <Arduino.h>

enum class LedState {
    Off,
    On
};

class CONFIG {
public:
    static constexpr uint8_t LED_RED = 17;
    static constexpr uint8_t LED_BLUE = 15;
    static constexpr uint8_t LED_WHITE = 16;
    static constexpr uint32_t INTERVAL_RED = 200;
    static constexpr uint32_t INTERVAL_BLUE = 500;
    static constexpr uint32_t INTERVAL_WHITE = 1000;
};

class LED {
private:
    uint8_t pin;
    uint32_t interval;
    uint32_t currentTime;
    uint32_t lastBlinkTime = 0;
    LedState ledState = LedState::Off;
    
    void set(LedState state) {
        digitalWrite(pin, state == LedState::On ? HIGH : LOW);
    }

public:
    LED(uint8_t ledPin, uint32_t ledInterval)
        : pin(ledPin), interval(ledInterval) {}

    void init() {
        pinMode(pin, OUTPUT);
        set(ledState);
    }

    void blinkLed(uint32_t currentTime) {
      if (currentTime - lastBlinkTime >= interval) {
        lastBlinkTime = currentTime;
        if (ledState == LedState::On) {
          ledState = LedState::Off;
        } else {
          ledState = LedState::On;
        }
        set(ledState);
      }
    }
};

LED red(CONFIG::LED_RED, CONFIG::INTERVAL_RED);
LED blue(CONFIG::LED_BLUE, CONFIG::INTERVAL_BLUE);
LED white(CONFIG::LED_WHITE, CONFIG::INTERVAL_WHITE);


void setup() {
  red.init();
  blue.init();
  white.init();
}

void loop() {
    uint32_t currentTime = millis();
    red.blinkLed(currentTime);
    blue.blinkLed(currentTime);
    white.blinkLed(currentTime);
}
