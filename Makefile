default:all
	
all:
	arduino-cli compile \
		-b stm32duino:STM32F1:genericSTM32F103R:device_variant=STM32F103RB,upload_method=DFUUploadMethod,cpu_speed=speed_48mhz \
		--build-path=./build

flash:all
	scp build/uconsole_keyboard.ino.bin lessu@uconsole:/home/lessu/Developer/uconsole_keyboard_flash/uconsole_keyboard.ino.bin
	ssh lessu@uconsole "cd /home/lessu/Developer/uconsole_keyboard_flash;./flash.sh"