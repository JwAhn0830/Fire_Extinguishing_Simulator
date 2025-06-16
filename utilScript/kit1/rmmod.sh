#!/bin/bash

MODULES=(
        buzzerdrv.ko
        fnddrv.ko
        leddrv.ko
        textlcddrv.ko
)

for module in "${MODULES[@]}"; do
        echo "Unloading $module...."
        sudo rmmod "./$module"
done

echo "all modules unloaded"