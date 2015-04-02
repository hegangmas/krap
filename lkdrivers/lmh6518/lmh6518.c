/*
 * LMH6518 900 MHz, Digitally Controlled, Variable Gain Amplifier
 *
 * Licensed under the GPL-2 or later.
 *
 */
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/sysfs.h>
#include <linux/spi/spi.h>
#include <linux/module.h>
#include <linux/err.h>
/*
#include <linux/hwmon.h>
#include <linux/hwmon-sysfs.h>
*/
#define LMH6518_READ	(1 << 7)
#define LMH6518_WRITE	(0 << 7)

#define   LMH6518_RW_OPERATION_OFFSET   23 //8(command)+16(data)-1
#define   LMH6518_RW_OPERATION_MASK     0x01

/*
 * LMH6518 fpower modes
 */
#define   LMH6518_FPOWER_OFFSET      10
#define   LMH6518_FPOWER_MASK        0x01

#define LMH6518_FPOWER_FULL		0x0
#define LMH6518_FPOWER_AUXHIZ		0x1

/*
 * LMH6518 preamp modes
 */
#define   LMH6518_PREAMP_OFFSET         4
#define   LMH6518_PREAMP_MASK           0x01

#define LMH6518_PREAMP_HG		0x1
#define LMH6518_PREAMP_LG		0x0

/*
 * LMH6518 filter modes in mhz
 */
#define   LMH6518_FILTER_OFFSET         6
#define   LMH6518_FILTER_MASK           0x07

#define LMH6518_FILTER_FULL 0x0
#define LMH6518_FILTER_20 0x1
#define LMH6518_FILTER_100 0x2
#define LMH6518_FILTER_200 0x3
#define LMH6518_FILTER_350 0x4
#define LMH6518_FILTER_650 0x5
#define LMH6518_FILTER_750 0x6

/*
 * LMH6518 ladder attenuation in negative decibel
 */
#define   LMH6518_ATTENUATOR_OFFSET     0
#define   LMH6518_ATTENUATOR_MASK       0x0F

#define LMH6518_LA_00 0x0
#define LMH6518_LA_02 0x1
#define LMH6518_LA_04 0x2
#define LMH6518_LA_06 0x3 
#define LMH6518_LA_08 0x4
#define LMH6518_LA_10 0x5
#define LMH6518_LA_12 0x6
#define LMH6518_LA_14 0x7
#define LMH6518_LA_16 0x8
#define LMH6518_LA_18 0x9
#define LMH6518_LA_20 0xA

enum LMH6518_variant {
	lmh6518,

};

struct lmh6518_data {
	struct spi_device *spi_dev;
/*	struct device *hwmon_dev;*/
};

static int lmh6518_write(struct lmh6518_data *chip, u16 data) {
	int ret;
//				write 			2.part 				1.part
	u8 tx[3] = { (LMH6518_WRITE), ((data & 0xff00) >> 8), (data & 0x00ff) };
	ret = spi_write(chip->spi_dev, (void*) &tx, sizeof(tx));

	if (ret < 0)
		dev_err(&chip->spi_dev->dev, "SPI write error\n");

	return ret;
}

static int lmh6518_read(struct lmh6518_data *chip, u16 *data) {
	int ret = 0;
	u16 rx = 0;
	u8 tx = LMH6518_READ;

	ret = spi_write_then_read(chip->spi_dev, (void*) &tx, /*sizeof(tx)*/1, &rx, /*sizeof(rx)*/
			2);

	if (ret < 0) {
		dev_err(&chip->spi_dev->dev, "SPI read error\n");
		return ret;
	}

	*data = rx;
	return ret;
}

static ssize_t lmh6518_show_preamp(struct device *dev,
		struct device_attribute *attr, char *buf) {
	struct lmh6518_data *chip;
	u16 rx;
	int ret;
	printk("VGA LMH6518 show preamp \n");

	chip = dev_get_drvdata(dev);

	ret = lmh6518_read(chip, &rx);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n",
			(rx & (LMH6518_PREAMP_OFFSET << LMH6518_PREAMP_MASK)));

}

static ssize_t lmh6518_show_filter(struct device *dev,
		struct device_attribute *attr, char *buf) {

	struct lmh6518_data *chip;
	int ret;
	u16 rx;
	printk("VGA LMH6518 show filter \n");
	chip = dev_get_drvdata(dev);

	ret = lmh6518_read(chip, &rx);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n",
			((rx >> LMH6518_FILTER_OFFSET) & LMH6518_FILTER_MASK));

}

static ssize_t lmh6518_show_attenuation(struct device *dev,
		struct device_attribute *attr, char *buf) {

	struct lmh6518_data *chip;
	int ret;
	u16 rx;
	printk("VGA LMH6518 show attenuation \n");

	chip = dev_get_drvdata(dev);

	ret = lmh6518_read(chip, &rx);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n",
			((rx >> LMH6518_ATTENUATOR_OFFSET) & LMH6518_ATTENUATOR_MASK));
}

static ssize_t lmh6518_show_fpower(struct device *dev,
		struct device_attribute *attr, char *buf) {

	struct lmh6518_data *chip;
	int ret;
	u16 rx;
	printk("VGA LMH6518 show fpower \n");

	chip = dev_get_drvdata(dev);

	ret = lmh6518_read(chip, &rx);
	if (ret < 0)
		return ret;

	return sprintf(buf, "%d\n",
			(rx & (LMH6518_FPOWER_MASK << LMH6518_FPOWER_OFFSET)));

}

