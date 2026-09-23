/**
 * QoS 0 MQTT client for the course broker.
 * Tools only publish and subscribe. GPIO stays on the LicheePi.
 */
import net from 'node:net'

const BROKER_HOST = process.env.BROKER_HOST || '192.168.31.206'
const BROKER_PORT = Number(process.env.BROKER_PORT || 1883)
const THERMO_CMD = 'course/thermo/cmd'
const THERMO_STATUS = 'course/thermo/status'
const LED_CMD = 'course/led/cmd'
const LED_STATUS = 'course/led/status'

function encodeRemaining(n) {
  const out = []
  do {
    let b = n % 128
    n = Math.floor(n / 128)
    if (n > 0) b |= 0x80
    out.push(b)
  } while (n > 0)
  return Buffer.from(out)
}

function packet(type, body) {
  return Buffer.concat([Buffer.from([type]), encodeRemaining(body.length), body])
}

function str(s) {
  const b = Buffer.from(s)
  const len = Buffer.alloc(2)
  len.writeUInt16BE(b.length)
  return Buffer.concat([len, b])
}

export function mqttExchange({ publishes, subscribe, timeoutMs = 4000 }) {
  return new Promise((resolve, reject) => {
    const sock = net.connect(BROKER_PORT, BROKER_HOST)
    const chunks = []
    let done = false
    const finish = (err, value) => {
      if (done) return
      done = true
      clearTimeout(timer)
      // 出错直接断；成功走优雅关闭，保证已写入的包发得出去
      if (err) sock.destroy()
      else sock.end()
      if (err) reject(err)
      else resolve(value)
    }
    const timer = setTimeout(() => finish(new Error('mqtt timeout')), timeoutMs)

    sock.on('error', (err) => finish(err))
    sock.on('connect', () => {
      const proto = str('MQTT')
      const client = str('course-board-' + process.pid)
      const vh = Buffer.concat([
        proto,
        Buffer.from([4, 2, 0, 60]),
        client,
      ])
      sock.write(packet(0x10, vh))
    })

    let buf = Buffer.alloc(0)
    let connack = false
    sock.on('data', (chunk) => {
      buf = Buffer.concat([buf, chunk])
      while (buf.length >= 2) {
        let mult = 1
        let rem = 0
        let i = 1
        let ok = false
        for (; i < buf.length && i < 5; i++) {
          rem += (buf[i] & 0x7f) * mult
          mult *= 128
          if ((buf[i] & 0x80) === 0) {
            ok = true
            i += 1
            break
          }
        }
        if (!ok || buf.length < i + rem) return
        const type = buf[0] >> 4
        const body = buf.subarray(i, i + rem)
        buf = buf.subarray(i + rem)
        if (type === 2 && !connack) {
          connack = true
          // CONNACK 第 2 字节是返回码：0 才算连上，其余一律当失败，别假装成功
          const code = body.length >= 2 ? body[1] : 255
          if (code !== 0) {
            finish(new Error(`mqtt connect refused (CONNACK code ${code})`))
            return
          }
          if (subscribe) {
            const sub = Buffer.concat([
              Buffer.from([0, 1]),
              str(subscribe),
              Buffer.from([0]),
            ])
            sock.write(packet(0x82, sub))
          }
          const pubPacket = (pub) =>
            packet(0x30, Buffer.concat([str(pub.topic), Buffer.from(pub.payload)]))
          if (!subscribe) {
            // 只发不收：等最后一包真正写出去再收尾，否则可能被
            // sock.destroy() 掐掉，命令发不到 broker 却报成功
            const last = publishes.length - 1
            if (last < 0) {
              finish(null, '')
            } else {
              publishes.forEach((pub, k) => {
                if (k === last) sock.write(pubPacket(pub), () => finish(null, ''))
                else sock.write(pubPacket(pub))
              })
            }
          } else {
            for (const pub of publishes)
              sock.write(pubPacket(pub))
          }
        } else if (type === 3) {
          const tlen = body.readUInt16BE(0)
          const payload = body.subarray(2 + tlen).toString()
          chunks.push(payload)
          finish(null, payload)
        }
      }
    })
  })
}

export async function readStatus() {
  const line = await mqttExchange({
    publishes: [{ topic: THERMO_CMD, payload: 'status' }],
    subscribe: THERMO_STATUS,
  })
  return line
}

const FAN_CMD = { on: 'fan on', off: 'fan off', auto: 'fan auto' }

export async function setFan(state) {
  // 只认 on/off/auto：非法值直接报错，不要悄悄退回 fan auto 解除强制控制
  if (!Object.hasOwn(FAN_CMD, state))
    throw new Error(`invalid fan state: ${state} (expected on|off|auto)`)
  const cmd = FAN_CMD[state]
  await mqttExchange({ publishes: [{ topic: THERMO_CMD, payload: cmd }] })
  return cmd
}

export async function setLed(state) {
  if (state !== 'on' && state !== 'off')
    throw new Error(`invalid led state: ${state} (expected on|off)`)
  const payload = state
  const line = await mqttExchange({
    publishes: [{ topic: LED_CMD, payload }],
    subscribe: LED_STATUS,
    timeoutMs: 4000,
  }).catch(async () => {
    await mqttExchange({ publishes: [{ topic: LED_CMD, payload }] })
    return 'published ' + payload
  })
  return line
}

export async function setThreshold(which, value) {
  const cmd = `set ${which} ${value}`
  await mqttExchange({ publishes: [{ topic: THERMO_CMD, payload: cmd }] })
  return cmd
}

import { pathToFileURL } from 'node:url'

if (process.argv[1] && import.meta.url === pathToFileURL(process.argv[1]).href) {
  const arg = process.argv[2]
  if (arg === 'status') console.log(await readStatus())
  else if (arg === 'fan') console.log(await setFan(process.argv[3]))
  else if (arg === 'led') console.log(await setLed(process.argv[3]))
  else if (arg === 'threshold') console.log(await setThreshold(process.argv[3], process.argv[4]))
  else {
    console.error('usage: node mqtt.mjs status|fan|led|threshold ...')
    process.exit(2)
  }
}
