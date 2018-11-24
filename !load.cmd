@rem Прошивка загрузчика на 1-е 3 сектора flash
@if exist .\Release\bootloader.bin (
	 cscript !make_programFlash.vbs --driver "C:\Program Files (x86)\Analog Devices\VisualDSP 5.1.2\Blackfin\Examples\ADSP-BF506F EZ-KIT Lite\Flash Programmer\BF50x4MBFlash\BF504FEzFlashDriver_BF50x4MBFlash.dxe" --format 1 --offsetL 0 --image %~dp0Release\bootloader.bin --boot --verifyWrites
) else echo "Error! File bootloader.bin doesn't exist!"
