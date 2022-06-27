#include "queue.h"
#include "driver.h"

static VOID
IoRead(
    _In_ WDFQUEUE   Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t     Length
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(Length);

    WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
}

static VOID
IoWrite(
    _In_ WDFQUEUE   Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t     Length
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(Length);

    WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
}

static VOID
IoControl(
    _In_ WDFQUEUE   Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t     OutputBufferLength,
    _In_ size_t     InputBufferLength,
    _In_ ULONG      IoControlCode
)
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);

	WdfRequestComplete(Request, STATUS_UNSUCCESSFUL);
}

NTSTATUS
QueueCreate(
    _In_ WDFDEVICE  Device
)
{
    WDFQUEUE queue = { 0 };
    WDF_IO_QUEUE_CONFIG queueConfig = { 0 };
    NTSTATUS status = STATUS_SUCCESS;

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
    queueConfig.EvtIoDeviceControl = IoControl;
    queueConfig.EvtIoRead = IoRead;
    queueConfig.EvtIoWrite = IoWrite;

    status = WdfIoQueueCreate(Device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &queue);
    if (!NT_SUCCESS(status))
        KdPrintEx((DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, "[%s] %s: WdfIoQueueCreate Failed\n", MOD, __func__));

    return status;
}