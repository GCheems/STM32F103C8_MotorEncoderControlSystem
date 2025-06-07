#include "MotorEncoderSystem.h"

MotorEncoderSystem::MotorEncoderSystem() :
    key1Button(PIN_KEY1),
    key2Button(PIN_KEY2),
    encoder(PIN_ENCODER_A, PIN_ENCODER_B),
    motor(PIN_MOTOR_PWM1, PIN_MOTOR_PWM2),
    oledDisplay()
{
    lastStatusTime = 0;
    lastMotorUpdateTime = 0;
    systemInitialized = false;
}

MotorEncoderSystem::~MotorEncoderSystem() {
    // 确保在析构时停止电机
    if (systemInitialized) {
        motor.forceStop();
    }
}

void MotorEncoderSystem::begin() {
    // 初始化串口 - 移到最前面确保输出正常
    Serial.begin(SERIAL_BAUDRATE);
    delay(1000); // 增加延时确保串口稳定
    
    // 发送基本的启动信息
    Serial.println("======================================");
    Serial.println("STM32F103C8 电机编码器控制系统启动");
    Serial.println("======================================");
    
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("开始初始化各模块...");
    #endif
    
    // 初始化按键
    key1Button.begin();
    key2Button.begin();
    
    // 初始化编码器
    encoder.begin();
    
    // 初始化电机控制器
    motor.begin();
    
    // 初始化OLED显示屏
    if (!oledDisplay.begin()) {
        #if DEBUG_ENABLED
        DEBUG_PRINTLN("警告：OLED显示屏初始化失败");
        #endif
    }
    
    systemInitialized = true;
    
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("======================================");
    DEBUG_PRINTLN("系统初始化完成！");
    DEBUG_PRINTLN("使用说明：");
    DEBUG_PRINTLN("- KEY1 (PB12): 启动电机");
    DEBUG_PRINTLN("- KEY2 (PB13): 停止电机");
    DEBUG_PRINTLN("- 旋转编码器: 控制速度和方向");
    DEBUG_PRINTLN("======================================");
    #endif
    
    // 系统自检
    if (!selfTest()) {
        #if DEBUG_ENABLED
        DEBUG_PRINTLN("警告：系统自检发现问题！");
        #endif
    }
    
    delay(500);
    printStatus();
}

void MotorEncoderSystem::loop() {
    if (!systemInitialized) return;
    
    // 处理按键输入
    handleButtons();
    
    // 处理编码器输入
    handleEncoder();
    
    // 更新电机状态
    updateMotor();
    
    // 更新OLED显示
    oledDisplay.update();
    
    // 定期打印状态和心跳
    if (millis() - lastStatusTime > SYSTEM_STATUS_INTERVAL) {
        Serial.print("系统运行中... 运行时间: ");
        Serial.print(millis());
        Serial.println(" ms");
        printStatus();
        
        // 更新OLED状态显示
        oledDisplay.showStatus(motor.isMotorEnabled(), encoder.getPosition(), motor.getSpeed());
        
        lastStatusTime = millis();
    }
    
    // 主循环延时
    delay(MAIN_LOOP_DELAY);
}

void MotorEncoderSystem::handleButtons() {
    key1Button.update();
    key2Button.update();
    
    // 处理KEY1（启动按键）
    if (key1Button.isClicked()) {
        Serial.println("KEY1 按键被按下！");
        oledDisplay.println("KEY1 Pressed");
        if (!motor.isMotorEnabled()) {
            motor.start();
            // 启动时设置编码器位置为默认值，确保电机有初始运动
            encoder.setPosition(ENCODER_DEFAULT_POS);
            motor.setSpeedFromEncoder(encoder.getPosition());
            
            Serial.println("电机已启动");
            oledDisplay.println("Motor ON");
        } else {
            Serial.println("电机已经在运行中");
            oledDisplay.println("Already ON");
        }
    }
    
    // 处理KEY2（停止按键）
    if (key2Button.isClicked()) {
        Serial.println("KEY2 按键被按下！");
        oledDisplay.println("KEY2 Pressed");
        if (motor.isMotorEnabled()) {
            motor.forceStop();
            Serial.println("电机已停止");
            oledDisplay.println("Motor OFF");
        } else {
            Serial.println("电机已经处于停止状态");
            oledDisplay.println("Already OFF");
        }
    }
}

void MotorEncoderSystem::handleEncoder() {
    encoder.update();
    
    // 如果编码器位置发生变化且电机已启用，更新电机速度
    if (encoder.hasChanged() && motor.isMotorEnabled()) {
        int position = encoder.getPosition();
        motor.setSpeedFromEncoder(position);
        
        // 在OLED上显示编码器变化
        oledDisplay.println("Encoder: " + String(position));
        
        #if DEBUG_ENCODER && DEBUG_ENABLED
        DEBUG_PRINTF_1("编码器位置变化: ", position);
        #endif
    }
}

