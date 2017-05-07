# NMOS
An OS I'm working on.

## Compiling ##

To compile you will need a <a href="http://wiki.osdev.org/GCC_Cross-Compiler">i686 cross-compiler</a>.

### All files: ###

	make

or

	make all

### Specific file: ###

	make foo.c

Replace foo.c with the name of the file.

### Make and run: ###

	make run
	
## ISO building tools: ##

# WARNING! #

ISOs are currently NOT SUPPORTED and attempting to run an ISO will result in the OS hanging.
ISO tools are only for development use and not for actual use. YOU HAVE BEEN WARNED!

### Build ISO: ###

	make iso
	
### Build ISO and run: ###

	make run-iso
