#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x611eb49e, "module_layout" },
	{ 0xd119bbde, "cdev_del" },
	{ 0xaaeaa98c, "cdev_init" },
	{ 0xfbc74f64, "__copy_from_user" },
	{ 0x67c2fa54, "__copy_to_user" },
	{ 0x2e5810c6, "__aeabi_unwind_cpp_pr1" },
	{ 0xd8e484f0, "register_chrdev_region" },
	{ 0xc8b57c27, "autoremove_wake_function" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xbed60566, "sub_preempt_count" },
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0xe46018b4, "spi_setup" },
	{ 0xfa2a45e, "__memzero" },
	{ 0x65d6d0f0, "gpio_direction_input" },
	{ 0x4666d0ea, "spi_busnum_to_master" },
	{ 0x27e1a049, "printk" },
	{ 0x42224298, "sscanf" },
	{ 0x4141f80, "__tracepoint_module_get" },
	{ 0xba129451, "driver_unregister" },
	{ 0x3dd31441, "spi_new_device" },
	{ 0x4c6ff041, "add_preempt_count" },
	{ 0xbd1b9920, "spi_bus_type" },
	{ 0xd6b8e852, "request_threaded_irq" },
	{ 0x43b0c9c3, "preempt_schedule" },
	{ 0x1d65cfdd, "spi_sync" },
	{ 0x70788176, "cdev_add" },
	{ 0x20a6ae6c, "module_put" },
	{ 0x11f447ce, "__gpio_to_irq" },
	{ 0x1000e51, "schedule" },
	{ 0x4e61d82f, "__wake_up" },
	{ 0xfe990052, "gpio_free" },
	{ 0x45e4d75c, "prepare_to_wait" },
	{ 0xfbb0456, "finish_wait" },
	{ 0xc0382997, "device_unregister" },
	{ 0xb81960ca, "snprintf" },
	{ 0xa9b897da, "spi_register_driver" },
	{ 0xf20dabd8, "free_irq" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "2CC242840B1163D387E411E");
