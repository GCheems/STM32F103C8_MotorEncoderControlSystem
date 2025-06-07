#include "Button.h"

Button::Button(uint8_t buttonPin) {
    pin = buttonPin;
    lastState = HIGH;
    currentState = HIGH;
    lastDebounceTime = 0;
    lastTriggerTime = 0;
    isPressed = false;
    wasPressed = false;
}

void Button::begin() {
    pinMode(pin, INPUT_PULLUP);
    lastState = digitalRead(pin);
    currentState = lastState;
    
    Serial.print("按键初始化完成，引脚: ");
    Serial.print(pin);
    Serial.print(", 初始状态: ");
    Serial.println(lastState == HIGH ? "HIGH" : "LOW");
}

void Button::update() {
    bool reading = digitalRead(pin);
    
    // 如果读取的状态与上次不同，重置防抖计时器
    if (reading != lastState) {
        lastDebounceTime = millis();
    }
    
    // 如果超过防抖时间，认为状态稳定
    if ((millis() - lastDebounceTime) > BUTTON_DEBOUNCE_MS) {
        // 如果按键状态真的改变了
        if (reading != currentState) {
            currentState = reading;
            
            // 检测按键按下（从高电平变为低电平）
            if (currentState == LOW && !wasPressed) {
                // 防止重复触发
                if ((millis() - lastTriggerTime) > BUTTON_REPEAT_DELAY_MS) {
                    isPressed = true;
                    wasPressed = true;
                    lastTriggerTime = millis();
                    
                    #if DEBUG_BUTTONS && DEBUG_ENABLED
                    DEBUG_PRINTF_1("Button clicked: ", pin);
                    #endif
                }
            }
            
            // 按键释放时重置wasPressed标志
            if (currentState == HIGH) {
                wasPressed = false;
            }
        }
    }
    
    lastState = reading;
}

bool Button::isClicked() {
    if (isPressed) {
        isPressed = false; // 清除标志，确保只触发一次
        return true;
    }
    return false;
}

bool Button::isHeld() {
    return (currentState == LOW);
}

void Button::reset() {
    isPressed = false;
    wasPressed = false;
    lastTriggerTime = 0;
}

uint8_t Button::getPin() const {
    return pin;
} 