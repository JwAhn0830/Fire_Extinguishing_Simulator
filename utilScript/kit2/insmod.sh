#!/bin/bash

MODULES=(
        buttondrv.ko
        buzzerdrv.ko
)

for module in "${MODULES[@]}"; do
        echo "Loading $module...."
        sudo insmod "./$module"
done

echo "all modules loaded"