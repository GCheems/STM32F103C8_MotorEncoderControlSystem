#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <U8g2lib.h>
#include "config.h"

// 尝试不同的OLED驱动类型
// 根据实际硬件情况选择合适的驱动

/**
 * @brief OLED显示控制类
 * 
 * 提供OLED显示屏的初始化、文本显示、状态显示等功能
 * 支持多行文本缓存和自动滚动
 */
class OledDisplay {
private:
    // 使用更通用的U8G2构造器，支持多种驱动类型
    U8G2_SH1106_128X64_NONAME_F_HW_I2C display1;  // SH1106驱动
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C display2; // SSD1306驱动
    U8G2 *activeDisplay;                            // 当前激活的显示对象
    String textBuffer[OLED_MAX_LINES];    // 文本缓存数组
    int currentLine;                      // 当前行指针
    unsigned long lastUpdateTime;         // 上次更新时间
    bool displayInitialized;              // 显示屏是否已初始化
    bool displayEnabled;                  // 显示功能是否启用
    
    /**
     * @brief 滚动文本缓存
     */
    void scrollBuffer();
    
    /**
     * @brief 刷新显示内容到屏幕
     */
    void refreshDisplay();

public:
    /**
     * @brief 构造函数
     */
    OledDisplay();
    
    /**
     * @brief 析构函数
     */
    ~OledDisplay();
    
    /**
     * @brief 初始化OLED显示屏
     * @return true 初始化成功, false 初始化失败
     */
    bool begin();
    
    /**
     * @brief 添加一行文本到显示缓存
     * @param text 要显示的文本
     */
    void println(const String& text);
    
    /**
     * @brief 添加文本到当前行（不换行）
     * @param text 要显示的文本
     */
    void print(const String& text);
    
    /**
     * @brief 清空显示内容
     */
    void clear();
    
    /**
     * @brief 显示系统状态信息
     * @param motorRunning 电机是否运行
     * @param encoderPos 编码器位置
     * @param motorSpeed 电机速度
     */
    void showStatus(bool motorRunning, int encoderPos, int motorSpeed);
    
    /**
     * @brief 显示欢迎信息
     */
    void showWelcome();
    
    /**
     * @brief 显示错误信息
     * @param errorMsg 错误消息
     */
    void showError(const String& errorMsg);
    
    /**
     * @brief 更新显示（需要在主循环中调用）
     */
    void update();
    
    /**
     * @brief 启用/禁用显示功能
     * @param enabled 是否启用
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief 检查显示屏是否工作正常
     * @return true 正常, false 异常
     */
    bool isHealthy();
    
    /**
     * @brief 设置显示亮度
     * @param brightness 亮度值 (0-255)
     */
    void setBrightness(uint8_t brightness);
    
    /**
     * @brief 显示测试图案
     */
    void showTestPattern();
};

#endif // OLED_DISPLAY_H 