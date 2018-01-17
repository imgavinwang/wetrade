## 1.introduce

wetrade is a trade and strategy engine based on ctp api with c++11

## 2.architecture

```                             
                               ╱ TsQueue 
                ╱ EventEngine ── TsTimer 
  GatewayEngine ── TdApi MdApi 
                ╲ StrategyEngine 
```

mainThreads:

- thread1: connect to TdApi
- thread2: connect to MdApi
- thread3: EventEngine.TsTimer(10seconds, QueryAccountAndPosition)
- thread3: EventEngine.ProcessMostEventQueue()
- thread4: EventEngine.ProcessTickEventQueue()
- thread5: StrategyEngine.OnTick(DoStrategy)