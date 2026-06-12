# Docs

存放公开课程大纲、课程计划、制作规范、模板和阶段项目定义。

## 课程基准

- `course-outline.md`：课程大纲，作为课程内容的最高基准；当前版本为 LicheePi 4A + RevyOS、6 章 24 讲。
- `course-plan.md`：课程计划表，负责把大纲拆成章节目录、交付物、阶段项目和运行验证。
- `course-production-spec-v1.md`：课程制作规范 V1.0，定义讲义、PPT、实验和代码工程标准。
- `《RuyiSDK RISC-V 嵌入式编程技术》课件开发进度和审核表.xlsx`：课件开发进度与审核表，用于跟踪课程文档、PPT、实验指导书的提交、审核、修改和终审状态。

## 审核与进度

- 课程内容以 `course-outline.md` 为准，开发排期和交付状态以 `course-plan.md` 与课件开发进度和审核表共同维护。
- 审核表用于记录每讲课程文档、PPT、实验指导书的提交时间、审核意见、修改状态和终审结果。
- 修改大纲或章节目录后，应同步检查审核表中的讲次、章节目录和交付物状态是否仍然一致。

## 阶段项目

- 第 2 章温控风扇：双温度传感器采集、阈值/回差控制、OLED/串口状态显示、风扇驱动与异常保护。
- 第 4 章 MQTT 灯光：手机 MQTT 客户端、mosquitto、JSON 指令、GPIO/PWM 控灯和状态回传。
- 第 6 章边缘智能：OpenCV、TinyML、LLM/VLM 中至少一个方向形成可演示作品。

## 模板

- `templates/course-document-template.md`：课程文档模板。
- `templates/lab-template.md`：实验指导书模板。
- `templates/ppt-template.md`：PPT 模板。
- `templates/code-project-template.md`：代码工程模板。
- `templates/asset-guidelines.md`：图片与图表规则。

## 后续可补

- `faq.md`：常见问题。
- `errata.md`：勘误。
