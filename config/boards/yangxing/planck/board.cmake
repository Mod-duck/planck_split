# SPDX-License-Identifier: MIT

board_runner_args(pyocd "--target=nrf52840")
board_runner_args(nrfjprog "--nrf-family=NRF52" "--softreset")
set(OPENOCD_NRF5_INTERFACE "cmsis-dap")
include(${ZEPHYR_BASE}/boards/common/pyocd.board.cmake)
include(${ZEPHYR_BASE}/boards/common/nrfjprog.board.cmake)
include(${ZEPHYR_BASE}/boards/common/uf2.board.cmake)
