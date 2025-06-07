#ifndef MOTOR_ENCODER_SYSTEM_H
#define MOTOR_ENCODER_SYSTEM_H

#include <Arduino.h>
#include "config.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "MotorController.h"
#include "OledDisplay.h"

/**
 * @brief 电机编码器控制系统
 * 
 * 集成按键、编码器和电机控制的完整系统
 * 使用静态分配避免STM32上的内存问题
 */
class MotorEncoderSystem {
private:
    // 使用静态分配而不是指针
    Button key1Button;                    // KEY1按键（启动）
    Button key2Button;                    // KEY2按键（停止）
    RotaryEncoder encoder;                // 旋转编码器
    MotorController motor;                // 电机控制器
    OledDisplay oledDisplay;              // OLED显示屏
    
    unsigned long lastStatusTime;         // 上次状态报告时间
    unsigned long lastMotorUpdateTime;    // 上次电机更新时间
    bool systemInitialized;              // 系统是否已初始化

public:
    /**
     * @brief 构造函数
     */
    MotorEncoderSystem();
    
    /**
     * @brief 析构函数
     */
    ~MotorEncoderSystem();
    
    /**
     * @brief 初始化系统
     */
    void begin();
    
    /**
     * @brief 主循环处理函数（需要在主循环中调用）
     */
    void loop();
    
    /**
     * @brief 处理按键输入
     */
    void handleButtons();
    
    /**
     * @brief 处理编码器输入
     */
    void handleEncoder();
    
    /**
     * @brief 更新电机状态
     */
    void updateMotor();
    
    /**
     * @brief 打印系统状态
     */
    void printStatus();
    
    /**
     * @brief 系统自检
     * @return true 自检通过, false 自检失败
     */
    bool selfTest();
    
    /**
     * @brief 紧急停止系统
     */
    void emergencyStop();
    
    /**
     * @brief 重置系统到初始状态
     */
    void reset();
    
    /**
     * @brief 获取按键对象引用（用于高级控制）
     * @param keyNumber 按键编号 (1 或 2)
     * @return 按键对象引用，如果无效返回key1Button
     */
    Button& getButton(int keyNumber);
    
    /**
     * @brief 获取编码器对象引用（用于高级控制）
     * @return 编码器对象引用
     */
    RotaryEncoder& getEncoder();
    
    /**
     * @brief 获取电机控制器对象引用（用于高级控制）
     * @return 电机控制器对象引用
     */
    MotorController& getMotor();
    
    /**
     * @brief 获取OLED显示对象引用（用于高级控制）
     * @return OLED显示对象引用
     */
    OledDisplay& getDisplay();
    
    /**
     * @brief 检查系统是否正常运行
     * @return true 正常, false 异常
     */
    bool isSystemHealthy();
    
    /**
     * @brief 设置调试模式
     * @param enabled 是否启用调试输出
     */
    void setDebugMode(bool enabled);
};

#endif // MOTOR_ENCODER_SYSTEM_H 