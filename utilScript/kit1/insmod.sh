#!/bin/bash

MODULES=(
        buzzerdrv.ko
        fnddrv.ko
        leddrv.ko
        textlcddrv.ko
)

for module in "${MODULES[@]}"; do
        echo "Loading $module...."
        sudo insmod "./$module"
done

echo "all modules loaded"