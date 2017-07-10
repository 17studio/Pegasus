"""
实现主控actor模式，主控actor负责启动任务actor，然后等待任务actor的callback

设想Main()起来后，会拉起MasterActor，然后进入等待状态，MasterActor完成所有的任务后，
通知Main()任务完成，此时Main()退出
"""

interface tag MasterActor[A: Any #send]
  be receive(a: A)
