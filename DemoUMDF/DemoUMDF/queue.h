#pragma once

#include <Windows.h>
#include <wdf.h>

#define IOCTL_SAYHELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x200, METHOD_BUFFERED, FILE_ANY_ACCESS)

NTSTATUS
QueueCreate(
    _In_ WDFDEVICE  Device
);