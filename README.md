{
    "version": "0.2.0",
    "configurations": [
        {
            "name": "OpenOCD",
            "cwd": "${workspaceRoot}",
            "executable": "./build/STM32F4_FREERTOS_EXAMPLE.elf",
            "request": "launch",
            "type": "cortex-debug",
            "device": "STM32F4xxx",
            "servertype": "openocd",
            "interface": "swd",
            "configFiles": [
                "${workspaceRoot}/stm32f4discovery.cfg"
            ]
        }
    ]
}





Link:
    http://www.justinmklam.com/posts/2017/10/vscode-debugger-setup/


Run OpenOCD:
    openocd -f board/stm32f4discovery.cfg