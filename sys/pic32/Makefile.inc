# TODO: make it relative to Target
installflash:
	sudo pic32prog sys/pic32/fubarino/unix.hex

# TODO: make it relative to Target
installboot:
	sudo pic32prog sys/pic32/fubarino/bootloader.hex

.PHONY: installflash installboot
