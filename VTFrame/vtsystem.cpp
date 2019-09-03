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
		Log("ERROR:���CPU��֧��VT!",0);
		return FALSE;
	}

	// 2. CR0 CR4
	*((PULONG)&uCr0) = Asm_GetCr0();
	*((PULONG)&uCr4) = Asm_GetCr4();

	if (uCr0.PE != 1||uCr0.PG!=1||uCr0.NE!=1)
	{
		Log("ERROR:���CPUû�п���VT!",0);
		return FALSE;
	}

	if (uCr4.VMXE == 1)
	{
		Log("ERROR:���CPU�Ѿ�������VT!",0);
		Log("�����Ǳ�������Ѿ�ռ����VT�������ر�������ܿ�����",0);
		return FALSE;
	}

	// 3. MSR
	*((PULONG)&msr) = Asm_ReadMsr(MSR_IA32_FEATURE_CONTROL);
	if (msr.Lock!=1)
	{
		Log("ERROR:VTָ��δ������!",0);
		return FALSE;
	}
	Log("SUCCESS:���CPU֧��VT!",0);
	return TRUE;
}