/**
 * @file OledDisplay.cpp
 * @brief OLED显示控制实现
 * 
 * @author 您的名字
 * @date 2024
 */

#include "OledDisplay.h"

/**
 * @brief 构造函数
 */
OledDisplay::OledDisplay() : 
    display1(U8G2_R0, /* reset=*/ U8X8_PIN_NONE),
    display2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE),
    activeDisplay(nullptr),
    currentLine(0),
    lastUpdateTime(0),
    displayInitialized(false),
    displayEnabled(true) {
    
    // 初始化文本缓存
    for (int i = 0; i < OLED_MAX_LINES; i++) {
        textBuffer[i] = "";
    }
}

/**
 * @brief 析构函数
 */
OledDisplay::~OledDisplay() {
    // 清理资源
}

/**
 * @brief 初始化OLED显示屏
 */
bool OledDisplay::begin() {
    // 手动初始化I2C引脚
    Wire.setSDA(PIN_OLED_SDA);
    Wire.setSCL(PIN_OLED_SCL);
    Wire.begin();
    
    DEBUG_PRINTLN("开始初始化OLED显示屏...");
    DEBUG_PRINT("使用地址: 0x");
    DEBUG_PRINTLN(String(OLED_I2C_ADDR, HEX));
    
    // 尝试SSD1306驱动 (更常见)
    display2.setI2CAddress(OLED_I2C_ADDR << 1);  // U8g2使用8位地址
    display2.begin();
    
    // 测试SSD1306驱动
    display2.clearBuffer();
    display2.setFont(u8g2_font_6x12_tf);  
    display2.drawStr(2, 10, "SSD1306 Test");
    display2.sendBuffer();
    
    delay(200);
    
    activeDisplay = &display2;
    displayInitialized = true;
    
    DEBUG_PRINTLN("OLED初始化成功 (SSD1306驱动)");
    
    // 清空显示并设置字体
    activeDisplay->clearBuffer();
    activeDisplay->setFont(u8g2_font_5x8_tf);  // 使用5x8字体，确保文本完整显示
    
    // 显示初始化成功信息
    showWelcome();
    
    return true;
}

/**
 * @brief 添加一行文本到显示缓存
 */
void OledDisplay::println(const String& text) {
    if (!displayInitialized || !displayEnabled) {
        return;
    }
    
    // 如果当前行已满，滚动缓存
    if (currentLine >= OLED_MAX_LINES) {
        scrollBuffer();
        currentLine = OLED_MAX_LINES - 1;
    }
    
    // 截断过长的文本
    String truncatedText = text;
    if (truncatedText.length() > OLED_MAX_CHARS_PER_LINE) {
        truncatedText = truncatedText.substring(0, OLED_MAX_CHARS_PER_LINE);
    }
    
    textBuffer[currentLine] = truncatedText;
    currentLine++;
    
    // 立即刷新显示
    refreshDisplay();
}

/**
 * @brief 添加文本到当前行（不换行）
 */
void OledDisplay::print(const String& text) {
    if (!displayInitialized || !displayEnabled) {
        return;
    }
    
    if (currentLine == 0) {
        currentLine = 1;
    }
    
    int lineIndex = currentLine - 1;
    if (lineIndex >= OLED_MAX_LINES) {
        scrollBuffer();
        lineIndex = OLED_MAX_LINES - 1;
    }
    
    // 添加文本到当前行
    String newText = textBuffer[lineIndex] + text;
    if (newText.length() > OLED_MAX_CHARS_PER_LINE) {
        newText = newText.substring(0, OLED_MAX_CHARS_PER_LINE);
    }
    
    textBuffer[lineIndex] = newText;
    refreshDisplay();
}

/**
 * @brief 清空显示内容
 */
void OledDisplay::clear() {
    if (!displayInitialized) {
        return;
    }
    
    // 清空缓存
    for (int i = 0; i < OLED_MAX_LINES; i++) {
        textBuffer[i] = "";
    }
    currentLine = 0;
    
    // 清空显示屏
    if (activeDisplay) {
        activeDisplay->clearBuffer();
        activeDisplay->sendBuffer();
    }
}

