# 测试手机操作与变更记录

设备：`PQY0220B11031530`（NOH-AN00）。本记录从 2026-10-22 继续测试起建立；此前操作见 `README.md`，尚未逐项追溯成完整审计记录。

| 日期 | 操作 | 操作前 / 结果 | 涉及范围 |
| --- | --- | --- | --- |
| 2026-10-22 | 连接 ADB 并只读查询设备、代理、私有 DNS、屏幕参数与包名 | 设备在线；全局 HTTP 代理 `127.0.0.1:21882`；私有 DNS `null`；物理分辨率 `1344x2772`，已有覆盖分辨率 `1152x2376`；物理密度 `560`；系统微信和测试 BlackBox 均已安装。未更改这些值 | 系统与网络状态只读 |
| 2026-10-22 | 尝试用 `adb install -r` 覆盖安装 PAD PoC 新版 BlackBox | 系统返回 `INSTALL_FAILED_UPDATE_INCOMPATIBLE`（GitHub Actions 两次构建的临时调试签名不同）；安装失败，原测试 BlackBox 及数据保持原状 | 只针对 `com.onebitmonochrome.blacksbbox`；未卸载或清数据 |
| 2026-10-22 | 使用 `adb install` 安装第二个独立 PAD PoC 测试壶 | 安装返回 `Success`；新包名 `com.onebitmonochrome.blacksbbox.padpoc2`。没有覆盖旧测试壶或其他应用 | 新增一个测试应用；尚未改系统/网络设置 |
| 2026-10-22 | 用 ADB 启动新测试壶；对首次“存储权限”提示选择 `LATER` | 新测试壶进入首页。未授予“管理所有文件”权限，也未更改系统权限设置 | 只启动新测试应用并关闭其提示 |
| 2026-10-22 | 在新测试壶的“已安装应用”列表里搜索并导入 `org.example.padprobe`，随后启动其虚拟副本 | 虚拟探针显示 `MODEL=HUAWEI MatePad`、华为 PAD 特性 `true`，但 `ro.build.characteristics=default`、`ro.product.model=NOH-AN00`。截图 `blackbox-padpoc2-probe-result.png` | 仅新测试壶内部增加 PAD Probe 副本并启动；本机探针及其他应用数据未修改 |
| 2026-10-22 | 用 `adb push` 将测试微信 APK 放入 `/sdcard/Download/WeChat-PAD-test-8.0.76.apk` | 推送成功，文件大小 266,102,064 字节；尚未安装或启动 | 只在共享 Download 目录新增一个 APK 文件；系统微信及其数据未修改 |
| 2026-10-22 | 在新测试壶中选择“手动选择”，经系统文件选择器选取上述 APK | 新测试壶首页出现“微信”图标，表示已导入虚拟副本；尚未登录 | 只在 `com.onebitmonochrome.blacksbbox.padpoc2` 内新增测试微信；未选择本机已安装的系统微信 |
| 2026-10-22 | 启动新测试壶中的虚拟微信，查看登录选择页 | 首屏出现唯一入口“仅在平板上使用”，截图 `wechat-virtual-pad-screen.png`；前台宿主为 `...padpoc2` 的代理 Activity，虚拟页面为 `com.tencent.mm.plugin.account.ui.LoginSelectUI` | 只启动虚拟微信；没有输入账号或登录 |
| 2026-10-22 | 点击虚拟微信的“仅在平板上使用”入口 | 进入“手机号登录”页面，截图 `wechat-after-tablet-option.png`；未输入号码，未接受条款或登录 | 仅测试微信内部页面导航 |
| 2026-10-22 | 在虚拟微信手机号登录页点“更多”查看入口 | 仅显示“冻结账号 / 前往安全中心 / 反馈问题 / 取消”；没有扫描二维码入口；未进行进一步操作 | 只读查看测试微信内部菜单 |
| 2026-10-22 | 多次用 `uiautomator dump` 读取测试应用界面 | 在 `/sdcard/` 临时生成 `window2.xml` 至 `window29.xml`，读取后仅删除这些本次生成的临时文件。已有 `/sdcard/window.xml` 保留，未删除 | 仅测试用临时 UI 树文件；无系统或网络设置变更 |
| 2026-10-22 | 结束前只读复查代理、私有 DNS、屏幕参数及相关包名 | 与开始时一致：代理 `127.0.0.1:21882`、DNS `null`、物理 `1344x2772`、覆盖 `1152x2376`、密度 `560`。原测试壶与系统微信仍已安装，新测试壶也已安装 | 系统/网络设置未在本次测试中改变 |
| 2026-10-22 | 在测试微信内从手机号页返回选择页、等待并截图 | 屏幕仍只显示“仅在平板上使用”，随后测试壶出现“无响应”提示；`dumpsys activity lastanr` 指向虚拟 `LoginSelectUI` 输入事件等待。截图 `wechat-login-select-after-wait.png`，过滤日志 `padpoc2-anr-filtered.log` | 仅新测试壶内页面导航；未关闭、清除或修改其他应用 |
| 2026-10-22 | 只读检查电脑代理端口、手机代理和 ADB reverse；随后执行 `adb reverse tcp:21882 tcp:21882` | 电脑 `127.0.0.1:21882` 正在监听；手机全局代理原已设为 `127.0.0.1:21882`，但此前没有 ADB reverse。当前临时映射为 `UsbFfs tcp:21882 tcp:21882` | 新增随 USB/ADB 连接存在的临时端口转发；**没有**改手机全局代理、DNS 或其他系统设置。可用 `adb reverse --remove tcp:21882` 撤销 |
| 2026-10-22 | 对新测试壶 ANR 弹窗选择“关闭应用”，随后尝试重开；最后按 Home 返回手机桌面 | 新测试壶重开后曾停在黑屏，未清除它的数据；没有启动原版 GBox 或系统微信 | 只结束并重启测试壶进程，改变前台界面；未改应用数据或系统设置 |
| 2026-10-22 | 用只读 ICMP 检查手机直连 `1.1.1.1` 和 `github.com` | 两者都可 ping 通；这只证明直连和 DNS 可用，不能证明微信接口可用 | 网络探测，无设置变更 |
| 2026-10-22 | 在电脑编译临时 Java 代理探针，两次放到手机 `/data/local/tmp/padpoc-proxyprobe.jar` 运行，运行后均删除 | 第一次 `app_process` 被系统终止；随后 `dalvikvm` 连接 `127.0.0.1:21882` 得 `ECONNREFUSED`，复查发现临时 ADB reverse 已消失。重新执行 `adb reverse tcp:21882 tcp:21882` 后立刻复测，代理返回 `HTTP/1.1 200 Connection established` | 手机仅短暂新增并删除本次探针文件；重新建立 USB 临时端口映射；未改手机全局代理、DNS 或其他应用数据。映射可能因 USB/ADB 断线消失 |
| 2026-10-22 | 对新测试壶执行 `am force-stop`（不清数据），重新启动并打开其中的测试微信 | 测试壶恢复正常显示，虚拟微信再次进入“仅在平板上使用”选择页。代理端口映射此时仍存在；等待约 30 秒后仍未出现“手机和平板同时使用”，截图 `wechat-after-proxy-fix.png` 和 `wechat-after-proxy-wait.png` | 仅结束/启动新测试壶进程及虚拟微信；未改应用数据或系统/网络设置 |
| 2026-10-22 | 按用户新要求停止网络测试，执行 `adb reverse --remove tcp:21882` | 本轮建立的 USB 临时转发已移除；复查手机全局代理为 `:0`（与先前读到的 `127.0.0.1:21882` 不同，变化来源未确认，**不是本轮设置**） | 只撤销本轮临时 ADB 映射；没有写手机全局代理、DNS 等设置 |
| 2026-10-22 | 在新测试壶 `Device Spoofing` 页面将虚拟机型设为 `HUAWEI Mate X6`，保留原 Android ID `2012257f56000a7f` | `DeviceSpoofProfiles.xml` 证实 `model_0=HUAWEI Mate X6`、`manufacturer_0=HUAWEI`。应用同时按机型重算壶内 `brand/device/product/fingerprint/serial`；一次中途输入失败后已重新核对，最终保存值正确 | **只修改新测试壶的虚拟配置**；手机真实机型、屏幕和其他应用未改 |
| 2026-10-22 | 不清数据重启新测试壶，依次启动壶内 PAD Probe 和测试微信 | 探针显示 `MODEL=HUAWEI Mate X6`、华为 PAD 特性 `true`、`screenWidthDp=384`、`ro.build.characteristics=default`；测试微信仍只有“仅在平板上使用”。截图 `matex6-probe.png`、`matex6-wechat-login.png` | 仅新测试壶和内部应用的进程/前台页面变化；未登录、未清除数据 |
| 2026-10-22 | 删除本次 UI 读取生成的 `/sdcard/padpoc2_main_menu.xml`、`padpoc2_spoof_edit.xml`、`padpoc2_profile_test.xml`，随后只读复查 | 物理分辨率 `1344x2772`、原有覆盖 `1152x2376`、密度 `560` 未变；全局代理仍为 `:0`；ADB reverse 列表为空 | 只删除本次生成的临时 UI 文件；未改系统屏幕/网络设置 |
| 2026-10-23 | 从 GitHub Actions 构建 39815448187 下载固定签名的 MatePad 参数测试壶，并用 ADB 安装 arm64 包 | 安装返回 `Success`；新包名为 `com.onebitmonochrome.blacksbbox.padpoc3`，APK SHA-256 `B7C70D4CB3D71E10DC296AD35EF2183FD88E6B0EF6D9F6608D737BF73A812EBA` | 仅新增第三个独立测试壶；没有覆盖、卸载或清除旧测试壶、系统微信或其他应用，也没有改系统/网络设置 |

