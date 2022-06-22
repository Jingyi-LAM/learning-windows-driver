#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <winioctl.h>
#include <initguid.h>

#pragma comment(lib, "setupapi.lib")

#define IOCTL_SAYHELLO CTL_CODE(FILE_DEVICE_UNKNOWN, 0x200, METHOD_BUFFERED, FILE_ANY_ACCESS)

DEFINE_GUID(GUID_DEVINTERFACE_DEMOKMDF,
    0x2a91a43a, 0x865e, 0x3a5d, 0xf0, 0x0f, 0x9f, 0x20, 0xf6, 0x6c, 0x1e, 0xc9);

PCHAR GetDevicePath(_In_ LPGUID Guid)
{
    HDEVINFO devInfo = { 0 };
    SP_DEVICE_INTERFACE_DATA devInterfaceData = { 0 };
    PSP_DEVICE_INTERFACE_DETAIL_DATA devInterfaceDetailData = NULL;
    ULONG length = 0;
    ULONG requiredLength = 0;
    BOOL ret;

    devInfo = SetupDiGetClassDevs(Guid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
    if (devInfo == INVALID_HANDLE_VALUE) {
        printf("SetupDiGetClassDevs Failed, Error = %d\n", GetLastError());
        exit(1);
    }

    devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

    ret = SetupDiEnumDeviceInterfaces(devInfo, 0, Guid, 0, &devInterfaceData);
    if (ret == FALSE) {
        printf("SetupDiEnumDeviceInterfaces Failed, Error = %d\n", GetLastError());
        SetupDiDestroyDeviceInfoList(devInfo);
        exit(1);
    }

    SetupDiGetDeviceInterfaceDetail(devInfo, &devInterfaceData, NULL, 0, &requiredLength, NULL);
    devInterfaceDetailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)LocalAlloc(LMEM_FIXED, requiredLength);
    if (devInterfaceDetailData == NULL)
    {
        printf("LocalAlloc Failed\n");
        SetupDiDestroyDeviceInfoList(devInfo);
        exit(1);
    }

    devInterfaceDetailData->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
    length = requiredLength;

    ret = SetupDiGetDeviceInterfaceDetail(devInfo, &devInterfaceData, devInterfaceDetailData, length, &requiredLength, NULL);
    if (ret == FALSE) {
        printf("SetupDiGetDeviceInterfaceDetail Failed, Error = %d\n", GetLastError());
        SetupDiDestroyDeviceInfoList(devInfo);
        LocalFree(devInterfaceDetailData);
        exit(1);
    }

    return devInterfaceDetailData->DevicePath;
}

int main(int argc, char* argv[])
{
    PCHAR  devicePath;
    HANDLE deviceHandle = INVALID_HANDLE_VALUE;
    CHAR input[128] = "Hi";
    CHAR output[256] = { 0 };
    INT returnLength = 0;
    int ret = 0;

    devicePath = GetDevicePath((LPGUID)&GUID_DEVINTERFACE_DEMOKMDF);

    deviceHandle = CreateFile(devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (deviceHandle == INVALID_HANDLE_VALUE) {
        printf("CreateFile Failed, Error = %d\n", GetLastError());
        return -1;
    }

    // Call device IO Control interface (CharSample_IOCTL_800) in driver
    if (!DeviceIoControl(deviceHandle, IOCTL_SAYHELLO, input, 128, output, 256, &returnLength, NULL))
    {
        printf("DeviceIoControl Failed, Error = %d\n", GetLastError());
        ret = -1;
    }

    printf("Got a reply from the driver: %s\n", output);

    if (deviceHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(deviceHandle);
    }

    return ret;
}