void MotorEncoderSystem::updateMotor() {
    // 定期更新电机状态
    if (millis() - lastMotorUpdateTime > MOTOR_UPDATE_INTERVAL) {
        motor.update();
        lastMotorUpdateTime = millis();
    }
}

void MotorEncoderSystem::printStatus() {
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("------------- 系统状态 -------------");
    DEBUG_PRINT("编码器位置: ");
    DEBUG_PRINT(encoder.getPosition());
    DEBUG_PRINT(" [");
    DEBUG_PRINT(encoder.getPercentage());
    DEBUG_PRINTLN("%]");
    
    DEBUG_PRINT("电机状态: ");
    DEBUG_PRINT(motor.getStateString());
    DEBUG_PRINT(" (速度: ");
    DEBUG_PRINT(motor.getSpeed());
    DEBUG_PRINTLN(")");
    
    DEBUG_PRINT("电机启用: ");
    DEBUG_PRINTLN(motor.isMotorEnabled() ? "是" : "否");
    
    DEBUG_PRINT("系统健康: ");
    DEBUG_PRINTLN(isSystemHealthy() ? "正常" : "异常");
    
    DEBUG_PRINT("运行时间: ");
    DEBUG_PRINT(millis());
    DEBUG_PRINTLN(" ms");
    DEBUG_PRINTLN("-------------------------------------");
    #endif
}

bool MotorEncoderSystem::selfTest() {
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("开始系统自检...");
    #endif
    
    bool testResult = true;
    
    // 电机自检
    if (!motor.selfTest()) {
        #if DEBUG_ENABLED
        DEBUG_PRINTLN("错误：电机自检失败");
        #endif
        testResult = false;
    }
    
    // 编码器基本检测
    int encoderPos = encoder.getPosition();
    if (encoderPos < ENCODER_MIN_POS || encoderPos > ENCODER_MAX_POS) {
        #if DEBUG_ENABLED
        DEBUG_PRINT("警告：编码器位置异常 (");
        DEBUG_PRINT(encoderPos);
        DEBUG_PRINTLN(")");
        #endif
        encoder.reset(); // 重置到默认位置
    }
    
    #if DEBUG_ENABLED
    if (testResult) {
        DEBUG_PRINTLN("系统自检通过");
    } else {
        DEBUG_PRINTLN("系统自检失败");
    }
    #endif
    
    return testResult;
}

void MotorEncoderSystem::emergencyStop() {
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("!!! 紧急停止 !!!");
    #endif
    
    motor.forceStop();
    
    // 重置按键状态
    key1Button.reset();
    key2Button.reset();
}

void MotorEncoderSystem::reset() {
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("重置系统到初始状态");
    #endif
    
    // 停止电机
    motor.forceStop();
    
    // 重置编码器
    encoder.reset();
    
    // 重置按键
    key1Button.reset();
    key2Button.reset();
    
    // 重置时间戳
    lastStatusTime = 0;
    lastMotorUpdateTime = 0;
    
    delay(100);
    
    #if DEBUG_ENABLED
    DEBUG_PRINTLN("系统重置完成");
    #endif
}

Button& MotorEncoderSystem::getButton(int keyNumber) {
    switch (keyNumber) {
        case 1:
            return key1Button;
        case 2:
            return key2Button;
        default:
            return key1Button; // 默认返回key1
    }
}

RotaryEncoder& MotorEncoderSystem::getEncoder() {
    return encoder;
}

MotorController& MotorEncoderSystem::getMotor() {
    return motor;
}

OledDisplay& MotorEncoderSystem::getDisplay() {
    return oledDisplay;
}

bool MotorEncoderSystem::isSystemHealthy() {
    if (!systemInitialized) return false;
    
    // 检查编码器位置是否在有效范围内
    int encoderPos = encoder.getPosition();
    if (encoderPos < ENCODER_MIN_POS || encoderPos > ENCODER_MAX_POS) {
        return false;
    }
    
    return true;
}

void MotorEncoderSystem::setDebugMode(bool enabled) {
    // 这个功能需要在config.h中修改DEBUG_ENABLED定义
    // 或者可以添加运行时调试控制变量
    #if DEBUG_ENABLED
    if (enabled) {
        DEBUG_PRINTLN("调试模式已启用");
    } else {
        DEBUG_PRINTLN("调试模式已禁用");
    }
    #endif
} 