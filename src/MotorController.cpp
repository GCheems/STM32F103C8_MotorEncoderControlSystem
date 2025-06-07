#include "MotorController.h"

MotorController::MotorController(uint8_t motorPWM1, uint8_t motorPWM2) {
    pinPWM1 = motorPWM1;
    pinPWM2 = motorPWM2;
    currentState = MotorState::STOPPED;
    currentSpeed = 0;
    isEnabled = false;
    lastUpdateTime = 0;
}

void MotorController::begin() {
    pinMode(pinPWM1, OUTPUT);
    pinMode(pinPWM2, OUTPUT);
    
    // 初始化为停止状态
    forceStop();
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTF_2("Motor initialized: PWM1=", pinPWM1, pinPWM2);
    #endif
}

void MotorController::setPWM(int pwm1Value, int pwm2Value) {
    // 确保PWM值在有效范围内
    pwm1Value = constrain(pwm1Value, 0, 255);
    pwm2Value = constrain(pwm2Value, 0, 255);
    
    analogWrite(pinPWM1, pwm1Value);
    analogWrite(pinPWM2, pwm2Value);
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    if (millis() - lastUpdateTime > MOTOR_UPDATE_INTERVAL) {
        DEBUG_PRINTF_2("Motor PWM: PWM1=", pwm1Value, pwm2Value);
        lastUpdateTime = millis();
    }
    #endif
}

void MotorController::update() {
    // 可以在这里添加电机状态监控逻辑
    // 例如：过热保护、过流保护等
}

void MotorController::start() {
    isEnabled = true;
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTLN("Motor started");
    #endif
}

void MotorController::stop() {
    isEnabled = false;
    setPWM(0, 0);
    currentState = MotorState::STOPPED;
    currentSpeed = 0;
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTLN("Motor stopped");
    #endif
}

void MotorController::forceStop() {
    // 立即停止电机，无论状态如何
    digitalWrite(pinPWM1, LOW);
    digitalWrite(pinPWM2, LOW);
    analogWrite(pinPWM1, 0);
    analogWrite(pinPWM2, 0);
    
    isEnabled = false;
    currentState = MotorState::STOPPED;
    currentSpeed = 0;
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTLN("Motor force stopped");
    #endif
}

void MotorController::setSpeed(int speed) {
    if (!isEnabled) {
        return; // 电机未启用时不响应速度设置
    }
    
    // 限制速度范围
    speed = constrain(speed, -255, 255);
    currentSpeed = speed;
    
    if (speed == 0) {
        // 停止
        setPWM(0, 0);
        currentState = MotorState::STOPPED;
    } else if (speed > 0) {
        // 正转
        setPWM(speed, 0);
        currentState = MotorState::FORWARD;
    } else {
        // 反转
        setPWM(0, -speed);
        currentState = MotorState::REVERSE;
    }
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINT("Motor speed set to: ");
    DEBUG_PRINT(speed);
    DEBUG_PRINT(" (");
    DEBUG_PRINT(getStateString());
    DEBUG_PRINTLN(")");
    #endif
}

void MotorController::setDirection(bool forward) {
    if (!isEnabled) {
        return;
    }
    
    int speedValue = abs(currentSpeed);
    if (speedValue == 0) {
        speedValue = MOTOR_MIN_SPEED; // 使用最小速度
    }
    
    if (forward) {
        setSpeed(speedValue);
    } else {
        setSpeed(-speedValue);
    }
}

int MotorController::getSpeed() const {
    return currentSpeed;
}

MotorState MotorController::getState() const {
    return currentState;
}

bool MotorController::isMotorEnabled() const {
    return isEnabled;
}

bool MotorController::isRunning() const {
    return (currentState != MotorState::STOPPED) && isEnabled;
}

void MotorController::setSpeedFromEncoder(int encoderPosition) {
    if (!isEnabled) {
        return;
    }
    
    if (encoderPosition == 0) {
        setSpeed(0);
        return;
    }
    
    // 计算速度：基础速度 + 编码器位置调节
    int calculatedSpeed = MOTOR_BASE_SPEED + abs(encoderPosition) * MOTOR_SPEED_MULTIPLIER;
    calculatedSpeed = constrain(calculatedSpeed, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
    
    // 根据编码器位置确定方向
    if (encoderPosition > 0) {
        setSpeed(calculatedSpeed);  // 正转
    } else {
        setSpeed(-calculatedSpeed); // 反转
    }
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINT("Encoder pos=");
    DEBUG_PRINT(encoderPosition);
    DEBUG_PRINT(" -> Motor speed=");
    DEBUG_PRINTLN((encoderPosition > 0) ? calculatedSpeed : -calculatedSpeed);
    #endif
}

const char* MotorController::getStateString() const {
    switch (currentState) {
        case MotorState::STOPPED:
            return "STOPPED";
        case MotorState::FORWARD:
            return "FORWARD";
        case MotorState::REVERSE:
            return "REVERSE";
        default:
            return "UNKNOWN";
    }
}

bool MotorController::selfTest() {
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTLN("Motor self test starting...");
    #endif
    
    // 测试正转
    setPWM(100, 0);
    delay(100);
    
    // 测试反转
    setPWM(0, 100);
    delay(100);
    
    // 停止
    setPWM(0, 0);
    delay(100);
    
    #if DEBUG_MOTOR && DEBUG_ENABLED
    DEBUG_PRINTLN("Motor self test completed");
    #endif
    
    return true; // 简单实现，实际应用中可以添加更复杂的检测逻辑
} 