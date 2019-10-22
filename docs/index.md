# Documentation

## CSIMDX

### Constants

```
CX_ARM_ARCH == 32 // if arm32 with neon
CX_ARM_ARCH == 64 // if aarch64 with neon

CX_X86_LEVEL_NONE   0
CX_X86_LEVEL_MMX    10
CX_X86_LEVEL_SSE    20
CX_X86_LEVEL_SSE2   30
CX_X86_LEVEL_AVX    40

CX_X86_ARCH == CX_X86_LEVEL_NONE // If x86 instructions are not availabel
CX_X86_ARCH == CX_X86_LEVEL_MMX // If no more than mmx is available
CX_X86_ARCH == CX_X86_LEVEL_SSE // If no more than sse is available
CX_X86_ARCH == CX_X86_LEVEL_SSE2 // If no more than sse2 is available
CX_X86_ARCH == CX_X86_LEVEL_AVX // If no more than avx is available
```
