#include "device.h"
#include "driver.h"
#include "queue.h"

VOID
DeviceClose(
    _In_ WDFFILEOBJECT FileObject
)
{
    UNREFERENCED_PARAMETER(FileObject);
}

NTSTATUS
DeviceCreate(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    WDFDEVICE device = { 0 };
    WDF_FILEOBJECT_CONFIG config = { 0 };
    WDF_OBJECT_ATTRIBUTES attributes = { 0 };
    NTSTATUS status = STATUS_SUCCESS;

    WDF_FILEOBJECT_CONFIG_INIT(&config, NULL, DeviceClose, NULL);
    WdfDeviceInitSetFileObjectConfig(DeviceInit, &config, WDF_NO_OBJECT_ATTRIBUTES);

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status))
    {
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[%s] %s: WdfDeviceCreate Failed\n", MOD, __func__));
        return status;
    }

    status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_TEMPLATE, NULL);
    if (!NT_SUCCESS(status))
    {
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[%s] %s: WdfDeviceCreateDeviceInterface Failed\n", MOD, __func__));
        return status;
    }

    status = QueueCreate(device);
    if (!NT_SUCCESS(status))
    {
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[%s] %s: QueueCreate Failed\n", MOD, __func__));
        return status;
    }

    return status;
}