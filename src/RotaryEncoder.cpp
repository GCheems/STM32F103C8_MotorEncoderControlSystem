#include "RotaryEncoder.h"

RotaryEncoder::RotaryEncoder(uint8_t encoderPinA, uint8_t encoderPinB, 
                             int minPos, int maxPos, int defaultPos) {
    pinA = encoderPinA;
    pinB = encoderPinB;
    minPosition = minPos;
    maxPosition = maxPos;
    position = defaultPos;
    lastStateA = HIGH;
    lastStateB = HIGH;
    lastDebounceTime = 0;
    positionChanged = false;
}

void RotaryEncoder::begin() {
    pinMode(pinA, INPUT_PULLUP);
    pinMode(pinB, INPUT_PULLUP);
    
    lastStateA = digitalRead(pinA);
    lastStateB = digitalRead(pinB);
    
    #if DEBUG_ENCODER && DEBUG_ENABLED
    DEBUG_PRINT("Encoder initialized: A=");
    DEBUG_PRINT(pinA);
    DEBUG_PRINT(", B=");
    DEBUG_PRINT(pinB);
    DEBUG_PRINT(", pos=");
    DEBUG_PRINT(position);
    DEBUG_PRINT(" [");
    DEBUG_PRINT(minPosition);
    DEBUG_PRINT("-");
    DEBUG_PRINT(maxPosition);
    DEBUG_PRINTLN("]");
    #endif
}

void RotaryEncoder::update() {
    bool currentStateA = digitalRead(pinA);
    bool currentStateB = digitalRead(pinB);
    
    // 检测A相状态变化
    if (currentStateA != lastStateA) {
        // 防抖处理
        if ((millis() - lastDebounceTime) > ENCODER_DEBOUNCE_MS) {
            // A相从高到低或从低到高变化时
            if (currentStateA == LOW) {
                // A相下降沿，检查B相状态确定方向
                if (currentStateB == HIGH) {
                    // 顺时针旋转
                    if (position < maxPosition) {
                        position++;
                        positionChanged = true;
                        
                        #if DEBUG_ENCODER && DEBUG_ENABLED
                        DEBUG_PRINTF_1("Encoder CW: pos=", position);
                        #endif
                    }
                } else {
                    // 逆时针旋转
                    if (position > minPosition) {
                        position--;
                        positionChanged = true;
                        
                        #if DEBUG_ENCODER && DEBUG_ENABLED
                        DEBUG_PRINTF_1("Encoder CCW: pos=", position);
                        #endif
                    }
                }
            } else if (currentStateA == HIGH) {
                // A相上升沿，检查B相状态确定方向
                if (currentStateB == LOW) {
                    // 顺时针旋转
                    if (position < maxPosition) {
                        position++;
                        positionChanged = true;
                        
                        #if DEBUG_ENCODER && DEBUG_ENABLED
                        DEBUG_PRINTF_1("Encoder CW: pos=", position);
                        #endif
                    }
                } else {
                    // 逆时针旋转
                    if (position > minPosition) {
                        position--;
                        positionChanged = true;
                        
                        #if DEBUG_ENCODER && DEBUG_ENABLED
                        DEBUG_PRINTF_1("Encoder CCW: pos=", position);
                        #endif
                    }
                }
            }
            
            lastDebounceTime = millis();
        }
    }
    
    lastStateA = currentStateA;
    lastStateB = currentStateB;
}

int RotaryEncoder::getPosition() const {
    return position;
}

void RotaryEncoder::setPosition(int pos) {
    if (pos >= minPosition && pos <= maxPosition) {
        position = pos;
        positionChanged = true;
        
        #if DEBUG_ENCODER && DEBUG_ENABLED
        DEBUG_PRINTF_1("Encoder position set to: ", position);
        #endif
    }
}

bool RotaryEncoder::hasChanged() {
    if (positionChanged) {
        positionChanged = false; // 清除标志
        return true;
    }
    return false;
}

void RotaryEncoder::reset() {
    position = ENCODER_DEFAULT_POS;
    positionChanged = true;
    
    #if DEBUG_ENCODER && DEBUG_ENABLED
    DEBUG_PRINTF_1("Encoder reset to: ", position);
    #endif
}

void RotaryEncoder::getRange(int& minPos, int& maxPos) const {
    minPos = minPosition;
    maxPos = maxPosition;
}

void RotaryEncoder::setRange(int minPos, int maxPos) {
    minPosition = minPos;
    maxPosition = maxPos;
    
    // 确保当前位置在新范围内
    if (position < minPosition) {
        position = minPosition;
        positionChanged = true;
    } else if (position > maxPosition) {
        position = maxPosition;
        positionChanged = true;
    }
    
    #if DEBUG_ENCODER && DEBUG_ENABLED
    DEBUG_PRINT("Encoder range set to: [");
    DEBUG_PRINT(minPosition);
    DEBUG_PRINT("-");
    DEBUG_PRINT(maxPosition);
    DEBUG_PRINT("], current pos=");
    DEBUG_PRINTLN(position);
    #endif
}

int RotaryEncoder::getPercentage() const {
    if (maxPosition == minPosition) return 0;
    
    return ((position - minPosition) * 100) / (maxPosition - minPosition);
}

bool RotaryEncoder::isPositiveDirection() const {
    return position > 0;
}

bool RotaryEncoder::isNegativeDirection() const {
    return position < 0;
} 