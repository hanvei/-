# Android 空间管理演示版

这是一个由 Island 的工作资料管理与文件穿梭能力、以及 Shelter 风格的熄屏自动冻结整合而成的 Android 演示版。项目不要求 Root，但创建工作资料仍须按 Android 系统界面完成授权。

## 当前已构建功能

- Island 的个人空间（Mainland）和工作资料管理、克隆/移除应用、冻结/解冻。
- 在应用操作菜单中勾选“熄屏时自动冻结”，该应用从 Island 启动后会在下次熄屏时冻结。自动冻结只对已选应用生效。
- Island 原有文件穿梭入口；上游说明该功能需要用电脑通过 ADB 授权。
- CI 已通过并生成可安装的 debug APK。APK 使用 Android debug 签名，仅适合演示测试。

## 安装与第一次打开

1. 打开 GitHub 仓库的 [Android demo 构建记录](https://github.com/hanvei/-/actions/runs/39267615244)，在页面下方 **Artifacts** 下载 `android-space-demo-debug` 并解压。
2. 把 `assembly-complete-debug.apk` 传到手机，点开安装；如果系统询问，允许当前来源安装应用。
3. 打开应用，按系统提示创建工作资料。完成后在 Island 标签页选择工作资料，再克隆要管理的应用。
4. 长按工作资料中的应用，在操作菜单打开“熄屏时自动冻结”。启动该应用后，锁屏/熄屏时会冻结；下次需要使用时从管理器启动即可解冻并打开。

应用 ID 暂时沿用 `com.oasisfeng.island`。如果手机已经安装正式版 Island，debug 签名不同，不能作为更新覆盖安装。此演示 APK 也没有在实体手机上完成工作资料创建测试；安装、注册管理器和设备兼容性仍需实机验证。

## SD 卡边界

Android 不允许普通无 Root 应用把工作资料本身及其私有应用数据整体搬到可移除 SD 卡运行；安装 APK 到 SD 卡也由系统、设备厂商和工作资料策略决定，不能保证可用。当前 APK 没有实现“把整个空间放在 SD 卡运行”。现有文件穿梭可以协助跨空间访问文件，但不等同于把工作资料迁移到 SD 卡。若设备的系统文件选择器提供 SD 卡，可在支持 SAF 的应用中单独选择 SD 卡文件。

## 重新构建

在 GitHub 仓库选择 **Actions → Android demo → Run workflow**。成功后从该次运行的 Artifacts 下载 APK。工作流使用 JDK 17、Android API 33/35 和 Gradle Wrapper。

Island、Deagle、Condom 与 Shelter 的上游源码及许可证文件保存在各自目录；本项目没有直接复制 Shelter 的业务源代码，而是在 Island 的工作资料控制能力上新写熄屏冻结服务。
