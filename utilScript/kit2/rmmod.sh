#!/bin/bash

MODULES=(
        buttondrv.ko
        buzzerdrv.ko
)

for module in "${MODULES[@]}"; do
        echo "Unloading $module...."
        sudo rmmod "./$module"
done

echo "all modules unloaded"