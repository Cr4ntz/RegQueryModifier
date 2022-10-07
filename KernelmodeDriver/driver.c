#pragma warning(disable : 4100)
#pragma warning(disable : 4305)

#include "driver.h"

LARGE_INTEGER cookie;

NTSTATUS ExCallbackFunction(
	          PVOID CallbackContext,
	 PVOID Argument1,
	 PVOID Argument2
)
{
	if ((REG_NOTIFY_CLASS)Argument1 == RegNtPostQueryValueKey)
	{
		UNICODE_STRING keyValName;
		RtlInitUnicodeString(&keyValName, L"ProxyEnable");

		REG_POST_OPERATION_INFORMATION* regPostInfo = (REG_POST_OPERATION_INFORMATION*)Argument2;
		REG_QUERY_VALUE_KEY_INFORMATION* regPreInfo = (REG_QUERY_VALUE_KEY_INFORMATION*)regPostInfo->PreInformation;

		if (RtlEqualUnicodeString(regPreInfo->ValueName, &keyValName, FALSE))
		{
			if (regPreInfo->KeyValueInformationClass == KeyValuePartialInformation)
			{	
				PEPROCESS procInfo = 0;
				PUNICODE_STRING pathName;
				HANDLE pId = PsGetCurrentProcessId();		
				PsLookupProcessByProcessId(pId, &procInfo);
				SeLocateProcessImageName(procInfo, &pathName);

				if (wcsstr(pathName->Buffer, L"spotterfy"))
				{
					KEY_VALUE_PARTIAL_INFORMATION* keyPartial = (KEY_VALUE_PARTIAL_INFORMATION*)regPreInfo->KeyValueInformation;
					keyPartial->Data[0] = 0;
				}

				ExFreePool(pathName);
			}

			
		}

		
	
	}
	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING pRegistryPath)
{
	pDriverObject->DriverUnload = UnloadDriver;
	DbgPrintEx(0, 0, "RegMon driver has been loaded");

	UNICODE_STRING alt;
	RtlInitUnicodeString(&alt, L"32666");

	NTSTATUS status = CmRegisterCallbackEx(
		ExCallbackFunction,
		&alt,
		pDriverObject,
		0,
		&cookie,
		0
	);
	if (status != STATUS_SUCCESS)
		DbgPrintEx(0, 0, "Failed to register callback");

	return STATUS_SUCCESS;
}

NTSTATUS UnloadDriver(PDRIVER_OBJECT pDriverObject)
{
	DbgPrintEx(0, 0, "RegMon driver has been unloaded");
	CmUnRegisterCallback(cookie);

	return STATUS_SUCCESS;
}

