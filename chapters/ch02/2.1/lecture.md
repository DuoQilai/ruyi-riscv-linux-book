# 2.1 host、target、sysroot 概念

> 状态：骨架草稿，待充实。

## 本讲目标

- 能区分 host（编译机）、target（运行板）与 sysroot（目标根文件系统）。
- 能说明交叉编译与本地编译的区别。
- 能指出课程中三要素分别对应哪些路径或命令。

## 前置条件

- 完成第一章 1.1–1.5。

## 知识简介

交叉编译：在 PC（host）上生成在开发板（target）上运行的程序。工具链通过 sysroot 引用目标系统的头文件与库。

## 环境准备

| 项目 | 要求 | 检查方式 |
| --- | --- | --- |
| 工具链 | 已通过 `ruyi` 安装 RISC-V GNU 工具链 | `riscv64-unknown-linux-gnu-gcc --version` |
| 板端连接 | SSH 可用 | `ssh user@board-ip` |

## 操作步骤

（待补充：host/target/sysroot 示意图与 `ruyi` 工具链路径示例。）

## 运行验证

| 验证项 | 预期现象 | 记录 |
| --- | --- | --- |
| 能说出 host 与 target | 口头或书面说明 | |

## 常见问题

（待补充。）

## 本讲成果

- 一份 host/target/sysroot 对照表。
- 对应实验 2.1 的验收记录。
