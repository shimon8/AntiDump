
#ifndef __CRYPT_H_
#define __CRYPT_H_

/*	Just a shortcut	*/
#define	ASM			__asm__ __volatile__

/*	A shortcut to memory protection functions.
 * 	Who would want to write that much every time anyway,
 * 	especially, taking into consideration that we are building both
 * 	Windows and Linux versions.
 */
#ifdef 	WIN32
#include <windows.h>

/*	Make code page writable	*/
#define	SETRWX(addr, len)	{\
								DWORD		attr;\
								VirtualProtect((LPVOID) ((addr) & ~0xFFF), (len) + ((addr) - ((addr) &~0xFFF)), PAGE_EXECUTE_READWRITE, &attr);\
							}

/*	Make code page read only	*/
#define	SETROX(addr, len)	{\
								DWORD		attr;\
								VirtualProtect((LPVOID) ((addr) & ~0xFFF), (len) + ((addr) - ((addr) &~0xFFF)), PAGE_EXECUTE_READ, &attr);\
							}
#else
#include <sys/mman.h>

/*	Make code page writable	*/
#define	SETRWX(addr, len)	mprotect((void*)((addr) & ~0xFFF), (len) + ((addr) - ((addr) &~0xFFF)), PROT_READ | PROT_EXEC | PROT_WRITE)

/*	Make code page read only	*/
#define	SETROX(addr, len)	mprotect((void*)((addr) & ~0xFFF), (len) + ((addr) - ((addr) &~0xFFF)), PROT_READ | PROT_EXEC)
#endif
						
/*	Simple obfuscation of a function call
 * 	(only calls functions which accept two arguments	
 */
#define	ourCall(address, param1, param2)	__asm__ __volatile__( \
	"movl	%%ebx, -4(%%esp)\n\t"		/*	Store EBX register	*/ \
	"movl	$1f, %%ebx\n\t" 			/*	Some obfuscation junk 
											which simply brings us to the next
											instruction	*/ \
	"addl	$2, %%ebx\n\t"\
	"cmpl	%%ebx, %%ebx\n\t"\
	"jnz	2f\n\t"\
	"addl	$3, %%ebx\n\t"\
	"jmpl	*%%ebx\n\t"					/*	Indirect jump to the next 
											instruction	*/ \
	".byte	0xC7, 0xE8\n\t"\
	"1:\n\t"\
	".byte	0xC7, 0x84, 0xAB\n\t"\
	"2:\n\t"\
	".byte	0xC3, 0x8D\n\t"\
	"movl	%0, %%ebx\n\t"				/*	Start "pushing" parameters
											in reverse order	*/ \
	"jmp	3f\n\t"\
	".byte	0xC7, 0x02\n\t"\
	"3:\n\tmovl	%%ebx, -8(%%esp)\n\t"\
	"jmp	4f\n\t"\
	".byte	0xC7\n\t"\
	"4:\n\tmovl	%1, %%ebx\n\t"\
	"movl	%%ebx, -12(%%esp)\n\t"		/*	The first parameter has been 
											"pushed"	*/ \
	"movl	%2, %%ebx\n\t"				/*	Load the address of the function
											into EBX register	*/ \
	"xorl	%3, %%ebx\n\t"				/*	Apply XOR mask	*/ \
	"subl	$12, %%esp\n\t"				/*	Adjust ESP	*/ \
	"call	*%%ebx\n\t"					/*	Call the function.
											It is strongly recommended to use 
											indirect calls	*/ \
	"addl	$8, %%esp\n\t"				/*	Restore ESP	*/ \
	/*	Restore EBX and list parameters	*/ \
	/*	List clobbered registers. It is suggested to list as less as possible,
		but you should check with your compiler implementation (for example, 
		this code compiled with GCC works great without listing EBX, but it
		fails if compiled with MinGW)	*/ \
	"popl	%%ebx\n\t" : : "g"(param2), "g"(param1), "g"(address), "m"(_mask) \
	: "ebx", "eax", "edx", "ecx")


#endif
