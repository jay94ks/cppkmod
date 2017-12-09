/*

	Relocation Script for Linker.
	Author: Jay K (jay94ks@gmail.com, jaehoon joe)

	This will perform merge sections and create a ctor helper symbols
		for C++ global constructors, partially.
*/

SECTIONS
{
	/* Text area for executable codes */
	.text BLOCK(4K) : ALIGN(4K)
	{
		*(.text)
		*(.text.*)
	}
	
	/* Read-only data for constants */
	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
		*(.rodata.*)
	}

	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
		*(.data.*)
	}

	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
		*(.bss.*)
	}
	
	/*
		Init array for global constructors.
		These will be refered by /framework/abi.cpp.
	 */
	.init_array BLOCK(4K) : ALIGN(4K)
	{
		__jayabi_init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array*))
		__jayabi_init_array_end = .;
	}
}