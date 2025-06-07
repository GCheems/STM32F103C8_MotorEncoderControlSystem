#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include <Arduino.h>
#include "config.h"

/**
 * @brief 旋转编码器处理类
 * 
 * 提供编码器位置检测、方向判断等功能
 */
class RotaryEncoder {
private:
    uint8_t pinA;                   // A相引脚
    uint8_t pinB;                   // B相引脚
    bool lastStateA;                // A相上次状态
    bool lastStateB;                // B相上次状态
    int position;                   // 当前位置
    int minPosition;                // 最小位置
    int maxPosition;                // 最大位置
    unsigned long lastDebounceTime; // 上次防抖时间
    bool positionChanged;           // 位置是否改变

public:
    /**
     * @brief 构造函数
     * @param encoderPinA A相引脚号
     * @param encoderPinB B相引脚号
     * @param minPos 最小位置值
     * @param maxPos 最大位置值
     * @param defaultPos 默认位置值
     */
    RotaryEncoder(uint8_t encoderPinA, uint8_t encoderPinB, 
                  int minPos = ENCODER_MIN_POS, 
                  int maxPos = ENCODER_MAX_POS,
                  int defaultPos = ENCODER_DEFAULT_POS);
    
    /**
     * @brief 初始化编码器
     */
    void begin();
    
    /**
     * @brief 更新编码器状态（需要在主循环中调用）
     */
    void update();
    
    /**
     * @brief 获取当前位置
     * @return 当前位置值
     */
    int getPosition() const;
    
    /**
     * @brief 设置位置
     * @param pos 新的位置值
     */
    void setPosition(int pos);
    
    /**
     * @brief 检查位置是否发生改变
     * @return true 位置已改变, false 位置未改变
     */
    bool hasChanged();
    
    /**
     * @brief 重置位置到默认值
     */
    void reset();
    
    /**
     * @brief 获取位置范围
     * @param minPos 输出最小位置
     * @param maxPos 输出最大位置
     */
    void getRange(int& minPos, int& maxPos) const;
    
    /**
     * @brief 设置位置范围
     * @param minPos 最小位置
     * @param maxPos 最大位置
     */
    void setRange(int minPos, int maxPos);
    
    /**
     * @brief 获取位置百分比 (0-100)
     * @return 位置百分比
     */
    int getPercentage() const;
    
    /**
     * @brief 检查是否在正方向
     * @return true 正方向, false 负方向或零
     */
    bool isPositiveDirection() const;
    
    /**
     * @brief 检查是否在负方向
     * @return true 负方向, false 正方向或零
     */
    bool isNegativeDirection() const;
};

#endif // ROTARY_ENCODER_H 