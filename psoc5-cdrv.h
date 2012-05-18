#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/module.h>

#define PSOC5_MAJOR 62
#define PSOC5_MINOR 0
#define MAXLEN			512

static int __init psoc5_cdrv_init(void);
static void __exit psoc5_cdrv_exit(void);

int psoc5_cdrv_open(struct inode *inodep, struct file *filep);
int psoc5_cdrv_release(struct inode *inodep, struct file *filep);

ssize_t psoc5_cdrv_write(struct file *filep, const char __user *ubuf,
												size_t count, loff_t *f_pos);
ssize_t psoc5_cdrv_read(struct file *filep, char __user *ubuf,
												size_t count, loff_t *f_pos);
