# 第三方与来源边界

两目录的Library、Startup及原user/stm32f10x_conf.h、stm32f10x_it.c/h为ST SPL/CMSIS/厂商模板；本地副本以旧版权/免责为主，没有找到清楚、可核对的完整再分发许可文件。本项目不据此断言它们不能使用，也不以根MIT覆盖或重新发布。62个独立文件的名称、字节数、SHA保存在dependencies/stm32f10x-files.json；由使用者按其取得版本与协议独立准备。官方产品入口是https://www.st.com/en/embedded-software/stsw-stm32054.html，网页本身不代替软件包许可。

原独立OLED.c/h在root与user重复，含PB8/9驱动和GPIO供电示例，缺所引用的OLED_Font.h；精确教程/作者/再分发许可未确立，因此4次副本留本地，未纳入公开自有MIT。公开active main是另一个内嵌PB6/7数字显示练习，来源标为本机课堂增量，未将整套厂商/教程驱动冒称原创。

Arm Compiler、Keil/µVision、STM32CubeIDE/CubeMX均需独立安装并遵守各自许可；不捆绑软件安装文件、注册资料、device pack缓存、ST-Link日志或生成固件。原代码/字模的课堂与AI辅助属性按现有文件保留，不声称自研SDK或完整字体。

旧CMSIS的Armclang兼容宏使用Arm公开的编译器接口；__DSB映射参考[Arm官方CMSIS实现](https://github.com/ARM-software/CMSIS_5/blob/develop/CMSIS/Core/Include/cmsis_armclang.h)，未复制整个新版CMSIS或改原SDK。