后续每项对手机的安装、启动、导入、设置或网络更改均追加在这里。不得清除或更改系统微信与其他既有应用的数据。

### 2026-10-23 约 01:00 — 首次启动独立测试壶 padpoc3
- 设备：PQY0220B11031530（NOH-AN00）
- 操作：通过 ADB 启动 `com.onebitmonochrome.blacksbbox.padpoc3` 的 WelcomeActivity。
- 系统提示：应用请求存储管理权限；选择 `LATER`，未授予“所有文件访问”权限。
- 取证：临时生成并拉取 `padpoc3_first.xml`、`padpoc3_home.xml`，随后仅删除手机 `/sdcard/` 下这两个临时 XML；本地副本保留。
- 网络：未修改。
- 数据影响：未清除任何应用数据；未操作系统微信或其他应用资料。

### 2026-10-23 约 01:05 — 导入 PAD Probe 到 padpoc3
- 操作：在 padpoc3 的已安装应用选择器中搜索 `padprobe`，只选择并导入 `org.example.padprobe`。
- 未选择/导入系统微信或其他手机应用。
- 网络：未修改。
- 数据影响：仅在新建的 padpoc3 虚拟环境中创建 PAD Probe 副本；未清除数据。
- 取证：临时 UI XML 拉取后删除手机端临时文件，本地保留。

