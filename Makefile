all:
	echo make init --- create build environment
	echo make rom ---- make romdata.bin

init:
	git submodule init
	git submodule update
	pod update
	pod install
	make rom
	open VGSKeyboard.xcworkspace

clean-pod:
	-rm -f Podfile.lock
	-rm -rf Pods
	-rm -rf VGSKeyboard.xcworkspace

rom:
	cd romdata && make

