# 综合项目：K3 上的板端工具

工具只转发 MQTT。GPIO 仍在荔枝派的 tri-thread / mqtt-led 里。

## 依赖

- **第五、六章先合入**：本插件只发 MQTT，板上真正干活的是第五章的 `mqtt-led`（`course/led/*`）与
  第六章的 `tri-thread`（`course/thermo/*`）。两章未合入时，工具能发消息但板上无人应答。
- **本机/板端**：Node 22 + `pnpm`；K3（riscv64）上跑 `dsh`。
- **MQTT broker**：默认 `192.168.31.206:1883`，用环境变量 `BROKER_HOST` / `BROKER_PORT` 覆盖。
- **riscv64 的 flock 依赖**：见下方 `flock-addon`。

## 安装

```bash
# K3，Node 22。密钥放环境变量，不要写进仓库。
npm i -g @deepseek-ai/dsh@next pnpm
dsh plugin --profile tui add @dsh-tui/dsh-tui

# headless 用已经装进 dsh 的包，避免再从 npm 拉未发布的依赖
mkdir -p ~/.dsh/profiles/headless
# package.json 的 bundles 含 @deepseek-ai/dsh-base 与 @deepseek-ai/dsh-headless
ln -s ~/.npm-global/lib/node_modules/@deepseek-ai/dsh/node_modules ~/.dsh/profiles/headless/node_modules
```

### riscv64 上的 flock（flock-addon）

上游 `@deepseek-ai/node-addon-system-linux-riscv64` 没有 riscv64 预编译产物，本仓自带一个最小
flock 实现（`flock-addon/`），需要在目标机（K3，riscv64）上就地编译：

```bash
cd project/course-board/flock-addon
npx node-gyp rebuild        # 依据 binding.gyp 编译 system.c
ls build/Release/system.node
```

编出的 `system.node` 要放到 dsh 解析该包的位置（覆盖上游那个空包）：

```bash
# 路径按你机器上 dsh 的安装位置为准（`npm root -g` 帮助定位）
DEST=$(npm root -g)/@deepseek-ai/dsh/node_modules/@deepseek-ai/node-addon-system-linux-riscv64
mkdir -p "$DEST"
cp build/Release/system.node "$DEST/"
```

> 该步必须在 riscv64 的 K3 上做（交叉编译 N-API 附件不在本课程范围）。

## 运行

**在仓库的 `project/` 目录下**启动（补丁里的插件路径是相对路径，不写死任何绝对路径）：

```bash
export DEEPSEEK_API_KEY=...
export LOCAL_LLAMA_KEY=local
export BROKER_HOST=192.168.31.206

cd project
dsh --profile headless \
  --patch course-board/course.patch.yml \
  --patch course-board/model-cloud.patch.yml \
  "现在热不热？必须调用 read_status，再根据工具结果用一句话回答。"
```

本地小模型把 `model-cloud.patch.yml` 换成 `model-local.patch.yml` 即可。

## 连接与错误处理

- `mqtt.mjs` 会校验 CONNACK 返回码：broker 拒绝连接（返回码非 0）时**直接报错**，
  `set_fan` / `set_threshold` 不再假装成功返回 `fan on`。
- `set_fan` 只接受 `on` / `off` / `auto`，其它值（如 `OFF`）会报错而不是悄悄退回 `fan auto`
  解除强制控制；工具 schema 同步用 `enum` 约束了合法值。

## 已知坑

`@dsh-tui/dsh-tui@0.1.2` 会再次 insert `storage`，和 `dsh` 0.1.5-rc.2 的 base 冲突。删掉该包
`cordis.patch.yml` 里重复的 `storage` / `storage-json` / `storage-domain` 三行后，`dsh --profile tui`
才能起来。云端 `deepseek-flash` 要把 `llm-deepseek.thinking` 设为 `disabled`，否则便宜模型把 token
花在推理上，正文是空的。
