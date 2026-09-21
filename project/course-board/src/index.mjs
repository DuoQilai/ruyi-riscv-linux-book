import { defineTool } from '@deepseek-ai/dsh-tools'
import { readStatus, setFan, setLed, setThreshold } from '../mqtt.mjs'

export const name = 'course-board'
export const inject = ['tools']

function textTool(spec) {
  return defineTool({
    name: spec.name,
    description: spec.description,
    parameters: spec.parameters,
    output: {
      schema: { type: 'string' },
      render: (_args, value) => [{ type: 'text', text: value }],
    },
    async execute(args) {
      return spec.run(args)
    },
  })
}

export function apply(ctx) {
  ctx.tools.register(textTool({
    name: 'read_status',
    description: 'Read the LicheePi temperature, humidity, fan, and thresholds over MQTT.',
    parameters: {},
    run: () => readStatus(),
  }))
  ctx.tools.register(textTool({
    name: 'set_fan',
    description: 'Force the fan on or off, or return it to temperature hysteresis. state is on, off, or auto.',
    parameters: {
      state: { type: 'string', required: true, description: 'on, off, or auto' },
    },
    run: (args) => setFan(args.state),
  }))
  ctx.tools.register(textTool({
    name: 'set_led',
    description: 'Turn the chapter 5 LED on or off over MQTT.',
    parameters: {
      state: { type: 'string', required: true, description: 'on or off' },
    },
    run: (args) => setLed(args.state),
  }))
  ctx.tools.register(textTool({
    name: 'set_threshold',
    description: 'Set the high or low temperature threshold used by hysteresis. which is high or low.',
    parameters: {
      which: { type: 'string', required: true, description: 'high or low' },
      value: { type: 'string', required: true, description: 'Celsius, for example 30' },
    },
    run: (args) => setThreshold(args.which, args.value),
  }))
}
