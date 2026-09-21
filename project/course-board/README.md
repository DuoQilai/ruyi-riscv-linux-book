# 综合项目：K3 上的板端工具

工具只转发 MQTT。GPIO 仍在荔枝派的 tri-thread / mqtt-led 里。

```bash
# K3，Node 22。密钥放环境变量，不要写进仓库。
npm i -g @deepseek-ai/dsh@next pnpm
dsh plugin --profile tui add @dsh-tui/dsh-tui

# headless 用已经装进 dsh 的包，避免再从 npm 拉未发布的依赖
mkdir -p ~/.dsh/profiles/headless
# package.json 的 bundles 含 @deepseek-ai/dsh-base 与 @deepseek-ai/dsh-headless
ln -s ~/.npm-global/lib/node_modules/@deepseek-ai/dsh/node_modules ~/.dsh/profiles/headless/node_modules

export DEEPSEEK_API_KEY=...
export LOCAL_LLAMA_KEY=local
export BROKER_HOST=192.168.31.206
dsh --profile headless \
  --patch course.patch.yml \
  --patch model-cloud.patch.yml \
  "现在热不热？必须调用 read_status，再根据工具结果用一句话回答。"
```

`@dsh-tui/dsh-tui@0.1.2` 会再次 insert `storage`，和 `dsh` 0.1.5-rc.2 的 base 冲突。删掉该包 `cordis.patch.yml` 里重复的 `storage` / `storage-json` / `storage-domain` 三行后，`dsh --profile tui` 才能起来。云端 `deepseek-flash` 要把 `llm-deepseek.thinking` 设为 `disabled`，否则便宜模型把 token 花在推理上，正文是空的。
