# MatePad Pro 参数采集

这份采集只读取系统公开的设备与显示参数，用于比较实体平板与壶内环境。默认不读取序列号、Android ID、IMEI、MAC、账号或用户文件。

连接 MatePad Pro、打开 USB 调试并允许电脑后，在 PowerShell 中运行：

```powershell
powershell -ExecutionPolicy Bypass -File .\tools\collect-matepad-profile.ps1 `
  -Adb "D:\platform-tools-latest-windows\platform-tools\adb.exe" `
  -Output ".\matepad-profile.txt"
```

如果同时连接了多台设备，先运行 `adb devices`，再增加：

```powershell
-Serial "平板的ADB序列号"
```

报告覆盖以下类别：

- Build 品牌、型号、device、product、board、hardware 和 fingerprint；
- Android、EMUI/HarmonyOS 可见版本与安全补丁；
- CPU ABI 与平台；
- 物理/覆盖分辨率及 density；
- 平板、触摸屏和传感器相关 feature。

这些数据只能用来修正壶内公开参数的一致性。它不会复制实体平板的硬件密钥，也不会修改微信内部安全判断或伪造硬件证明。
