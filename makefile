include config.mk
#-C是指定目录
#make -C signal   
all:
	@for dir in $(BUILD_DIR); \
	do \
		make -C $$dir; \
	done


.PHONY: clean

clean:
#-rf：删除文件夹，强制删除
	rm -rf  build
	rm -rf signal/*.gch app/*.gch