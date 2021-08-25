asm1:
	push   ebp
	mov    ebp,esp
	cmp    DWORD PTR [ebp+0x8],0x421
		jg     0x512 
		cmp    DWORD PTR [ebp+0x8],0x1b4
		jne    0x50a
		mov    eax,DWORD PTR [ebp+0x8]
		add    eax,0x13
		jmp    0x529
		mov    eax,DWORD PTR [ebp+0x8]
		sub    eax,0x13
		jmp    0x529
		cmp    DWORD PTR [ebp+0x8],0x7f7
		jne    0x523
		mov    eax,DWORD PTR [ebp+0x8]
		sub    eax,0x13
		jmp    0x529 
		mov    eax,DWORD PTR [ebp+0x8]
		add    eax,0x13
		pop    ebp
		ret    

