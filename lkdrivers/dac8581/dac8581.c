/*
 * 16-BIT, HIGH-SPEED, LOW-NOISE, VOLTAGE OUTPUT 
 * DIGITAL-TO-ANALOG CONVERTER
 *
 * Licensed under the GPL-2 or later.
 *
 */
#include <linux/device.h>
#include <asm/string.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/err.h>

enum dac8581_variant {
	dac8581,
};

struct dac8581_data {
	struct spi_device *spi_dev;

};

static int dac8581_write(struct dac8581_data *chip, u8 *data, u8 len) {
	int ret = 0;



	ret = spi_write(chip->spi_dev, (void*)data, len);
	if (ret < 0)
		dev_err(&chip->spi_dev->dev, "SPI write error\n");

	return ret;
}

static ssize_t dac8581_store_dac(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct dac8581_data *chip;
	int ret = 0;

	long unsigned int userinput;
	u8 data[2] = {0};

	userinput = 0;

	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;
		
	data[1] = (u8) (userinput&0xff);
	data[0] = (u8) ((userinput&0xff00)>>8);

	chip = dev_get_drvdata(dev);

	ret = dac8581_write(chip, data,2);
	if (ret < 0)
		return ret;
		
	printk("%d\n", (int)userinput);
	return size;
}

static DEVICE_ATTR( dac, S_IWUSR, NULL, dac8581_store_dac);

static struct attribute *dac8581_attributes[] = {
		&dev_attr_dac.attr, NULL, };

static const struct attribute_group dac8581_group = {
		.attrs = dac8581_attributes,
};

static int __devinit dac8581_probe(struct spi_device *spi_dev)
{
	int ret;
	struct dac8581_data *chip;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}

	chip->spi_dev=spi_dev;


	dev_set_drvdata(&spi_dev->dev, chip);

	ret = sysfs_create_group(&spi_dev->dev.kobj, &dac8581_group);
	if (ret < 0)
	goto error_free_chip;


	return 0;

error_free_chip:
	kfree(chip);
error_ret:
	return ret;
}

static int __devexit dac8581_remove(struct spi_device *spi_dev)
{

	struct dac8581_data *chip;

	chip = dev_get_drvdata(&spi_dev->dev);
	printk("Spi protocol driver for dac8581 remove \n");

	sysfs_remove_group(&spi_dev->dev.kobj, &dac8581_group);
	kfree(chip);

	return 0;
}

static const struct spi_device_id dac8581_id[] = { { "dac8581", dac8581 }, { } };

MODULE_DEVICE_TABLE( spi, dac8581_id);

static struct spi_driver dac8581_driver = {
		.driver = {
				.name = "dac8581",
				.owner = THIS_MODULE,
		},
		.probe = dac8581_probe,
		.remove = __devexit_p(dac8581_remove),
		.id_table = dac8581_id,
};

static __init int dac8581_init(void)
{
	printk("Spi protocol driver for  dac8581 \n");
	return spi_register_driver(&dac8581_driver);
}
module_init( dac8581_init);

static __exit void dac8581_exit(void)
{
	printk("Spi protocol driver for dac8581 bye :) \n");
	spi_unregister_driver(&dac8581_driver);
}
module_exit( dac8581_exit);

MODULE_AUTHOR("Uros Golob <uros@golob.mobi>");
MODULE_DESCRIPTION("dac8581" "16bit digital to analog converter(hs,ln)");
MODULE_LICENSE("GPL v2");
