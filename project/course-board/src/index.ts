import type { Context } from '@deepseek-ai/cordis'
import { defineTool } from '@deepseek-ai/dsh-tools'
import { readStatus, setFan, setLed, setThreshold } from '../mqtt.mjs'

export const name = 'course-board'
export const inject = ['tools']

function textTool(spec: {
  name: string
  description: string
  parameters: Record<string, unknown>
  run: (args: Record<string, string>) => Promise<string>
}) {
  return defineTool({
    name: spec.name,
    description: spec.description,
    parameters: spec.parameters,
    output: {
      schema: { type: 'string' },
      render: (_args: unknown, value: string) => [{ type: 'text', text: value }],
    },
    async execute(args: Record<string, string>) {
      return spec.run(args)
    },
  })
}

export function apply(ctx: Context) {
  ctx.tools.register(textTool({
    name: 'read_status',
    description: 'Read the LicheePi temperature, humidity, fan, and thresholds over MQTT.',
    parameters: {},
    run: async () => readStatus(),
  }))
  ctx.tools.register(textTool({
    name: 'set_fan',
    description: 'Force the fan on or off, or return it to temperature hysteresis.',
    parameters: {
      state: {
        type: 'string',
        required: true,
        description: 'on, off, or auto',
      },
    },
    run: async (args) => setFan(args.state),
  }))
  ctx.tools.register(textTool({
    name: 'set_led',
    description: 'Turn the chapter 5 LED on or off over MQTT.',
    parameters: {
      state: {
        type: 'string',
        required: true,
        description: 'on or off',
      },
    },
    run: async (args) => setLed(args.state),
  }))
  ctx.tools.register(textTool({
    name: 'set_threshold',
    description: 'Set the high or low temperature threshold used by the hysteresis loop.',
    parameters: {
      which: {
        type: 'string',
        required: true,
        description: 'high or low',
      },
      value: {
        type: 'string',
        required: true,
        description: 'Temperature in Celsius, for example 30',
      },
    },
    run: async (args) => setThreshold(args.which, args.value),
  }))
}
