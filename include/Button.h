#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief 按键处理类
 * 
 * 提供按键防抖、状态检测等功能
 */
class Button {
private:
    uint8_t pin;                    // 按键引脚
    bool lastState;                 // 上次按键状态
    bool currentState;              // 当前按键状态
    unsigned long lastDebounceTime; // 上次防抖时间
    unsigned long lastTriggerTime;  // 上次触发时间
    bool isPressed;                 // 按键是否被按下
    bool wasPressed;                // 按键是否已经被处理

public:
    /**
     * @brief 构造函数
     * @param buttonPin 按键引脚号
     */
    Button(uint8_t buttonPin);
    
    /**
     * @brief 初始化按键
     */
    void begin();
    
    /**
     * @brief 更新按键状态（需要在主循环中调用）
     */
    void update();
    
    /**
     * @brief 检查按键是否被按下（只触发一次）
     * @return true 按键被按下, false 按键未被按下
     */
    bool isClicked();
    
    /**
     * @brief 检查按键当前是否处于按下状态
     * @return true 按键被按住, false 按键未被按住
     */
    bool isHeld();
    
    /**
     * @brief 重置按键状态
     */
    void reset();
    
    /**
     * @brief 获取按键引脚号
     * @return 引脚号
     */
    uint8_t getPin() const;
};

#endif // BUTTON_H 