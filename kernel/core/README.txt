
ABOUT kernel/core

kernel/core/ contains everything fundamental, basic and necessary,
which does not include file systems, program loading, etc.

Ideally, kernel/core and kernel/system will be independant. Once
kernel/system is accessed, at which point kernel/core will be
pointless, it can be overridden.

