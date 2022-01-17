# Simple tool to read / write SSE registers

# Building

Build with mvn package

If you want to rebuild the native bits, you need a reasonably recent Visual Studio. See src/native/buildDll.bat

# What does it do

We jam XMM0..XMM7 full of random data, then read back that data. If native code called via JNA was able to clobber XMM registers, sooner or later you'll trigger weirdness caused by https://github.com/corretto/corretto-8/pull/337

# Why?

Though MS calling conventions state that on x86, XMM registers can be freely clobbered by callee, in practice C2 JIT will crash if your native code clobbers them.
