//#include "stdafx.h"
#include "vtsystem.h"
#include "vtasm.h"

BOOLEAN IsVTEnabled()
{
	ULONG uRet_EAX,uRet_ECX,uRet_EDX,uRet_EBX;
	_CPUID_ECX uCPUID;
	_CR0 uCr0;
	_CR4 uCr4;
	IA32_FEATURE_CONTROL_MSR msr;
	//1. CPUID
	Asm_CPUID(1,&uRet_EAX,&uRet_EBX,&uRet_ECX,&uRet_EDX);
	*((PULONG)&uCPUID) = uRet_ECX;

	if (uCPUID.VMX != 1)
	{
		Log("ERROR:这个CPU不支持VT!",0);
		return FALSE;
	}

	// 2. CR0 CR4
	*((PULONG)&uCr0) = Asm_GetCr0();
	*((PULONG)&uCr4) = Asm_GetCr4();

	if (uCr0.PE != 1||uCr0.PG!=1||uCr0.NE!=1)
	{
		Log("ERROR:这个CPU没有开启VT!",0);
		return FALSE;
	}

	if (uCr4.VMXE == 1)
	{
		Log("ERROR:这个CPU已经开启了VT!",0);
		Log("可能是别的驱动已经占用了VT，你必须关闭它后才能开启。",0);
		return FALSE;
	}

	// 3. MSR
	*((PULONG)&msr) = Asm_ReadMsr(MSR_IA32_FEATURE_CONTROL);
	if (msr.Lock!=1)
	{
		Log("ERROR:VT指令未被锁定!",0);
		return FALSE;
	}
	Log("SUCCESS:这个CPU支持VT!",0);
	return TRUE;
}