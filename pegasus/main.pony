
use "time"

actor Main
  """
  主控Actor，根据命令行的输入，决定进入相应的处理模式
    - 初始化
    - 程序运行
  具体逻辑参考实现说明
  """
  let _timers: Timers;

  new create(env:Env) =>
    _start(env, this)
    let _timers = Timers
    let timer = Timer(_TimerNotify(h.env, _timers), 0, 1_000_000_000) // wait: 0 secs, interval: 1 secs
    timers(consume timer)

  be finished() =>
    _timers.dispose()

  fun _start(env: Env, from: Main) =>
    var is_init: Bool = false
    match env.args.size()
    | 0
    | match env.args(0)
      | "init" => is_init = true
      end
    end

    if is_init then
      var init_task: _Init = _Init(this)
    else
      var run_task: _Run = _Run(this)
    end


class _TimerNotify is TimerNotify
  let _env: Env

  new iso create(env: Env, timers: Timers) => _env = env
  fun ref apply(timer: Timer, count: U64): Bool => true