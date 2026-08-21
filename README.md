# Planck Split

[![Build ZMK firmware](https://github.com/Mod-duck/planck_split/actions/workflows/build.yml/badge.svg)](https://github.com/Mod-duck/planck_split/actions/workflows/build.yml)
[![Draw keymap](https://github.com/Mod-duck/planck_split/actions/workflows/draw.yml/badge.svg)](https://github.com/Mod-duck/planck_split/actions/workflows/draw.yml)

自定义分体 Planck 键盘（4×12 split ortholinear）的 [ZMK](https://zmk.dev/) 固件，基于自研 nRF52840 PCB。键盘以 ZMK module 形式托管，键位采用 [Miryoku](https://github.com/manna-harbour/miryoku) 布局的手写移植，左右半独立电量显示。

## 特性

- **10 层 Miryoku 键位**：QWERTY + home row mods、拇指层按、导航/鼠标/媒体/数字/符号/功能层
- **分体 BLE**：左半 central（USB 输出），右半 BLE peripheral，双侧配对持久化（NVS）
- **电量指示**：每侧 3 颗 PWM LED 组成连续三段电量条，显示本半 VDDH 采样值
- **低功耗**：DCDC 供电、内部 RC 时钟 + 校准（无外部晶振）、深度睡眠
- **UF2 引导**：双击 reset 进入 bootloader，拖拽刷写
- **自动 CI**：左右半固件构建 + keymap-drawer 键位图渲染

## 硬件

| 项目 | 规格 |
|------|------|
| MCU | nRF52840（板载，非开发板） |
| 布局 | 4×12 split ortholinear（每侧 24 键） |
| 扫描方式 | Direct GPIO（无二极管矩阵） |
| LED | 每侧 3× PWM LED（本地电量条） |
| 电池检测 | nRF VDDH 内部采样（`zmk,battery-nrf-vddh`） |
| 连接 | BLE 5.0（split）+ USB（左半） |
| 电源 | DCDC reg0 + reg1；无外部 32.768 kHz 晶振 |

## 键位

布局为 [Miryoku](https://github.com/manna-harbour/miryoku) 的手写移植（QWERTY 字母区），放置在原始 4×12 位置骨架：每侧 5 个内容列、底行中间内收的 3 个拇指键、外侧工具列与底角修饰键。

### 层级

| # | Layer | 触发 | 说明 |
|---|-------|------|------|
| 0 | BASE | 默认 | QWERTY + home row mods |
| 1 | EXTRA | 双击 `&u_to_U_EXTRA` | 同 BASE 的 QWERTY 备份层 |
| 2 | TAP | 双击 `&u_to_U_TAP` | 无双功能键的纯打字层 |
| 3 | BUTTON | 按住 `Z` 或 `/` | 鼠标按键、剪贴板、修饰键 |
| 4 | NAV | Hold `Space` | 方向键、剪贴板、caps word |
| 5 | MOUSE | Hold `Tab` | 鼠标移动与滚轮 |
| 6 | MEDIA | Hold `Esc` | 媒体控制、RGB/外部电源、蓝牙 |
| 7 | NUM | Hold `Bksp` | 数字键盘 |
| 8 | SYM | Hold `Enter` | 符号 |
| 9 | FUN | Hold `Del` | F1–F12 |

关键行为：

- **Home row mods**：`A=LGUI` `S=LALT` `D=LCTRL` `F=LSHFT`；`J=LSHFT` `K=LCTRL` `L=LALT` `;=LGUI`
- **拇指键**（BASE）：左 `Esc`/`Space`/`Tab`（按 → MEDIA/NAV/MOUSE），右 `Enter`/`Bksp`/`Del`（按 → SYM/NUM/FUN）
- **顶排两角**：`Esc` / `Backspace`（所有层一致）
- **返回 BASE**：在覆盖层双击 `&u_to_U_BASE`
- **Caps Word**：NAV 层 `Caps Word`（Shift+按 = Caps Lock）
- **BT 切换**：MEDIA 层 `BT 0–3`；Shift+按 = 选择并清除配对

### 键位图

> 由 [keymap-drawer](https://github.com/caksoylar/keymap-drawer) 自动生成，push keymap 后自动更新。

![Planck Split Keymap](keymap-drawer/planck.svg)

## 构建

### 前置

- 本地构建需要 [ZMK 开发环境](https://zmk.dev/docs/development/setup)（west、Zephyr SDK）
- `config/west.yml` 使用 `revision: main`，不固定 commit

### GitHub Actions

push `config/`、`boards/`、`src/`、`CMakeLists.txt`、`Kconfig`、`build.yaml` 或 workflow 即触发：

1. 进入 **Actions** → **Build ZMK firmware**
2. 下载 artifact，包含 `planck_left.uf2` 与 `planck_right.uf2`

### 本地构建

```bash
# 在仓库根目录执行；使用独立 workspace，避免 west checkout 与 module 文件冲突。
REPO_ROOT="$(pwd)"
mkdir -p /tmp/planck-zmk-workspace
cd /tmp/planck-zmk-workspace
west init -l "$REPO_ROOT/config"
west update

# 左手（central，使用 zmk-usb-logging snippet）
west build -s zmk/app -d build/left -b planck_left//zmk -- \
  -DZMK_CONFIG="$REPO_ROOT/config" -DZMK_EXTRA_MODULES="$REPO_ROOT"

# 右手（peripheral）
west build -s zmk/app -d build/right -b planck_right//zmk -- \
  -DZMK_CONFIG="$REPO_ROOT/config" -DZMK_EXTRA_MODULES="$REPO_ROOT"
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
3. 先刷左半（central），再刷右半（peripheral）

## 蓝牙

- 5 个 BT 配置文件（0–4），在 MEDIA 层切换
- 点按选择配置文件，Shift+点按选择并清除
- 左半同时支持 USB 输出（MEDIA 层 `Output Toggle`）
- `BT_MAX_CONN`/`BT_MAX_PAIRED` 采用 ZMK central 默认值（6/6）

## 电量指示

`src/battery_leds.c` 监听本半 `zmk_battery_state_changed` 事件，将 VDDH 电量映射为三段连续亮度条（0% 全灭，100% 三颗全亮），由 `CONFIG_PLANCK_BATTERY_LEDS` 控制。

## 目录结构

```
├── config/
│   ├── west.yml                     # West manifest（ZMK main）
│   ├── planck.keymap                # 键位映射（Miryoku 移植）
│   ├── planck.conf                  # 左右共享用户配置
│   ├── planck_left.conf             # 左半应用配置（central/USB/HID/pointing）
│   ├── planck.json                  # 布局定义（keymap-drawer 用）
│   ├── include/
│   │   ├── layers.h                 # 层编号
│   │   ├── miryoku.h                # Miryoku 键位宏（U_*）
│   │   ├── behaviors.dtsi           # u_mt/u_lt hold-tap、双击层切换
│   │   └── macros.dtsi              # BT/输出切换 mod-morph 宏
├── boards/yangxing/planck/          # 板级定义（ZMK HWMv2 + zmk variant）
│   ├── board.yml                    # Board 元数据
│   ├── Kconfig.planck_left          # 左半 Kconfig（SOC/board compat）
│   ├── Kconfig.planck_right         # 右半 Kconfig
│   ├── Kconfig.defconfig            # 板级默认值（BLE/split/flash/NVS）
│   ├── board.cmake                  # Flash runners
│   ├── planck.dtsi                  # 共享：SoC、矩阵变换、布局、分区、电池
│   ├── leds.dtsi                    # PWM + pinctrl + 三颗 LED
│   ├── planck_{left,right}_nrf52840_zmk.dts        # 左右半 DTS
│   └── planck_{left,right}_nrf52840_zmk_defconfig  # 左右半 defconfig
├── zephyr/module.yml                # Zephyr module 声明
├── CMakeLists.txt                   # 电量 LED module 源码
├── Kconfig                          # PLANCK_BATTERY_LEDS 配置
├── src/battery_leds.c               # 三段电量指示实现
├── keymap-drawer/                   # 自动生成的键位图
├── keymap_drawer.config.yaml        # keymap-drawer 配置
├── build.yaml                       # 构建矩阵
└── .github/workflows/
    ├── build.yml                    # 固件构建 CI
    └── draw.yml                     # 键位图渲染 CI
```

## License

[MIT](LICENSE)

## 致谢

- [ZMK Firmware](https://zmk.dev/) — 固件框架
- [Miryoku](https://github.com/manna-harbour/miryoku) / [Miryoku ZMK](https://github.com/manna-harbour/miryoku_zmk) — 键位设计与 ZMK 移植参考
- [keymap-drawer](https://github.com/caksoylar/keymap-drawer) — 键位图渲染