/**
 * @brief 显示系统状态信息
 */
void OledDisplay::showStatus(bool motorRunning, int encoderPos, int motorSpeed) {
    if (!displayInitialized || !displayEnabled) {
        return;
    }
    
    clear();
    
    println("== System Status ==");
    println("Motor: " + String(motorRunning ? "ON" : "OFF"));
    println("Encoder: " + String(encoderPos));
    println("Speed: " + String(motorSpeed));
    println("Time: " + String(millis() / 1000) + "s");
    println("==================");
}

/**
 * @brief 显示欢迎信息
 */
void OledDisplay::showWelcome() {
    if (!displayInitialized) {
        return;
    }
    
    clear();
    
    println("== Motor Control ==");
    println("System Ready");
    println("KEY1: Start");
    println("KEY2: Stop");
    println("Encoder: Speed");
    println("=================");
}

/**
 * @brief 显示错误信息
 */
void OledDisplay::showError(const String& errorMsg) {
    if (!displayInitialized) {
        return;
    }
    
    println("Error: " + errorMsg);
}

/**
 * @brief 更新显示（需要在主循环中调用）
 */
void OledDisplay::update() {
    unsigned long currentTime = millis();
    
    // 检查是否需要更新
    if (currentTime - lastUpdateTime >= OLED_UPDATE_INTERVAL) {
        lastUpdateTime = currentTime;
        
        if (displayInitialized && displayEnabled) {
            // 这里可以添加定期更新的内容
            // 目前主要是响应式更新，所以暂时留空
        }
    }
}

/**
 * @brief 启用/禁用显示功能
 */
void OledDisplay::setEnabled(bool enabled) {
    displayEnabled = enabled;
    if (!enabled && displayInitialized && activeDisplay) {
        activeDisplay->clearBuffer();
        activeDisplay->sendBuffer();
    }
}

/**
 * @brief 检查显示屏是否工作正常
 */
bool OledDisplay::isHealthy() {
    return displayInitialized;
}

/**
 * @brief 设置显示亮度
 */
void OledDisplay::setBrightness(uint8_t brightness) {
    if (!displayInitialized) {
        return;
    }
    
    // SH1106 不直接支持亮度控制，这里留空
    // 如果需要可以通过发送特定命令实现
}

/**
 * @brief 显示测试图案
 */
void OledDisplay::showTestPattern() {
    if (!displayInitialized || !activeDisplay) {
        return;
    }
    
    clear();
    
    // 显示测试图案
    activeDisplay->clearBuffer();
    activeDisplay->drawFrame(0, 0, OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT);
    activeDisplay->drawLine(0, 0, OLED_SCREEN_WIDTH, OLED_SCREEN_HEIGHT);
    activeDisplay->drawLine(0, OLED_SCREEN_HEIGHT, OLED_SCREEN_WIDTH, 0);
    activeDisplay->drawStr(30, 30, "TEST");
    activeDisplay->sendBuffer();
    
    delay(2000);
    clear();
}

/**
 * @brief 滚动文本缓存
 */
void OledDisplay::scrollBuffer() {
    // 向上滚动一行
    for (int i = 0; i < OLED_MAX_LINES - 1; i++) {
        textBuffer[i] = textBuffer[i + 1];
    }
    textBuffer[OLED_MAX_LINES - 1] = "";
}

/**
 * @brief 刷新显示内容到屏幕
 */
void OledDisplay::refreshDisplay() {
    if (!displayInitialized || !displayEnabled || !activeDisplay) {
        return;
    }
    
    activeDisplay->clearBuffer();
    
    // 显示所有缓存的文本行
    for (int i = 0; i < OLED_MAX_LINES; i++) {
        if (textBuffer[i].length() > 0) {
            activeDisplay->drawStr(2, (i + 1) * 8 + 2, textBuffer[i].c_str());  // 左边距2像素，行间距8像素，垂直偏移2像素
        }
    }
    
    activeDisplay->sendBuffer();
} 