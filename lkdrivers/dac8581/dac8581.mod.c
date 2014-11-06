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
	{ 0x80df453d, "module_layout" },
	{ 0xaafbc7ff, "driver_unregister" },
	{ 0x68fb7a3b, "spi_register_driver" },
	{ 0xbac3211c, "dev_err" },
	{ 0x98f07719, "spi_sync" },
	{ 0x54a9db5f, "_kstrtoul" },
	{ 0xb4390f9a, "mcount" },
	{ 0x82771744, "sysfs_create_group" },
	{ 0xb85fcb40, "dev_set_drvdata" },
	{ 0x2e41c828, "kmem_cache_alloc_trace" },
	{ 0x4030303d, "kmalloc_caches" },
	{ 0x37a0cba, "kfree" },
	{ 0x72c93d7f, "sysfs_remove_group" },
	{ 0x50eedeb8, "printk" },
	{ 0xe7c1c6c7, "dev_get_drvdata" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

MODULE_ALIAS("spi:dac8581");
