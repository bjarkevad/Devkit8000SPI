#include "psoc5-spi.h"
struct spi_board_info omap3devkit8000_spi_board_info[] = {
	{
		.modalias			= "psoc5",
		.bus_num			= 1,
		.chip_select 	= 0, 
		.max_speed_hz =  2000000, //1500000, //Should be ok, can be set lower if transfer errors occur
		.controller_data = &psoc5_mcspi_device_config,
		.mode					= SPI_MODE_3,
	},
};

static int __devinit psoc5_probe(struct spi_device* spi) {

	psoc5_spi_device->bits_per_word = 8;
	spi_setup(psoc5_spi_device);

	printk(KERN_DEBUG "Probing PSoC5\n");

	return 0;
}

static int __devexit psoc5_remove(struct spi_device* spi) {
	return 0;
}

static struct spi_driver psoc5_spi_driver = {
	.driver = {
		.name = "psoc5",
		.bus = &spi_bus_type,
		.owner = THIS_MODULE,
	},
	.probe = psoc5_probe,
	.remove = __devexit_p(psoc5_remove),

};

int psoc5_spi_init(void) {
	int err, bus_num;
	struct spi_master *psoc5_spi_master;

	bus_num = omap3devkit8000_spi_board_info[0].bus_num;
	psoc5_spi_master = spi_busnum_to_master(bus_num);
	psoc5_spi_device = spi_new_device(psoc5_spi_master,
											&omap3devkit8000_spi_board_info[0]);
	
	if((err=spi_register_driver(&psoc5_spi_driver))< 0)
		printk (KERN_ALERT "Error %d registering the PSoC5 SPI driver\n", err);

	return err;
}

int psoc5_spi_exit(void) {
	spi_unregister_driver(&psoc5_spi_driver);
	spi_unregister_device(psoc5_spi_device);

	return 0;
}

char psoc5_spi_read(void) {
	struct spi_transfer t[1];
	struct spi_message m;
	unsigned char data;

	char data2;
	data2 = 65;

	memset(t, 0, sizeof(t));
	spi_message_init(&m);
	m.spi = psoc5_spi_device;

	t[0].tx_buf = NULL; //&data2;
	t[0].rx_buf = &data;
	t[0].len = 1;
	spi_message_add_tail(&t[0], &m);

	spi_sync(m.spi, &m);

	//printk("PSoC5-spi: read: '%X'\n", data); 

	return data;
}

void psoc5_spi_write(char *dataToWrite, unsigned int len) {
	struct spi_transfer t[1];
	struct spi_message m;

	memset(t, 0, sizeof(t));
	spi_message_init(&m);
	m.spi = psoc5_spi_device;

	t[0].tx_buf = dataToWrite;
	t[0].rx_buf = NULL;
	t[0].len = 1;
	spi_message_add_tail(&t[0], &m);

	// enable this to print out what is sent in HEX
	  int i;
	for( i = 0 ; i < len ; i++ )
	{
		printk("PSoC5-spi: write: '%c' \nLen: %i \n", dataToWrite[i], len);
	}

	if(spi_sync(m.spi, &m)!= 0)
		printk(KERN_ALERT "SPI syncronization error!");
}

