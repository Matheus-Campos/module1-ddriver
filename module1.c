#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init init_mod(void)
{
  printk(KERN_INFO "Module1 started...\n");
  return 0;
}

static void __exit end_mod(void)
{
  printk(KERN_INFO "Module1 ended...\n");
}

module_init(init_mod);
module_exit(end_mod);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Matheus Campos");
MODULE_DESCRIPTION("Test Driver Module");