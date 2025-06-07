#ifndef CONFIG_H
#define CONFIG_H

// ====================================
// 硬件引脚配置
// ====================================

// 按键引脚定义
#define PIN_KEY1         PB12  // 启动按键
#define PIN_KEY2         PB13  // 停止按键

// 旋转编码器引脚定义
#define PIN_ENCODER_A    PA8   // 编码器A相
#define PIN_ENCODER_B    PA9   // 编码器B相

// 电机驱动引脚定义
#define PIN_MOTOR_PWM1   PA0   // 电机PWM1输出
#define PIN_MOTOR_PWM2   PA1   // 电机PWM2输出

// OLED显示屏引脚定义
#define PIN_OLED_SCL     PB6   // OLED I2C 时钟线
#define PIN_OLED_SDA     PB7   // OLED I2C 数据线
#define OLED_I2C_ADDR    0x3D  // OLED I2C地址 (扫描发现)

// ====================================
// 系统参数配置
// ====================================

// 串口配置
#define SERIAL_BAUDRATE  9600

// 编码器配置
#define ENCODER_MIN_POS     -10   // 编码器最小位置
#define ENCODER_MAX_POS      10   // 编码器最大位置
#define ENCODER_DEFAULT_POS   5   // 编码器默认位置
#define ENCODER_DEBOUNCE_MS  10   // 编码器防抖时间(毫秒)

// 按键配置
#define BUTTON_DEBOUNCE_MS      100  // 按键防抖时间(毫秒)
#define BUTTON_REPEAT_DELAY_MS  500  // 按键重复触发延时(毫秒)

// 电机配置
#define MOTOR_BASE_SPEED        80   // 电机基础速度
#define MOTOR_SPEED_MULTIPLIER  18   // 速度调节系数
#define MOTOR_MIN_SPEED         50   // 最小电机速度
#define MOTOR_MAX_SPEED        255   // 最大电机速度
#define MOTOR_UPDATE_INTERVAL  100   // 电机状态更新间隔(毫秒)

// 系统配置
#define SYSTEM_STATUS_INTERVAL 5000  // 状态报告间隔(毫秒)
#define MAIN_LOOP_DELAY           2  // 主循环延时(毫秒)

// OLED配置
#define OLED_SCREEN_WIDTH  128    // OLED显示宽度（像素）
#define OLED_SCREEN_HEIGHT  64    // OLED显示高度（像素）
#define OLED_UPDATE_INTERVAL 300  // OLED刷新间隔(毫秒)
#define OLED_MAX_LINES        8   // OLED最大显示行数（8行适合5x8字体）
#define OLED_MAX_CHARS_PER_LINE 24 // 每行最大字符数（5x8字体可显示更多字符）

// ====================================
// 调试配置
// ====================================
#define DEBUG_ENABLED        1    // 启用调试输出
#define DEBUG_ENCODER        1    // 启用编码器调试
#define DEBUG_BUTTONS        1    // 启用按键调试
#define DEBUG_MOTOR          1    // 启用电机调试

// 调试宏定义 - STM32兼容版本
#if DEBUG_ENABLED
  #define DEBUG_PRINT(x)     Serial.print(x)
  #define DEBUG_PRINTLN(x)   Serial.println(x)
  // 使用简单的字符串连接替代printf
  #define DEBUG_PRINTF_SIMPLE(msg) Serial.println(msg)
  #define DEBUG_PRINTF_1(msg, val) do { Serial.print(msg); Serial.println(val); } while(0)
  #define DEBUG_PRINTF_2(msg, val1, val2) do { Serial.print(msg); Serial.print(val1); Serial.print(" "); Serial.println(val2); } while(0)
  #define DEBUG_PRINTF_3(msg, val1, val2, val3) do { Serial.print(msg); Serial.print(val1); Serial.print(" "); Serial.print(val2); Serial.print(" "); Serial.println(val3); } while(0)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF_SIMPLE(msg)
  #define DEBUG_PRINTF_1(msg, val)
  #define DEBUG_PRINTF_2(msg, val1, val2)
  #define DEBUG_PRINTF_3(msg, val1, val2, val3)
#endif

#endif // CONFIG_H 