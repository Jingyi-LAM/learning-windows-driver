#include <windows.h>
#include <setupapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <winioctl.h>
#include <initguid.h>

#pragma comment(lib, "setupapi.lib")

DEFINE_GUID(GUID_DEVINTERFACE_TEMPLATE,
    0x2a91a43a, 0x865e, 0x3a5d, 0xf0, 0x0f, 0x9f, 0x20, 0xf6, 0x6c, 0x1e, 0xc9);

PCHAR GetDevicePath(LPGUID Guid, int MaxTriesNumber)
{
    HDEVINFO devInfo = { 0 };
    SP_DEVICE_INTERFACE_DATA devInterfaceData = { 0 };
    PSP_DEVICE_INTERFACE_DETAIL_DATA devInterfaceDetailData = NULL;
    ULONG length = 0;
    ULONG requiredLength = 0;
    BOOL ret;
    int i = 0;

    devInfo = SetupDiGetClassDevs(Guid, NULL, NULL, (DIGCF_PRESENT | DIGCF_DEVICEINTERFACE));
    if (devInfo == INVALID_HANDLE_VALUE) {
        printf("SetupDiGetClassDevs Failed, Error = %d\n", GetLastError());
        exit(1);
    }

    ret = FALSE;
    do {
        devInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

        ret = SetupDiEnumDeviceInterfaces(devInfo, 0, Guid, i, &devInterfaceData);
        if (ret == TRUE)
            break;
        i++;
    } while (i < MaxTriesNumber);

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
    int ret = 0;

    devicePath = GetDevicePath((LPGUID)&GUID_DEVINTERFACE_TEMPLATE, 128);
    deviceHandle = CreateFile(devicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
    if (deviceHandle == INVALID_HANDLE_VALUE) {
        printf("CreateFile Failed, Error = %d\n", GetLastError());
        return -1;
    }

    if (deviceHandle != INVALID_HANDLE_VALUE) {
        CloseHandle(deviceHandle);
    }

    return ret;
}