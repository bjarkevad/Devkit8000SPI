#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/gpio.h>

#include "psoc5-spi.h"
#include "psoc5-cdrv.h"

irqreturn_t psoc5_isr(int irq, void *dev_id) {
	//printk(KERN_ALERT "PSoC5 interrupt!\n");
	newData = 1;
	wake_up_interruptible(&wait_queue);
	return IRQ_HANDLED;
}

static int __init psoc5_cdrv_init(void) {
	int err;

	printk("PSoC5 driver initializing \n");
	cdev_init(&psoc5Dev, &psoc5_Fops);
	
	psoc5Dev.owner = THIS_MODULE;
	psoc5Dev.ops = &psoc5_Fops;
	devno = MKDEV(PSOC5_MAJOR, PSOC5_MINOR);
	err = register_chrdev_region(devno, 1, "PSoC5");
	err = cdev_add(&psoc5Dev, devno, 1);

	if(err) {
		printk(KERN_ALERT "Error %d adding PSoC5 device\n", err);
		return err;
	}
	
	if((err = gpio_request(INTGPIO, "psocIntIO")) < 0) {
		printk("Error requesting gpio resources.. \n");
		return err;
	}

	gpio_direction_input(INTGPIO);

	err = psoc5_spi_init();

	return err;
}

static void __exit psoc5_cdrv_exit(void) {
	printk("PSoC5 cdrv exit\n");
    psoc5_spi_exit();
	unregister_chrdev_region(devno, 1);
	cdev_del(&psoc5Dev);
	gpio_free(INTGPIO);
}

int psoc5_cdrv_open(struct inode *inodep, struct file *filep) {
	int err;

	printk("Opening PSoC5 device..\n");
	
	if((err = request_irq(gpio_to_irq(INTGPIO), psoc5_isr, IRQF_TRIGGER_RISING, "psoc5IRQ", NULL) < 0)) {
		printk("Error requesting irq resources.. \n");
		return err;
	}
	
	if(!try_module_get(psoc5_Fops.owner))
		return -ENODEV;

	return err;
}


int psoc5_cdrv_release(struct inode *inode, struct file *filep) {

	free_irq(gpio_to_irq(INTGPIO), NULL);
	module_put(psoc5_Fops.owner);

	printk("Closing PSoC5 device..\n");

	return 0;
}

ssize_t psoc5_cdrv_write(struct file *filep, const char __user *ubuf,
										size_t count, loff_t *f_pos) {
	unsigned int len;
	char kbuf[MAXLEN];

	len = count < MAXLEN ? count : MAXLEN;
  len--;
	if(copy_from_user(kbuf, ubuf, len))
		return -EFAULT;

	//kbuf[len] = '\0';

	psoc5_spi_write(kbuf, len);

	return count;
}

ssize_t psoc5_cdrv_read(struct file *filep, char __user *ubuf,
										size_t count, loff_t *f_pos) {
	char result;
	char resultBuf[5];

	//wait_event_interruptible(wait_queue, newData == 1);
	if(wait_event_interruptible_timeout(wait_queue, newData == 1, 1000) == 0) {
		result = 0;
		copy_to_user(ubuf, &result, 1);
		printk(KERN_ALERT "cdrv timeout read: '%X' \n", result);
		return 1;
	}

	newData = 0; 

	result = psoc5_spi_read();

	count = snprintf(resultBuf, sizeof(resultBuf), "%c", result);

	/*if(result != 0x00 && result != 0x0F)
		printk(KERN_ALERT "cdrv read: '%X' \n", result);*/

	if(copy_to_user(ubuf, resultBuf, sizeof(resultBuf)) != 0) {
		printk("Error copying to user \n");
		return -EFAULT;
	}
 	return count;
}

struct file_operations psoc5_Fops = {
	.owner		= THIS_MODULE,
	.open			= psoc5_cdrv_open,
	.release	= psoc5_cdrv_release,
	.write		= psoc5_cdrv_write,
	.read			= psoc5_cdrv_read,
};

module_init(psoc5_cdrv_init);
module_exit(psoc5_cdrv_exit);

MODULE_AUTHOR("Bjarke Vad Andersen <bjarke.vad90@gmail.com>");
MODULE_LICENSE("GPL");
