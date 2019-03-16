### STM32CubeMX 真的不要太好用

由于工作内容的变动，我已经很久没有正经的玩过单片机了，近期又要用它做个小玩意了，还是选 stm32 吧，外设库开发不要太方便，哈哈哈

先去 stm32 社区逛了逛，发现了一个新字眼 STM32CubeMX，简单看了下，大概明白是个 ST 公司新推出的一个配置工具，直接由图形界面简单配置下，生成初始化代码，并对外设做了进一步的抽象，让开发人员更只专注应用的开发，挺不错的样子，下来玩玩。

详细的入门教程我就不写了，网上太多，这里说下我是怎么用它快速的实现了一个虚拟串口终端和点灯吧，也算是入门教程吧，哈哈哈

我需要用的外设有 usb 接口和一个普通的 GPIO，需要使用的中间件库有 FreeRTOS 和 USB_DEVICE，要完成的功能是实现一个虚拟串口，然后适配上 FreeRTOS 的 CLI 制作一个虚拟串口终端，同时让一个 LED 闪烁，这里我选用经典的 STM32F103C8T6 来完成这些。

那让我们开始吧，

第一步，直接打开 STM32CubeMX，在主界面选到 ACCESS TO MCU SELECTOR 开始一个工程，这里需要注意的一点是此时 STM32CubeMX 会去联网检查一些更新，如果你的防火墙是开的话，很可能会检测失败，关掉防火墙就好。

第二步，在 MCU Filters 栏直接输入我们使用的型号，只需要输入 103C8 就找到啦，然后选中它，右边有它的介绍，而且还可以直接下载它的 Datasheet 也不要太方便了，然后点击 Start Project 按钮开始一个工程。

第三步，把我们需要的外设和中间件库选上，有 USB 接口，GPIO 接口，FREERTOS 和 USB_DEVICE 的支持，最后如下图：

