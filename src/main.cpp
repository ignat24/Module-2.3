#include <Arduino.h>

enum class LedState {
    Off,
    On
};

struct Config {
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
        uint32_t currentTime = 0;
        uint32_t lastBlinkTime = 0;
        LedState ledState = LedState::Off;
        
        void set(LedState state) {
            digitalWrite(pin, state == LedState::On ? HIGH : LOW);
        }

    public:
        constexpr LED(uint8_t ledPin, uint32_t ledInterval)
            : pin(ledPin), interval(ledInterval) {}

        void init() {
            pinMode(pin, OUTPUT);
            set(ledState);
        }

        void blinkLed() {
            uint32_t currentTime = millis();
            if (currentTime - lastBlinkTime >= interval) {
                lastBlinkTime += interval;
                if (ledState == LedState::On) {
                ledState = LedState::Off;
                } else {
                ledState = LedState::On;
                }
                set(ledState);
            }
        }

        static void task(void* parameter) {
            LED* led = static_cast<LED*>(parameter);
            while (true) {
            led->blinkLed();
            vTaskDelay(pdMS_TO_TICKS(led->interval));
            }
        }
};

static LED red(Config::LED_RED, Config::INTERVAL_RED);
static LED blue(Config::LED_BLUE, Config::INTERVAL_BLUE);
static LED white(Config::LED_WHITE, Config::INTERVAL_WHITE);


void setup() {
    red.init();
    blue.init();
    white.init();
    xTaskCreate(LED::task, "Red", 2048, &red, 1, nullptr);
    xTaskCreate(LED::task, "Blue", 2048, &blue, 1, nullptr);
    xTaskCreate(LED::task, "White", 2048, &white, 1, nullptr);
}

void loop() {
    vTaskDelay(pdMS_TO_TICKS(1000));
}
