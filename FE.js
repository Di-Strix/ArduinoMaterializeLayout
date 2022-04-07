class Handler {
  onInit() {}
  update(el, value) {}
}

class DynamicUpdateService {
  handlers = {}

  constructor() {
    this.init = this.init.bind(this)
    this.addHandler = this.addHandler.bind(this)
    this.emitEvent = this.emitEvent.bind(this)
  }

  init(wsPath) {
    Object.keys(this.handlers).forEach(key => this.handlers[key].onInit())
    if (!wsPath) throw new Error('Websocket path must be specified')

    this.ws = new WebSocket(`ws://${location.host}/${wsPath}`)

    this.ws.addEventListener('message', ({ data }) => {
      const { value, handlerId, id } = JSON.parse(data)
      const el = document.querySelector(`[data-id='${id}']`)
      const handler = this.handlers[handlerId] || {
        update: () => console.error(`'${handlerId}' handler wasn't found`),
      }

      handler.update(el, value)
    })
  }

  emitEvent({ target: { dataset, value } }) {
    this.ws.send(JSON.stringify({ id: dataset.id, value }))
  }

  addHandler(handlerId, handler) {
    if (handlerId in this.handlers) return false

    this.handlers[handlerId] = handler
    return true
  }
}

const dynamicUpdateService = new DynamicUpdateService()
