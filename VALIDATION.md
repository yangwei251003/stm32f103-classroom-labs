# 验证范围

2026-10-26，Windows/Python3.12、MDK Plus5.25的Arm Compiler6.9实际CLI。

- 外部SDK严格恢复62文件，SHA与课堂原件全同；未改SDK，也不将其上传。
- examples/led-key与oled-digits分别完成5个C编译、1个启动汇编、1个真实链接，共14步骤退出0。
- 64KiB Flash/20KiB RAM取自原Keil目标设置；解析实际link map核对各区间未越界。LED ROM1440B、RW+ZI1132B；OLED压缩ROM2284B、RW+ZI1452B。这不是实芯片容量检测或板级验证。
- 日志警告计数与逐步结果、地址布局见[build-validation.json](docs/build-validation.json)。实际原LED main的错误include编译失败已复现，未把历史快照称作可构建原成品。
- 342原目录文件和独立hh.ioc最终SHA保全核对；12公开原课堂源码/工程与hh.ioc均保持原字节。最小修正版与原始快照分开。
- Keil XML读回通过，未启动µVision/CubeIDE/EDA/Bridge；没有设备连接、烧录、实际屏幕/按键/波形测试。CLI当前能用不代表他人自动具有工具链授权。

原先未定义__INLINE的旧CMSIS与Armclang组合编译失败；首次补inline后出现旧头文件__DSB声明警告，按Arm官方CMSIS映射补编译宏后14步重建0错误/0警告；不修改厂商文件。软件成功不证明OLED地址、ACK、上拉、电源、晶振、字模布局和SysTick实际延时正确。厂商许可/未明来源驱动的缺口保留，不据归档删除本地原件。
