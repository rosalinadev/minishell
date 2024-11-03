export CFLAGS='-fsanitize=fuzzer,address -fPIE -g' ASAN_SYMBOLIZER_PATH=/bin/llvm-symbolizer-12
