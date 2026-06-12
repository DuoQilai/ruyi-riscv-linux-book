# 2.1 面包板基础与 GPIO 输出

## 对应大纲

大纲讲次原文：第1讲 面包板基础与 GPIO 输出。
大纲知识点原文：面包板接线规范；电路安全基础；Linux GPIO 子系统；字符设备方式控制 GPIO；LED 闪烁（Blink）；多路 LED 流水灯。

面包板接线规范、电路安全、Linux GPIO 子系统、字符设备方式控制 GPIO、LED Blink、多路 LED 流水灯。

## 目标

学生能安全完成 LED 接线，并用 gpiod 控制 4 路 LED 多种点亮模式。

## 知识点

| # | 知识点 | 本讲说明 |
| --- | --- | --- |
| 1 | 面包板接线规范 | 认识电源轨、横向/纵向连通、颜色约定和万用表测通断 |
| 2 | 电路安全基础 | 计算 LED 限流电阻，识别极性，区分 3.3V/5V 电平 |
| 3 | Linux GPIO 子系统 | 了解 `/sys/class/gpio` 导出、方向和值，以及 GPIO 编号来源 |
| 4 | 字符设备方式控制 GPIO | 使用 gpiod chip/line 模型，理解其相对 sysfs 的优势 |
| 5 | LED 闪烁 | 用 C 程序输出高低电平，控制延时和闪烁频率 |
| 6 | 多路 LED 流水灯 | 使用数组管理多路 GPIO，形成轮流、来回、随机模式 |

## 讲授要点

- 先接电源和地，再接信号；每次新增一路 LED 都要断电检查。
- LicheePi 4A GPIO 编号不能凭物理针脚猜测，应以板卡引脚图、设备树和 `gpioinfo` 为准。
- 课堂可以展示 sysfs 的历史用法，但课程实验优先使用 gpiod 字符设备接口。
- 流水灯程序的重点不是花样，而是把 GPIO 资源抽象成数组、状态和时序。

## 操作或演示

```bash
sudo apt update
sudo apt install -y gpiod libgpiod-dev
gpioinfo
gpiodetect
```

演示用 `gpioset` 点亮单个 LED，再切换到 C 程序控制。

```bash
gpioset gpiochipX Y=1
gpioset gpiochipX Y=0
```

C 程序演示结构：打开 chip、按 line 数组申请输出、循环写入高低电平、退出时熄灭所有 LED。

## 运行验证

| 验证项 | 命令或操作 | 预期现象 |
| --- | --- | --- |
| GPIO 发现 | `gpioinfo` | 能看到可用 gpiochip 和 line 名称 |
| 单路输出 | `gpioset gpiochipX Y=1` | 对应 LED 点亮 |
| Blink | 运行 blink 程序 | LED 周期闪烁 |
| 流水灯 | 运行 led_chase 程序 | 4 路 LED 按指定模式稳定变化 |
| 退出保护 | Ctrl+C 退出 | 所有 LED 熄灭或回到安全状态 |

## 常见问题

| 现象 | 可能原因 | 处理方式 |
| --- | --- | --- |
| LED 不亮 | 极性接反、GPIO line 错误、未共地 | 断电检查极性和接线，用 `gpioinfo` 重新确认 line |
| LED 很暗或过亮 | 电阻阻值不合适 | 使用 220 欧姆到 1 千欧姆范围试验，并记录实测效果 |
| 程序提示 busy | line 已被其他进程占用 | 退出 `gpioset` 或查找占用进程 |
| 高低电平逻辑相反 | LED 接法为低电平点亮 | 在程序中配置 active_low 或反转输出 |

## 本讲成果

- 4 路 LED 安全接线记录。
- GPIO chip/line 映射表。
- gpiod 版 Blink 和流水灯程序，为后续按键交互复用。

