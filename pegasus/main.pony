
use "time"

actor Main
  let _timers: Timers;

  new create(env:Env) =>
    _start()
    let _timers = Timers
    let timer = Timer(_TimerNotify(h.env, _timers), 0, 1_000_000_000) // wait: 0 secs, interval: 1 secs
    timers(consume timer)

  be finished() =>
    _timers.dispose()

  fun _start() =>
    None


class _TimerNotify is TimerNotify
  let _env: Env

  new iso create(env: Env, timers: Timers) =>
    _env = env

  fun ref apply(timer: Timer, count: U64): Bool =>
    true