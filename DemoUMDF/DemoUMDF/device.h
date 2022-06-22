#pragma once

#include <Windows.h>
#include <wdf.h>
#include <initguid.h>

DEFINE_GUID(GUID_DEVINTERFACE_DEMOUMDF,
            0xb2ca070f, 0x6ce1, 0xd3be, 0xba, 0x58, 0x6d, 0x80, 0x0a, 0xbb, 0x8e, 0xa6);


NTSTATUS
DeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
);