deps_config := \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/app_trace/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/aws_iot/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/bt/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/esp32/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/ethernet/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/fatfs/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/freertos/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/heap/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/log/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/lwip/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/mbedtls/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/openssl/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/pthread/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/spi_flash/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/spiffs/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/tcpip_adapter/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/wear_levelling/Kconfig \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/c/Users/Michael/AppData/Roaming/SPB_Data/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
