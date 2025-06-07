#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief 电机控制状态枚举
 */
enum class MotorState {
    STOPPED,    // 停止
    FORWARD,    // 正转
    REVERSE     // 反转
};

/**
 * @brief 电机控制类
 * 
 * 提供电机速度控制、方向控制等功能
 */
class MotorController {
private:
    uint8_t pinPWM1;                    // PWM1引脚
    uint8_t pinPWM2;                    // PWM2引脚
    MotorState currentState;            // 当前状态
    int currentSpeed;                   // 当前速度 (0-255)
    bool isEnabled;                     // 电机是否启用
    unsigned long lastUpdateTime;       // 上次更新时间
    
    /**
     * @brief 内部设置电机PWM
     * @param pwm1Value PWM1值
     * @param pwm2Value PWM2值
     */
    void setPWM(int pwm1Value, int pwm2Value);

public:
    /**
     * @brief 构造函数
     * @param motorPWM1 PWM1引脚号
     * @param motorPWM2 PWM2引脚号
     */
    MotorController(uint8_t motorPWM1, uint8_t motorPWM2);
    
    /**
     * @brief 初始化电机控制器
     */
    void begin();
    
    /**
     * @brief 更新电机状态（可选择性调用）
     */
    void update();
    
    /**
     * @brief 启动电机
     */
    void start();
    
    /**
     * @brief 停止电机
     */
    void stop();
    
    /**
     * @brief 强制停止电机（立即停止，不考虑状态）
     */
    void forceStop();
    
    /**
     * @brief 设置电机速度和方向
     * @param speed 速度值，正数为正转，负数为反转，0为停止
     */
    void setSpeed(int speed);
    
    /**
     * @brief 设置电机方向
     * @param forward true为正转，false为反转
     */
    void setDirection(bool forward);
    
    /**
     * @brief 获取当前速度
     * @return 当前速度 (-255 到 255)
     */
    int getSpeed() const;
    
    /**
     * @brief 获取当前状态
     * @return 当前电机状态
     */
    MotorState getState() const;
    
    /**
     * @brief 检查电机是否启用
     * @return true 已启用, false 未启用
     */
    bool isMotorEnabled() const;
    
    /**
     * @brief 检查电机是否运行中
     * @return true 运行中, false 已停止
     */
    bool isRunning() const;
    
    /**
     * @brief 根据编码器位置计算并设置电机速度
     * @param encoderPosition 编码器位置值
     */
    void setSpeedFromEncoder(int encoderPosition);
    
    /**
     * @brief 获取状态字符串（用于调试）
     * @return 状态描述字符串
     */
    const char* getStateString() const;
    
    /**
     * @brief 电机自检
     * @return true 自检通过, false 自检失败
     */
    bool selfTest();
};

#endif // MOTOR_CONTROLLER_H 