![STM32配置图](http://wx2.sinaimg.cn/mw690/006kxAfbgy1g14o9z9p7fj30vp0p476s.jpg)

配置的时候有以下几点要注意的：

1、时钟的配置在 RCC 选项，根据实际情况是选内部 RC 还是外部晶振，外部晶振的话要占用两个引脚，我这里是 8 Mhz 外部晶振，配置如下：

![RCC外部晶振配置](http://wx2.sinaimg.cn/mw690/006kxAfbgy1g14oa2ffajj30q008dt8v.jpg)

2、关于 debug 口子的配置，默认它是认为我们不需要 debug 口的，所以如果不更改的话生成代码下载进去后 debug 口就关闭了，哈哈哈

如果你真的不小心 debug 口子被关闭了也不用担心，可以通过如下方法再次下载程序：

按住复位按钮要不要放，然后在 Keil 下点击下载按钮，等它提示错误并说检测到被锁掉 debug 口的片子，然后点击确定的同时，松开复位按钮就可以下载啦，如果不行则再多试几次，时序和时机很重要。

然后在 SYS 里面选择 debug 工具连接的类型，我这里是 SWD 的方式连接的，选择串行线，如下图：

![debug接口配置](http://wx1.sinaimg.cn/mw690/006kxAfbgy1g14o9q01j8j30pv08zmxf.jpg)

3、由于 STM32CubeMX 使用的 HAL 库有一个 delay 接口默认是使用 SysTick 定时器来实现的，而由于这里我们选用了 FreeRTOS，它使用的心跳时钟在 Cortex 内核上的移植都是依赖于 SysTick 定时器，这里会有冲突，这一点在点击生成代码按钮时 STM32CubeMX 也会发出警告，这里我们为 HAL 库使用的定时器选过一个就可以了，如下图，这里我把它选到了 TIM1，

![HAL库时钟配置](http://wx1.sinaimg.cn/mw690/006kxAfbgy1g14o9t0qcfj30pr05tdft.jpg)

4、由于后续我们还要在 FreeRTOS 上加入许多线程，我们把他的堆大小设置大一点，在 FREERTOS 的 Config parameters 里面的 TOTAL_HEAP_SIZE，这里比较任性，我配置了 1024 字节，哈哈哈

第四步，我们再来为 FreeRTOS 上加入一个 LED 闪烁的线程。这里我们选到 FREERTOS 的 Taks and Queues，然后点击 Add 按钮即可添加了，取名为 ledTask。

到这里配置工作就差不多了，接下来见证奇迹的时刻到了，点击 GENERATE CODE 然后整个工程就生成了，然后编译下载到板子上，插上电脑，叮咚，成功识别为串口，在 win10 上驱动都不用自己装，是不是很爽，哈哈哈

到这里我们的 LED 还是不能闪的，这需要我们加入几条代码，这个简单，找到我们在工具上创建的 ledTask 线程，在线程函数实体里面加入如下代码再编译下载后灯就开始闪了，简单吧。

![LED闪烁线程](http://wx3.sinaimg.cn/mw690/006kxAfbgy1g14o9w11nwj30jz0a4t90.jpg)

我想我的介绍就到这里结束吧，这才简单嘛，哈哈哈，但等等，说好的虚拟终端呢，

那好吧，那我们先再说个注意点，接下来往上加代码的时候，往由工具生成的文件里面加代码一定要加在标注了 USER CODE BEGIN 和 USER CODE END 的注释里面，这样下次更新配置的时候你加的代码才不会消失，自己加入的文件则无所谓。具体实现我这里也说个大致思路和要注意的点，具体实现，请到我的 github 上去取吧，如果你能点颗星星我就更开心了，哈哈哈

串口终端我们都上 FreeRTOS 了当然有现成的实现了，这里我们主要做的工作就是把 FreeRTOS 提供的串口终端实现即 CLI ，对接上我们的虚拟串口。

第一步，下载 FreeRTOS 的 CLI 扩展库，这里 STM32CubeMX 默认是没有提供的，没关系，我们去 FreeRTOS 的官网下载，注意最好下载当前使用 FreeRTOS 版本对应版本的完整包，然后解压得到 CLI 扩展库加入到我们的工程，同时把它的 Demo 也加进来。

第二步，经阅读 CLI 的 Demo 后发现它依赖的如下四个接口我们实现一下就好了。我们要做的内容就是完善这四个接口，把虚拟串口下来的数据传进来，然后把输出传回去。

	xComPortHandle xSerialPortInitMinimal(int baudRate, int queueLength);

	int xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, int timeout);

	int vSerialPutString(xComPortHandle xPort, signed char *pcWelcomeMessage, unsigned short len);

	int xSerialPutChar(xComPortHandle xPort, signed char c, int timeout);

这里实际上设计是依赖实际的串口的，但没关系我们虚拟的也可以，只要把数据接收发送打通即可。

第三步，完善发送数据接口。发送接口比较简单，经阅读 ST 对虚拟串口的实现了解到往 USB 发送数据，使用 CDC\_Transmit\_FS 接口即可，所以实现如下：

	int vSerialPutString(xComPortHandle xPort, signed char *pMessage, unsigned short len)
	{
		xPort = xPort;
	
	retry:
	
		if(0x00 != CDC_Transmit_FS(pMessage, len))
		{
			osDelay(10);
			goto retry;
		}
	
		return 0x00;
	}

请放过不要吐槽这里的 goto，哈哈哈，这里一定要去检查 CDC\_Transmit\_FS 接口的返回值，刚开始我只检查了，没有动作，后面发现会漏字符，后面才把 retry 加上去的，然后就使用了 goto，我觉的这样很简洁也好理解，当然加上超时机制就更好了，这里勉强先这样吧，哈哈哈

第四步，对接数据接收。这里稍微有点麻烦，CDC 的数据是在 CDC\_Receive\_FS 以 callback 的方式传上来的，应该是使用中断接收，我们串口终端是一个线程通过 xSerialGetChar 接口来获取数据的，这里当然首先想到的就是使用 FreeRTOS 提供的 queue 功能了，但最后发现 queue 只能一个个入列，效果很不理想，会漏字符，如果只传递数据的指针过来的话，又要不停的 malloc 和 free 内存我想效率也不会太高，这里索性搬出来我经常使用的一个 fifo 实现，配合 FreeRTOS 任务通知功能，也算是达到预期吧，哈哈哈，最终它们的实现如下：

	static int8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
	{
		/* USER CODE BEGIN 6 */
		extern TaskHandle_t xConsoleHandle;
		uint32_t count = 0x00;
		BaseType_t xHigherPriorityTaskWoken;
			
		USBD_CDC_SetRxBuffer(&hUsbDeviceFS, &Buf[0]);
		USBD_CDC_ReceivePacket(&hUsbDeviceFS);
		
		count = kfifo_put(&cdcRxFifo, Buf, *Len);
		
		if(	count != *Len)
		{
			CDC_Transmit_FS((uint8_t*) "cdc rx buffer full!!!", strlen("cd rx buffer full!!!"));
		}
		
		if(NULL != xConsoleHandle)
		{
			vTaskNotifyGiveFromISR(xConsoleHandle, &xHigherPriorityTaskWoken);
		
			if(xHigherPriorityTaskWoken)
			{
				taskYIELD();
			}
		}
		
		return (USBD_OK);
		/* USER CODE END 6 */
	}

	int xSerialGetChar(xComPortHandle xPort, signed char *cRxedChar, int timeout)
	{
		static uint32_t i = 0x00;
	
		xPort = xPort;
	
		if(i == 0x00)
		{
			ulTaskNotifyTake(pdTRUE, timeout);
	
			i = kfifo_len(&cdcRxFifo);
		}
	
		if(0x01 == kfifo_get(&cdcRxFifo, cRxedChar, 0x01))
		{
			i--;
		}
	
		return pdPASS;
	}

到这里就介绍完啦，最终串口终端效果图如下：

![终端效果](http://wx2.sinaimg.cn/mw690/006kxAfbgy1g14oalft5wj30da0d9mxd.jpg)

想玩的朋友赶紧去试试吧，欢迎向我留言提问，如果想获取完整工程请到如下链接获取：

	https://github.com/Lingminzou/STM32CubeMX-example

你们的点赞加星星，是我输出的最大动力 ^_^

扫码关注我了解更多

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)
