# STM32F103C8 电机编码器控制系统

这是一个基于STM32F103C8微控制器的电机编码器控制系统，使用PlatformIO开发环境。系统通过旋转编码器控制DRV8833电机驱动器，实现电机的速度和方向控制。

## 🔧 硬件需求

- **主控板**: STM32F103C8T6开发板
- **电机驱动**: DRV8833双路电机驱动模块
- **编码器**: 旋转编码器（带按键，可选）
- **按键**: 2个独立按键（KEY1, KEY2）
- **电机**: 直流减速电机

## 📋 引脚连接

| 功能 | STM32引脚 | 说明 |
|------|-----------|------|
| 编码器A相 | PA8 | 编码器A相输出 |
| 编码器B相 | PA9 | 编码器B相输出 |
| KEY1按键 | PB12 | 电机启动按键 |
| KEY2按键 | PB13 | 电机停止按键 |
| 电机PWM1 | PA0 | DRV8833 IN1输入 |
| 电机PWM2 | PA1 | DRV8833 IN2输入 |

> **注意**: 所有输入引脚都使用内部上拉电阻，按键和编码器请连接到GND。

## 🎯 功能特性

### 基本功能
- ✅ **按键控制**: KEY1启动电机，KEY2停止电机
- ✅ **编码器控制**: 旋转编码器控制电机速度和方向
- ✅ **速度调节**: 支持-10到+10共21个速度等级
- ✅ **方向控制**: 正值正转，负值反转
- ✅ **防抖处理**: 按键和编码器都有防抖功能

### 高级功能
- ✅ **串口命令**: 支持串口遥控和状态查询
- ✅ **状态监控**: 实时状态显示和调试信息
- ✅ **系统自检**: 启动时自动检测硬件状态
- ✅ **紧急停止**: 多种停止方式确保安全
- ✅ **调试输出**: 可配置的调试信息等级

## 🏗️ 项目结构

```
├── include/                 # 头文件目录
│   ├── config.h            # 系统配置和引脚定义
│   ├── Button.h            # 按键处理类
│   ├── RotaryEncoder.h     # 编码器处理类
│   ├── MotorController.h   # 电机控制类
│   └── MotorEncoderSystem.h # 系统集成类
├── src/                    # 源代码目录
│   ├── main.cpp            # 主程序
│   ├── Button.cpp          # 按键处理实现
│   ├── RotaryEncoder.cpp   # 编码器处理实现
│   ├── MotorController.cpp # 电机控制实现
│   └── MotorEncoderSystem.cpp # 系统集成实现
├── platformio.ini          # PlatformIO配置文件
└── README.md              # 项目说明文档
```

## 🚀 快速开始

### 1. 环境准备
```bash
# 安装PlatformIO
pip install platformio

# 克隆项目
git clone [your-repo-url]
cd stm32f103c8test01
```

### 2. 编译上传
```bash
# 编译项目
pio run

# 上传到开发板
pio run --target upload

# 打开串口监视器
pio device monitor
```

### 3. 基本使用
1. 上电后系统会自动初始化并进行自检
2. 按下KEY1启动电机
3. 旋转编码器调节速度和方向
4. 按下KEY2停止电机

## 💻 串口命令

通过串口可以发送以下命令控制系统：

| 命令 | 功能 |
|------|------|
| `start` 或 `1` | 启动电机 |
| `stop` 或 `0` | 停止电机 |
| `reset` 或 `r` | 重置系统 |
| `status` 或 `s` | 显示系统状态 |
| `test` 或 `t` | 执行系统自检 |
| `emergency` 或 `e` | 紧急停止 |
| `help` 或 `h` | 显示帮助信息 |

## ⚙️ 配置说明

所有系统参数都在 `include/config.h` 中定义，主要包括：

### 硬件配置
- 引脚定义
- 串口波特率
- PWM参数

### 系统参数
- 编码器范围和防抖时间
- 按键防抖时间
- 电机速度参数
- 状态更新间隔

### 调试配置
- 调试输出开关
- 各模块调试等级

## 🔧 自定义开发

### 添加新功能
1. 在对应的类中添加新方法
2. 在 `config.h` 中添加相关配置
3. 在主程序中调用新功能

### 修改硬件连接
1. 修改 `config.h` 中的引脚定义
2. 重新编译上传即可

### 调整参数
- 速度范围：修改 `ENCODER_MIN_POS` 和 `ENCODER_MAX_POS`
- 电机速度：修改 `MOTOR_BASE_SPEED` 和 `MOTOR_SPEED_MULTIPLIER`
- 防抖时间：修改各种 `DEBOUNCE_MS` 参数

## 🐛 故障排除

### 常见问题

1. **编译错误**
   - 检查PlatformIO环境是否正确安装
   - 确认依赖库已安装

2. **上传失败**
   - 检查开发板连接
   - 确认COM端口正确
   - 尝试按住BOOT按键上传

3. **功能异常**
   - 检查硬件连接
   - 查看串口调试信息
   - 运行系统自检

### 调试技巧

1. **启用调试输出**
   ```cpp
   #define DEBUG_ENABLED        1
   #define DEBUG_ENCODER        1
   #define DEBUG_BUTTONS        1
   #define DEBUG_MOTOR          1
   ```

2. **查看系统状态**
   - 串口发送 `status` 命令
   - 观察定期状态报告

3. **运行自检**
   - 串口发送 `test` 命令
   - 检查自检结果

## 📚 API参考

### Button类
```cpp
Button key1(PIN_KEY1);          // 创建按键对象
key1.begin();                   // 初始化
key1.update();                  // 更新状态（主循环调用）
bool clicked = key1.isClicked(); // 检查是否被点击
```

### RotaryEncoder类
```cpp
RotaryEncoder encoder(PIN_A, PIN_B);  // 创建编码器对象
encoder.begin();                      // 初始化
encoder.update();                     // 更新状态
int pos = encoder.getPosition();      // 获取位置
```

### MotorController类
```cpp
MotorController motor(PIN_PWM1, PIN_PWM2); // 创建电机对象
motor.begin();                             // 初始化
motor.start();                             // 启动
motor.setSpeed(100);                       // 设置速度
motor.stop();                              // 停止
```

### MotorEncoderSystem类
```cpp
MotorEncoderSystem system;  // 创建系统对象
system.begin();             // 初始化整个系统
system.loop();              // 主循环处理
```

## 📄 版本历史

- **v2.0** - 重构为模块化架构，添加完整的类库支持
- **v1.0** - 基本功能实现，单文件架构

## 🤝 贡献

欢迎提交Issue和Pull Request来改进这个项目！

## 📄 许可证

MIT License - 详见 LICENSE 文件 