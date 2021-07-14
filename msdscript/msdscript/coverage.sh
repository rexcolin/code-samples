
make coverage
LLVM_PROFILE_FILE="coverage.profraw" ./msd_coverage --test
xcrun llvm-profdata merge -sparse coverage.profraw -o coverage.profdata
xcrun llvm-cov report ./msd_coverage -instr-profile=coverage.profdata > coverage_report.txt
xcrun llvm-cov show ./msd_coverage -instr-profile=coverage.profdata > coverage_line.txt
