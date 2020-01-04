
#pragma once
EXTERN_C_START


#define BUFFER_SIZE 128
#define INPUT_INSTANCES 1
#define PID_INSTANCES 1

DWORD
CreateNamedPipeServer(
	LPVOID Params
);


DWORD WINAPI 
InputPipeServerThread(
	LPVOID Params);

DWORD WINAPI 
PidPipeServerThread(
	LPVOID Params);

VOID
WriteResponseToPidClient(
	PQUEUE_CONTEXT queueContext);

VOID
DisconnectPidServer(
	PDEVICE_CONTEXT devContext);

VOID
CompleteReadRequest(
	PDEVICE_CONTEXT devContext,
	UCHAR ReportId);

BOOL 
CreatePipeSecurity(
	PSECURITY_ATTRIBUTES* pSecurityAttributes);

VOID
FreePipeSecurity(
	PSECURITY_ATTRIBUTES psa);

EXTERN_C_END