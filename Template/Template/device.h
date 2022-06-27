#pragma once

#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

DEFINE_GUID(GUID_DEVINTERFACE_TEMPLATE,
            0x2a91a43a, 0x865e, 0x3a5d, 0xf0, 0x0f, 0x9f, 0x20, 0xf6, 0x6c, 0x1e, 0xc9);

typedef struct _DEVICE_CONTEXT {
    ULONG Reserve;
} DEVICE_CONTEXT, * PDEVICE_CONTEXT;
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

NTSTATUS
DeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
);