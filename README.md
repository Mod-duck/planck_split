# Planck Split

[![Build ZMK firmware](https://github.com/Modulus010/planck_split/actions/workflows/build.yml/badge.svg)](https://github.com/Modulus010/planck_split/actions/workflows/build.yml)
[![Draw keymap](https://github.com/Modulus010/planck_split/actions/workflows/draw.yml/badge.svg)](https://github.com/Modulus010/planck_split/actions/workflows/draw.yml)

自定义分体 Planck 键盘的 ZMK 固件，基于 nRF52840 自研 PCB。

## 硬件

| 项目 | 规格 |
|------|------|
| MCU | nRF52840（板载，非开发板） |
| 布局 | 4×12 split ortholinear（每侧 24 键） |
| 扫描方式 | Direct GPIO（无二极管矩阵） |
| LED | 每侧 3× PWM LED |
| 电池检测 | nRF VDDH 内部采样 |
| 连接 | BLE 5.0（split）+ USB |
| 电源 | DCDC reg0 + reg1 |

## 层级

7 层 Miryoku 风格布局，通过拇指键 hold 触发：

| # | Layer | 触发 | 说明 |
|---|-------|------|------|
| 0 | BASE | 默认 | QWERTY + home row mods (GACS) |
| 1 | NAV | Hold Space | 导航、剪贴板、caps word |
| 2 | MOUSE | Hold Tab | 鼠标移动和滚轮 |
| 3 | MEDIA | Hold Esc | 媒体控制、RGB、蓝牙 |
| 4 | NUM | Hold Bksp | 数字键盘 |
| 5 | SYM | Hold Enter | 符号 |
| 6 | FUN | Hold Del | F1–F12 |

左上/右上角 `&to` 键可循环切层：BASE → NAV → MOUSE → MEDIA → NUM → SYM → FUN → BASE

### 键位图

> 由 [keymap-drawer](https://github.com/caksoylar/keymap-drawer) 自动生成，修改 keymap 后 push 即更新。

![Planck Split Keymap](keymap-drawer/planck.svg)

## 构建

固件通过 GitHub Actions 自动构建，push `config/` 或 `build.yaml` 即触发。

### 下载固件

1. Push 代码到仓库
2. 进入 **Actions** → **Build ZMK firmware**
3. 下载 artifact（`planck_left` 和 `planck_right` 的 UF2 文件）

### 本地构建

```bash
west init -l config
west update

# 左手
west build -s zmk/app -b planck_left//zmk -- -DZMK_CONFIG="$(pwd)/config"

# 右手
west build -s zmk/app -b planck_right//zmk -- -DZMK_CONFIG="$(pwd)/config"
```

## 刷写

1. 双击 reset 进入 UF2 bootloader
2. 复制 `.uf2` 到挂载的驱动器：
   ```bash
   # macOS
   cp build/zephyr/zmk.uf2 /Volumes/NRF52BOOT/
   # Linux
   cp build/zephyr/zmk.uf2 /media/$USER/NRF52BOOT/
   ```
3. 先刷左手（central），再刷右手（peripheral）

## 目录结构

```
├── config/
│   ├── west.yml                     # West manifest
│   ├── planck.keymap                # 键位映射
│   ├── planck.conf                  # 用户配置（功能开关）
│   ├── planck.json                  # Physical layout（编辑器用）
│   ├── include/
│   │   ├── layers.h                 # 层级定义
│   │   ├── behaviors.dtsi           # Hold-tap behaviors
│   │   └── macros.dtsi              # BT 选择器宏
│   └── boards/modifier_key/planck/  # 板级定义
│       ├── board.yml                # HWMv2 板级描述
│       ├── board.cmake              # Flash runners
│       ├── planck.dtsi              # 共享硬件：SoC、矩阵、布局
│       ├── planck_left_nrf52840_zmk.dts   # 左手 DTS
│       ├── planck_right_nrf52840_zmk.dts  # 右手 DTS
│       ├── planck_left_nrf52840_zmk_defconfig  # 左手 defconfig
│       ├── planck_right_nrf52840_zmk_defconfig # 右手 defconfig
│       ├── leds_left.dtsi           # 左手 PWM LED
│       ├── leds_right.dtsi          # 右手 PWM LED
│       ├── Kconfig.defconfig        # 板级 Kconfig 默认值
│       ├── Kconfig.planck_left      # 左手 Kconfig
│       └── Kconfig.planck_right     # 右手 Kconfig
├── keymap-drawer/                   # 自动生成的键位图
│   ├── planck.yaml                  # 解析后的 keymap YAML
│   └── planck.svg                   # 键位布局 SVG
├── keymap_drawer.config.yaml        # keymap-drawer 配置
├── build.yaml                       # 构建矩阵
├── .github/workflows/
│   ├── build.yml                    # 固件构建 CI
│   └── draw.yml                     # 键位图生成 CI
```

## 蓝牙

- 5 个 BT 配置文件（0–4），在 MEDIA 层切换
- 点按选择配置文件，Shift+点按选择并清除
- 发射功率 +8 dBm
- 左手获取并代理右手电池电量
