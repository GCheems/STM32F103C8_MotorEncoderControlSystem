/**
 * @file test_i2c_scan.cpp
 * @brief I2C设备扫描程序 - 用于调试OLED连接问题
 * 
 * 使用方法：
 * 1. 将此文件内容复制到main.cpp
 * 2. 编译并上传到STM32
 * 3. 打开串口监视器查看扫描结果
 * 4. 找到OLED的实际I2C地址
 */

#include <Arduino.h>
#include <Wire.h>

void setup() {
    Serial.begin(9600);
    delay(2000);
    
    Serial.println("I2C设备扫描器");
    Serial.println("=============");
    
    // 手动设置I2C引脚 (根据你的配置)
    Wire.setSDA(PB7);  // OLED SDA
    Wire.setSCL(PB6);  // OLED SCL
    Wire.begin();
    
    Serial.println("I2C引脚配置:");
    Serial.println("SDA: PB7");
    Serial.println("SCL: PB6");
    Serial.println();
    
    delay(1000);
}

void loop() {
    Serial.println("开始扫描I2C设备...");
    
    int deviceCount = 0;
    
    for (byte address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        byte error = Wire.endTransmission();
        
        if (error == 0) {
            Serial.print("发现I2C设备! 地址: 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.print(address, HEX);
            Serial.print(" (十进制: ");
            Serial.print(address);
            Serial.println(")");
            deviceCount++;
        }
        else if (error == 4) {
            Serial.print("未知错误，地址: 0x");
            if (address < 16) {
                Serial.print("0");
            }
            Serial.println(address, HEX);
        }
    }
    
    if (deviceCount == 0) {
        Serial.println("未发现任何I2C设备!");
        Serial.println();
        Serial.println("可能的问题:");
        Serial.println("1. 检查SDA和SCL接线");
        Serial.println("2. 检查电源连接(3.3V和GND)");
        Serial.println("3. 检查OLED模块是否损坏");
        Serial.println("4. 确认引脚配置是否正确");
    } else {
        Serial.print("扫描完成! 总共发现 ");
        Serial.print(deviceCount);
        Serial.println(" 个I2C设备");
    }
    
    Serial.println();
    Serial.println("等待5秒后重新扫描...");
    Serial.println("==============================");
    
    delay(5000);
} 