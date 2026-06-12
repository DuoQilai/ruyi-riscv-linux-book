# 图片与图表规则

## 存放位置

每章图片、截图和图表统一放在本章的 `assets/` 目录。图片内容必须服务于 LicheePi 4A + RevyOS 主线，截图应来自实际验证环境或明确标注为示意图。

```text
chapters/chXX_topic/
├── assets/
├── class1.md
├── class2.md
└── lab/
```

## 命名规则

建议使用：

```text
fig-章节号-序号-说明.png
```

示例：

```text
fig-01-01-ruyi-version.png
fig-02-04-fan-wiring.png
fig-04-04-mqtt-light-status.png
fig-06-02-detection-result.png
```

## 引用规则

正文中引用图片时，必须有编号和解释：

```text
图片编号：图 1-1
图片标题：ruyi version 输出
图片路径：assets/fig-01-01-ruyi-version.png

图 1-1 展示了 `ruyi version` 的输出结果，用于确认 `ruyi` 已正确安装。
```

## 重点资产清单

| 场景 | 必备资产 |
| --- | --- |
| LicheePi 4A + RevyOS | 板卡实物、引脚说明、烧录流程、首次登录、系统信息截图 |
| 第二章温控风扇 | DHT22 接线、风扇/继电器或 MOSFET 接线、OLED/串口状态、阈值与回差运行结果 |
| 第四章 MQTT 灯光 | broker 配置、topic/payload 示例、手机 MQTT 客户端、GPIO/PWM 控灯效果、状态回传 |
| 第六章边缘智能 | 摄像头采集、OpenCV 处理、目标检测框、TinyML 控灯或 LLM/VLM 演示结果 |

## 审核要求

- 图片应来自本机或目标环境验证后的截图。
- 图片中关键区域应框出或标注。
- 多张图片应使用统一编号。
- 代码应使用源码文本，不使用代码截图。
- 引用第三方图片或资料时，应注明来源。
- 不使用旧版章节、旧板卡或非主线系统截图作为主线资产。