### 2026-10-23 约 01:24 — 覆盖安装 padpoc3 参数模拟修正版
- GitHub Actions：`39821367471`，成功。
- APK：`BlackBox_4.7.1_arm64-v8a-debug.apk`，SHA-256 `E97EC972BA9CC40C49A37462506C625B99816792BA1BF6F757BD469CFE45E9DD`。
- 操作：ADB 使用 `install --no-streaming -r` 覆盖安装 `com.onebitmonochrome.blacksbbox.padpoc3`，结果 Success。
- 目的：补充现代系统属性读取拦截，并在虚拟 Application 创建后重新应用 MatePad 资源配置。
- 数据影响：`-r` 保留 padpoc3 数据；未清除任何应用数据；未操作系统微信或其他手机应用。
- 网络：未修改。

### 2026-10-23 约 01:48 — 覆盖安装 Activity 平板资源修正版
- GitHub Actions：`39825693122`，成功。
- APK SHA-256：`33FAD8E941FEB94F88495EC1A0529EA7E4754A7CFF2941574B873653AAF97031`。
- 操作：ADB `install --no-streaming -r` 覆盖安装 padpoc3，结果 Success；保留虚拟应用数据。
- 目的：在每个虚拟 Activity 的 `onCreate` 前应用 MatePad 屏幕配置。
- 数据/网络：未清除数据，未操作系统微信或其他应用，未修改网络。

### 2026-10-23 约 02:03 — 导入独立测试微信到 padpoc3
- 来源：手机内部存储 `/sdcard/Download/WeChat-PAD-test-8.0.76.apk`。
- 操作：通过 padpoc3“手动选择”文件选择器导入；壶主页出现“微信”。
- 边界：未从已安装应用列表复制 `com.tencent.mm`，未启动或修改手机系统微信。
- 数据/网络：仅在 padpoc3 内新建测试微信；未清除数据，未修改网络。

### 2026-10-23 02:02–02:07 — 完整 MatePad 参数与微信 PAD 登录验证成功
- PAD Probe 在 padpoc3 内实测：
  - `MODEL: DBY-W09`
  - `BRAND/MANUFACTURER: HUAWEI`
  - `ro.product.model: DBY-W09`
  - `ro.build.characteristics: tablet`
  - `Huawei PAD feature: true`
  - `screenWidthDp: 914`
- 启动 padpoc3 内独立安装的测试微信 8.0.76，登录页出现“平板和手机同时登录”。
- 继续点入该入口后，出现“同时在手机和平板上使用”和“仅在平板上使用”两种选择，确认 PAD 登录流程被启用。
- 证据文件：`padpoc3_probe_v3.xml`、`wxpadresult.xml/png`、`wxpadnext.xml/png`、`wxpadresult_logcat.txt`。
- 判断：参数模拟路线已使微信自身的平板资格检查通过。未修改微信内部 `checkLoginAsPad()`，未强制返回 true；结果来自壶内统一设备参数后微信展示的原生流程。
- 数据/网络：未清除数据；未操作系统微信或手机其他应用资料；未修改网络。
- 当前手机状态：停留在测试微信的 PAD 使用方式选择页；不再继续操作。
