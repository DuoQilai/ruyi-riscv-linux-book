# LicheePi 4A · 本课相关摘录（自官方资料整理）

来源：
- `SIPEED_LPi4A_DataSheet_EN_V1.0.pdf`（已下载）
- Wiki 外设页源码 `LicheePi4A_外设使用_wiki.md`
- 插针图 `lpi4a-wiki-assets/io_map.png`、`gpio_num.png`

## 结论（写实验接线时用）

1. **20Pin 排针电源脚**：有 **3.3V、5V、两路 GND**。  
   **没有**标出的 **1.8V 电源脚**。
2. **信号脚电平**：SoC 原生 IO 为 **1.8V**，接 3.3V 模块必须电平转换。
3. **推荐当普通 GPIO 用的四脚**（官方示例也用它们）：  
   `IO1_3` / `IO1_4` / `IO1_5` / `IO1_6`
4. **libgpiod 官方示例**：  
   - 设备：`/dev/gpiochip1`  
   - `IO1_3` → line **3**  
   - 同理：`IO1_4`→line 4，`IO1_5`→line 5，`IO1_6`→line 6  
5. **sysfs 内核号（Wiki 表）**：  
   - IO1_3 → 427，IO1_4 → 428，IO1_5 → 429，IO1_6 → 430  
6. **本课建议默认脚位（待你板上确认丝印方向后写死）**：  
   - 继电器信号（风扇）：`gpiochip1` line **3**（IO1_3）→ 经电平转换 → 继电器 IN  
   - DHT22 数据：`gpiochip1` line **4**（IO1_4）→ 经电平转换 → DAT  
7. **供电建议**：  
   - 继电器线圈 / 风扇：排针 **5V + GND**（经继电器 NO–COM）  
   - DHT22 VCC、电平转换 VCCB：排针 **3.3V**  
   - 电平转换 VCCA（低压侧）：需要 **1.8V**，排针没有 → 需另取（见下）

## 插针功能概览（俯视底板，TOP 左 / BOTTOM 右）

| 行 | TOP | BOTTOM |
|----|-----|--------|
| 1 | IO1_5 (GPIO) | IO1_6 (GPIO) |
| 2 | IO1_3 (GPIO) | IO1_4 (GPIO) |
| 3 | I2C2-SCL / U2-TX | I2C2-SDA / U2-RX |
| 4 | U3-TX | U3-RX |
| 5 | U1-TX | U1-RX |
| 6 | U0-TX | U0-RX |
| 7 | QSPI1-SO | QSPI1-CS |
| 8 | QSPI1-SI | QSPI1-CLK |
| 9 | GND | GND |
| 10 | 3V3 | 5V |

注意：早期丝印可能有误，以文档图为准。

## 电平转换板（TXS0108E）与排针对应

| 转换板脚 | 接到 |
|----------|------|
| VCCA | **1.8V**（排针无此脚 → 待解决） |
| VCCB | 排针 **3.3V** |
| GND | 排针 **GND**（共地） |
| OE | 高电平使能（接 VCCA 侧高，或按 HW-221） |
| A1/A2… | 荔枝派 GPIO（1.8V 侧） |
| B1/B2… | DHT DAT / 继电器 IN（3.3V 侧） |

## 未解决

- **VCCA 的 1.8V 从哪来**：外接 3.3→1.8 LDO（推荐），或板上测试点（需实测/查原理图；Sipeed 下载站原理图直链当前不可用）。
- 更完整的课程向整理见：`LicheePi4A_课程相关摘录.md`。
