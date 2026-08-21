/* SPDX-License-Identifier: MIT */

#pragma once

#define U_NP &none // key is not present
#define U_NA &none // present but not available for use
#define U_NU &none // available but not used

#define U_MT(MOD, TAP) &u_mt MOD TAP
#define U_LT(LAYER, TAP) &u_lt LAYER TAP

#define U_BTN1 &mkp MB1
#define U_BTN2 &mkp MB2
#define U_BTN3 &mkp MB3

#define U_MS_D &mmv MOVE_DOWN
#define U_MS_L &mmv MOVE_LEFT
#define U_MS_R &mmv MOVE_RIGHT
#define U_MS_U &mmv MOVE_UP
#define U_WH_D &msc SCRL_DOWN
#define U_WH_L &msc SCRL_LEFT
#define U_WH_R &msc SCRL_RIGHT
#define U_WH_U &msc SCRL_UP

#define U_RDO &kp K_AGAIN
#define U_PST &kp LS(INS)
#define U_CPY &kp LC(INS)
#define U_CUT &kp LS(DEL)
#define U_UND &kp K_UNDO

#define U_RGB_TOG &rgb_ug RGB_TOG
#define U_RGB_EFF &rgb_ug RGB_EFF
#define U_RGB_HUI &rgb_ug RGB_HUI
#define U_RGB_SAI &rgb_ug RGB_SAI
#define U_RGB_BRI &rgb_ug RGB_BRI
#define U_EP_TOG &ext_power EP_TOG

#define U_BOOT &bootloader
