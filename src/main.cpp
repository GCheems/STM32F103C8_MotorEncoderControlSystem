/**
 * @file main.cpp
 * @brief STM32F103C8 电机编码器控制系统主程序 - 静态分配版本
 * 
 * 功能说明：
 * - KEY1 (PB12): 启动电机
 * - KEY2 (PB13): 停止电机  
 * - 旋转编码器 (PA8/PA9): 控制电机速度和方向
 * - 电机驱动 (PA0/PA1): DRV8833驱动器PWM输出
 * 
 * 硬件连接：
 * - 编码器A相: PA8, 编码器B相: PA9
 * - 按键KEY1: PB12, 按键KEY2: PB13
 * - 电机PWM1: PA0, 电机PWM2: PA1
 * - OLED SDA: PB7, OLED SCL: PB6
 * - 所有输入都使用内部上拉电阻
 * 
 * @author 您的名字
 * @date 2024
 */

#include <Arduino.h>
#include "MotorEncoderSystem.h"

// 创建系统控制对象 - 使用静态分配
MotorEncoderSystem motorSystem;

/**
 * @brief 系统初始化函数
 */
void setup() {
    // 初始化整个系统
    motorSystem.begin();
    
    // 等待系统稳定
    delay(1000);
}

/**
 * @brief 主循环函数
 */
void loop() {
    // 系统主循环处理
    motorSystem.loop();
}

/**
 * @brief 串口命令处理（可选）
 * 
 * 可以通过串口发送命令来控制系统
 * 命令格式：
 * - "start" 或 "1": 启动电机
 * - "stop" 或 "0": 停止电机
 * - "reset" 或 "r": 重置系统
 * - "status" 或 "s": 打印状态
 * - "test" 或 "t": 系统自检
 * - "emergency" 或 "e": 紧急停止
 * - "oled" 或 "o": OLED测试
 */
void serialEvent() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        command.toLowerCase();
        
        if (command == "start" || command == "1") {
            motorSystem.getMotor().start();
            motorSystem.getEncoder().setPosition(ENCODER_DEFAULT_POS);
            motorSystem.getMotor().setSpeedFromEncoder(ENCODER_DEFAULT_POS);
            Serial.println("电机已通过串口启动");
            motorSystem.getDisplay().println("CMD: Start");
        }
        else if (command == "stop" || command == "0") {
            motorSystem.getMotor().forceStop();
            Serial.println("电机已通过串口停止");
            motorSystem.getDisplay().println("CMD: Stop");
        }
        else if (command == "reset" || command == "r") {
            motorSystem.reset();
            Serial.println("系统已重置");
            motorSystem.getDisplay().println("CMD: Reset");
        }
        else if (command == "status" || command == "s") {
            motorSystem.printStatus();
            motorSystem.getDisplay().showStatus(
                motorSystem.getMotor().isMotorEnabled(),
                motorSystem.getEncoder().getPosition(),
                motorSystem.getMotor().getSpeed()
            );
        }
        else if (command == "test" || command == "t") {
            if (motorSystem.selfTest()) {
                Serial.println("系统自检通过");
                motorSystem.getDisplay().println("CMD: Test OK");
            } else {
                Serial.println("系统自检失败");
                motorSystem.getDisplay().println("CMD: Test FAIL");
            }
        }
        else if (command == "emergency" || command == "e") {
            motorSystem.emergencyStop();
            Serial.println("紧急停止已执行");
            motorSystem.getDisplay().println("CMD: Emergency");
        }
        else if (command == "oled" || command == "o") {
            motorSystem.getDisplay().showTestPattern();
            Serial.println("OLED测试图案已显示");
        }
        else if (command == "help" || command == "h") {
            Serial.println("可用命令：");
            Serial.println("  start/1     - 启动电机");
            Serial.println("  stop/0      - 停止电机");
            Serial.println("  reset/r     - 重置系统");
            Serial.println("  status/s    - 显示状态");
            Serial.println("  test/t      - 系统自检");
            Serial.println("  emergency/e - 紧急停止");
            Serial.println("  oled/o      - OLED测试");
            Serial.println("  help/h      - 显示帮助");
            motorSystem.getDisplay().println("CMD: Help");
        }
        else {
            Serial.println("未知命令，输入 'help' 查看可用命令");
            motorSystem.getDisplay().println("CMD: Unknown");
        }
    }
}