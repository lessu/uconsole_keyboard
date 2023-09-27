Arduino 1.8.13
Prepare

http://dan.drown.org/stm32duino/package_STM32duino_index.json

STM32F1xx/GD32F1xx boards 
by stm32duino version 2021.2.22

  GENERIC STM32F103R series

  gd32f1_generic_boot20_pc13.bin
  generic_boot20_pc13.bin
# Fork form Uconsole keyboard
forked from https://github.com/clockworkpi/uConsole.
commit:1924163912c207fbd8f2e29927a7798f70807ce5

## What is the difference
### The problem
Original uconsole keyboard code doing a lot of float calculating 
in interrupt,which may cauing losing of interrupt.

The may reason of doing floating calucating is to improve the response
curve of the trackball.When it moves fast,the pointer moves faster,
when percise is needed,the pointer can also act more percise.

But this implementation cause moving trackball glitching and not being sensitive.
(Up/Down direction responsed very bad).
And it doesn't clamp the int8 overflow,causing pointer jumppy.

### What did I do
I removed the float calutation.and replace it to a more simple solution.
It act more smoothly now.

### What to do
I use 'squired response curve' now.
It is not the best obe but worked.
This should replaced to a more accurate and fast one in the future.

## Build With Arduino-cli
install `arduino-cli`
```
arduino-cli compile \
		-b stm32duino:STM32F1:genericSTM32F103R:device_variant=STM32F103RB,upload_method=DFUUploadMethod,cpu_speed=speed_48mhz \
		--build-path=./build
```

Why -b is "stm32duno....."?
Refer to(MacOs):
`~/Library/Arduino15/packages/stm32duino/hardware/STM32F1/2021.2.22/boards.txt`

## Download
Using the offical tool `uconsole_keyboard_flash`.
copy the bin to "uconsole_keyboard_flash" dir on uconsole,and rename to 
`uconsole_keyboard.ino.bin`

run cmd
```bash
flash.sh
```

### When it crashes
This project using Maple as bootloader.
when the bin casued crashing,It will stuck.
The `maple_upload` will try to reboot to bootloader first,but in this case it cannot communicated with serial,and causing failure.
So we can remove the code in `maple_upload` and reboot using bare hand.