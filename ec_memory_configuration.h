#ifndef __MSI_EC_REGISTERS_CONFIG__
#define __MSI_EC_REGISTERS_CONFIG__

#include <linux/types.h>

#define MSI_EC_DRIVER_NAME "msi-ec-ng"

#define MSI_EC_ADDR_UNKNOWN 0xff01 // unknown address
#define MSI_EC_ADDR_UNSUPP  0xff01 // unsupported parameter

/* Optional feature bits, set per configuration (CONF_*) */
#define MSI_EC_FEAT_FAN_CURVE  (1U << 0)
#define MSI_EC_FEAT_FAN_RPM    (1U << 1)
#define MSI_EC_FEAT_USB_SHARE  (1U << 2)
#define MSI_EC_FEAT_QC         (1U << 3)
#define MSI_EC_FEAT_PANEL      (1U << 4)

#define MSI_EC_FAN_TACH_CONST  480000 /* 60e6 / (2 * 62.5) */

// Firmware info addresses are universal
#define MSI_EC_FW_VERSION_ADDRESS 0xa0
#define MSI_EC_FW_DATE_ADDRESS    0xac
#define MSI_EC_FW_TIME_ADDRESS    0xb4
#define MSI_EC_FW_VERSION_LENGTH  12
#define MSI_EC_FW_DATE_LENGTH     8
#define MSI_EC_FW_TIME_LENGTH     8

struct msi_ec_webcam_conf {
	int address;
	int block_address;
	int bit;
};

struct msi_ec_fn_win_swap_conf {
	int address;
	int bit;
	bool invert;
};

struct msi_ec_cooler_boost_conf {
	int address;
	int bit;
};

#define MSI_EC_MODE_NULL { NULL, 0 }
struct msi_ec_mode {
	const char *name;
	int value;
};

#define MSI_EC_SHIFT_MODE_NAME_LIMIT 20
struct msi_ec_shift_mode_conf {
	int address;
	struct msi_ec_mode modes[8]; // fixed size for easier hard coding
};

struct msi_ec_super_battery_conf {
	int address;
	int mask;
};

struct msi_ec_fan_mode_conf {
	int address;
	struct msi_ec_mode modes[8]; // fixed size for easier hard coding
};

/*
 * Six temperature points followed by six duty points, plus an optional
 * idle duty used below the first temperature point.
 */
#define MSI_EC_FAN_CURVE_POINTS 6
struct msi_ec_fan_curve_conf {
	int idle_address;
	int temp_address;
	int duty_address;
	int points;
};

/* 16-bit tachometer, low byte at tach_address, high byte at tach_address - 1 */
struct msi_ec_fan_rpm_conf {
	int tach_address;
};

struct msi_ec_usb_power_share_conf {
	int address;
};

struct msi_ec_qc_conf {
	int address;
	int bit;
};

struct msi_ec_panel_conf {
	int address;
	int bit;
};

struct msi_ec_cpu_conf {
	int rt_temp_address;
	int rt_fan_speed_address; // realtime % RPM
};

struct msi_ec_gpu_conf {
	int rt_temp_address;
	int rt_fan_speed_address; // realtime % RPM
};

struct msi_ec_led_conf {
	int micmute_led_address;
	int mute_led_address;
	int bit;
};

#define MSI_EC_KBD_BL_STATE_MASK 0x3
struct msi_ec_kbd_bl_conf {
	int bl_mode_address;
	int bl_modes[2];
	int max_mode;

	int bl_state_address;
	int state_base_value;
	int max_state;
};

struct msi_ec_conf {
	const char **allowed_fw;

	unsigned int features;

	int charge_control_address;
	struct msi_ec_webcam_conf         webcam;
	struct msi_ec_fn_win_swap_conf    fn_win_swap;
	struct msi_ec_cooler_boost_conf   cooler_boost;
	struct msi_ec_shift_mode_conf     shift_mode;
	struct msi_ec_super_battery_conf  super_battery;
	struct msi_ec_fan_mode_conf       fan_mode;
	struct msi_ec_cpu_conf            cpu;
	struct msi_ec_gpu_conf            gpu;
	struct msi_ec_led_conf            leds;
	struct msi_ec_kbd_bl_conf         kbd_bl;

	struct msi_ec_fan_curve_conf      cpu_curve;
	struct msi_ec_fan_curve_conf      gpu_curve;
	struct msi_ec_fan_rpm_conf        cpu_rpm;
	struct msi_ec_fan_rpm_conf        gpu_rpm;
	struct msi_ec_usb_power_share_conf usb_power_share;
	struct msi_ec_qc_conf             qc;
	struct msi_ec_panel_conf          panel;
};

#endif // __MSI_EC_REGISTERS_CONFIG__
