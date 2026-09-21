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
      sock.destroy()
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
          if (subscribe) {
            const sub = Buffer.concat([
              Buffer.from([0, 1]),
              str(subscribe),
              Buffer.from([0]),
            ])
            sock.write(packet(0x82, sub))
          }
          for (const pub of publishes) {
            const payload = Buffer.from(pub.payload)
            sock.write(packet(0x30, Buffer.concat([str(pub.topic), payload])))
          }
          if (!subscribe) finish(null, '')
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

export async function setFan(state) {
  const cmd = state === 'on' ? 'fan on' : state === 'off' ? 'fan off' : 'fan auto'
  await mqttExchange({ publishes: [{ topic: THERMO_CMD, payload: cmd }] })
  return cmd
}

export async function setLed(state) {
  const payload = state === 'on' ? 'on' : 'off'
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