static ssize_t lmh6518_store_filter(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct lmh6518_data *chip;
	int ret;

	long unsigned int userinput;
	u16 data = 0;

	printk("VGA LMH6518 store filter \n");

	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (userinput << LMH6518_FILTER_OFFSET);
	ret = lmh6518_write(chip, data);

	if (ret < 0)
		return ret;

	return size;
}

static ssize_t lmh6518_store_fpower(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct lmh6518_data *chip;
	int ret;

	long unsigned int userinput;
	u16 data = 0;

	printk("VGA LMH6518 store fpower\n");
	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (userinput << LMH6518_FPOWER_OFFSET);
	ret = lmh6518_write(chip, data);

	if (ret < 0)
		return ret;

	return size;
}

static ssize_t lmh6518_store_preamp(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct lmh6518_data *chip;
	int ret;
	long unsigned int userinput;
	u16 data = 0;

	printk("VGA LMH6518 store fpower\n");
	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (userinput << LMH6518_PREAMP_OFFSET);
	ret = lmh6518_write(chip, data);

	if (ret < 0)
		return ret;

	return size;
}

static ssize_t lmh6518_store_attenuation(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size) {

	struct lmh6518_data *chip;
	int ret;

	long unsigned int userinput;
	u16 data = 0;

	printk("VGA LMH6518 store attenuation\n");
	chip = dev_get_drvdata(dev);

	userinput = 0;
	ret=strict_strtoul(buf, 0, &userinput);
	if(ret<0)
		return ret;

	data = (userinput << LMH6518_ATTENUATOR_OFFSET);
	ret = lmh6518_write(chip, data);

	if (ret < 0)
		return ret;

	return size;
}

static DEVICE_ATTR(filters, S_IRUGO | S_IWUSR, lmh6518_show_filter, lmh6518_store_filter);
static DEVICE_ATTR(fpower, S_IRUGO | S_IWUSR, lmh6518_show_fpower, lmh6518_store_fpower);
static DEVICE_ATTR(preamp, S_IRUGO | S_IWUSR, lmh6518_show_preamp, lmh6518_store_preamp);
static DEVICE_ATTR(attenuation, S_IRUGO | S_IWUSR, lmh6518_show_attenuation, lmh6518_store_attenuation);

static struct attribute *lmh6518_attributes[] = {
		&dev_attr_filters.attr,
		&dev_attr_fpower.attr,
		&dev_attr_preamp.attr,
		&dev_attr_attenuation.attr,
		NULL,
};

static const struct attribute_group lmh6518_group = {
		.attrs =lmh6518_attributes,
};

static int __devinit lmh6518_probe(struct spi_device *spi_dev)
{
	int ret;
	struct lmh6518_data *chip;

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (chip == NULL) {
		ret = -ENOMEM;
		goto error_ret;
	}
	chip->spi_dev=spi_dev;

	dev_set_drvdata(&spi_dev->dev, chip);

	ret = sysfs_create_group(&spi_dev->dev.kobj, &lmh6518_group);
	if (ret < 0)
	goto error_free_chip;

/*	chip->hwmon_dev = hwmon_device_register(&spi_dev->dev);

	if (IS_ERR(chip->hwmon_dev)) {
		ret = PTR_ERR(chip->hwmon_dev);
		goto error_remove_group;
	}
*/
	return 0;
/*error_remove_group:

	sysfs_remove_group(&spi_dev->dev.kobj, &lmh6518_group);*/
error_free_chip:

	kfree(chip);
error_ret:
	return ret;
}

static int __devexit lmh6518_remove(struct spi_device *spi_dev)
{

	struct lmh6518_data *chip;

	chip = dev_get_drvdata(&spi_dev->dev);
	printk("Spi protocol driver for VGA LMH6518 remove \n");
/*	hwmon_device_unregister(chip->hwmon_dev);*/
	sysfs_remove_group(&spi_dev->dev.kobj, &lmh6518_group);
	kfree(chip);

	return 0;
}

static const struct spi_device_id lmh6518_id[] = { { "lmh6518", lmh6518 }, { } };

MODULE_DEVICE_TABLE( spi, lmh6518_id);

static struct spi_driver lmh6518_driver = {
		.driver = {
				.name = "lmh6518",
				.owner = THIS_MODULE,
		},
		.probe = lmh6518_probe,
		.remove = __devexit_p(lmh6518_remove),
		.id_table = lmh6518_id,
};

static __init int lmh6518_init(void)
{
	printk("Spi protocol driver for VGA LMH6518 \n");
	return spi_register_driver(&lmh6518_driver);
}
module_init( lmh6518_init);

static __exit void lmh6518_exit(void)
{
	printk("Spi protocol driver for VGA LMH6518 bye :) \n");
	spi_unregister_driver(&lmh6518_driver);
}
module_exit( lmh6518_exit);

MODULE_AUTHOR("Uros Golob <uros@golob.mobi>");
MODULE_DESCRIPTION("TI LMH6518" "900 MHz, Digitally Controlled, Variable Gain Amplifier");
MODULE_LICENSE("GPL v2");
